#include <opencv2/opencv.hpp>
#include<iostream>
#include<vector>
#include<cmath>

std::vector<int> horizontalSobelKernel() {
	std::vector<int> kernel = {
	-1,0,1,
	-2,0,2,
	-1,0,1
	};

	return kernel;

}

std::vector<int> verticalSobelKernel() {
	std::vector<int> kernel = {
	-1,-2,-1,
	0,0,0,
	1,2,1
	};

	return kernel;

}

void monoLinearFilter(cv::Mat &img, std::vector<int> kernel, int kernel_size) {
	int img_rows = img.rows;
	int img_cols = img.cols;
	int kernel_h = (kernel_size - 1) / 2;
	cv::Mat img_zero_pdd = cv::Mat::zeros(img_cols + (2 * kernel_h), img_rows + (2 * kernel_h), CV_8UC1);
	for (int i = 0; i < img_cols; ++i) {
		for (int j = 0; j < img_rows; ++j) {
			img_zero_pdd.at<uchar>(j + kernel_h, i + kernel_h) = img.at<uchar>(j, i);
		}
	}

	for (int i = 0; i < img_cols; ++i) {
		for (int j = 0; j < img_rows; ++j) {
			int V = 0;

			for (int k = 0; k < kernel_size; ++k) {
				for (int l = 0; l < kernel_size; ++l) {
					V += img_zero_pdd.at<uchar>(j + l, i + k) * kernel[k + kernel_size * l];
				}
			}
			if (V < 0) {
				img.at<uchar>(j, i) = 0;
			}
			img.at<uchar>(j, i) = V;

		}
	}

}

void sobelFilter(cv::Mat &img) {
	int kernel_size=3;
	int img_cols = img.cols;
	int img_rows = img.rows;
	std::vector<int> horizontal_kernel = horizontalSobelKernel();
	std::vector<int> vertical_kernel = verticalSobelKernel();
	cv::Mat gray_img= cv::Mat::zeros(img_cols,img_rows, CV_8U);

	for (int i = 0; i < img_cols; ++i) {
		for (int j = 0; j < img_rows; ++j) {
			gray_img.at<uchar>(j,i)=
			img.at<cv::Vec3b>(j, i)[0]*0.144+
			img.at<cv::Vec3b>(j, i)[1]*0.587+
			img.at<cv::Vec3b>(j, i)[2]*0.299;

		}
	}
	//BGR to grayscale‚Ì•ÏŠ·

	cv::imshow("", gray_img);
	cv::waitKey(0);
	cv::Mat gray_img_clone = gray_img.clone();
	monoLinearFilter(gray_img, horizontal_kernel, kernel_size);
	monoLinearFilter(gray_img_clone, vertical_kernel, kernel_size);
	for (int i = 0; i < img_cols; ++i) {
		for (int j = 0; j < img_rows; ++j) {
			gray_img.at<uchar>(j, i) =
				pow(gray_img.at<uchar>(j, i)*gray_img.at<uchar>(j, i)
				+ gray_img_clone.at<uchar>(j, i) *gray_img_clone.at<uchar>(j, i),0.5);

		}
	}
	cv::imshow("", gray_img);
	cv::waitKey(0);
}

int main()
{
	cv::Mat image = cv::imread("..\\images\\color\\Lenna.bmp");
	sobelFilter(image);
	cv::imshow("", image);
	cv::waitKey(0);

	return 0;
}