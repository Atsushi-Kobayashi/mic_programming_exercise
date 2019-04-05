#define _USE_MATH_DEFINES
#include <opencv2/opencv.hpp>
#include"filters.h"
#include<iostream>
#include<vector>
#include<cmath>

//normal direction����邽�߂�Opencv_sobel�����ǂ�������

//���������̃G�b�W���o�p�J�[�l��
std::vector<double> horizontalSobelKernel() {
	return std::vector<double> {
		-1, 0, 1,
			-2, 0, 2,
			-1, 0, 1
	};
}

//���������̃G�b�W���o�p�J�[�l��
std::vector<double> verticalSobelKernel() {
	return std::vector<double> {
		-1, -2, -1,
			0, 0, 0,
			1, 2, 1
	};
}

cv::Mat GrayConvertedFromBGR(cv::Mat &BGR_img) {

	const int img_rows = BGR_img.rows;
	const int img_cols = BGR_img.cols;
	cv::Mat gray_img = cv::Mat::zeros(img_cols, img_rows, CV_8U);

	for (int j = 0; j < img_rows; ++j) {
		cv::Vec3b *BGR_img_row_ptr = BGR_img.ptr<cv::Vec3b>(j);
		uchar *gray_img_row_ptr = gray_img.ptr<uchar>(j);
		for (int i = 0; i < img_cols; ++i) {
			gray_img_row_ptr[i] =
				BGR_img_row_ptr[i][0] * 0.144 +
				BGR_img_row_ptr[i][1] * 0.587 +
				BGR_img_row_ptr[i][2] * 0.299;
		}
	}
	return gray_img;
}

//img_tmp�Ɋi�[���ꂽ�l��ucahr�^��[0,255]�ɃX�P�[�����O����gray_img�ɑ������֐�
void substituteScaledValue(cv::Mat &gray_img, std::vector<double> img_tmp, double max, double min) {

	const int img_rows = gray_img.rows;
	const int img_cols = gray_img.cols;
	for (int j = 0; j < img_rows; ++j) {
		uchar *img_row_ptr = gray_img.ptr<uchar>(j);
		for (int i = 0; i < img_cols; ++i) {
			img_row_ptr[i] = convertToUchar(255 * (img_tmp[i + j * img_cols] - min) / (max - min));
		}
	}
}


std::vector<double> sobelConvolutionValues(cv::Mat &gray_img, std::vector<double> kernel, int kernel_size) {
	const int img_rows = gray_img.rows;
	const int img_cols = gray_img.cols;

	const int kernel_h = (kernel_size - 1) / 2;
	double max = 0, min = 0;

	//vector �͂��炩���߃T�C�Y���w�肵�Ă��ƃ������̈�̂Ƃ蒼�����Ȃ��čς݁C��荂��
	std::vector<double> img_tmp(img_rows*img_cols);

	for (int j = 0; j < img_rows; ++j) {

		for (int i = 0; i < img_cols; ++i) {
			double conv_sum = 0;
			for (int k = -kernel_h; k < kernel_h + 1; ++k) {
				if (j + k < 0 || j + k >= img_rows) {
					continue;
				}
				uchar *img_row_ptr = gray_img.ptr<uchar>(j + k);
				for (int l = -kernel_h; l < kernel_h + 1; ++l) {
					if (i + l < 0 || i + l >= img_cols) {
						continue;
					}
					conv_sum += (double)img_row_ptr[i + l] * kernel[(kernel_h + l) + (kernel_h + k)*kernel_size];
					//std::cout << (double)img_row_ptr[i + l] * kernel[(kernel_h + l) + (kernel_h + k)*kernel_size] << " :conv value ("<<l+1<<","<<k+1<<")\n";
				}
			}

			//��ݍ��݌��ʂ̐�Βl���Ƃ�
			/*if (conv_sum < 0) {
				conv_sum = -conv_sum;
			}
*/
			double conv_sum_abs = std::abs(conv_sum);
			//�X�P�[�����O�ɕK�v�ȍő�l�C�ŏ��l��ۑ�
			if (conv_sum_abs >= max) {
				max = conv_sum_abs;
			}
			if (conv_sum_abs <= min) {
				min = conv_sum_abs;
			}

			//img�̉�f(i,j)�̌v�Z���ʂ�img_dst[i+j*img_cols]�Ɋi�[�����
			img_tmp[i + j * img_cols] = conv_sum;


		}
	}

	//�X�P�[�����O����gray_img�ɑ��
	//substituteScaledValue(gray_img, img_tmp, max, min);


	//img_tmp�̊e�l��[-1,1]�͈̔͂ɃX�P�[�����O
	for (int i = 0; i < img_cols*img_rows; ++i) {
		if (img_tmp[i] >= 0) {
			img_tmp[i] = (img_tmp[i] - min) / (max - min);
		}
		else {
			img_tmp[i] = -(-img_tmp[i] - min) / (max - min);
		}
	}
	return img_tmp;
}


