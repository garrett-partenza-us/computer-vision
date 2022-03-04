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



// namespace
using namespace cv;
using namespace std;

//sorting operator for distance vector pairs
struct sort_pred {
    bool operator()(const std::pair<float,int> &left, const std::pair<float,int> &right) {
        return left.first < right.first;
    }
};


// main function to iterate over images and store features in csv file
int main(int argc, char *argv[]) {
    char dirname[256];
    char features_path[256];
    char buffer_dir[256];
    char buffer_img[256];
    int K;
    FILE *fp;
    DIR *dirp;
    struct dirent *dp;

    // check for sufficient arguments
    if( argc < 3) {
        printf("Not enough arguments\n");
        exit(-1);
    }

    // get the directory path
    strcpy(dirname, argv[1]);
    printf("Processing directory %s\n", dirname );

    strcpy(features_path, argv[2]);
    printf("Processing dataset %s\n", features );

    //user inputed K
    K = stod(argv[3]);

    //read feature vector file
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

    //create maps for converting labels to ints
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

    float correct = 0;
    float total = 0;


    //iterate over fruits 
    for (vector<string>::iterator fruit=fruits.begin(); fruit!=fruits.end(); ++fruit){

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

                Mat src, src_threshold, src_components, src_stats, src_centoids;
                vector<float> src_features;
                vector<pair<float,int>> distances;

                string str_buffer;
                str_buffer += buffer_img;

                //detect objects
                objects(str_buffer, src_threshold, src, src_components, src_stats, src_centoids);
                
                int label = 0;
                //get feature vector of target image
                features(src, src_threshold, src_components, label, src_stats, src_features);

                //make prediction
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

                prediction = label2fruit[round( (int) ( sum_predictions / K)  )];

                if (prediction == fruit->c_str()){
                    correct+=1;
                }
                total+=1;

                cout << prediction << "," << fruit->c_str() << endl;
                
            }
            
        }
    
    }

    cout << "Accuracy: " << correct/total << endl;

    printf("Terminating\n");

    return(0);
}