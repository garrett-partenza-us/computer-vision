#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <set>
#include <string>


using namespace cv;
using namespace std;

// Input: RBG image, component region map, label id
// Output: Feature vector for region of component
int features( Mat &src, Mat &thresh, Mat &map, int label, Mat &dst){
    Mat masked = src.clone();
    for(int i=0; i<src.rows; i++){
        for(int j=0; j<src.cols; j++){
            if ( map.at<int>(i,j) != label ){
                masked.at<Vec3b>(i,j)[0]=0; 
                masked.at<Vec3b>(i,j)[1]=0;
                masked.at<Vec3b>(i,j)[2]=0;
            }
        }
    }

    Moments mom = moments(thresh, false);
    HuMoments(mom, dst);

    return 0;
}

int objects( string &str_buffer, Mat &src_threshold, Mat &src, Mat &src_components, Mat &src_stats, Mat &src_centoid ){

    // Declare cv::Mats
    Mat src_gray, src_dialate, src_centoids;

    // Read image
    src = imread( str_buffer , IMREAD_COLOR );

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

}

// DEAD CODE

    // vector<Mat> bgr_planes;
    // split( masked, bgr_planes );
    // int histSize = 256;
    // float range[] = { 0, 256 };
    // const float* histRange[] = { range };
    // bool uniform = true, accumulate = false;
    // Mat b_hist, g_hist, r_hist;
    // calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, histRange, uniform, accumulate );
    // calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, histRange, uniform, accumulate );
    // calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, histRange, uniform, accumulate );
    // vconcat(b_hist, g_hist, dst);
    // vconcat(dst, r_hist, dst);