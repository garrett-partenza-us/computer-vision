// includes
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <cmath>
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {

    //path to image
    char path[256];

    //check for sufficient arguments
    if( argc < 1) {
        printf("Not enough arguments\n");
        exit(-1);
    }

    //copy argument to path
    strcpy(path, argv[1]);

    Mat frame; 
    namedWindow("Video Player");
    VideoCapture cap(path);

    if (!cap.isOpened()){ 
        cout << "No video stream detected" << endl;
        system("pause");
        return-1;
    }

    while (true){

        cap >> frame;

        if (frame.empty()){
            break;
        }

        cv::Mat resized;
        cv::resize(frame, resized, cv::Size(1000, 500));

        Mat grey;
        cvtColor(resized, grey, CV_RGB2GRAY);

        Mat dst, dst_norm, dst_norm_scaled;
        dst = Mat::zeros( grey.size(), CV_32FC1 );

        cornerHarris(grey, dst, 2, 3, 0.04);
        normalize( dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat() );
        convertScaleAbs( dst_norm, dst_norm_scaled ); 

        imshow("Video Player", dst_norm_scaled);
        char c = (char)waitKey(25);
        if (c == 113){ 
            break;
        }

    }

    cap.release();
    return 0;
}