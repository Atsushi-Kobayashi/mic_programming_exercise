#define _USE_MATH_DEFINES
#include <opencv2/opencv.hpp>
#include"filters.h"
#include<iostream>
#include<vector>
#include<cmath>

//normal directionを取るためにOpencv_sobelを改良したもの

//水平方向のエッジ検出用カーネル
std::vector<double> horizontalSobelKernel() {
	return std::vector<double> {
		-1, 0, 1,
			-2, 0, 2,
			-1, 0, 1
	};
}

//鉛直方向のエッジ検出用カーネル
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

//img_tmpに格納された値をucahr型の[0,255]にスケーリングしてgray_imgに代入する関数
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

	//vector はあらかじめサイズを指定してやるとメモリ領域のとり直ししなくて済み，より高速
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

			//畳み込み結果の絶対値をとる
			/*if (conv_sum < 0) {
				conv_sum = -conv_sum;
			}
*/
			double conv_sum_abs = std::abs(conv_sum);
			//スケーリングに必要な最大値，最小値を保存
			if (conv_sum_abs >= max) {
				max = conv_sum_abs;
			}
			if (conv_sum_abs <= min) {
				min = conv_sum_abs;
			}

			//imgの画素(i,j)の計算結果がimg_dst[i+j*img_cols]に格納される
			img_tmp[i + j * img_cols] = conv_sum;


		}
	}

	//スケーリングしてgray_imgに代入
	//substituteScaledValue(gray_img, img_tmp, max, min);


	//img_tmpの各値を[-1,1]の範囲にスケーリング
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
	
	//水平方向のsobel処理をgray_imgに実行
	horizontal_sobel_values = sobelConvolutionValues(gray_img, horizontal_kernel, kernel_size);
	

	//鉛直方向のsobel処理をgray_imgに実行
	vertical_sobel_values = sobelConvolutionValues(gray_img, vertical_kernel, kernel_size);
	
	for (int i = 0; i < img_cols*img_rows; ++i) {

		//水平・鉛直の処理結果の二乗和平方根をとり，img_fused_tmpに格納
		img_fused_tmp[i] = pow(horizontal_sobel_values[i] * horizontal_sobel_values[i]
			+ vertical_sobel_values[i] * vertical_sobel_values[i], 0.5);

		//エッジの法線方向をnormal_direction(4方向)に格納
		double theta = 2; //y軸方向に対応する2で初期化

		//x方向の勾配が0ならば法線方向は2のまま、それ以外の場合を以下if文内で処理
		if (horizontal_sobel_values[i] != 0.0) {
			//memo: atan2は[-PI,PI]の範囲で値を返す
			theta = std::atan2(vertical_sobel_values[i], horizontal_sobel_values[i]);
			//互いに逆向きの法線方向を同一として扱うので、thetaが負なら逆向きにして場合分け処理してよい
			if (theta < 0) {
				theta += M_PI;
			}

			//方向0: [0, +PI/8),[+PI*7/8,+PI] x軸方向
			if (theta < M_PI / 8.0 || theta >= M_PI * 7.0 / 8.0) {
				theta = 0;
			}
			//方向1: [PI/8, PI*3/8)
			if (theta >= M_PI / 8.0&&theta < M_PI * 3.0 / 8.0) {
				theta = 1;
			}
			//方向2: [PI*3/8, PI*5/8) y軸方向
			if (theta > M_PI*3.0 / 8.0&&theta <= M_PI * 5.0 / 8.0) {
				theta = 2;
			}
			//方向3: [PI*5/8, PI*7/8)
			if (theta > M_PI *5.0 / 8.0&&theta <= M_PI * 7.0 / 8.0) {
				theta = 3;
			}
		}

		normal_direction.emplace_back((uchar)theta);

		//スケーリングに必要な最大値，最小値を保存
		if (img_fused_tmp[i] >= fused_sobel_max) {
			fused_sobel_max = img_fused_tmp[i];
		}
		if (img_fused_tmp[i] <= fused_sobel_min) {
			fused_sobel_min = img_fused_tmp[i];
		}
	}

	//合成した値をスケーリングしてgray_imgに代入
	substituteScaledValue(gray_img, img_fused_tmp, fused_sobel_max, fused_sobel_min);

}
