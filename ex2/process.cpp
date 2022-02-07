/*
  Bruce A. Maxwell
  S21
  
  Sample code to identify image fils in a directory
*/
#include <cstdio>
#include <cstring>
#include <cstdlib>
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
  char featuretype[256];
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
  strcpy(featuretype, argv[2]);
  printf("Using feature type %s\n", featuretype );

  // open the directory
  dirp = opendir( dirname );
  if( dirp == NULL) {
    printf("Cannot open directory %s\n", dirname);
    exit(-1);
  }

  fstream myfile;
  myfile.open("baseline.csv");

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

      // begin project code here
      Mat img = imread(buffer, IMREAD_COLOR);
      Mat features;
      baseline(img, features);
      cout << buffer << "\n";
      myfile << strcat(buffer, ",") << cv::format(features, cv::Formatter::FMT_CSV) << std::endl;
    }
  }
  
  myfile.close();
  printf("Terminating\n");

  return(0);
}


