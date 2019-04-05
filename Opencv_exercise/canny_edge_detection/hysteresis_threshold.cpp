#include <opencv2/opencv.hpp>
#include"filters.h"
#include<iostream>
#include<vector>

void hysteresisThreshold(cv::Mat &img_edge,uchar high_threshold, uchar low_threshold) {
	const int img_rows = img_edge.rows;
	const int img_cols = img_edge.cols;
	std::vector<int> strong_edge_index;

	for (int j = 0; j < img_rows; ++j) {
		uchar *img_row_ptr = img_edge.ptr<uchar>(j);
		for (int i = 0; i < img_cols; ++i) {
			if (img_row_ptr[i] < low_threshold) {
				img_row_ptr[i] = 0;
			}
			else if (img_row_ptr[i] >= low_threshold && img_row_ptr[i] <= high_threshold) {
				//low以上high以下
				img_row_ptr[i] = 100;
			}
			else if (img_row_ptr[i] > high_threshold) {
				img_row_ptr[i] = 255;
				//強いエッジのある場所のインデックスを保管する
				strong_edge_index.emplace_back(i + j * img_cols);
			}
		}

	}

	while (!strong_edge_index.empty()) {

		int row_index_j = strong_edge_index[strong_edge_index.size()-1] / img_cols;
		int col_index_i = strong_edge_index[strong_edge_index.size()-1] % img_cols;

		strong_edge_index.pop_back();

		for (int k = -1; k < 2; ++k) {
			if (row_index_j + k < 0 || row_index_j + k >= img_rows) {
				continue;
			}
			uchar *img_row_ptr = img_edge.ptr<uchar>(row_index_j + k);
			for (int l = -1; l < 2; ++l) {
				if (col_index_i + l < 0 || col_index_i >= img_cols) {
					continue;
				}

				if (img_row_ptr[col_index_i + l] == 100) {
					img_row_ptr[col_index_i + l] = 255;
					//std::cout<< col_index_i + l << ", " << row_index_j + k << "\n";
					strong_edge_index.emplace_back(col_index_i + l+(row_index_j + k)*img_cols);
				}
			}
		}

	}

	for (int j = 0; j < img_rows; ++j) {
		uchar *img_row_ptr = img_edge.ptr<uchar>(j);
		for (int i = 0; i < img_cols; ++i) {
			if (img_row_ptr[i]==100) {
				img_row_ptr[i] = 0;
			}
		}

	}
}