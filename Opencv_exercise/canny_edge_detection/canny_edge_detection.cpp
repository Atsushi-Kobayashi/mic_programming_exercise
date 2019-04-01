#include <opencv2/opencv.hpp>
#include"filters.h"
#include<iostream>
#include<vector>
#include<cmath>

void cannyEdgeDetection(cv::Mat &img) {

	gaussianFilter(img);
	cv::imshow("gaussian", img);
	cv::waitKey(0);

	cv::Mat img_sobel_edge = GrayConvertedFromBGR(img);
	std::vector<uchar> sobel_edge_normal_direction = {};
	cv::imshow("gray", img_sobel_edge);
	cv::waitKey(0);
	sobelFilter(img_sobel_edge,sobel_edge_normal_direction);
	cv::imshow("fused sobel", img_sobel_edge);
	cv::waitKey(0);
}