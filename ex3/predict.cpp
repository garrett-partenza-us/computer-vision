
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "functions.h"
#include <iostream>
#include <set>

using namespace cv;
using namespace std;


int main(){
    // Declare cv::Mats
    Mat src, src_gray, src_threshold, src_dialate, src_components, src_stats, src_centoids, src_features_0, src_features_1;

    // Create window
    const char* window_name = "Threshold Demo";
    namedWindow( window_name, WINDOW_AUTOSIZE );

    // Read image
    src = imread( "apple.jpg" , IMREAD_COLOR );

    // Convert to grayscale    
    cvtColor( src, src_gray, COLOR_BGR2GRAY );

    // PART ONE

    // Threshold    
    threshold( src_gray, src_threshold, 200, 255, THRESH_BINARY );

    // PART TWO

    // Morphological operator
    erode(src_threshold, src_dialate, Mat(), Point(-1, -1), 3);

    // PART THREE

    connectedComponentsWithStats(src_threshold, src_components, src_stats, src_centoids, 4, CV_32S);

    // Draw bounding boxes for detected components

    // First largest
    int horizontal_1 = src_stats.at<int>(0,2);
    int vertical_1 = src_stats.at<int>(0,3);
    double x_1 = src_centoids.at<double>(0,0);
    double y_1 = src_centoids.at<double>(0,1);
    cv::Point pt11( x_1 - horizontal_1 / 2, y_1 - vertical_1 / 2 );
    cv::Point pt21( x_1 + horizontal_1 / 2, y_1 + vertical_1 / 2 );
    rectangle(src_threshold, pt11, pt21, Scalar( 0, 255, 255 ));

    // Second Largest
    int horizontal_2 = src_stats.at<int>(1,2);
    int vertical_2 = src_stats.at<int>(1,3);
    double x_2 = src_centoids.at<double>(1,0);
    double y_2 = src_centoids.at<double>(1,1);
    cv::Point pt12( x_2 - horizontal_2 / 2, y_2 - vertical_2 / 2 );
    cv::Point pt22( x_2 + horizontal_2 / 2, y_2 + vertical_2 / 2 );
    rectangle(src_threshold, pt12, pt22, Scalar( 0, 255, 255 ));

    // PART FOUR

    features(src, src_threshold, src_components, 0, src_features_0);
    features(src, src_threshold, src_components, 1, src_features_1);

    // Display image
    imwrite( "result.jpg", src_threshold );
}