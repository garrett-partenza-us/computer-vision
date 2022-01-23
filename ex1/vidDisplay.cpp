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

bool isGrey = false;
bool isAltGrey = false;

int main()
{
    VideoCapture cap("vidDisplay.mp4");

    if ( !cap.isOpened() ) 
    {
        cout << "Video file not found\n";
        return -1;
    }

    // Cap FPS
    double fps = cap.get(CV_CAP_PROP_FPS); 

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

        if (isGrey){
            cvtColor(frame, frame, CV_RGB2GRAY);
        }
        else if (isAltGrey){
            frame = altGrey(frame);
        }

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
    }

    cap.release();

    return 0;
}
