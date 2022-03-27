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

//namespaces
using namespace std;
using namespace cv;

//function to find checkerboard corners in a given image 
int findcorners(Mat &frame, std::vector<std::vector<cv::Point2f>> &corner_list, std::vector<std::vector<cv::Vec3f>> &point_list){

    //initialize variables
    cv::Size patternsize(9,6);
    std::vector<cv::Point2f> corner_set;
    std::vector<cv::Vec3f> point_set;
    Mat grey;
    cvtColor(frame, grey, CV_RGB2GRAY);
    int counter = 0; 

    //attempt to find checkerboard corners
    bool patternfound = findChessboardCorners(
        grey, 
        patternsize, 
        corner_set
    );

    //append corners and points to main list if found
    if (patternfound){
        cornerSubPix(
            grey,
            corner_set,
            cv::Size(11, 11),
            cv::Size(-1, -1),
            cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1)
            );

            for(auto itr : corner_set){
                point_set.push_back(cv::Vec3f(counter%9, -1*floor(counter/9), 0));
                counter+=1;
            }

            //push the newly found points and corners back to the 2D parent list
            point_list.push_back(point_set);
            corner_list.push_back(corner_set);
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

    //initialize frame and player window
    Mat frame; 
    namedWindow("Video Player");
    VideoCapture cap(path);

    //2D parent lists to keep track of sets of points and corners
    std::vector<std::vector<cv::Vec3f>> point_list;
	std::vector<std::vector<cv::Point2f>> corner_list;

    //initialize variables for calibrating camera
    cv::Mat cameraMatrix = cv::Mat(3, 3, CV_64FC1);
    cv::Mat distCoeffs;
    std::vector<cv::Mat> rvecs;
    std::vector<cv::Mat> tvecs;
    std::vector<cv::Mat> stdInt;
    std::vector<cv::Mat> stdExt;
    std::vector<cv::Mat> error;

    //return error if cap cant be opened
    if (!cap.isOpened()){ 
        cout << "No video stream detected" << endl;
        system("pause");
        return-1;
    }
    
    //iterate frames
    while (true){

        cap >> frame;

        //break when video is finished
        if (frame.empty()){
            break;
        }

        //video was taken with iphone, resize due to weird meta flags
        cv::Mat resized;
        cv::resize(frame, resized, cv::Size(1000, 500));

        //show frame
        imshow("Video Player", resized);

        //monitor user keys
        // q = quit 
        // p = find corners
        // s = calibrate camera
        char c = (char)waitKey(25);
        //quit
        if (c == 113){ 
            break;
        }
        //find corners
        else if (c==112){
            cout << "finding corners..." << endl;
            findcorners(resized, corner_list, point_list);
        }
        //calibrate camera
        else if (c==115){
            //only calibrate if more than one set of points/corners has been generated
            if (point_list.size() < 2){
                cout << "not enough points..." << endl;
            }
            //calibrate happens here
            else{
                cout << "callibrating camera..." << endl;
                //run calibration function with 2d point and corners list
                calibrateCamera(
                    point_list, 
                    corner_list, 
                    resized.size(),
                    cameraMatrix, 
                    distCoeffs, 
                    rvecs, 
                    tvecs
                );

                //print K and distCoeffs when done
                cout << cameraMatrix << endl;
                cout << distCoeffs << endl;

            }
        }


    }

    //exit
    cap.release();
    return 0;
}