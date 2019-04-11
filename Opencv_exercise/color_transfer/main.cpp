#include <opencv2/opencv.hpp>
#include "MyVec3d.h"
#include <cmath>

//���FBGR�ł͂Ȃ�RGB
const std::vector<double> matRGB2LMS = {
	0.3811,0.5783,0.0402,
	0.1967,0.7244,0.0782,
	0.0241, 0.1288,0.8444
};

const std::vector<double> mat1LMS2lab = {
	1,1,1,
	1,1,-2,
	1,-1,0
};

const std::vector<double> mat2LMS2lab = {
	1 / sqrt(3),0,0,
	0,1 / sqrt(6),0,
	0,0,1 / sqrt(2)
};

const std::vector<double> mat1lab2LMS = {
	sqrt(3) / 3,0,0,
	0,sqrt(6) / 6,0,
	0,0,sqrt(2) / 2
};

const std::vector<double> mat2lab2LMS = {
		1,1,1,
		1,1,-1,
		1,-2,0
};
const std::vector<double> matLMS2RGB = {
	4.4679, -3.5873, 0.1193,
	-1.2189,2.3809,-0.1624,
	0.0497,-0.2439,1.2045
};


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

void colorTransfer(cv::Mat &img_trg, cv::Mat &img_src) {
	const int trg_rows = img_trg.rows;
	const int trg_cols = img_trg.cols;
	const int src_rows = img_src.rows;
	const int src_cols = img_src.cols;
	double eps = 0.00001;

	std::vector<MyVec3d> trg_lab(trg_rows*trg_cols);
	std::vector<MyVec3d> src_lab(src_rows*src_cols);
	//trg�̕��ρA�W���΍����i�[���邽�߂̃x�N�g��
	MyVec3d trg_lab_avr;
	MyVec3d trg_lab_std_dvt;

	//src�̕��ρA�W���΍����i�[���邽�߂̃x�N�g��
	MyVec3d src_lab_avr;
	MyVec3d src_lab_std_dvt;

	//trg ��RGB����lab�ɂ���trg_lab�Ɋi�[
	for (int j = 0; j < trg_rows; ++j) {
		cv::Vec3b *trg_row_ptr = img_trg.ptr<cv::Vec3b>(j);
		for (int i = 0; i < trg_cols; ++i) {
			//RGB�̏��œ���
			MyVec3d pixel_color_3d = MyVec3d((double)trg_row_ptr[i][2] / 255, (double)trg_row_ptr[i][1] / 255, (double)trg_row_ptr[i][0] / 255);
			//RGB to LMS 
			pixel_color_3d.matProduct(matRGB2LMS);

			//OF�̊m�F�p
			/*if (pixel_color_3d.x < eps) {
				std::cout << pixel_color_3d.x << ": x too small\n";
				std::cout << std::log10(pixel_color_3d.x) << ": log10(x)\n";
			}
			if (pixel_color_3d.y < eps) {
				std::cout << pixel_color_3d.y << ": y too small\n";
				std::cout << std::log10(pixel_color_3d.y) << ": log10(y)\n";
			}
			if (pixel_color_3d.z < eps) {
				std::cout << pixel_color_3d.z << ": z too small\n";
				std::cout << std::log10(pixel_color_3d.z) << ": log10(z)\n";
			}*/

			////LMS ��log10���Ƃ�(OF�΍��max�Ƃ�)
			pixel_color_3d.x = std::log10(std::max(eps,pixel_color_3d.x));
			pixel_color_3d.y = std::log10(std::max(eps,pixel_color_3d.y));
			pixel_color_3d.z = std::log10(std::max(eps,pixel_color_3d.z));
			////LMS to lab
			pixel_color_3d.matProduct(mat1LMS2lab);
			pixel_color_3d.matProduct(mat2LMS2lab);
			//����ł��Ă�H
			trg_lab[i + j * trg_cols] = pixel_color_3d;
			//avr�ɒl��������
			trg_lab_avr = trg_lab_avr + pixel_color_3d;
		}
	}

	//avr��v�f���Ŋ���,scalarMultiple�̈����̃L���X�g����
	trg_lab_avr.scalarMultiple((double)1 / (trg_rows * trg_cols));


	//src ��RGB����lab�ɂ���src_lab�Ɋi�[
	for (int j = 0; j < src_rows; ++j) {
		cv::Vec3b *src_row_ptr = img_src.ptr<cv::Vec3b>(j);
		for (int i = 0; i < src_cols; ++i) {

			//RGB�̏��œ���
			MyVec3d pixel_color_3d = MyVec3d((double)src_row_ptr[i][2] / 255, (double)src_row_ptr[i][1] / 255, (double)src_row_ptr[i][0] / 255);
			//RGB to LMS 
			pixel_color_3d.matProduct(matRGB2LMS);
			//LMS ��log10���Ƃ�(OF�΍��eps�Ƃ�max���Ƃ�)
			pixel_color_3d.x = std::log10(std::max(eps,pixel_color_3d.x));
			pixel_color_3d.y = std::log10(std::max(eps,pixel_color_3d.y));
			pixel_color_3d.z = std::log10(std::max(eps,pixel_color_3d.z));
			//LMS to lab
			pixel_color_3d.matProduct(mat1LMS2lab);
			pixel_color_3d.matProduct(mat2LMS2lab);
			//����ł��Ă�H
			src_lab[i + j * src_cols] = pixel_color_3d;
			//avr�ɒl��������
			src_lab_avr = src_lab_avr + pixel_color_3d;
		}
	}
	//avr��v�f���Ŋ���,scalarMultiple�̈����̃L���X�g����
	src_lab_avr.scalarMultiple((double)1 / (src_rows * src_cols));

	//trg�̕W���΍����v�Z
	for (int i = 0; i < trg_cols*trg_rows; ++i) {
		trg_lab_std_dvt = trg_lab_std_dvt + (trg_lab[i] - trg_lab_avr)*(trg_lab[i] - trg_lab_avr);
	}
	trg_lab_std_dvt.x = pow(trg_lab_std_dvt.x / trg_lab.size(), 0.5);
	trg_lab_std_dvt.y = pow(trg_lab_std_dvt.y / trg_lab.size(), 0.5);
	trg_lab_std_dvt.z = pow(trg_lab_std_dvt.z / trg_lab.size(), 0.5);

	//src�̕W���΍����v�Z
	for (int i = 0; i < src_cols*src_rows; ++i) {
		src_lab_std_dvt = src_lab_std_dvt + (src_lab[i] - src_lab_avr)*(src_lab[i] - src_lab_avr);
	}
	src_lab_std_dvt.x = pow(src_lab_std_dvt.x / src_lab.size(), 0.5);
	src_lab_std_dvt.y = pow(src_lab_std_dvt.y / src_lab.size(), 0.5);
	src_lab_std_dvt.z = pow(src_lab_std_dvt.z / src_lab.size(), 0.5);


	//src��lab��trg�̓�����]�ʂ���
	for (int i = 0; i < src_cols*src_rows; ++i) {
		src_lab[i] = (src_lab[i] - src_lab_avr)*trg_lab_std_dvt / src_lab_std_dvt + trg_lab_avr;
	}

	//lab��RGB�ɖ߂�
	for (int j = 0; j < src_rows; ++j) {
		cv::Vec3b *src_row_ptr = img_src.ptr<cv::Vec3b>(j);
		for (int i = 0; i < src_cols; ++i) {
			MyVec3d pixel_color_3d = src_lab[i + j * src_cols];
			//lab to LMS
			pixel_color_3d.matProduct(mat1lab2LMS);
			pixel_color_3d.matProduct(mat2lab2LMS);
			//LMS ��10^���Ƃ�
			pixel_color_3d.x = std::max(eps,pow(10,pixel_color_3d.x));
			pixel_color_3d.y = std::max(eps,pow(10,pixel_color_3d.y));
			pixel_color_3d.z = std::max(eps,pow(10,pixel_color_3d.z));
			//LMS to RGB
			pixel_color_3d.matProduct(matLMS2RGB);
			//src ��Mat��RGB����
			src_row_ptr[i][2] = convertToUchar(255 * pixel_color_3d.x);
			src_row_ptr[i][1] = convertToUchar(255 * pixel_color_3d.y);
			src_row_ptr[i][0] = convertToUchar(255 * pixel_color_3d.z);
		}
	}

}

int main(int argc, char *argv[])
{
	cv::Mat img_src = cv::imread(argv[1]);
	cv::imshow("source image", img_src);
	cv::waitKey(0);

	cv::Mat img_trg = cv::imread(argv[2]);
	cv::imshow("target image", img_trg);
	cv::waitKey(0);

	colorTransfer(img_trg, img_src);
	cv::imshow("result", img_src);
	cv::waitKey(0);

	system("pause");
	return 1;
}