/*
  Bruce A. Maxwell
  S21
  
  Sample code to identify image fils in a directory
*/
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <map>
#include "extract.h"
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

int main(int argc, char *argv[]) {
  char dirname[256];
  char buffer[256];
  char method[256];
  FILE *fp;
  DIR *dirp;
  struct dirent *dp;
  int i;

  // check for sufficient arguments
  if( argc < 3) {
    printf("Not enough arguments\n");
    exit(-1);
  }

  // get the directory path
  strcpy(dirname, argv[1]);
  printf("Processing directory %s\n", dirname );

  // get the feature type
  strcpy(method, argv[2]);
  printf("Using feature type %s\n", method );

  // open the directory
  dirp = opendir( dirname );
  if( dirp == NULL) {
    printf("Cannot open directory %s\n", dirname);
    exit(-1);
  }

  fstream myfile;
  if (std::string(method)==std::string("baseline")){
    myfile.open("baseline.csv");
  }
  else if (std::string(method)==std::string("histogram")){
    myfile.open("histogram.csv");
  }
  else if (std::string(method)==std::string("multi")){
    myfile.open("multi.csv");
  }
  else if (std::string(method)==std::string("texture")){
    myfile.open("texture.csv");
  }
  else if (std::string(method)==std::string("canny")){
    myfile.open("canny.csv");
  }

  // loop over all the files in the image file listing
  while( (dp = readdir(dirp)) != NULL ) {

    // check if the file is an image
    if( strstr(dp->d_name, ".jpg") ||
	      strstr(dp->d_name, ".png") ||
	      strstr(dp->d_name, ".ppm") ||
	      strstr(dp->d_name, ".tif") ) {

      // build the overall filename
      strcpy(buffer, dirname);
      strcat(buffer, "/");
      strcat(buffer, dp->d_name);

      if (std::string(method)==std::string("baseline")){
        Mat img = imread(buffer, IMREAD_COLOR);
        Mat features;
        baseline(img, features);
        cout << buffer << "\n";
        myfile << strcat(buffer, ",") << cv::format(features, cv::Formatter::FMT_CSV) << std::endl;
      }
      else if (std::string(method)==std::string("histogram")){
        Mat img = imread(buffer, IMREAD_COLOR);
        Mat features(1, 64, CV_32FC1);
        histogram(img, features);
        cout << buffer << "\n";
        myfile << strcat(buffer, ",") << cv::format(features, cv::Formatter::FMT_CSV) << std::endl;
      }
      else if (std::string(method)==std::string("multi")){
        Mat img = imread(buffer, IMREAD_COLOR);
        Mat histogram_features(1, 64, CV_32FC1);
        histogram(img, histogram_features);
        Mat middle_features(1, 64, CV_32FC1);
        middle(img, middle_features);
        Mat features(1, 128, CV_32FC1);
        hconcat(histogram_features, middle_features, features);
        cout << buffer << "\n";
        myfile << strcat(buffer, ",") << cv::format(features, cv::Formatter::FMT_CSV) << std::endl;
      }
      else if (std::string(method)==std::string("texture")){
        Mat img = imread(buffer, IMREAD_COLOR);
        Mat histogram_features(1, 64, CV_32FC1);
        histogram(img, histogram_features);
        Mat texture_features(1, 64, CV_32FC1);
        texture(img, texture_features);
        Mat features(1, 128, CV_32FC1);
        hconcat(histogram_features, texture_features, features);
        cout << buffer << "\n";
        myfile << strcat(buffer, ",") << cv::format(features, cv::Formatter::FMT_CSV) << std::endl;
      }
      else if (std::string(method)==std::string("canny")){
        Mat img = imread(buffer, IMREAD_COLOR);
        float edge_intensity;
        canny( img, edge_intensity );
        Mat histogram_features(1, 64, CV_32FC1);
        histogram(img, histogram_features);
        Mat edge_intensity_mat(1, 1, CV_32FC1);
        edge_intensity_mat.at<float>(0,0) = (float)edge_intensity;
        Mat features(1, 65, CV_32FC1);
        hconcat(edge_intensity_mat, histogram_features, features);
        myfile << strcat(buffer, ",") << cv::format(features, cv::Formatter::FMT_CSV) << std::endl;
        cout << buffer << "\n";
      }
    }
  }
  
  myfile.close();
  printf("Terminating\n");

  return(0);
}


