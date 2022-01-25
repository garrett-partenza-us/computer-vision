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
                                          1, 1, 1, 1, 1,
                                          1, 1, 1, 1, 1,
                                          1, 1, 1, 1, 1,
                                          1, 1, 1, 1, 1);
  
  cv::Mat blurimg = cv::Mat(image.rows - 4, image.cols - 4, image.type());
  kern.convertTo(kern, blurimg.type());

  cout<<image.rows<<" "<<image.cols<<endl;
  cout<<blurimg.rows<<" "<<blurimg.cols<<endl;
  cout<<image.type()<<endl;
  cout<<blurimg.type()<<endl;

  for(int i=2; i<image.rows-2; i++){
      for(int j=2; j<image.cols-2; j++){
          cv::Mat pixels = image(cv::Range(i-2,i+3), cv::Range(j-2,j+3)).clone();
          //blurimg.at<Vec3b>(i-2,j-2) = image.at<Vec3b>(i,j);

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

          double red_val = cv::sum(red)[0] / (25);
          double green_val = cv::sum(green)[0] / (25);
          double blue_val = cv::sum(blue)[0] / (25);

          cv::Vec3b color;
          color[0] = red_val;
          color[1] = green_val;
          color[2] = blue_val;
          blurimg.at<Vec3b>(i-2,j-2) = color;
      }
  }
  return blurimg;
}

 int restart(cv::Mat image) {

  cv::Mat kern = (cv::Mat_<char>(5, 5) << 1, 1, 1, 1, 1,
                                          1, 1, 1, 1, 1,
                                          1, 1, 1, 1, 1,
                                          1, 1, 1, 1, 1,
                                          1, 1, 1, 1, 1);
  kern.convertTo(kern, 0);

  Mat leftcorner = image(cv::Range(0,5), cv::Range(0,5));
  cout<<"Original pixels:"<<endl;
  cout<<leftcorner<<endl;

  cv::Mat channels[3];
  cv::split(leftcorner, channels);
  cout<<kern.type()<<endl;
  cout<<channels[0].type()<<endl;
  cv::Mat red;
  cv::Mat green;
  cv::Mat blue;
  channels[0].convertTo(red, kern.type());
  channels[1].convertTo(green, kern.type());
  channels[2].convertTo(blue, kern.type());
  cout<<"RGB channels:"<<endl;
  cout<<channels[0]<<endl;
  cout<<channels[1]<<endl;
  cout<<channels[2]<<endl;

  red = kern.mul(red);
  green = kern.mul(green);
  blue = kern.mul(blue);

  cout<<"Post kern.mul red channel:"<<endl;
  cout<<red<<endl;
 
  double red_val = cv::sum(red)[0] / (25);
  double green_val = cv::sum(green)[0] / (25);
  double blue_val = cv::sum(blue)[0] / (25);

  cout<<"New RGB for center pixel:"<<endl;
  cout<<red_val<<", "<<green_val<<", "<<blue_val<<endl;  

  cv::Vec3b color;
  color[0] = red_val;
  color[1] = green_val;
  color[2] = blue_val;
  cout<<"New RGB vector"<<endl;
  cout<<color<<endl;


  image.at<Vec3b>(Point(2,2)) = color;
  cout<<"New written point"<<endl;
  cout<<image.at<Vec3b>(Point(2,2))<<endl;

  cout<<"New pixels:"<<endl;
  cout<<image(cv::Range(0,5), cv::Range(0,5))<<endl;
  return 0;

}