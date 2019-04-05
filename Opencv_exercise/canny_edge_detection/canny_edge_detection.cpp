#include <opencv2/opencv.hpp>
#include"filters.h"
#include<iostream>
#include<vector>
#include<cmath>

void cannyEdgeDetection(cv::Mat &img) {
	int high_threshold=90;
	int low_threshold=15;

	/*std::cout << "Gaussian filter\n";*/
	gaussianFilter(img);
	cv::imshow("Gaussian", img);
	cv::waitKey(0);

	cv::Mat img_edge = GrayConvertedFromBGR(img);
	std::vector<uchar> sobel_edge_normal_direction = {};

	sobelFilter(img_edge,sobel_edge_normal_direction);
	cv::imshow("Fused sobel", img_edge);
	cv::waitKey(0);

	nonMaximumSuppression(img_edge, sobel_edge_normal_direction);
	cv::imshow("Non Maximum Suppression", img_edge);
	cv::waitKey(0);

/*
	std::cout << "Hysteresis threshold\n";
	while (true)
	{
		std::cout << "Input high threshold (0,255): \n";
		std::cin >> high_threshold;
		std::cout << "Input low threshold (0,255): \n";
		std::cin >> low_threshold;
		if (high_threshold > 0 && high_threshold < 255 && low_threshold>0 && low_threshold < 255 && high_threshold>low_threshold) {
			break;
		}
		std::cout << "Error\n";
	}*/
	


	hysteresisThreshold(img_edge, (uchar)high_threshold, (uchar)low_threshold);
	cv::imshow("Result", img_edge);
	cv::waitKey(0);

}