void sobelFilter(cv::Mat &gray_img, std::vector<uchar> &normal_direction) {
	const int kernel_size = 3;
	const int img_cols = gray_img.cols;
	const int img_rows = gray_img.rows;

	std::vector<double> img_fused_tmp(img_rows*img_cols);
	double fused_sobel_max = 0, fused_sobel_min = 0;

	std::vector<double> horizontal_kernel = horizontalSobelKernel();
	std::vector<double> vertical_kernel = verticalSobelKernel();

	std::vector<double> horizontal_sobel_values(img_rows*img_cols);
	std::vector<double> vertical_sobel_values(img_rows*img_cols);
	
	//����������sobel������gray_img�Ɏ��s
	horizontal_sobel_values = sobelConvolutionValues(gray_img, horizontal_kernel, kernel_size);
	

	//����������sobel������gray_img�Ɏ��s
	vertical_sobel_values = sobelConvolutionValues(gray_img, vertical_kernel, kernel_size);
	
	for (int i = 0; i < img_cols*img_rows; ++i) {

		//�����E�����̏������ʂ̓��a���������Ƃ�Cimg_fused_tmp�Ɋi�[
		img_fused_tmp[i] = pow(horizontal_sobel_values[i] * horizontal_sobel_values[i]
			+ vertical_sobel_values[i] * vertical_sobel_values[i], 0.5);

		//�G�b�W�̖@��������normal_direction(4����)�Ɋi�[
		double theta = 2; //y�������ɑΉ�����2�ŏ�����

		//x�����̌��z��0�Ȃ�Ζ@��������2�̂܂܁A����ȊO�̏ꍇ���ȉ�if�����ŏ���
		if (horizontal_sobel_values[i] != 0.0) {
			//memo: atan2��[-PI,PI]�͈̔͂Œl��Ԃ�
			theta = std::atan2(vertical_sobel_values[i], horizontal_sobel_values[i]);
			//�݂��ɋt�����̖@�������𓯈�Ƃ��Ĉ����̂ŁAtheta�����Ȃ�t�����ɂ��ďꍇ�����������Ă悢
			if (theta < 0) {
				theta += M_PI;
			}

			//����0: [0, +PI/8),[+PI*7/8,+PI] x������
			if (theta < M_PI / 8.0 || theta >= M_PI * 7.0 / 8.0) {
				theta = 0;
			}
			//����1: [PI/8, PI*3/8)
			if (theta >= M_PI / 8.0&&theta < M_PI * 3.0 / 8.0) {
				theta = 1;
			}
			//����2: [PI*3/8, PI*5/8) y������
			if (theta > M_PI*3.0 / 8.0&&theta <= M_PI * 5.0 / 8.0) {
				theta = 2;
			}
			//����3: [PI*5/8, PI*7/8)
			if (theta > M_PI *5.0 / 8.0&&theta <= M_PI * 7.0 / 8.0) {
				theta = 3;
			}
		}

		normal_direction.emplace_back((uchar)theta);

		//�X�P�[�����O�ɕK�v�ȍő�l�C�ŏ��l��ۑ�
		if (img_fused_tmp[i] >= fused_sobel_max) {
			fused_sobel_max = img_fused_tmp[i];
		}
		if (img_fused_tmp[i] <= fused_sobel_min) {
			fused_sobel_min = img_fused_tmp[i];
		}
	}

	//���������l���X�P�[�����O����gray_img�ɑ��
	substituteScaledValue(gray_img, img_fused_tmp, fused_sobel_max, fused_sobel_min);

}
