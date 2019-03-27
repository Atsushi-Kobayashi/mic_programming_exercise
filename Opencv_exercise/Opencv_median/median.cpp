#include <opencv2/opencv.hpp>
#include<iostream>
#define median_error (-1)

void insertionSort(std::vector<uchar> &vct, int size) {
	int i, j;
	for (i = 1; i < size; ++i) {
		for (j = i; j > 0; --j)
		{
			if (vct[j - 1] > vct[j]) {
				int tmp = vct[j - 1];
				vct[j - 1] = vct[j];
				vct[j] = tmp;
			}
			else {
				break;
			}
		}
	}

}


int medianFilter(cv::Mat &img) {
	int window_size;
	std::cout << "Input window size: \n";
	std::cin >> window_size;
	if (window_size <= 1) {
		return median_error;
	}

	int img_rows = img.rows;
	int img_cols = img.cols;
	int window_h = (window_size) / 2;
	cv::Mat img_zero_pdd = cv::Mat::zeros(img_cols + (2 * window_h), img_rows + (2 * window_h), CV_8UC3);
	for (int i = 0; i < img_cols; ++i) {
		for (int j = 0; j < img_rows; ++j) {
			img_zero_pdd.at<cv::Vec3b>(j + window_h, i + window_h)[0] = img.at<cv::Vec3b>(j, i)[0];
			img_zero_pdd.at<cv::Vec3b>(j + window_h, i + window_h)[1] = img.at<cv::Vec3b>(j, i)[1];
			img_zero_pdd.at<cv::Vec3b>(j + window_h, i + window_h)[2] = img.at<cv::Vec3b>(j, i)[2];
		}
	}


	for (int i = 0; i < img_cols - 1; ++i) {

		for (int j = 0; j < img_rows - 1; ++j) {

			std::vector<uchar> B(0), G(0), R(0);
			for (int k = 0; k < window_size; ++k) {
				for (int l = 0; l < window_size; ++l) {
					B.push_back(img_zero_pdd.at<cv::Vec3b>(j + l, i + k)[0]);
					G.push_back(img_zero_pdd.at<cv::Vec3b>(j + l, i + k)[1]);
					R.push_back(img_zero_pdd.at<cv::Vec3b>(j + l, i + k)[2]);
				}
			}
			insertionSort(B, window_size*window_size);
			insertionSort(G, window_size*window_size);
			insertionSort(R, window_size*window_size);

			img.at<cv::Vec3b>(j, i)[0] = B[(window_size*window_size) / 2];
			img.at<cv::Vec3b>(j, i)[1] = G[(window_size*window_size) / 2];
			img.at<cv::Vec3b>(j, i)[2] = R[(window_size*window_size) / 2];
		}
	}
	return 1;
}

int main(int argc,char *argv[])
{
	int error_check = 0;
	cv::Mat image = cv::imread(argv[1]);
	error_check = medianFilter(image);
	if (error_check == 1) {
		std::cout << "Median filter processing completed.\n";
		cv::imshow("", image);
		cv::waitKey(0);
	}
	if (error_check == -1) {
		std::cout << "Median filter processing failed.\n";
	}

	system("pause");
	return 0;
}