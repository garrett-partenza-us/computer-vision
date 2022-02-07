#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>  
#include <dirent.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

int baseline( cv::Mat &src, cv::Mat &dst ) {
    cv::Mat pixels = src(cv::Range((src.rows/2)-4,(src.rows/2)+5), cv::Range((src.cols/2)-4,(src.cols/2)+5));
    cv::Mat channels[3];
    cv::split(pixels, channels);
    cv::hconcat(channels[0].reshape(9*9), channels[1].reshape(9*9), dst);
    cv::hconcat(dst, channels[2].reshape(9*9), dst);
    dst = dst.reshape(1);
    return 0;
}