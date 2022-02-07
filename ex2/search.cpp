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
#include <dirent.h>
#include <tgmath.h>
#include <cstdlib>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <map>

 
using namespace std;
 
int main(int argc, char *argv[]) {
	char filename[256];
    char image[256];
    char buffer[256];
    std::map<string, int> my_map;

    if( argc < 3) {
        cout << "Not enough arguments\n";
        exit(-1);
    }

    // get the directory path
    strcpy(filename, argv[1]);
    printf("Processing feature set %s\n", filename );

    // get the feature type
    strcpy(image, argv[2]);
    printf("Searching for image %s\n", image );
 
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

 
	// find features of arg image
	int target [9*9*3]; 
	for(int i=0;i<content.size();i++)
	{
		if (image == std::string(content[i][0].c_str())){
			for(int j=1;j<content[i].size();j++){
				target[j-1]=int(atoi(content[i][j].data()));
			}
			break;
		}
	}

	for(int i=0;i<content.size();i++)
	{

		long ssd = 0;
		for(int j=1;j<content[i].size();j++){
			ssd = ssd+pow((target[j-1]-long(atoi(content[i][j].c_str()))), 2);
		}
		my_map[std::string(content[i][0].c_str())] = ssd;
	}
 
	return 0;
}
 