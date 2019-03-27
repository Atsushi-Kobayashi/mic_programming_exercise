#include <opencv2/opencv.hpp>
#include<iostream>
#include<vector>
#include<cmath>


//double ->ucharのマイナス値およびオーバーフローの対策
uchar convertToUchar(double value) {
	if (value < 0) {
		return 0;
	}
	else if (value > 255) {
		return 255;
	}

	return (uchar)value;
}

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

	int img_rows = BGR_img.rows;
	int img_cols = BGR_img.cols;
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

//img_tmpに格納された値をスケーリングしてgray_imgに代入する関数
void substituteScaledValue(cv::Mat &gray_img, std::vector<double> img_tmp, double max, double min) {

	int img_rows = gray_img.rows;
	int img_cols = gray_img.cols;
	for (int j = 0; j < img_rows; ++j) {
		uchar *img_row_ptr = gray_img.ptr<uchar>(j);
		for (int i = 0; i < img_cols; ++i) {
			img_row_ptr[i] = convertToUchar(255 * (img_tmp[i + j * img_cols] - min) / (max - min));
		}
	}
}

void sobelConvolution(cv::Mat &gray_img, std::vector<double> kernel, int kernel_size) {
	int img_rows = gray_img.rows;
	int img_cols = gray_img.cols;

	int kernel_h = (kernel_size - 1) / 2;
	double max = 0, min = 0;
	std::vector<double> img_tmp = {};

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
			if (conv_sum < 0) {
				conv_sum = -conv_sum;
			}

			//スケーリングに必要な最大値，最小値を保存
			if (conv_sum >= max) {
				max = conv_sum;
			}
			if (conv_sum <= min) {
				min = conv_sum;
			}

			//imgの画素(i,j)の計算結果がimg_dst[i+j*img_cols]に格納される
			img_tmp.emplace_back(conv_sum);

		}
	}

	//スケーリングしてgray_imgに代入
	substituteScaledValue(gray_img, img_tmp, max, min);
}


void sobelFilter(cv::Mat &gray_img) {
	int kernel_size = 3;
	int img_cols = gray_img.cols;
	int img_rows = gray_img.rows;

	std::vector<double> img_fused_tmp = {};
	double fused_sobel_max = 0, fused_sobel_min = 0;

	std::vector<double> horizontal_kernel = horizontalSobelKernel();
	std::vector<double> vertical_kernel = verticalSobelKernel();

	cv::Mat gray_img_clone = gray_img.clone();

	//水平方向のsobel処理をgray_imgに実行し，結果を表示
	sobelConvolution(gray_img, horizontal_kernel, kernel_size);
	cv::imshow("horizontal sobel", gray_img);
	cv::waitKey(0);

	//鉛直方向のsobel処理をgray_img_cloneに実行し，結果を表示
	sobelConvolution(gray_img_clone, vertical_kernel, kernel_size);
	cv::imshow("vertical sobel", gray_img_clone);
	cv::waitKey(0);

	//水平・鉛直の処理結果の合成
	for (int j = 0; j < img_rows; ++j) {
		uchar *img_hrz_row_ptr = gray_img.ptr<uchar>(j);
		uchar *img_vrt_row_ptr = gray_img_clone.ptr<uchar>(j);
		for (int i = 0; i < img_cols; ++i) {

			//水平・鉛直の処理結果の二乗和平方根をとり，img_fused_tmpに格納
			img_fused_tmp.emplace_back(
				(double)pow(img_hrz_row_ptr[i] * img_hrz_row_ptr[i]
					+ img_vrt_row_ptr[i] * img_vrt_row_ptr[i], 0.5));

			//スケーリングに必要な最大値，最小値を保存
			if (img_fused_tmp[i + j * img_cols] >= fused_sobel_max) {
				fused_sobel_max = img_fused_tmp[i + j * img_cols];
			}
			if (img_fused_tmp[i + j * img_cols] <= fused_sobel_min) {
				fused_sobel_min = img_fused_tmp[i + j * img_cols];
			}
		}
	}

	//合成した値をスケーリングしてgray_imgに代入
	substituteScaledValue(gray_img, img_fused_tmp, fused_sobel_max, fused_sobel_min);

}

int main(int argc, char *argv[])
{
	cv::Mat image = cv::imread(argv[1]);
	cv::imshow("original", image);
	cv::waitKey(0);

	cv::Mat gray = GrayConvertedFromBGR(image);
	cv::imshow("gray", gray);
	cv::waitKey(0);

	sobelFilter(gray);
	cv::imshow("fused sobel", gray);
	cv::waitKey(0);

	return 0;
}