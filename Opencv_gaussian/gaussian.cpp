#include <opencv2/opencv.hpp>
#include<iostream>
#include<vector>
#include<cmath>

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

std::vector<double> gaussianKernel(int kernel_size,double var) {

	std::vector<double> kernel;
	double kernel_value_sum = 0;
	for (int j = 0; j < kernel_size; ++j) {
		for (int i = 0; i < kernel_size; ++i) {
			double kernel_value = 0;
			int sq_dst_form_center = (i-(kernel_size-1)/2)*(i - (kernel_size - 1) / 2)+ (j - (kernel_size - 1) / 2)*(j - (kernel_size - 1) / 2);
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
	//正規化

	return kernel;
}

void linearFilter(cv::Mat &img, std::vector<double> kernel, int kernel_size) {
	int img_rows = img.rows;
	int img_cols = img.cols;
	int kernel_h = (kernel_size - 1) / 2;
	cv::Mat img_zero_pdd = cv::Mat::zeros(img_cols + (2 * kernel_h), img_rows + (2 * kernel_h), CV_8UC3);
	
	for (int j = 0; j < img_rows; ++j) {
		cv::Vec3b *row_ptr = img_zero_pdd.ptr<cv::Vec3b>(j + kernel_h);
		for (int i = 0; i < img_cols; ++i) {
			row_ptr[i + kernel_h] = cv::Vec3b(img.at<cv::Vec3b>(j, i)[0], img.at<cv::Vec3b>(j, i)[1], img.at<cv::Vec3b>(j, i)[2]);
		}
	}
	//zero padding (Matの要素にポインタでアクセスする方法)

	for (int j = 0; j < img_rows; ++j) {
		cv::Vec3b *img_row_ptr = img.ptr<cv::Vec3b>(j);
		//img（加工する画像）のMatのj行目のポインタ取得

		for (int i = 0; i < img_cols; ++i) {
			double B = 0, G = 0, R = 0;
			for (int l = 0; l< kernel_size; ++l) {
				cv::Vec3b *pdd_row_ptr = img_zero_pdd.ptr<cv::Vec3b>(j + l);
				//pdd（ゼロパディング済み画像，書き換えない）のMatのj行目のポインタ取得
				for (int k = 0; k < kernel_size; ++k) {

					B += (double)pdd_row_ptr[i+k][0] * kernel[k + kernel_size * l];
					G += (double)pdd_row_ptr[i+k][1] * kernel[k + kernel_size * l];
					R += (double)pdd_row_ptr[i+k][2] * kernel[k + kernel_size * l];
				}
			}

			img_row_ptr[i][0] = convertToUchar(B);
			img_row_ptr[i][1] = convertToUchar(G);
			img_row_ptr[i][2] = convertToUchar(R);

		}
	}

}


// Comment by Kayukawa
// ここvoid型の関数じゃなくて、intとかbool型にして、Errorがあった時にErrorの値を返す関数を設計するのも便利です。
void gaussianFilter(cv::Mat &img) {
	int kernel_size;
	std::cout << "Input kernel size(odd): \n";
	std::cin >> kernel_size;
	if (kernel_size%2==0||kernel_size<=0) {
		std::cout << "Size error.\n";
		return;
	}
	double var = 1;
	std::cout << "Input variance: " << "\n";
	std::cin >> var;
	if (var<=0) {  // 分散0もダメなので修正
		std::cout << "Variance error.\n";
		return;
	}

	std::vector<double> kernel = gaussianKernel(kernel_size,var);
	linearFilter(img, kernel, kernel_size);
}

int main()
{

	cv::Mat image = cv::imread("..\\images\\color\\Lenna.bmp");
	gaussianFilter(image);
	cv::imshow("", image);
	cv::waitKey(0);

}
