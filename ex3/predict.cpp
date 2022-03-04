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
#include <map>

using namespace cv;
using namespace std;

struct sort_pred {
    bool operator()(const std::pair<float,int> &left, const std::pair<float,int> &right) {
        return left.first < right.first;
    }
};


int main(int argc, char *argv[]){
    
    char path_img[256];
    char path_features[256];
    int K;

    // check for sufficient arguments
    if( argc < 3) {
        printf("Not enough arguments\n");
        exit(-1);
    }

    strcpy(path_img, argv[1]);
    string path_img_str;
    path_img_str += path_img;
    strcpy(path_features, argv[2]);
    K = stod(argv[3]);

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

    map<int, string> label2fruit = {{0, "Avocado",},
                             {1, "Banana",},
                             {2, "Blueberry",},
                             {3, "Guava",},
                             {4, "Kiwi",},
                             {5, "Lemon",},
                             {6, "Orange",},
                             {7, "Pear",},
                             {8, "Strawberry",},
                             {9, "Watermelon",},
                             };

    map<string, int> fruit2label = {{"Avocado", 0,},
                             {"Banana", 1,},
                             {"Blueberry", 2,},
                             {"Guava", 3,},
                             {"Kiwi", 4,},
                             {"Lemon", 5,},
                             {"Orange", 6,},
                             {"Pear", 7,},
                             {"Strawberry", 8,},
                             {"Watermelon", 9,},
                             };
    
    Mat src, src_threshold, src_components, src_stats, src_centoids;
    vector<float> src_features;
    vector<pair<float,int>> distances;

    cout << path_img << endl;

    objects(path_img_str, src_threshold, src, src_components, src_stats, src_centoids);
    
    int label = 0;
    features(src, src_threshold, src_components, label, src_stats, src_features);

    string prediction;
    for(int i=0;i<content.size();i++)
    {
        double ssd = 0;
        for(int j=0;j<content[i].size()-1;j++){
            ssd += pow( src_features[j] - stod(content[i][j].c_str()), 2 );
        }

        double euclidean_distance = sqrt(ssd);
        string label = content[i][content[i].size()-1].c_str();
        distances.push_back( make_pair(euclidean_distance, fruit2label[label]) );
    }


    sort(distances.begin(), distances.end(), sort_pred());

    vector<pair<float, int>> predictions(distances.begin(), distances.begin() + K);

    float sum_predictions = 0;
    for(int i = 0; i < predictions.size(); i++)
    {
        sum_predictions += predictions[i].second;
    }

    cout << sum_predictions << endl;

    prediction = label2fruit[(int)( sum_predictions / K)];

    cout << "Predicted: " << prediction << endl;
    
    
}