#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>  
#include <dirent.h>
#include <map>
#include <math.h>       /* sqrt */
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>
#include "opencv2/imgproc.hpp"

#define PI 3.14159265

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

int middle( cv::Mat &src, cv::Mat &dst ) {
    // generate image feature vector using histogram method
    std::map<std::string, int> histogram;
    for(int i=0;i<4;i++){
      for(int j=0;j<4;j++){
        for(int z=0;z<4;z++){
          histogram[to_string(i)+to_string(j)+to_string(z)]=0;
        }
      }
    }

    cv::Mat pixels = src(cv::Range((src.rows/2)-32,(src.rows/2)+32), cv::Range((src.cols/2)-32,(src.cols/2)+32));
    // Bin 0 --> 0-63
    // Bin 1 --> 64-127
    // Bin 2 --> 128-191 
    // Bin 3 --> 192-255
    for(int i=0;i<pixels.rows;i++){
      for(int j=0;j<pixels.cols;j++){
          cv::Vec3b RGB = pixels.at<cv::Vec3b>(i,j);
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

double clamp( double val ) {
    if (val < 0) {
      val = 0;
    }
    else if (val > 255) {
      val = 255;
    }
    return val;
}

cv::Vec3b sobel3x3helper( cv::Mat &kern, cv::Mat &pixels ) {

  cv::Mat channels[3];
  cv::split(pixels, channels);
  channels[0].convertTo(channels[0], 19);
  channels[1].convertTo(channels[1], 19);
  channels[2].convertTo(channels[2], 19);

  cv::Mat red = kern.mul(channels[0]);
  cv::Mat green = kern.mul(channels[1]);
  cv::Mat blue = kern.mul(channels[2]);
  
  cv::Vec3b color;
  color[0] = clamp(cv::sum(red)[0] / 3);;
  color[1] = clamp(cv::sum(green)[0] / 3);
  color[2] = clamp(cv::sum(blue)[0] / 3);
  
  return color;

}

// XY Magnitude Filter
int magnitude( cv::Mat &src, cv::Mat &dst ) {

  cv::Mat xkern = (cv::Mat_<char>(3, 3) << -1, 0, 1, -1, 0, 1, -1, 0, 1);
  cv::Mat ykern = (cv::Mat_<char>(3, 3) << -1, -1, -1, 0, 0, 0, 1, 1, 1);
  
  xkern.convertTo(xkern, 19);
  ykern.convertTo(ykern, 19);

  for(int i=2; i<src.rows-2; i++){
      for(int j=2; j<src.cols-2; j++){

          // Get pixels
          cv::Mat pixels = src(cv::Range(i-1,i+2), cv::Range(j-1,j+2)).clone();
          // X sobel
          cv::Vec3b xcolor = sobel3x3helper(xkern, pixels);
          // Y sobel
          cv::Vec3b ycolor = sobel3x3helper(ykern, pixels);

          cv::Vec3b zcolor;
          zcolor[0] = sqrt(xcolor[0] * xcolor[0] + ycolor[0] * ycolor[0]);
          zcolor[1] = sqrt(xcolor[1] * xcolor[1] + ycolor[1] * ycolor[1]);
          zcolor[2] = sqrt(xcolor[2] * xcolor[2] + ycolor[2] * ycolor[2]);
          dst.at<Vec3b>(i-2,j-2) = zcolor;
      }
  }

  return 0;

}

int texture( cv::Mat &src, cv::Mat &dst ) {
  
    // generate image feature vector using histogram method
    std::map<std::string, int> histogram;
    for(int i=0;i<4;i++){
      for(int j=0;j<4;j++){
        for(int z=0;z<4;z++){
          histogram[to_string(i)+to_string(j)+to_string(z)]=0;
        }
      }
    }

    cv::Mat mag = Mat(src.rows, src.cols, CV_32FC1);
    magnitude( src, mag);
    // Bin 0 --> 0-63
    // Bin 1 --> 64-127
    // Bin 2 --> 128-191 
    // Bin 3 --> 192-255
    for(int i=0;i<mag.rows;i++){
      for(int j=0;j<mag.cols;j++){
          cv::Vec3b RGB = mag.at<cv::Vec3b>(i,j);
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

int thetabin ( float theta ){
  if (theta>0 and theta<22.5){
    return 0;
  }
  else if (theta>22.5 and theta<67.5){
    return 45;
  } 
  else if (theta>67.5 and theta<112.5){
    return 90;
  }
  else{
    return 135;
  }    
}
int canny( cv::Mat &src, float &edge_intensity){
  
  // pad, grey, and blur
  cv::Mat padded;
  copyMakeBorder(src, padded, 4,4,4,4, BORDER_CONSTANT, Scalar(0));
  cv::Mat grey;
  cv::cvtColor(padded, grey, CV_BGR2GRAY);
  cv::Mat blur;
  GaussianBlur( grey, blur, Size( 5, 5 ), 1, 1 );
  
  // gradient magnitude
  cv::Mat magn(blur.rows, blur.cols, CV_32FC2);
  cv::Mat xkern = (cv::Mat_<float>(3, 3) << -1, 0, 1, -1, 0, 1, -1, 0, 1);
  cv::Mat ykern = (cv::Mat_<float>(3, 3) << -1, -1, -1, 0, 0, 0, 1, 1, 1);
  for(int i=4;i<blur.rows-4;i++){
    for(int j=4;j<blur.cols-4;j++){
      cv::Mat blob = blur(cv::Range(i-1,i+2), cv::Range(j-1,j+2));
      blob.convertTo(blob, 5);
      float G_x = cv::sum(xkern.mul(blob))[0];
      float G_y = cv::sum(ykern.mul(blob))[0];
      float G = (float)clamp(sqrt(pow(G_x, 2) + pow(G_y, 2)));
      float theta = atan2(G_y,G_x);
      cv::Vec2f gtheta;
      gtheta[0] = G;
      gtheta[1] = thetabin(theta);
      magn.at<Vec2f>(i,j) = gtheta;
    }
  }

  // lower bounding
  cv::Mat lowerbounding(magn.rows, magn.cols, CV_32FC1);
  for(int i=4;i<magn.rows-4;i++){
    for(int j=4;j<magn.cols-4;j++){
      cv::Vec2f gtheta = magn.at<cv::Vec2f>(i,j);
      int G = gtheta[0];
      int theta = gtheta[1];
      int neighbors [2];
      if (theta==0){
        neighbors[0]=magn.at<cv::Vec2f>(i,j-1)[0];
        neighbors[1]=magn.at<cv::Vec2f>(i,j+1)[0];
      }
      else if (theta==90){
        neighbors[0]=magn.at<cv::Vec2f>(i-1,j-1)[0];
        neighbors[1]=magn.at<cv::Vec2f>(i+1,j+1)[0];
      }
      else if (theta==135){
        neighbors[0]=magn.at<cv::Vec2f>(i+1,j+1)[0];
        neighbors[1]=magn.at<cv::Vec2f>(i-1,j-1)[0];
      }
      else{
        neighbors[0]=magn.at<cv::Vec2f>(i-1,j)[0];
        neighbors[1]=magn.at<cv::Vec2f>(i+1,j)[0];
      }
      if (G<max(neighbors[0], neighbors[1])){
        G = 0;
      }
      lowerbounding.at<float>(i,j) = (float)G;
    }
  }

  // double thresholding
  cv::Mat doublethreshold(lowerbounding.rows, lowerbounding.cols, CV_32FC2);
  for(int i=4;i<lowerbounding.rows-4;i++){
    for(int j=4;j<lowerbounding.cols-4;j++){
      float G = lowerbounding.at<float>(i,j);
      cv::Vec2f temp;
      if (G>150){
        temp[0] = G;
        temp[1] = 2;
      }
      else if (G>50){
        temp[0] = G;
        temp[1] = 1;
      }
      else{
        temp[0] = G;
        temp[1] = 0;
      }
      doublethreshold.at<Vec2f>(i,j) = temp;
    }
  }

  // hysteresis
  cv::Mat hysteresis(doublethreshold.rows, doublethreshold.cols, CV_32FC1);
  for(int i=4;i<doublethreshold.rows-4;i++){
    for(int j=4;j<doublethreshold.cols-4;j++){
      float G = doublethreshold.at<Vec2f>(i,j)[0];
      float indicator = doublethreshold.at<Vec2f>(i,j)[1];
      if (indicator==2){
        hysteresis.at<float>(i,j) = G;
      }
      else{
        cv::Mat blob = doublethreshold(cv::Range(i-4,i+5), cv::Range(j-4,j+5));
        bool add = false;
        for(int q=0;q<blob.rows;q++){
          for(int t=0;t<blob.cols;t++){
            float indicator = blob.at<Vec2f>(q,t)[1];
            if (indicator==2){
              add = true;
            }
            if (add){
              break;
            }
          }
          if (add){
            break;
          }
        }
        if (add){
          hysteresis.at<float>(i,j) = G;
        }
        else{
          hysteresis.at<float>(i,j) = 0;
        }
      }
    }
  }


  edge_intensity = (cv::sum(hysteresis)[0])/(hysteresis.rows*hysteresis.cols);
  return 0;

}