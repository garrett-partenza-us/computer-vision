#include "opencv2/highgui/highgui.hpp"
#include <iostream> 

using namespace cv;
using namespace std;

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

        imshow("Video", frame);

        // Press 'q' to close vidoe
        if(waitKey(10) == 'q') 
        { 
            break; 
        }
    }

    cap.release();

    return 0;
}
