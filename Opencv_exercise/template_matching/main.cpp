#include <opencv2/opencv.hpp>
#include<cmath>
double ZeroMeansNormalizedCrossCorrelation(int row_index_j, int col_index_i, cv::Mat &img, cv::Mat &tml, int img_mean, int tml_mean) {
	int img_tml_rows = tml.rows;
	int img_tml_cols = tml.cols;
	double img_var = 0, tml_var = 0, covariance = 0;
	//Ç±ÇÃ"covariance"ÇÕê≥ämÇ…ÇÕã§ï™éUÇÃóvëfêîî{

	for (int k = 0; k < img_tml_rows; ++k) {
		uchar *img_row_ptr = img.ptr<uchar>(row_index_j + k);
		uchar *tml_row_ptr = tml.ptr<uchar>(k);

		for (int l = 0; l < img_tml_cols; ++l) {
			covariance += (img_row_ptr[col_index_i + l] - img_mean)*(tml_row_ptr[l] - tml_mean);
			img_var += (img_row_ptr[col_index_i + l] - img_mean)*(img_row_ptr[col_index_i + l] - img_mean);
			tml_var += (tml_row_ptr[l] - tml_mean)*(tml_row_ptr[l] - tml_mean);
		}
	}

	return covariance / (pow(img_var*tml_var, 0.5));
}

std::vector<int> template_matching(cv::Mat &gray, cv::Mat &gray_tml) {
	int img_rows = gray.rows;
	int img_cols = gray.cols;
	int img_tml_rows = gray_tml.rows;
	int img_tml_cols = gray_tml.cols;
	double ZNCC_max = 0;
	std::vector<int> max_index = { 0,0 };
	int img_mean = 0;
	int tml_mean = 0;

	for (int j = 0; j < img_rows - img_tml_rows; ++j) {
		uchar *img_row_ptr = gray.ptr<uchar>(j);

		for (int i = 0; i < img_cols - img_tml_cols; ++i) {
			img_mean += img_row_ptr[i];
		}
	}
	img_mean /= img_cols * img_rows;

	for (int j = 0; j < img_tml_rows; ++j) {
		uchar *img_tml_row_ptr = gray_tml.ptr<uchar>(j);

		for (int i = 0; i < img_tml_cols; ++i) {
			tml_mean += img_tml_row_ptr[i];
		}
	}
	tml_mean /= img_tml_cols * img_tml_rows;

	for (int j = 0; j < img_rows - img_tml_rows; ++j) {

		for (int i = 0; i < img_cols - img_tml_cols; ++i) {
			double ZNCC = ZeroMeansNormalizedCrossCorrelation(j, i, gray, gray_tml, img_mean, tml_mean);
			if (ZNCC >= ZNCC_max) {
				ZNCC_max = ZNCC;
				max_index[0] = j;
				max_index[1] = i;
			}
		}
	}
	std::cout << ZNCC_max << "\n";
	return max_index;
}

int main(int argc, char *argv[])
{
	cv::Mat img = cv::imread(argv[1]);
	cv::imshow("original", img);
	cv::waitKey(0);

	cv::Mat img_template = cv::imread(argv[2]);
	cv::imshow("template", img_template);
	cv::waitKey(0);

	cv::Mat img_gray;
	cv::Mat img_tml_gray;
	cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);
	cv::cvtColor(img_template, img_tml_gray, cv::COLOR_BGR2GRAY);
	std::vector<int> matched_point = template_matching(img_gray, img_tml_gray);



	std::cout << "matched point: (" << matched_point[0] << ",";
	std::cout << matched_point[1] << ")\n";

	int tml_rows = img_tml_gray.rows;
	int tml_cols = img_tml_gray.cols;
	int x = matched_point[1];
	int y = matched_point[0];

	//cv::Mat img_matched_area = img_template.clone();
	//for (int j = 0; j < tml_rows; ++j) {
	//	cv::Vec3b *img_row_ptr = img.ptr<cv::Vec3b>(y + j);
	//	cv::Vec3b *matched_area_row_ptr = img_matched_area.ptr<cv::Vec3b>(j);

	//	for (int i = 0; i < tml_cols; ++i) {
	//		matched_area_row_ptr[i] = img_row_ptr[x + i];
	//	}
	//}

	//cv::imshow("matched area", img_matched_area);
	//cv::waitKey(0);

	for (int j = 0; j < tml_rows; ++j) {
		cv::Vec3b *img_row_ptr = img.ptr<cv::Vec3b>(y + j);

		for (int i = 0; i < tml_cols; ++i) {
			img_row_ptr[x + i][0] = 255 - img_row_ptr[x + i][0];
			img_row_ptr[x + i][1] = 255 - img_row_ptr[x + i][1];
			img_row_ptr[x + i][2] = 255 - img_row_ptr[x + i][2];
		}
	}

	cv::imshow("result", img);
	cv::waitKey(0);

	system("pause");
	return 0;
}