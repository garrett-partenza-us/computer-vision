#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <set>

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
    Mat huMoments;
    HuMoments(mom, huMoments);
    cout << huMoments << endl;

    return 0;
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