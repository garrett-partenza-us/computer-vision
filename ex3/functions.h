#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;

int features( Mat &src, Mat &thresh, Mat &map, int label, Mat &dst );