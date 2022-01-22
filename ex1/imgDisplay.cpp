#include <opencv2/highgui.hpp> 
#include <iostream>
 
int main( int argc, char** argv ) {
  
  cv::Mat image;
  image = cv::imread("imgDisplay.jpg" ,cv::IMREAD_COLOR);
  
  if(! image.data ) {
      std::cout <<  "Image does not exist" << std::endl ;
      return -1;
    }
  
  cv::namedWindow( "Image Display", cv::WINDOW_AUTOSIZE );
  cv::imshow( "Image", image );
  
  cv::waitKey(0);
  return 0;
}