#include <string>
#include <vector>
#include <sstream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "extract.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm> // for sort function
#include <dirent.h>
#include <tgmath.h>
#include <cstdlib>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <map>

 
using namespace std;

// utility comparator function to pass to the sort() module
bool sortByVal(const pair<string, int> &a, 
               const pair<string, int> &b) 
{ 
    return (a.second < b.second); 
}

int baseline_search(vector<vector<string>> content, string image, int N){
	// put target image features into array
	int target [9*9*3];
	cv::Mat img = cv::imread(image, cv::IMREAD_COLOR);
	cv::Mat features;
	baseline(img, features);
	for(int i=0;i<features.cols;i++)
	{
		target[i] = features.at<uchar>(0, i);
	}

	// build map of SSD
	std::map<string, int> my_map;
	for(int i=0;i<content.size();i++)
	{
		long ssd = 0;
		for(int j=1;j<content[i].size();j++){
			ssd = ssd+pow((target[j-1]-long(atoi(content[i][j].c_str()))), 2);
		}
		my_map[std::string(content[i][0].c_str())] = ssd;
	}	

	// copy key-value pairs from the map to the vector
	vector<pair<string, int>> vec;
	map<string, int> :: iterator it2;
	for (it2=my_map.begin(); it2!=my_map.end(); it2++) 
	{
		vec.push_back(make_pair(it2->first, it2->second));
	}

	// sort
	sort(vec.begin(), vec.end(), sortByVal); 
 
	// print n most similar vector
	cout << "The map, sorted by value is: " << endl;
	for (int i = 1; i < N+1; i++)
	{
		cout << vec[i].first << ": " << vec[i].second << endl;
	}
	return 0;
}
 
int main(int argc, char *argv[]) {
	char filename[256];
    char image[256];
    char buffer[256];
	char method[256];
    
    if( argc < 5) {
        cout << "Not enough arguments\n";
        exit(-1);
    }

	// get the directory path
    strcpy(method, argv[1]);
    printf("Using method %s\n", method );

    // get the feature file
    strcpy(filename, argv[2]);
    printf("Using feature file %s\n", filename );

    // get the feature type
    strcpy(image, argv[3]);
    printf("Searching for image %s\n", image );

	// get number of images
	int N = atoi(argv[4]);
    printf("Finding %d images\n", N );

 
	// create content vector
	vector<vector<string>> content;
	vector<string> row;
	string line, word;
	fstream file (filename, ios::in);
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
	else
		cout<<"Could not open the file\n";

	if (std::string(method)=="baseline"){
		baseline_search(content, std::string(image), N);
	}

	return 0;
}
 