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

// XY Magnitude Filter
cv::Mat xyMag(cv::Mat image) {


  cv::Mat xkern = (cv::Mat_<char>(3, 3) << -1, 0, 1, -1, 0, 1, -1, 0, 1);
  cv::Mat ykern = (cv::Mat_<char>(3, 3) << -1, -1, -1, 0, 0, 0, 1, 1, 1);
  
  cv::Mat blurimg = cv::Mat(image.rows, image.cols, 16);
  xkern.convertTo(xkern, 19);
  ykern.convertTo(ykern, 19);
  image.convertTo(image, 16);

  int border = 2;
  cv::Mat bigimg;
  cv::copyMakeBorder(image, bigimg, border, border, border, border, BORDER_REPLICATE);

  for(int i=2; i<image.rows-2; i++){
      for(int j=2; j<image.cols-2; j++){
          // Get pixels
          cv::Mat pixels = image(cv::Range(i-1,i+2), cv::Range(j-1,j+2)).clone();

          // X sobel
          cv::Mat xchannels[3];
          cv::split(pixels, xchannels);

          cv::Mat xred;
          cv::Mat xgreen;
          cv::Mat xblue;
          xchannels[0].convertTo(xred, xkern.type());
          xchannels[1].convertTo(xgreen, xkern.type());
          xchannels[2].convertTo(xblue, xkern.type());

          xred = xkern.mul(xred);
          xgreen = xkern.mul(xgreen);
          xblue = xkern.mul(xblue);

          double xred_val = cv::sum(xred)[0] / 3;
          double xgreen_val = cv::sum(xgreen)[0] / 3;
          double xblue_val = cv::sum(xblue)[0] / 3;

          if (xred_val < 0) {
            xred_val = 0;
          }
          else if (xred_val > 255) {
            xred_val = 255;
          }
          if (xgreen_val < 0) {
            xgreen_val = 0;
          }
          else if (xgreen_val > 255) {
            xgreen_val = 255;
          }
          if (xblue_val < 0) {
            xblue_val = 0;
          }
          else if (xblue_val > 255) {
            xblue_val = 255;
          }

          cv::Vec3b xcolor;
          xcolor[0] = xred_val;
          xcolor[1] = xgreen_val;
          xcolor[2] = xblue_val;


          // Y sobel
          cv::Mat ychannels[3];
          cv::split(pixels, ychannels);

          cv::Mat yred;
          cv::Mat ygreen;
          cv::Mat yblue;
          ychannels[0].convertTo(yred, ykern.type());
          ychannels[1].convertTo(ygreen, ykern.type());
          ychannels[2].convertTo(yblue, ykern.type());

          yred = ykern.mul(yred);
          ygreen = ykern.mul(ygreen);
          yblue = ykern.mul(yblue);

          double yred_val = cv::sum(yred)[0] / 3;
          double ygreen_val = cv::sum(ygreen)[0] / 3;
          double yblue_val = cv::sum(yblue)[0] / 3;

          if (yred_val < 0) {
            yred_val = 0;
          }
          else if (yred_val > 255) {
            yred_val = 255;
          }
          if (ygreen_val < 0) {
            ygreen_val = 0;
          }
          else if (ygreen_val > 255) {
            ygreen_val = 255;
          }
          if (yblue_val < 0) {
            yblue_val = 0;
          }
          else if (yblue_val > 255) {
            yblue_val = 255;
          }

          cv::Vec3b ycolor;
          ycolor[0] = yred_val;
          ycolor[1] = ygreen_val;
          ycolor[2] = yblue_val;

          cv::Vec3b mcolor;
          mcolor[0] = sqrt(xred_val * xred_val + yred_val * yred_val);
          mcolor[1] = sqrt(xgreen_val * xgreen_val + ygreen_val * ygreen_val);
          mcolor[2] = sqrt(xblue_val * xblue_val + yblue_val * yblue_val);
          blurimg.at<Vec3b>(i-2,j-2) = mcolor;

      }
  }

  return blurimg;

}

// Quantize an image based on n-number of levels
cv::Mat quantScale(cv::Mat image, int levels) {

  cv::Mat quantImg(image.rows, image.cols, CV_8UC3);

  float bucket_size = 255/levels;

  for(int i=0; i<image.rows; i++){
      for(int j=0; j<image.cols; j++){
          cv::Vec3b intensity = image.at<cv::Vec3b>(i, j);
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
          quantImg.at<Vec3b>(i,j) = color;
      }
  }

  return quantImg;
}

cv::Mat cartoon(cv::Mat image, int levels, int threshold) {

  cv::Mat magnitude = xyMag(image);
  cv::Mat quantblur = quantScale(image, levels);

  for(int i=0; i<quantblur.rows; i++){
      for(int j=0; j<quantblur.cols; j++){

          cv::Vec3b mag = magnitude.at<cv::Vec3b>(i, j);
          cv::Vec3b qb = quantblur.at<cv::Vec3b>(i, j);

          float mag_blue = mag.val[0];
          float mag_green = mag.val[1];
          float mag_red = mag.val[2];

          float qb_blue = qb.val[0];
          float qb_green = qb.val[1];
          float qb_red = qb.val[2];

          if (mag_blue > threshold){
            qb_blue = 0;
          }
          if (mag_green > threshold){
            qb_green = 0;
          }
          if (mag_red > threshold){
            qb_red = 0;
          }

          cv::Vec3b color;
          color[2] = qb_red;
          color[1] = qb_green;
          color[0] = qb_blue;
          quantblur.at<Vec3b>(i,j) = color;
      }
  }

  return quantblur;

}
