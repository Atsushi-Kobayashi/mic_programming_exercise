#include <opencv2/opencv.hpp>
#include<iostream>
#include<vector>
#include<cmath>


//double ->uchar�̃}�C�i�X�l����уI�[�o�[�t���[�̑΍�
uchar convertToUchar(double value) {
	if (value < 0) {
		return 0;
	}
	else if (value > 255) {
		return 255;
	}

	return (uchar)value;
}

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

//img_tmp�Ɋi�[���ꂽ�l���X�P�[�����O����gray_img�ɑ������֐�
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

			//��ݍ��݌��ʂ̐�Βl���Ƃ�
			if (conv_sum < 0) {
				conv_sum = -conv_sum;
			}

			//�X�P�[�����O�ɕK�v�ȍő�l�C�ŏ��l��ۑ�
			if (conv_sum >= max) {
				max = conv_sum;
			}
			if (conv_sum <= min) {
				min = conv_sum;
			}

			//img�̉�f(i,j)�̌v�Z���ʂ�img_dst[i+j*img_cols]�Ɋi�[�����
			img_tmp.emplace_back(conv_sum);

		}
	}

	//�X�P�[�����O����gray_img�ɑ��
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

	//����������sobel������gray_img�Ɏ��s���C���ʂ�\��
	sobelConvolution(gray_img, horizontal_kernel, kernel_size);
	cv::imshow("horizontal sobel", gray_img);
	cv::waitKey(0);

	//����������sobel������gray_img_clone�Ɏ��s���C���ʂ�\��
	sobelConvolution(gray_img_clone, vertical_kernel, kernel_size);
	cv::imshow("vertical sobel", gray_img_clone);
	cv::waitKey(0);

	//�����E�����̏������ʂ̍���
	for (int j = 0; j < img_rows; ++j) {
		uchar *img_hrz_row_ptr = gray_img.ptr<uchar>(j);
		uchar *img_vrt_row_ptr = gray_img_clone.ptr<uchar>(j);
		for (int i = 0; i < img_cols; ++i) {

			//�����E�����̏������ʂ̓��a���������Ƃ�Cimg_fused_tmp�Ɋi�[
			img_fused_tmp.emplace_back(
				(double)pow(img_hrz_row_ptr[i] * img_hrz_row_ptr[i]
					+ img_vrt_row_ptr[i] * img_vrt_row_ptr[i], 0.5));

			//�X�P�[�����O�ɕK�v�ȍő�l�C�ŏ��l��ۑ�
			if (img_fused_tmp[i + j * img_cols] >= fused_sobel_max) {
				fused_sobel_max = img_fused_tmp[i + j * img_cols];
			}
			if (img_fused_tmp[i + j * img_cols] <= fused_sobel_min) {
				fused_sobel_min = img_fused_tmp[i + j * img_cols];
			}
		}
	}

	//���������l���X�P�[�����O����gray_img�ɑ��
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