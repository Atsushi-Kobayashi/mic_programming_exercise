#include <opencv2/opencv.hpp>
#include<iostream>
#include<vector>
#define smooth_error (-1)

uchar convertToUchar(double value) {
	if (value < 0) {
		return 0;
	}
	else if (value > 255) {
		return 255;
	}
	//マイナス値およびオーバーフローの対策
	return (uchar)value;
}


std::vector<double> smoothKernel(int kernel_size) {
	double cell_value = 1 / ((double)kernel_size * kernel_size);
	std::vector<double> kernel(kernel_size*kernel_size, cell_value);

	return kernel;

}

void linearFilter(cv::Mat &img, std::vector<double> kernel, int kernel_size) {
	int img_rows = img.rows;
	int img_cols = img.cols;
	int kernel_h = (kernel_size - 1) / 2;
	cv::Mat img_zero_pdd = cv::Mat::zeros(img_cols + (2 * kernel_h), img_rows + (2 * kernel_h), CV_8UC3);
	for (int i = 0; i < img_cols; ++i) {
		for (int j = 0; j < img_rows; ++j) {
			img_zero_pdd.at<cv::Vec3b>(j + kernel_h, i + kernel_h)[0] = img.at<cv::Vec3b>(j, i)[0];
			img_zero_pdd.at<cv::Vec3b>(j + kernel_h, i + kernel_h)[1] = img.at<cv::Vec3b>(j, i)[1];
			img_zero_pdd.at<cv::Vec3b>(j + kernel_h, i + kernel_h)[2] = img.at<cv::Vec3b>(j, i)[2];
		}
		//img.at<>()[]のアクセスは可読性は良いが、
		//画像全体に処理を施すときなどは高速なポインタを用いたアクセスのほうが良い
		//gausianのlinearfilterはポインタで実装
	}
	//zero padding

	for (int i = 0; i < img_cols; ++i) {
		for (int j = 0; j < img_rows; ++j) {
			double B = 0, G = 0, R = 0;

			for (int k = 0; k < kernel_size; ++k) {
				for (int l = 0; l < kernel_size; ++l) {
					B += (double)img_zero_pdd.at<cv::Vec3b>(j + l, i + k)[0] * kernel[k + kernel_size * l];
					G += (double)img_zero_pdd.at<cv::Vec3b>(j + l, i + k)[1] * kernel[k + kernel_size * l];
					R += (double)img_zero_pdd.at<cv::Vec3b>(j + l, i + k)[2] * kernel[k + kernel_size * l];
				}
			}

			img.at<cv::Vec3b>(j, i)[0] = convertToUchar(B);
			img.at<cv::Vec3b>(j, i)[1] = convertToUchar(G);
			img.at<cv::Vec3b>(j, i)[2] = convertToUchar(R);

		}
	}

}

int smoothFilter(cv::Mat &img) {
	int kernel_size;
	std::cout << "Input kernel size: \n";
	std::cin >> kernel_size;
	if (kernel_size <= 1 || kernel_size % 2 == 0) {
		return smooth_error;
	}
	std::vector<double> kernel = smoothKernel(kernel_size);
	linearFilter(img, kernel, kernel_size);
	return 1;
}

int main()
{
	int error_check = 0;
	cv::Mat image = cv::imread("..\\images\\color\\Lenna.bmp");
	error_check =smoothFilter(image);
	if (error_check == 1) {
		std::cout << "Smooth filter processing completed.\n";
		cv::imshow("", image);
		cv::waitKey(0);
	}
		if (error_check == -1) {
			std::cout << "Smooth filter processing failed.\n";
	}



	system("pause");
	return 0;
}