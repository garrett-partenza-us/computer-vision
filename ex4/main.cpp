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

    std::cout << path << std::endl;
    
    cv::Mat img = cv::imread(path, cv::IMREAD_GRAYSCALE);

    cv::Size patternsize(9,6);

    std::vector<cv::Point2f> corner_set;
    std::vector<cv::Vec3f> point_set;
	std::vector<std::vector<cv::Vec3f> > point_list;
	std::vector<std::vector<cv::Point2f> > corner_list;

    bool patternfound = findChessboardCorners(
        img, 
        patternsize, 
        corner_set
    );

    if (patternfound){
        printf("FOUND\n");
        cornerSubPix(
            img,
            corner_set,
            cv::Size(11, 11),
            cv::Size(-1, -1),
            cv::TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1)
            );
    }

    drawChessboardCorners(img, patternsize, cv::Mat(corner_set), patternfound);

    int counter = 0;
    for(auto itr : corner_set){
        point_set.push_back(cv::Vec3f(counter%9, -1*floor(counter/9), 0));
        counter+=1;
    }

    point_list.push_back(point_set);
    corner_list.push_back(corner_set);

    cv::imwrite("out.png", img);

}