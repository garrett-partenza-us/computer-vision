
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(){
    // Declare cv::Mats
    Mat src, src_gray, src_threshold, src_dialate;

    // Create window
    const char* window_name = "Threshold Demo";
    namedWindow( window_name, WINDOW_AUTOSIZE );

    // Read image
    src = imread( "apple.jpg" , IMREAD_COLOR );

    // Convert to grayscale    
    cvtColor( src, src_gray, COLOR_BGR2GRAY );

    // Threshold    
    threshold( src_gray, src_threshold, 200, 255, THRESH_BINARY );

    // Morphological operator
    erode(src_threshold, src_dialate, Mat(), Point(-1, -1), 3);

    // Display image
    imshow( window_name, src_dialate );
    waitKey();  
}