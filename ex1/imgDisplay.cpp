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
  cv::namedWindow( "Image Display", cv::WINDOW_AUTOSIZE );
  cv::imshow( "Image", ySobel(image));
  
  cv::waitKey(0);
  return 0;
}