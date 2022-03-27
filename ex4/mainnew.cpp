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

int findcorners(Mat &frame, std::vector<std::vector<cv::Point2f>> &corner_list, std::vector<std::vector<cv::Vec3f>> &point_list){

    cv::Size patternsize(9,6);
    std::vector<cv::Point2f> corner_set;
    std::vector<cv::Vec3f> point_set;

    Mat grey;
    cvtColor(frame, grey, CV_RGB2GRAY); 

    bool patternfound = findChessboardCorners(
        grey, 
        patternsize, 
        corner_set
    );

    if (patternfound){
        cornerSubPix(
            grey,
            corner_set,
            cv::Size(11, 11),
            cv::Size(-1, -1),
            cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1)
            );
    }

    int counter = 0;
    for(auto itr : corner_set){
        point_set.push_back(cv::Vec3f(counter%9, -1*floor(counter/9), 0));
        counter+=1;
    }

    point_list.push_back(point_set);
    corner_list.push_back(corner_set);

    return 0;

}

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

    std::vector<std::vector<cv::Vec3f>> point_list;
	std::vector<std::vector<cv::Point2f>> corner_list;
    cv::Mat cameraMatrix = cv::Mat(3, 3, CV_64FC1);
    cv::Mat distCoeffs;
    std::vector<cv::Mat> rvecs;
    std::vector<cv::Mat> tvecs;
    std::vector<cv::Mat> stdInt;
    std::vector<cv::Mat> stdExt;
    std::vector<cv::Mat> error;

    
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

        imshow("Video Player", frame);
        char c = (char)waitKey(25);
        if (c == 113){ 
            break;
        }
        else if (c==112){
            cout << "finding corners..." << endl;
            findcorners(frame, corner_list, point_list);
        }
        else if (c==115){
            if (point_list.size() < 2){
                cout << "not enough points..." << endl;
            }
            else{
                cout << "callibrating camera..." << endl;
                calibrateCamera(
                    point_list, 
                    corner_list, 
                    frame.size(),
                    cameraMatrix, 
                    distCoeffs, 
                    rvecs, 
                    tvecs
                );

                cout << cameraMatrix << endl;
                cout << distCoeffs << endl;

            }
        }


    }

    cap.release();
    return 0;
}