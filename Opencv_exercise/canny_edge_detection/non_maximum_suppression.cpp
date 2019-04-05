#include <opencv2/opencv.hpp>
#include"filters.h"
#include<iostream>
#include<vector>

void nonMaximumSuppression(cv::Mat &sobel_img, std::vector<uchar> &normal_direction) {
	const int img_rows = sobel_img.rows;
	const int img_cols = sobel_img.cols;


	for (int j = 0; j < img_rows; ++j) {

		uchar *img_row_ptr = sobel_img.ptr<uchar>(j);

		for (int i = 0; i < img_cols; ++i) {
			uchar next_pixel_value_1 = 0, next_pixel_value_2 = 0;
			if (normal_direction[i + j * img_cols] == 0) {
				if (i - 1 >= 0) {
					next_pixel_value_1 = img_row_ptr[i - 1];
				}
				if (i + 1 < img_cols) {
					next_pixel_value_2 = img_row_ptr[i + 1];
				}
			}
			else if (normal_direction[i + j * img_cols] == 1) {
				if (i - 1 >= 0 && j - 1 >= 0) {
					//疑問：やむなくpixelごとに行ポインタ呼び出しているが、遅い？
					uchar *img_prev_row_ptr = sobel_img.ptr<uchar>(j - 1);
					next_pixel_value_1 = img_prev_row_ptr[i - 1];
				}
				if (i + 1 < img_cols&&j + 1 < img_rows) {
					uchar *img_next_row_ptr = sobel_img.ptr<uchar>(j + 1);
					next_pixel_value_2 = img_next_row_ptr[i + 1];
				}

			}
			else if (normal_direction[i + j * img_cols] == 2) {
				if (j - 1 >= 0) {
					uchar *img_prev_row_ptr = sobel_img.ptr<uchar>(j - 1);
					next_pixel_value_1 = img_prev_row_ptr[i];
				}
				if (j + 1 < img_rows) {
					uchar *img_next_row_ptr = sobel_img.ptr<uchar>(j + 1);
					next_pixel_value_2 = img_next_row_ptr[i];
				}
			}
			else if (normal_direction[i + j * img_cols] == 3) {
				if (i + 1 < img_cols && j - 1 >= 0) {
					uchar *img_prev_row_ptr = sobel_img.ptr<uchar>(j - 1);
					next_pixel_value_1 = img_prev_row_ptr[i + 1];
				}
				if (i - 1 >= 0 && j + 1 < img_rows) {
					uchar *img_next_row_ptr = sobel_img.ptr<uchar>(j + 1);
					next_pixel_value_2 = img_next_row_ptr[i - 1];
				}
			}
			
			if (img_row_ptr[i] < next_pixel_value_1 || img_row_ptr[i] < next_pixel_value_2) {
				img_row_ptr[i] = 0;
			}

		}
	}
}