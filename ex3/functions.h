#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <string>

using namespace cv;
using namespace std;

int objects( string &str_buffer, Mat &src_threshold, Mat &src, Mat &src_components, Mat &src_stats, Mat &src_centoid );
int features( Mat &src, Mat &thresh, Mat &map, int label, Mat &dst );