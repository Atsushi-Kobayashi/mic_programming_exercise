#pragma once
#include<vector>

uchar convertToUchar(double value);

std::vector<double> gaussianKernel(int kernel_size, double var);
void linearFilter(cv::Mat &img, std::vector<double> kernel, int kernel_size);
int gaussianFilter(cv::Mat &img);

std::vector<double> horizontalSobelKernel();
std::vector<double> verticalSobelKernel();
cv::Mat GrayConvertedFromBGR(cv::Mat &BGR_img);
void substituteScaledValue(cv::Mat &gray_img, std::vector<double> img_tmp, double max, double min);
std::vector<double> sobelConvolutionValues(cv::Mat &gray_img, std::vector<double> kernel, int kernel_size);
void sobelFilter(cv::Mat &gray_img,std::vector<uchar> &normal_direction);
void nonMaximumSuppression(cv::Mat &sobel_img, std::vector<uchar> &normal_direction);
void hysteresisThreshold(cv::Mat &img_edge,uchar high_threshold,uchar low_threshold);

void cannyEdgeDetection(cv::Mat &img);