#include <opencv2/opencv.hpp>


int main()
{
	cv::Mat img = cv::imread("Lenna.bmp");
	cv::imshow("imshow test", img);
	cv::waitKey(0);
	system("pause");
	return 0;
}