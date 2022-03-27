// includes
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <string>
#include <stdio.h>
#include <vector>
#include <cmath>
#include <math.h>
#include <opencv2/calib3d.hpp>
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include <opencv2/calib3d.hpp>

//namespace
using namespace std;
using namespace cv;

//function to find checkerboard corners and draw 3d axis and objects 
int findcorners(Mat &frame){

    //initialize variables
    cv::Size patternsize(9,6);
    std::vector<cv::Point2f> corner_set;
    std::vector<cv::Vec3f> point_set;
    Mat grey;
    cvtColor(frame, grey, CV_RGB2GRAY); 

    //try to find checkerboard corners 
    bool patternfound = findChessboardCorners(
        grey, 
        patternsize, 
        corner_set
    );

    //only run if checkboard corners found 
    if (patternfound){

        //append to local 1d parent lists of corners and points
        cornerSubPix(
            grey,
            corner_set,
            cv::Size(11, 11),
            cv::Size(-1, -1),
            cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1)
            );

        //append to local 1d sub list 
        int counter = 0;
        for(auto itr : corner_set){
            point_set.push_back(cv::Vec3f(counter%9, -1*floor(counter/9), 0));
            counter+=1;
        }

        //intrinsic camera paramters
        float cam_data[9] = { 
            881.9317380618082, 
            0, 
            529.3922351799072,
            0,
            791.1121212766294,
            252.2271817419502,
            0,
            0,
            1
        };
        //add cam_parameters to cv:Mat
        cv::Mat cameraMatrix = cv::Mat(3, 3, cv::DataType<double>::type, cam_data);
        cv::setIdentity(cameraMatrix);

        //create cv of distortion coefficients
        cv::Mat distCoeffs(5,1,cv::DataType<double>::type);
        distCoeffs.at<double>(0) = 0;
        distCoeffs.at<double>(1) = 0;
        distCoeffs.at<double>(2) = 0;
        distCoeffs.at<double>(3) = 0;
        distCoeffs.at<double>(4) = 0;

        //initialize vectors for rotation and translation 
        cv::Mat rvec(3,1,cv::DataType<double>::type);
        cv::Mat tvec(3,1,cv::DataType<double>::type);

        //solve pnp using intrinsics
        cv::solvePnP(point_set, corner_set, cameraMatrix, distCoeffs, rvec, tvec);

        //reproject all points and calculate reprojection error
        std::vector<cv::Point2f> testError;
        projectPoints(point_set, rvec, tvec, cameraMatrix, distCoeffs, testError);
        float total = 0;
        for (int i = 0; i < corner_set.size(); i++){
            total += sqrt( 
                pow((corner_set[i].x-testError[i].x), 2)+ pow((corner_set[i].y-testError[i].y),2)
             );
        }
        cout << "Avg: RP = " << total/corner_set.size() << endl;

        //print the rotation and translation vectors in real time
        cout << rvec << endl;
        cout << tvec << endl;

        //initialize variables for drawing vectors and objects
        std::vector<cv::Point2f> projectedPoints;
        std::vector<cv::Vec3f> xyz_add_three;

        // draw object (UNCOMMENT AND COMMENT OUT AXIS)
        xyz_add_three.push_back(cv::Vec3f(3,0,0));
        xyz_add_three.push_back(cv::Vec3f(0,0,-0.01));
        xyz_add_three.push_back(cv::Vec3f(0,0,0));
        xyz_add_three.push_back(cv::Vec3f(3,0,-0.01));
        xyz_add_three.push_back(cv::Vec3f(0,0,-0.01));
        xyz_add_three.push_back(cv::Vec3f(3,0,-0.01));
        xyz_add_three.push_back(cv::Vec3f(3,-1,0));
        xyz_add_three.push_back(cv::Vec3f(0,-1,-0.01));
        xyz_add_three.push_back(cv::Vec3f(0,-1,0));
        xyz_add_three.push_back(cv::Vec3f(3,-1,-0.01));
        xyz_add_three.push_back(cv::Vec3f(0,-1,-0.01));
        xyz_add_three.push_back(cv::Vec3f(3,-1,-0.01));

        //draw axis 
        // xyz_add_three.push_back(cv::Vec3f(3,0,0));
        // xyz_add_three.push_back(cv::Vec3f(0,-3,0));
        // xyz_add_three.push_back(cv::Vec3f(0,0,-0.1));

        projectPoints(xyz_add_three, rvec, tvec, cameraMatrix, distCoeffs, projectedPoints);

        line(frame, corner_set[0], projectedPoints[0], Scalar(255,0,0), 5);
        line(frame, corner_set[0], projectedPoints[1], Scalar(0,0,255), 5);
        line(frame, corner_set[3], projectedPoints[2], Scalar(255,0,0), 5);
        line(frame, corner_set[3], projectedPoints[3], Scalar(0,0,255), 5);
        line(frame, projectedPoints[4], projectedPoints[5], Scalar(0,0,255), 5);
        line(frame, corner_set[9], projectedPoints[6], Scalar(255,0,0), 5);
        line(frame, corner_set[9], projectedPoints[7], Scalar(0,0,255), 5);
        line(frame, corner_set[12], projectedPoints[8], Scalar(255,0,0), 5);
        line(frame, corner_set[12], projectedPoints[9], Scalar(0,0,255), 5);
        line(frame, projectedPoints[10], projectedPoints[11], Scalar(0,0,255), 5);

        //draw lines onto frame
        // line(frame, corner_set[0], projectedPoints[0], Scalar(255,0,0), 5);
        // line(frame, corner_set[0], projectedPoints[1], Scalar(0,255,0), 5);
        // line(frame, corner_set[0], projectedPoints[2], Scalar(0,0,255), 5);

        drawChessboardCorners(frame, Size(9,6), corner_set, patternfound);

    }



    //exit
    return 0;

}

//main
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

    //initialize frame and window player
    Mat frame; 
    namedWindow("Video Player");
    VideoCapture cap(path);

    //throw error if video not found
    if (!cap.isOpened()){ 
        cout << "No video stream detected" << endl;
        system("pause");
        return-1;
    }

    //iterate frames
    while (true){

        cap >> frame;

        //exit when video complete
        if (frame.empty()){
            break;
        }

        //resize image due to weird iphone meta flags
        cv::Mat resized;
        cv::resize(frame, resized, cv::Size(1000, 500));

        //find corners in each image and draw 
        findcorners(resized);

        imwrite("img.jpg", resized);

        //show augmented frame
        cout << " showing frame " << endl;
        imshow("Video Player", resized);
        char c = (char)waitKey(25);

        //press 'q' to break
        if (c == 113){ 
            break;
        }

    }

    //exit
    cap.release();
    return 0;
}