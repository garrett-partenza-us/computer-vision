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
    char features_path[256];
    char buffer_dir[256];
    char buffer_img[256];
    FILE *fp;
    DIR *dirp;
    struct dirent *dp;

    // check for sufficient arguments
    if( argc < 2) {
        printf("Not enough arguments\n");
        exit(-1);
    }

    // get the directory path
    strcpy(dirname, argv[1]);
    printf("Processing directory %s\n", dirname );

    strcpy(features_path, argv[2]);
    printf("Processing dataset %s\n", features );

    vector<vector<string>> content;
	vector<string> row;
	string line, word;
	fstream file (features_path, ios::in);
	if(file.is_open())
	{
		while(getline(file, line))
		{
			row.clear();
			stringstream str(line);
			while(getline(str, word, ','))
				row.push_back(word);
			content.push_back(row);
		}
	}

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

    float correct = 0;
    float total = 0;


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

                double min = 100000;
                string prediction;
                for(int i=0;i<content.size();i++)
                {
                    double ssd = 0;
                    for(int j=1;j<content[i].size()-1;j++){
                        ssd += pow( src_features.at<double>(j, 0) - stod(content[i][j].c_str()), 2);
                    }

                    double euclidean_distance = sqrt(ssd);
                    if (euclidean_distance < min){
                        prediction = content[i][content[i].size()-1];
                        min = euclidean_distance;
                    }
                }

                if (prediction == fruit->c_str()){
                    correct+=1;
                }
                total+=1;

                cout << "Predicted: " << prediction << ", Actual: " << fruit->c_str() << endl;
                
            }
            
        }
    
    }

    cout << "Accuracy: " << correct/total << endl;

    printf("Terminating\n");

    return(0);
}