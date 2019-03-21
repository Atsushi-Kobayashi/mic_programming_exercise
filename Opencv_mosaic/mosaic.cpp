#include <opencv2/opencv.hpp>
#include<iostream>

uchar convertToUchar(int value) {
	if (value < 0) {
		return 0;
	}
	else if(value > 255){
		return 255;
	}
	return (uchar)value;
}

void mosaicFilter(cv::Mat &img) {
	int mosaic_size;
	std::cout << "Input mosaic size: \n";
	std::cin >> mosaic_size;
	if (mosaic_size <= 1) {
		return;
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
}

int main()
{

	cv::Mat image = cv::imread("..\\..\\images\\color\\Lenna.bmp");
	mosaicFilter(image);
	cv::imshow("", image);
	cv::waitKey(0);


	return 0;
}