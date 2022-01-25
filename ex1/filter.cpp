#include <opencv2/highgui.hpp> 
#include <opencv2/core/hal/interface.h>
#include "filter.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/core.hpp>



using namespace cv;
using namespace std;

// Alternate greyscale image
cv::Mat altGrey(cv::Mat image) {

  cv::Mat greyimg(image.rows, image.cols, CV_8UC1);

  for(int i=0; i<image.rows; i++){
      for(int j=0; j<image.cols; j++){
          cv::Vec3b intensity = image.at<cv::Vec3b>(i, j);
          float blue = intensity.val[0];
          float green = intensity.val[1];
          float red = intensity.val[2];
          float ntsc = 0.21 * red + 0.72 * green + 0.07 * blue;
          greyimg.at<char>(i,j) = ntsc;
      }
  }

  return greyimg;
}

// 5x5 Gaussian Filter
cv::Mat blurImg(cv::Mat image) {

  cv::Mat kern = (cv::Mat_<char>(5, 5) << 1, 1, 1, 1, 1,
                                          1, 2, 2, 2, 1,
                                          1, 2, 4, 2, 1,
                                          1, 2, 2, 2, 1,
                                          1, 1, 1, 1, 1);
  
  cv::Mat blurimg = cv::Mat(image.rows, image.cols, image.type());
  kern.convertTo(kern, blurimg.type());

  int border = 2;
  cv::Mat bigimg;
  cv::copyMakeBorder(image, bigimg, border, border, border, border, BORDER_REPLICATE);

  for(int i=2; i<image.rows-2; i++){
      for(int j=2; j<image.cols-2; j++){
          cv::Mat pixels = image(cv::Range(i-2,i+3), cv::Range(j-2,j+3)).clone();

          cv::Mat channels[3];
          cv::split(pixels, channels);

          cv::Mat red;
          cv::Mat green;
          cv::Mat blue;
          channels[0].convertTo(red, kern.type());
          channels[1].convertTo(green, kern.type());
          channels[2].convertTo(blue, kern.type());

          red = kern.mul(red);
          green = kern.mul(green);
          blue = kern.mul(blue);

          double red_val = cv::sum(red)[0] / cv::sum(kern)[0];
          double green_val = cv::sum(green)[0] / cv::sum(kern)[0];
          double blue_val = cv::sum(blue)[0] / cv::sum(kern)[0];

          cv::Vec3b color;
          color[0] = red_val;
          color[1] = green_val;
          color[2] = blue_val;
          blurimg.at<Vec3b>(i-2,j-2) = color;

      }
  }

  return blurimg;

}

// X Sobel Filter
cv::Mat xSobel(cv::Mat image) {


  cv::Mat kern = (cv::Mat_<char>(3, 3) << -1, 0, 1, -1, 0, 1, -1, 0, 1);
  
  cv::Mat blurimg = cv::Mat(image.rows, image.cols, 16);
  kern.convertTo(kern, 19);
  image.convertTo(image, 16);

  int border = 2;
  cv::Mat bigimg;
  cv::copyMakeBorder(image, bigimg, border, border, border, border, BORDER_REPLICATE);

  for(int i=2; i<image.rows-2; i++){
      for(int j=2; j<image.cols-2; j++){
          cv::Mat pixels = image(cv::Range(i-1,i+2), cv::Range(j-1,j+2)).clone();

          cv::Mat channels[3];
          cv::split(pixels, channels);

          cv::Mat red;
          cv::Mat green;
          cv::Mat blue;
          channels[0].convertTo(red, kern.type());
          channels[1].convertTo(green, kern.type());
          channels[2].convertTo(blue, kern.type());

          red = kern.mul(red);
          green = kern.mul(green);
          blue = kern.mul(blue);

          double red_val = cv::sum(red)[0] / 3;
          double green_val = cv::sum(green)[0] / 3;
          double blue_val = cv::sum(blue)[0] / 3;

          if (red_val < 0) {
            red_val = 0;
          }
          else if (red_val > 255) {
            red_val = 255;
          }
          if (green_val < 0) {
            green_val = 0;
          }
          else if (green_val > 255) {
            green_val = 255;
          }
          if (blue_val < 0) {
            blue_val = 0;
          }
          else if (blue_val > 255) {
            blue_val = 255;
          }
          

          cv::Vec3b color;
          color[0] = red_val;
          color[1] = green_val;
          color[2] = blue_val;
          blurimg.at<Vec3b>(i-2,j-2) = color;
      }
  }

  return blurimg;

}

// Y Sobel Filter
cv::Mat ySobel(cv::Mat image) {


  cv::Mat kern = (cv::Mat_<char>(3, 3) << -1, -1, -1, 0, 0, 0, 1, 1, 1);
  
  cv::Mat blurimg = cv::Mat(image.rows, image.cols, 16);
  kern.convertTo(kern, 19);
  image.convertTo(image, 16);

  int border = 2;
  cv::Mat bigimg;
  cv::copyMakeBorder(image, bigimg, border, border, border, border, BORDER_REPLICATE);

  for(int i=2; i<image.rows-2; i++){
      for(int j=2; j<image.cols-2; j++){
          cv::Mat pixels = image(cv::Range(i-1,i+2), cv::Range(j-1,j+2)).clone();

          cv::Mat channels[3];
          cv::split(pixels, channels);

          cv::Mat red;
          cv::Mat green;
          cv::Mat blue;
          channels[0].convertTo(red, kern.type());
          channels[1].convertTo(green, kern.type());
          channels[2].convertTo(blue, kern.type());

          red = kern.mul(red);
          green = kern.mul(green);
          blue = kern.mul(blue);

          double red_val = cv::sum(red)[0] / 3;
          double green_val = cv::sum(green)[0] / 3;
          double blue_val = cv::sum(blue)[0] / 3;

          if (red_val < 0) {
            red_val = 0;
          }
          else if (red_val > 255) {
            red_val = 255;
          }
          if (green_val < 0) {
            green_val = 0;
          }
          else if (green_val > 255) {
            green_val = 255;
          }
          if (blue_val < 0) {
            blue_val = 0;
          }
          else if (blue_val > 255) {
            blue_val = 255;
          }

          cv::Vec3b color;
          color[0] = red_val;
          color[1] = green_val;
          color[2] = blue_val;
          blurimg.at<Vec3b>(i-2,j-2) = color;
      }
  }

  return blurimg;

}


