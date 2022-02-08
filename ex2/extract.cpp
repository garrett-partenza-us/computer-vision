#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>  
#include <dirent.h>
#include <map>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

int bin( int pixel ){
    if (pixel<=63){
        return 0;
    }
    else if (pixel<=127){
        return 1;
    }
    else if (pixel<=191){
        return 2;
    }
    else if (pixel<=255){
        return 3;
    }
}

int baseline( cv::Mat &src, cv::Mat &dst ) {
    cv::Mat pixels = src(cv::Range((src.rows/2)-4,(src.rows/2)+5), cv::Range((src.cols/2)-4,(src.cols/2)+5));
    cv::Mat channels[3];
    cv::split(pixels, channels);
    cv::hconcat(channels[0].reshape(9*9), channels[1].reshape(9*9), dst);
    cv::hconcat(dst, channels[2].reshape(9*9), dst);
    dst = dst.reshape(1);
    return 0;
}

int histogram( cv::Mat &src, cv::Mat &dst ) {
    // generate image feature vector using histogram method
    std::map<std::string, int> histogram;
    for(int i=0;i<4;i++){
      for(int j=0;j<4;j++){
        for(int z=0;z<4;z++){
          histogram[to_string(i)+to_string(j)+to_string(z)]=0;
        }
      }
    }
    // Bin 0 --> 0-63
    // Bin 1 --> 64-127
    // Bin 2 --> 128-191 
    // Bin 3 --> 192-255
    for(int i=0;i<src.rows;i++){
      for(int j=0;j<src.cols;j++){
          cv::Vec3b RGB = src.at<cv::Vec3b>(i,j);
          histogram[std::to_string(bin(RGB[0]))+std::to_string(bin(RGB[1]))+std::to_string(bin(RGB[2]))]++;
      }
    }

    int counter = 0;
    for(const auto& elem : histogram){
        dst.at<float>(0,counter) = elem.second;
        counter++;
    }

    return 0;
}