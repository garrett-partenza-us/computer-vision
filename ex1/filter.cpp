#include <opencv2/highgui.hpp> 
#include <opencv2/core/hal/interface.h>
#include "filter.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core.hpp>
#include <math.h>


using namespace cv;
using namespace std;

double clamp( double val ) {
    if (val < 0) {
      val = 0;
    }
    else if (val > 255) {
      val = 255;
    }
    return val;
}

cv::Vec3b sobel3x3helper( cv::Mat &kern, cv::Mat &pixels ) {

  cv::Mat channels[3];
  cv::split(pixels, channels);
  channels[0].convertTo(channels[0], 19);
  channels[1].convertTo(channels[1], 19);
  channels[2].convertTo(channels[2], 19);

  cv::Mat red = kern.mul(channels[0]);
  cv::Mat green = kern.mul(channels[1]);
  cv::Mat blue = kern.mul(channels[2]);
  
  cv::Vec3b color;
  color[0] = clamp(cv::sum(red)[0] / 3);;
  color[1] = clamp(cv::sum(green)[0] / 3);
  color[2] = clamp(cv::sum(blue)[0] / 3);
  
  return color;

}

// Alternate greyscale image
int greyscale( cv::Mat &src, cv::Mat &dst ) {

  for(int i=0; i<src.rows; i++){
      for(int j=0; j<src.cols; j++){
          cv::Vec3b intensity = src.at<cv::Vec3b>(i, j);
          float blue = intensity.val[0];
          float green = intensity.val[1];
          float red = intensity.val[2];
          float ntsc = 0.21 * red + 0.72 * green + 0.07 * blue;
          dst.at<char>(i,j) = ntsc;
      }
  }

  return 0;
}

// 5x5 Gaussian Filter
int blur5x5( cv::Mat &src, cv::Mat &dst ) {

  cv::Mat kern = (cv::Mat_<char>(5, 5) << 1, 1, 1, 1, 1,
                                          1, 2, 2, 2, 1,
                                          1, 2, 4, 2, 1,
                                          1, 2, 2, 2, 1,
                                          1, 1, 1, 1, 1);
  kern.convertTo(kern, dst.type());

  for(int i=2; i<src.rows-2; i++){
      for(int j=2; j<src.cols-2; j++){

          cv::Mat pixels = src(cv::Range(i-2,i+3), cv::Range(j-2,j+3));
          cv::Mat channels[3];
          cv::split(pixels, channels);

          cv::Mat red = kern.mul(channels[0]);
          cv::Mat green = kern.mul(channels[1]);
          cv::Mat blue = kern.mul(channels[2]);

          cv::Vec3b color;

          color[0] = cv::sum(red)[0] / cv::sum(kern)[0];
          color[1] = cv::sum(green)[0] / cv::sum(kern)[0];
          color[2] = cv::sum(blue)[0] / cv::sum(kern)[0];

          dst.at<Vec3b>(i-2,j-2) = color;
      }
  }

  return 0;

}

// X Sobel Filter
int sobelX3x3( cv::Mat &src, cv::Mat &dst ) {

  cv::Mat kern = (cv::Mat_<char>(3, 3) << -1, 0, 1, -1, 0, 1, -1, 0, 1);

  kern.convertTo(kern, 19);

  for(int i=2; i<src.rows-2; i++){
      for(int j=2; j<src.cols-2; j++){
          cv::Mat pixels = src(cv::Range(i-1,i+2), cv::Range(j-1,j+2));
          dst.at<Vec3b>(i-2,j-2) = sobel3x3helper(kern, pixels);
      }
  }

  return 0;

}

// Y Sobel Filter
int sobelY3x3( cv::Mat &src, cv::Mat &dst ) {

  cv::Mat kern = (cv::Mat_<char>(3, 3) << -1, -1, -1, 0, 0, 0, 1, 1, 1);

  kern.convertTo(kern, 19);

  for(int i=2; i<src.rows-2; i++){
      for(int j=2; j<src.cols-2; j++){
          cv::Mat pixels = src(cv::Range(i-1,i+2), cv::Range(j-1,j+2));
          dst.at<Vec3b>(i-2,j-2) = sobel3x3helper(kern, pixels);
      }
  }

  return 0;

}

