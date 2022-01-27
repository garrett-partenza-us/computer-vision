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
bool isBlur = false;
bool isXSobel = false;
bool isYSobel = false;
bool isMag = false;
bool isQuant = false;
bool isCartoon = false;
bool isContrast = false;
double contrastCoef = 1;

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

        if (isGrey){
            cvtColor(frame, frame, CV_RGB2GRAY);
        }
        if (isAltGrey){
            cv::Mat greyimg(frame.rows, frame.cols, CV_8UC1);
            greyscale(frame, greyimg);
            frame = greyimg;
        }
        if (isBlur){
            cv::Mat blurimg = cv::Mat(frame.rows, frame.cols, CV_8UC3);
            blur5x5(frame, blurimg);
            frame = blurimg;
        }
        if (isXSobel){
            cv::Mat xSobel = cv::Mat(frame.rows, frame.cols, CV_8UC3);
            sobelX3x3(frame, xSobel);
            frame = xSobel;
        }
        if (isYSobel){
            cv::Mat ySobel = cv::Mat(frame.rows, frame.cols, CV_8UC3);
            sobelY3x3(frame, ySobel);
            frame = ySobel;
        }
        if (isMag){
            cv::Mat magImg = cv::Mat(frame.rows, frame.cols, CV_8UC3);
            magnitude(frame, magImg);
            frame = magImg;
        }
        if (isQuant){
            cv::Mat blurQuantImg = cv::Mat(frame.rows, frame.cols, CV_8UC3);
            int levels = 10;
            blurQuantize(frame, blurQuantImg, levels);
            frame = blurQuantImg;
        }
        if (isCartoon){
            int levels = 10;
            int threshold = 20;
            cv::Mat cartoonImg = cv::Mat(frame.rows, frame.cols, CV_8UC3);
            cartoon(frame, cartoonImg, levels, threshold);
            frame = cartoonImg;
        }
        if (isContrast){
            cv::Mat contrastImg = cv::Mat(frame.rows, frame.cols, CV_8UC3);
            contrast(frame, contrastImg, contrastCoef);
            frame = contrastImg;
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
        else if (key == 'm' && (!isGrey && !isAltGrey)){
            isMag = !isMag;
        }
        else if (key == 'l' && (!isGrey && !isAltGrey)){
            isQuant = !isQuant;
        }
        else if (key == 'c' && (!isGrey && !isAltGrey)){
            isCartoon = !isCartoon;
        }
        else if (key == '+' && (!isGrey && !isAltGrey)){
            contrastCoef = contrastCoef + 0.5;
        }
        else if (key == '-' && (!isGrey && !isAltGrey)){
            contrastCoef = contrastCoef - 0.5;
        }
        if (contrastCoef != 1){
            isContrast = true;
        }
    }

    cap.release();

    return 0;
}
