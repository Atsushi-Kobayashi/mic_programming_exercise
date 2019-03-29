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

//vectorの中央値のインデックスを返す関数
int indexOfMedian(std::vector<uchar> vec) {
	std::vector<uchar> sorted_vec = vec;

	insertionSort(sorted_vec, sorted_vec.size());
	uchar median = sorted_vec[(sorted_vec.size() - 1) / 2];

	std::vector<uchar>::iterator itr_median = std::find(vec.begin(), vec.end(), median);
	size_t median_index = std::distance(vec.begin(), itr_median);

	return (int)median_index;
}

cv::Mat grayConvertedFromBGR(cv::Mat &BGR_img) {

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

int medianFilter(cv::Mat &img) {
	int window_size;
	std::cout << "Input window size: \n";
	std::cin >> window_size;
	if (window_size <= 1 || window_size % 2 == 0) {
		return median_error;
	}

	int img_rows = img.rows;
	int img_cols = img.cols;
	int window_h = (window_size) / 2;

	//ミラーリングでパディング
	cv::Mat img_mirror_pdd = cv::Mat::zeros(img_cols + (2 * window_h), img_rows + (2 * window_h), CV_8UC3);

	for (int j = -window_h; j < img_rows+window_h; ++j) {
		cv::Vec3b *pdd_row_ptr = img_mirror_pdd.ptr<cv::Vec3b>(j + window_h);
		int k = j;
		if (j < 0) {
			k = -j - 1;
		}
		else if (j >= img_rows) {
			k = 2 * img_rows - 1 - j;
		}
		cv::Vec3b *img_row_ptr = img.ptr<cv::Vec3b>(k);
		for (int i = -window_h; i < img_cols+window_h; ++i) {
			int l = i;
			if (i < 0) {
				l = -i - 1;
			}
			else if (i >= img_cols) {
				l = 2 * img_cols - 1 - i;
			}

			pdd_row_ptr[i + window_h] = cv::Vec3b(img_row_ptr[l][0], img_row_ptr[l][1],img_row_ptr[l][2]);
		}
	}

	//パディングした画像をグレースケール化
	cv::Mat gray_img = grayConvertedFromBGR(img_mirror_pdd);
	
	//以下，imgの各画素の色をウィンドウ内の中央値をとる画素の色で書き換える処理
	for (int j = 0; j < img_rows - 1; ++j) {
		cv::Vec3b *img_row_ptr = img.ptr<cv::Vec3b>(j);

		//(i,j)の画素まわりのウィンドウ内部の値をvalues_in_windowに格納
		for (int i = 0; i < img_cols - 1; ++i) {
			std::vector<uchar> values_in_windouw = {};

			for (int k = 0; k < window_size; ++k) {
				uchar *gray_img_row_ptr = gray_img.ptr<uchar>(j + k);

				for (int l = 0; l < window_size; ++l) {

					values_in_windouw.emplace_back(gray_img_row_ptr[i + l]);
				}
			}

			//values_in_windowの中央値があるインデックスを取得,この画素のimg_pdd_mirrorでの位置を計算
			int median_index_in_values = indexOfMedian(values_in_windouw);
			int median_row_index = j + (median_index_in_values/window_size);
			int median_col_index = i+ (median_index_in_values % window_size);

			//imgの(i,j)の色を書き換え
			cv::Vec3b *img_pdd_row_ptr = img_mirror_pdd.ptr<cv::Vec3b>(median_row_index);
			cv::Vec3b *img_row_ptr = img.ptr<cv::Vec3b>(j);
			img_row_ptr[i][0] = img_pdd_row_ptr[median_col_index][0];
			img_row_ptr[i][1] = img_pdd_row_ptr[median_col_index][1];
			img_row_ptr[i][2] = img_pdd_row_ptr[median_col_index][2];
		}
	}
	return 1;
}

int main(int argc, char *argv[])
{
	int error_check = 0;
	cv::Mat image = cv::imread(argv[1]);
	cv::imshow("original", image);
	cv::waitKey(0);

	error_check = medianFilter(image);
	if (error_check == 1) {
		std::cout << "Median filter processing completed.\n";
		cv::imshow("median", image);
		cv::waitKey(0);
	}
	if (error_check == -1) {
		std::cout << "Median filter processing failed.\n";
	}

	system("pause");
	return 0;
}