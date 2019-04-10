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

//���FBGR�ł͂Ȃ�RGB
//std::vector<double> matRGB2LMS() {
//	std::vector<double> mat = {
//	0.3811,0.5783,0.0402,
//	0.1967,0.7244,0.0782,
//	0.0241, 0.1288,0.8444
//	};
//	return mat;
//}
//
//std::vector<double> mat1LMS2lab() {
//	std::vector<double> mat = {
//	1,1,1,
//	1,1,-2,
//	1,-1,0
//	};
//	return mat;
//}
//
//std::vector<double> mat2LMS2lab() {
//	std::vector<double> mat = {
//	1/sqrt(3),0,0,
//	0,1/sqrt(6),0,
//	0,0,1/sqrt(2)
//	};
//	return mat;
//}
//
//std::vector<double> mat1lab2LMS() {
//	std::vector<double> mat = {
//	sqrt(3)/3,0,0,
//	0,sqrt(6)/6,0,
//	0,0,sqrt(2)/2
//	};
//	return mat;
//}
//
//std::vector<double> mat2lab2LMS() {
//	std::vector<double> mat = {
//		1,1,1,
//		1,1,-1,
//		1,-2,0
//	};
//	return mat;
//}
//
//std::vector<double> matLMS2RGB() {
//	std::vector<double> mat = {
//	4.4679, -3.5873, 0.1193,
//	-1.2189,2.3809,-0.1624,
//	0.0497,-0.2439,1.2045
//	};
//	return mat;
//}

void colorTransfer(cv::Mat &img_trg, cv::Mat &img_src) {
	const int trg_rows = img_trg.rows;
	const int trg_cols = img_trg.cols;
	const int src_rows = img_src.rows;
	const int src_cols = img_src.cols;
	float A = 0;

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

			//constructer �̎g��������ł悢�H
			//RGB�̏��œ���
			MyVec3d pixel_color_3d = MyVec3d((double)trg_row_ptr[i][2] / 255, (double)trg_row_ptr[i][1] / 255, (double)trg_row_ptr[i][0] / 255);
			//RGB to LMS 
			pixel_color_3d.matProduct(matRGB2LMS);
			////LMS ��log���Ƃ�
			pixel_color_3d.x = std::log(pixel_color_3d.x);
			pixel_color_3d.y = std::log(pixel_color_3d.y);
			pixel_color_3d.z = std::log(pixel_color_3d.z);
			////LMS to lab
			pixel_color_3d.matProduct(mat1LMS2lab);
			pixel_color_3d.matProduct(mat2LMS2lab);
			//����ł��Ă�H
			trg_lab[i + j * trg_cols] = pixel_color_3d;
			//avr�ɒl��������
			pixel_color_3d.scalarMultiple((double)1 / (trg_rows * trg_cols));

			//operater �������\������
			trg_lab_avr = trg_lab_avr + pixel_color_3d;
			A = A + pixel_color_3d.x;
		}
	}

	std::cout << A << "\n";
	//avr��v�f���Ŋ���,scalarMultiple�̈����̃L���X�g����
	//trg_lab_avr.scalarMultiple((double)1 / (trg_rows * trg_cols));


	//src ��RGB����lab�ɂ���src_lab�Ɋi�[
	for (int j = 0; j < src_rows; ++j) {
		cv::Vec3b *src_row_ptr = img_src.ptr<cv::Vec3b>(j);
		for (int i = 0; i < src_cols; ++i) {

			//constructer �̎g��������ł悢�H
			//RGB�̏��œ���
			MyVec3d pixel_color_3d = MyVec3d((double)src_row_ptr[i][2] / 255, (double)src_row_ptr[i][1] / 255, (double)src_row_ptr[i][0] / 255);
			//RGB to LMS 
			pixel_color_3d.matProduct(matRGB2LMS);
			//LMS ��log���Ƃ�
			pixel_color_3d.x = std::log(pixel_color_3d.x);
			pixel_color_3d.y = std::log(pixel_color_3d.y);
			pixel_color_3d.z = std::log(pixel_color_3d.z);
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

	////trg��lab��src�̓�����]�ʂ���
	for (int i = 0; i < trg_cols*trg_rows; ++i) {
		trg_lab[i] = (trg_lab[i] - trg_lab_avr)*trg_lab_std_dvt / src_lab_std_dvt + src_lab_avr;
	}

	//lab��RGB�ɖ߂�
	for (int j = 0; j < trg_rows; ++j) {
		cv::Vec3b *trg_row_ptr = img_trg.ptr<cv::Vec3b>(j);
		for (int i = 0; i < trg_cols; ++i) {
			MyVec3d pixel_color_3d = trg_lab[i + j * trg_cols];
			//lab to LMS
			pixel_color_3d.matProduct(mat1lab2LMS);
			pixel_color_3d.matProduct(mat2lab2LMS);
			//LMS ��exp���Ƃ�
			pixel_color_3d.x = std::exp(pixel_color_3d.x);
			pixel_color_3d.y = std::exp(pixel_color_3d.y);
			pixel_color_3d.z = std::exp(pixel_color_3d.z);
			//LMS to RGB
			pixel_color_3d.matProduct(matLMS2RGB);
			//trg ��Mat��RGB����
			trg_row_ptr[i][2] = convertToUchar(255 * pixel_color_3d.x);
			trg_row_ptr[i][1] = convertToUchar(255 * pixel_color_3d.y);
			trg_row_ptr[i][0] = convertToUchar(255 * pixel_color_3d.z);
		}
	}

}

int main(int argc, char *argv[])
{

	cv::Mat img_trg = cv::imread(argv[1]);
	cv::imshow("terget image", img_trg);
	cv::waitKey(0);

	cv::Mat img_src = cv::imread(argv[2]);
	cv::imshow("source image", img_src);
	cv::waitKey(0);

	//RGB->lab->RGB�͏o���Ă�
	colorTransfer(img_trg, img_src);
	cv::imshow("result", img_trg);
	cv::waitKey(0);

	system("pause");
	return 1;
}