// XY Magnitude Filter
int magnitude( cv::Mat &src, cv::Mat &dst ) {

  cv::Mat xkern = (cv::Mat_<char>(3, 3) << -1, 0, 1, -1, 0, 1, -1, 0, 1);
  cv::Mat ykern = (cv::Mat_<char>(3, 3) << -1, -1, -1, 0, 0, 0, 1, 1, 1);
  
  xkern.convertTo(xkern, 19);
  ykern.convertTo(ykern, 19);

  for(int i=2; i<src.rows-2; i++){
      for(int j=2; j<src.cols-2; j++){

          // Get pixels
          cv::Mat pixels = src(cv::Range(i-1,i+2), cv::Range(j-1,j+2)).clone();
          // X sobel
          cv::Vec3b xcolor = sobel3x3helper(xkern, pixels);
          // Y sobel
          cv::Vec3b ycolor = sobel3x3helper(ykern, pixels);

          cv::Vec3b zcolor;
          zcolor[0] = sqrt(xcolor[0] * xcolor[0] + ycolor[0] * ycolor[0]);
          zcolor[1] = sqrt(xcolor[1] * xcolor[1] + ycolor[1] * ycolor[1]);
          zcolor[2] = sqrt(xcolor[2] * xcolor[2] + ycolor[2] * ycolor[2]);
          dst.at<Vec3b>(i-2,j-2) = zcolor;

      }
  }

  return 0;

}

// Quantize an image based on n-number of levels
int blurQuantize( cv::Mat &src, cv::Mat &dst, int levels ) {

  blur5x5(src, dst);

  float bucket_size = 255/levels;

  for(int i=0; i<dst.rows; i++){
      for(int j=0; j<dst.cols; j++){
          cv::Vec3b intensity = dst.at<cv::Vec3b>(i, j);
          float blue = intensity.val[0];
          blue = ((int)(blue / bucket_size))*bucket_size;
          float green = intensity.val[1];
          green = ((int)(green / bucket_size))*bucket_size;
          float red = intensity.val[2];
          red = ((int)(red / bucket_size))*bucket_size;
          cv::Vec3b color;
          color[2] = red;
          color[1] = green;
          color[0] = blue;
          dst.at<Vec3b>(i,j) = color;
      }
  }

  return 0;
}

int cartoon( cv::Mat &src, cv::Mat&dst, int levels, int magThreshold ) {

  cv::Mat tempMag = cv::Mat(src.rows, src.cols, CV_8UC3);
  cv::Mat tempBlqt = cv::Mat(src.rows, src.cols, CV_8UC3);
  magnitude(src, tempMag);
  blurQuantize(src, tempBlqt, levels);

  for(int i=0; i<tempBlqt.rows; i++){
      for(int j=0; j<tempBlqt.cols; j++){

          cv::Vec3b mag = tempMag.at<cv::Vec3b>(i, j);
          cv::Vec3b qb = tempBlqt.at<cv::Vec3b>(i, j);

          if (mag.val[0] > magThreshold){
            qb.val[0] = 0;
          }
          if (mag.val[1] > magThreshold){
            qb.val[1] = 0;
          }
          if (mag.val[2] > magThreshold){
            qb.val[2] = 0;
          }

          dst.at<Vec3b>(i,j) = qb;
      }
  }

  return 0;

}

// Contrast Changer
int contrast( cv::Mat &src, cv::Mat &dst, double constant ) {

  for(int i=0; i<src.rows; i++){
      for(int j=0; j<src.cols; j++){

          cv::Vec3b rgb = src.at<Vec3b>(i,j);
          rgb[0] = clamp(rgb[0]*constant);
          rgb[1] = clamp(rgb[1]*constant);
          rgb[2] = clamp(rgb[2]*constant);
          dst.at<Vec3b>(i,j) = rgb;

      }
  }

  return 0;

}