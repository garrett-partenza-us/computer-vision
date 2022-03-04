// includes
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "functions.h"
#include <string>



// namespace
using namespace cv;
using namespace std;

// main function to iterate over images and store features in csv file
int main(int argc, char *argv[]) {
    char dirname[256];
    char buffer_dir[256];
    char buffer_img[256];
    FILE *fp;
    DIR *dirp;
    struct dirent *dp;

    // check for sufficient arguments
    if( argc < 1) {
        printf("Not enough arguments\n");
        exit(-1);
    }

    // get the directory path
    strcpy(dirname, argv[1]);
    printf("Processing directory %s\n", dirname );

    vector<string> fruits;
    fruits.push_back("Avocado");
    fruits.push_back("Banana");
    fruits.push_back("Blueberry");
    fruits.push_back("Guava");
    fruits.push_back("Kiwi");
    fruits.push_back("Lemon");
    fruits.push_back("Orange");
    fruits.push_back("Pear");
    fruits.push_back("Strawberry");
    fruits.push_back("Watermelon");


    for (vector<string>::iterator fruit=fruits.begin(); fruit!=fruits.end(); ++fruit){

        cout << "FRUIT: " << fruit->c_str() << endl;

        strcpy(buffer_dir, dirname);
        strcat(buffer_dir, fruit->c_str());

        // open the directory
        dirp = opendir( buffer_dir );
            if( dirp == NULL) {
                printf("Cannot open directory %s\n", buffer_dir);
                exit(-1);
        }

        // loop over all the files in the image file listing
        while( (dp = readdir(dirp)) != NULL ) {

            // check if the file is an image
            if( strstr(dp->d_name, ".jpg") ||
                strstr(dp->d_name, ".png") ||
                strstr(dp->d_name, ".ppm") ||
                strstr(dp->d_name, ".tif") ) {

                // build the overall filename
                strcpy(buffer_img, buffer_dir);
                strcat(buffer_img, "/");
                strcat(buffer_img, dp->d_name);

                Mat src, src_threshold, src_components, src_stats, src_centoids, src_features;

                string str_buffer;
                str_buffer += buffer_img;

                cout << str_buffer << endl;

                objects(str_buffer, src_threshold, src, src_components, src_stats, src_centoids);
                
                int label = 0;
                features(src, src_threshold, src_components, label, src_features);
                ofstream myfile("features.csv", std::ios::app);
                for(int i=0; i<src_features.rows; i++){
                    for(int j=0; j<src_features.cols; j++){
                        double value = src_features.at<double>(i, j);
                        myfile << value << ',';
                    }
                }
                myfile << fruit->c_str() << ',';
                myfile << '\n';

                myfile.close();
            }
            
        }
    
    }

    printf("Terminating\n");

    return(0);
}