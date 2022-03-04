
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "functions.h"
#include <iostream>
#include <set>
#include <fstream>
#include <math.h>

using namespace cv;
using namespace std;


int main(int argc, char *argv[]){
    
    char path_img[256];
    char path_features[256];

    // check for sufficient arguments
    if( argc < 2) {
        printf("Not enough arguments\n");
        exit(-1);
    }

    strcpy(path_img, argv[1]);
    string path_img_str;
    path_img_str += path_img;
    strcpy(path_features, argv[2]);

    Mat src, src_threshold, src_components, src_stats, src_centoids, src_features;

    objects(path_img_str, src_threshold, src, src_components, src_stats, src_centoids);
    
    features(src, src_threshold, src_components, 0, src_features);

	vector<vector<string>> content;
	vector<string> row;
	string line, word;
	fstream file (path_features, ios::in);
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

    double min = 100000;
    string label;
    for(int i=0;i<content.size();i++)
	{
        double ssd = 0;
	    for(int j=1;j<content[i].size()-1;j++){
            ssd += pow( src_features.at<double>(j, 0) - stod(content[i][j].c_str()), 2);
		}

        double euclidean_distance = sqrt(ssd);
        if (euclidean_distance < min){
            label = content[i][content[i].size()-1];
            min = euclidean_distance;
        }
	}

    cout << "Prediction: " << label << endl;
    
    
}