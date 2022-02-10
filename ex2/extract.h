#include "opencv2/imgproc/imgproc.hpp"
int baseline( cv::Mat &src, cv::Mat &dst );
int histogram( cv::Mat &src, cv::Mat &dst );
int middle( cv::Mat &src, cv::Mat &dst );
int texture( cv::Mat &src, cv::Mat &dst );
int canny( cv::Mat &src, float &edge_intensity );
