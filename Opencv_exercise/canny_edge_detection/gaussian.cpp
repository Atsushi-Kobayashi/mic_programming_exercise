#include <opencv2/opencv.hpp>
#include"filters.h"
#include<iostream>
#include<vector>
#include<cmath>
constexpr int gaussian_error = -1;

uchar convertToUchar(double value) {
	if (value < 0) {
		return 0;
	}
	else if (value > 255) {
		return 255;
	}
	//�}�C�i�X�l����уI�[�o�[�t���[�̑΍�
	return (uchar)value;
}

std::vector<double> gaussianKernel(int kernel_size, double var) {

	std::vector<double> kernel;
	double kernel_value_sum = 0;
	for (int j = 0; j < kernel_size; ++j) {
		for (int i = 0; i < kernel_size; ++i) {
			double kernel_value = 0;
			int sq_dst_form_center = (i - (kernel_size - 1) / 2)*(i - (kernel_size - 1) / 2) + (j - (kernel_size - 1) / 2)*(j - (kernel_size - 1) / 2);
			kernel_value = exp(-(double)sq_dst_form_center / (2 * var));
			kernel.emplace_back(kernel_value);
			kernel_value_sum += kernel_value;
		}
	}
	for (int j = 0; j < kernel_size; ++j) {
		for (int i = 0; i < kernel_size; ++i) {
			kernel[i + kernel_size * j] = kernel[i + kernel_size * j] / kernel_value_sum;
		}
	}
	//���K��

	return kernel;
}

void linearFilter(cv::Mat &img, std::vector<double> kernel, int kernel_size) {
	int img_rows = img.rows;
	int img_cols = img.cols;
	int kernel_h = (kernel_size - 1) / 2;
	/*cv::Mat img_zero_pdd = cv::Mat::zeros(img_cols + (2 * kernel_h), img_rows + (2 * kernel_h), CV_8UC3);

	for (int j = 0; j < img_rows; ++j) {
		cv::Vec3b *row_ptr = img_zero_pdd.ptr<cv::Vec3b>(j + kernel_h);
		for (int i = 0; i < img_cols; ++i) {
			row_ptr[i + kernel_h] = cv::Vec3b(img.at<cv::Vec3b>(j, i)[0], img.at<cv::Vec3b>(j, i)[1], img.at<cv::Vec3b>(j, i)[2]);
		}
	}*/

	//mirroring �Ńp�f�B���O
	cv::Mat img_mirror_pdd = cv::Mat::zeros(img_cols + (2 *kernel_h), img_rows + (2 * kernel_h), CV_8UC3);
	for (int j = -kernel_h; j < img_rows + kernel_h; ++j) {
		cv::Vec3b *pdd_row_ptr = img_mirror_pdd.ptr<cv::Vec3b>(j + kernel_h);
		int k = j;
		if (j < 0) {
			k = -j - 1;
		}
		else if (j >= img_rows) {
			k = 2 * img_rows - 1 - j;
		}
		cv::Vec3b *img_row_ptr = img.ptr<cv::Vec3b>(k);
		for (int i = -kernel_h; i < img_cols + kernel_h; ++i) {
			int l = i;
			if (i < 0) {
				l = -i - 1;
			}
			else if (i >= img_cols) {
				l = 2 * img_cols - 1 - i;
			}

			pdd_row_ptr[i + kernel_h] = cv::Vec3b(img_row_ptr[l][0], img_row_ptr[l][1], img_row_ptr[l][2]);
		}
	}
	//cv::imshow("", img_mirror_pdd);
	for (int j = 0; j < img_rows; ++j) {
		cv::Vec3b *img_row_ptr = img.ptr<cv::Vec3b>(j);
		//img�i���H����摜�j��Mat��j�s�ڂ̃|�C���^�擾

		for (int i = 0; i < img_cols; ++i) {
			double B = 0, G = 0, R = 0;
			for (int l = 0; l < kernel_size; ++l) {
				cv::Vec3b *pdd_row_ptr = img_mirror_pdd.ptr<cv::Vec3b>(j + l);
				//pdd�i�p�f�B���O�ς݉摜�C���������Ȃ��j��Mat��j�s�ڂ̃|�C���^�擾
				for (int k = 0; k < kernel_size; ++k) {

					B += (double)pdd_row_ptr[i + k][0] * kernel[k + kernel_size * l];
					G += (double)pdd_row_ptr[i + k][1] * kernel[k + kernel_size * l];
					R += (double)pdd_row_ptr[i + k][2] * kernel[k + kernel_size * l];
				}
			}

			img_row_ptr[i][0] = convertToUchar(B);
			img_row_ptr[i][1] = convertToUchar(G);
			img_row_ptr[i][2] = convertToUchar(R);

		}
	}

}

int gaussianFilter(cv::Mat &img) {
	int kernel_size=3;
	double var = 1.3;
/*
	std::cout << "Input kernel size(odd): \n";
	std::cin >> kernel_size;
	if (kernel_size % 2 == 0 || kernel_size <= 0) {
		std::cout << "Size error.\n";
		return gaussian_error;
	}

	std::cout << "Input variance: " << "\n";
	std::cin >> var;
	if (var <= 0) {
		std::cout << "Variance error.\n";
		return gaussian_error;
	}*/

	std::vector<double> kernel = gaussianKernel(kernel_size, var);
	linearFilter(img, kernel, kernel_size);
	return 1;
}
