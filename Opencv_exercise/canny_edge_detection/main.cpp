#include <opencv2/opencv.hpp>
#include"filters.h"


int main(int argc, char *argv[])
{
	cv::Mat img = cv::imread(argv[1]);
	cv::imshow("original", img);
	cv::waitKey(0);

	cannyEdgeDetection(img);

	system("pause");
	return 0;
}