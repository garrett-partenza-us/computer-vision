#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "filter.h"
#include <iostream> 
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <sstream>

using namespace cv;
using namespace std;

// OpenCV built in greyscale filtering flag 
bool isGrey = false;
// My custom greyscale filtering flag
bool isAltGrey = false;
bool isBlur = false;
bool isXSobel = false;
bool isYSobel = false;


int main()
{
    VideoCapture cap("vidDisplay.mp4");

    if ( !cap.isOpened() ) 
    {
        cout << "Video file not found\n";
        return -1;
    }

    namedWindow("Video Display",WINDOW_NORMAL); 

    while(1)
    {
        Mat frame;

        // Read next frame
        if (!cap.read(frame)) 
        {
            cout << "Failed to read frame\n";
            break;
        }

        // OpenCV greyscale filtering
        if (isGrey){
            cvtColor(frame, frame, CV_RGB2GRAY);
        }
        // My custom greyscale filtering
        if (isAltGrey){
            frame = altGrey(frame);
        }
        if (isBlur){
            frame = blurImg(frame);
        }
        if (isXSobel){
            frame = xSobel(frame);
        }
        if (isYSobel){
            frame = ySobel(frame);
        }


        // Display frame
        imshow("Video", frame);

        // Key commands
        char key = waitKey(10);
        if (key == 'q') 
        { 
            break; 
        }
        else if (key == 's'){
            imwrite("vidDisplay.png", frame);
        }
        else if (key == 'g'){
            isGrey = !isGrey; 
        }
        else if (key == 'h'){
            isAltGrey = !isAltGrey;
        }
        else if (key == 'b' && (!isGrey && !isAltGrey)){
            isBlur = !isBlur;
        }
        else if (key == 'x' && (!isGrey && !isAltGrey)){
            isXSobel = !isXSobel;
        }
        else if (key == 'y' && (!isGrey && !isAltGrey)){
            isYSobel = !isYSobel;
        }
    }

    cap.release();

    return 0;
}
