#include <opencv2/opencv.hpp>
#include<iostream>
#define mosaic_error (-1)

uchar convertToUchar(int value) {
	if (value < 0) {
		return 0;
	}
	else if (value > 255) {
		return 255;
	}
	return (uchar)value;
}

int mosaicFilter(cv::Mat &img) {
	int mosaic_size;
	std::cout << "Input mosaic size: \n";
	std::cin >> mosaic_size;
	if (mosaic_size <= 1) {
		return mosaic_error;
	}

	int img_rows = img.rows;
	int img_cols = img.cols;

	for (int i = 0; i < img_cols - 1; i += mosaic_size) {
		int mosaic_wid = mosaic_size;
		if (img_cols - 1 - i < mosaic_size) {
			mosaic_wid = img_cols - 1 - i;
		}

		for (int j = 0; j < img_rows - 1; j += mosaic_size) {
			int B = 0, G = 0, R = 0;
			int mosaic_len = mosaic_size;
			if (img_rows - 1 - j < mosaic_size) {
				mosaic_len = img_rows - 1 - j;
			}

			for (int k = 0; k < mosaic_wid; ++k) {
				for (int l = 0; l < mosaic_len; ++l) {
					B += (int)img.at<cv::Vec3b>(j + l, i + k)[0];
					G += (int)img.at<cv::Vec3b>(j + l, i + k)[1];
					R += (int)img.at<cv::Vec3b>(j + l, i + k)[2];
				}
			}
			B = B / (mosaic_len*mosaic_wid);
			G = G / (mosaic_len*mosaic_wid);
			R = R / (mosaic_len*mosaic_wid);

			for (int k = 0; k <= mosaic_wid; ++k) {
				for (int l = 0; l <= mosaic_len; ++l) {
					img.at<cv::Vec3b>(j + l, i + k)[0] = convertToUchar(B);
					img.at<cv::Vec3b>(j + l, i + k)[1] = convertToUchar(G);
					img.at<cv::Vec3b>(j + l, i + k)[2] = convertToUchar(R);
				}
			}
		}
	}
	return 1;
}

int main()
{
	int error_check = 0;
	cv::Mat image = cv::imread("..\\images\\color\\Lenna.bmp");
	error_check = mosaicFilter(image);
	if (error_check == 1) {
		std::cout << "Mosaic filter processing completed.\n";
		cv::imshow("", image);
		cv::waitKey(0);
	}
	if (error_check == -1) {
		std::cout << "Mosaic filter processing failed.\n";
	}

	system("pause");
	return 0;
}