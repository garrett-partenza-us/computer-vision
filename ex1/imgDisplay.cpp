#include <opencv2/highgui.hpp> 
#include <opencv2/core/hal/interface.h>
#include <iostream>
#include <string>
#include "filter.h"


using namespace cv;
using namespace std;


int main( int argc, char** argv ) {
  
  cv::Mat image;
  image = cv::imread("image.jpg", IMREAD_UNCHANGED);
  
  if(! image.data ) {
      std::cout <<  "Image does not exist" << std::endl ;
      return -1;
    }

  cv::Mat greyimg(image.rows, image.cols, CV_8UC1);
  greyscale(image, greyimg);
  // cv::Mat blurimg = cv::Mat(image.rows, image.cols, image.type());
  // blur5x5(image, blurimg);
  // cv::Mat xSobel = cv::Mat(image.rows, image.cols, CV_8UC3);
  // sobelX3x3(image, xSobel);
  // cv::Mat ySobel = cv::Mat(image.rows, image.cols, CV_8UC3);
  // sobelY3x3(image, ySobel);
  // cv::Mat magImg = cv::Mat(image.rows, image.cols, CV_8UC3);
  // magnitude(image, magImg);
  cv::Mat blurQuantImg = cv::Mat(image.rows, image.cols, CV_8UC3);
  int levels = 10;
  blurQuantize( image, blurQuantImg, levels );
  //int levels = 10;
  //int threshold = 20;
  //cv::Mat cartoonImg = cv::Mat(image.rows, image.cols, CV_8UC3);
  //cartoon( image, cartoonImg, levels, threshold );
  
  //cvtColor(image, image, CV_RGB2GRAY);
  cv::namedWindow( "Image Display", cv::WINDOW_AUTOSIZE );
  int l = 10;
  int t = 20;
  double contrastCoef = 0.5;
  //cv::Mat contrastImg = cv::Mat(image.rows, image.cols, CV_8UC3);
  //contrast(image, contrastImg, contrastCoef);
  cv::imshow( "Image", greyimg);
  
  cv::waitKey(0);
  return 0;
}
