#include <opencv2/opencv.hpp>
#include"filters.h"


int main(int argc, char *argv[])
{
	cv::Mat img = cv::imread(argv[1]);
	cv::Mat img_gray_for_GT = GrayConvertedFromBGR(img);
	cv::Mat ground_truth = img_gray_for_GT.clone();
	cv::imshow("original", img);
	cv::waitKey(0);

	cv::Canny(img_gray_for_GT, ground_truth, 150, 100);
	cannyEdgeDetection(img);

	cv::imshow("GT", ground_truth);
	cv::waitKey(0);
	system("pause");
	return 0;
}