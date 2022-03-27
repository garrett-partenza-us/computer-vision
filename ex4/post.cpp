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

int findcorners(Mat &frame){
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

        int counter = 0;
        for(auto itr : corner_set){
            point_set.push_back(cv::Vec3f(counter%9, -1*floor(counter/9), 0));
            counter+=1;
        }

        float cam_data[9] = { 
            1716.022949165638, 
            0, 
            1000.086112540036,
            0, 
            1729.734760723105, 
            532.3212318276209,
            0, 
            0, 
            1
        };
        cv::Mat cameraMatrix = cv::Mat(3, 3, cv::DataType<double>::type, cam_data);
        cv::setIdentity(cameraMatrix);

        cv::Mat distCoeffs(5,1,cv::DataType<double>::type);
        distCoeffs.at<double>(0) = 0.0;
        distCoeffs.at<double>(1) = 0.0;
        distCoeffs.at<double>(2) = 0.0;
        distCoeffs.at<double>(3) = 0.0;
        distCoeffs.at<double>(4) = 0.0;

        cv::Mat rvec(3,1,cv::DataType<double>::type);
        cv::Mat tvec(3,1,cv::DataType<double>::type);

        cv::solvePnP(point_set, corner_set, cameraMatrix, distCoeffs, rvec, tvec);

        cout << rvec << endl;
        cout << tvec << endl;

        std::vector<cv::Point2f> projectedPoints;
        std::vector<cv::Vec3f> xyz_add_three;

        // xyz_add_three.push_back(cv::Vec3f(3,0,0));
        // xyz_add_three.push_back(cv::Vec3f(0,0,-0.01));
        // xyz_add_three.push_back(cv::Vec3f(0,0,0));
        // xyz_add_three.push_back(cv::Vec3f(3,0,-0.01));
        // xyz_add_three.push_back(cv::Vec3f(0,0,-0.01));
        // xyz_add_three.push_back(cv::Vec3f(3,0,-0.01));
        // xyz_add_three.push_back(cv::Vec3f(3,-1,0));
        // xyz_add_three.push_back(cv::Vec3f(0,-1,-0.01));
        // xyz_add_three.push_back(cv::Vec3f(0,-1,0));
        // xyz_add_three.push_back(cv::Vec3f(3,-1,-0.01));
        // xyz_add_three.push_back(cv::Vec3f(0,-1,-0.01));
        // xyz_add_three.push_back(cv::Vec3f(3,-1,-0.01));

        xyz_add_three.push_back(cv::Vec3f(3,0,0));
        xyz_add_three.push_back(cv::Vec3f(0,-3,0));
        xyz_add_three.push_back(cv::Vec3f(0,0,-0.1));

        
        projectPoints(xyz_add_three, rvec, tvec, cameraMatrix, distCoeffs, projectedPoints);

        // line(frame, corner_set[0], projectedPoints[0], Scalar(255,0,0), 5);
        // line(frame, corner_set[0], projectedPoints[1], Scalar(0,0,255), 5);
        // line(frame, corner_set[3], projectedPoints[2], Scalar(255,0,0), 5);
        // line(frame, corner_set[3], projectedPoints[3], Scalar(0,0,255), 5);
        // line(frame, projectedPoints[4], projectedPoints[5], Scalar(0,0,255), 5);
        // line(frame, corner_set[9], projectedPoints[6], Scalar(255,0,0), 5);
        // line(frame, corner_set[9], projectedPoints[7], Scalar(0,0,255), 5);
        // line(frame, corner_set[12], projectedPoints[8], Scalar(255,0,0), 5);
        // line(frame, corner_set[12], projectedPoints[9], Scalar(0,0,255), 5);
        // line(frame, projectedPoints[10], projectedPoints[11], Scalar(0,0,255), 5);

        line(frame, corner_set[0], projectedPoints[0], Scalar(255,0,0), 5);
        line(frame, corner_set[0], projectedPoints[1], Scalar(0,255,0), 5);
        line(frame, corner_set[0], projectedPoints[2], Scalar(0,0,255), 5);

        cout << " DONE " << endl;

    }


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


        findcorners(resized);

        imshow("Video Player", resized);
        char c = (char)waitKey(25);
        if (c == 113){ 
            break;
        }

    }

    cap.release();
    return 0;
}