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

void CvVec3bPointerTest(cv::Vec3b *ptr) {
	ptr[0][0] = (uchar)100;
	ptr[0][1] = (uchar)100;
	ptr[0][2] = (uchar)100;
	/*note：
	Vec3bのポインタ参照を
	cv::Vec3b *img_row_ptr = img.ptr<cv::Vec3b>(j);
	hogehoge(img_row_ptr[i]);
	とやったとき，関数内のptr[x][y]はimgのMatの画素(i+x,j)の第yチャンネルの値をさす
	->j行目の値ならこの関数内で変更できる
	*/
}

//std::vector<double> HSVConvertedFromBGR(cv::Vec3b *ptr) {
//	double hue, saturation, value;
//	if (ptr[0][0] == ptr[0][1] && ptr[0][1] == ptr[0][2]) {
//		hue = 0;
//		saturation = 0;
//		value = (double)ptr[0][0];
//	}
//	else if (ptr[0][0] >= ptr[0][1] && ptr[0][0] >= ptr[0][2]) {
//		if (ptr[0][1] >= ptr[0][2]) {
//			hue = 60 * (4 + (double)(ptr[0][1] - ptr[0][2]) / (ptr[0][0] - ptr[0][2]));
//			saturation = 255 * (double)(ptr[0][0] - ptr[0][2]) / ptr[0][0];
//		}
//		else {
//			hue = 60 * (4 + (double)(ptr[0][1] - ptr[0][2]) / (ptr[0][0] - ptr[0][1]));
//			saturation = 255 * (double)(ptr[0][0] - ptr[0][1]) / ptr[0][0];
//		}
//		value = (double)ptr[0][0];
//	}
//	else if (ptr[0][1] >= ptr[0][2] && ptr[0][1] >= ptr[0][0]) {
//		if (ptr[0][2] >= ptr[0][0]) {
//			hue = 60 * (2 + (double)(ptr[0][2] - ptr[0][0]) / (ptr[0][1] - ptr[0][0]));
//			saturation = 255 * (double)(ptr[0][1] - ptr[0][0]) / ptr[0][1];
//		}
//		else {
//			hue = 60 * (2 + (double)(ptr[0][2] - ptr[0][0]) / (ptr[0][1] - ptr[0][2]));
//			saturation = 255 * (double)(ptr[0][1] - ptr[0][2]) / ptr[0][1];
//		}
//		value = (double)ptr[0][1];
//	}
//	else if (ptr[0][2] >= ptr[0][0] && ptr[0][2] >= ptr[0][1]) {
//		if (ptr[0][0] >= ptr[0][1]) {
//			hue = 60 * ((double)(ptr[0][1] - ptr[0][0]) / (ptr[0][2] - ptr[0][1]));
//			saturation = 255 * (double)(ptr[0][2] - ptr[0][1]) / ptr[0][2];
//		}
//		else {
//			hue = 60 * ((double)(ptr[0][1] - ptr[0][0]) / (ptr[0][2] - ptr[0][0]));
//			saturation = 255 * (double)(ptr[0][2] - ptr[0][0]) / ptr[0][2];
//		}
//		value = (double)ptr[0][2];
//	}
//
//	if (hue < 0) {
//		hue += 360;
//	}
//	else if (hue >= 360) {
//		hue -= 360;
//	}
//	//std::cout << "HSV: ("<<hue<<", " << saturation<< ", " << value << ")" << "\n";
//	return std::vector<double> {hue, saturation, value};
//}

std::vector<double> HSVConvertedFromBGR(double B,double G,double R) {
	double hue, saturation, value;

	/*std::cout << B << ":B\n";
	std::cout << G << ":G\n";
	std::cout << R << ":R\n";*/
	double max = 0, min = 0;
	if (B == G && G == R) {
		//B=G=R
		hue = 0;
		saturation = 0;
		value = B;
	}
	else if (B >= G && B >= R) {
		max = B;
		if (G >= R) {
			min = R;
		}
		else {
			min = G;
		}
		hue = 60 * (4 + (G - R) / (max-min));
	}
	else if (G >= R && G >= B) {
		max = G;
		if (R >= B) {
			min = B;
		}
		else {
			min = R;
		}
		hue = 60 * (2 + (R-B) / (max - min));
	}
	else if (R >= B && R >= G) {
		max = R;
		if (B >= G) {
			min = G;
		}
		else {
			min = B;
		}
		hue = 60 * ((B-G) / (max - min));
	}
	saturation = 255 *((max - min) / max);
	//std::cout << "S: "<<saturation << "\n";
	value = max;

	if (hue < 0) {
		hue += 360;
	}
	else if (hue >= 360) {
		hue -= 360;
	}
	//std::cout << "HSV: ("<<hue<<", " << saturation<< ", " << value << ")" << "\n";
	return std::vector<double> {hue, saturation, value};
}

std::vector<double> BGRConvertedFromHSV(std::vector<double> HSV) {
	double B = 0, G = 0, R = 0;
	int Hi = 0;
	double f = 0, p = 0, q = 0, t = 0;
	if (HSV[1] == 0) {
		B = HSV[2];
		G = HSV[2];
		R = HSV[2];
	}
	else {

		//修正！！

		Hi = (int)(HSV[0] / 60);
		Hi = Hi % 6;
		//std::cout << Hi << ":Hi\n";
		f = (HSV[0] / 60) - Hi;
		p = HSV[2] * (1 - HSV[1]/255);
		q = HSV[2] * (1 - f * HSV[1]/255);
		t = HSV[2] * (1 - (1 - f)*HSV[1]/255);
		/*if (Hi == 0) {
			G = p;
			B = t;
			R = HSV[2];
		}
		else if (Hi == 1) {
			R = p;
			G = HSV[2];
			B = q;
		}
		else if (Hi == 2) {
			R = t;
			G = HSV[2];
			B = p;
		}
		else if (Hi == 3) {
			B = HSV[2];
			R = q;
			G = p;
		}
		else if (Hi == 4) {
			B = HSV[2];
			R = p;
			G = t;
		}
		else if (Hi == 5) {
			G = q;
			B = p;
			R = HSV[2];
		}
		else {
			std::cout << "Hi Error!!: "<<Hi<<"\n";
		}*/
		//実験（BGR->HSV->BGRの復元）結果にもとづく

		if (Hi == 0) {
			B = p;
			G = t;
			R = HSV[2];
		}
		else if (Hi == 1) {
			B = p;
			G = HSV[2];
			R = q;
		}
		else if (Hi == 2) {
			B = t;
			G = HSV[2];
			R = p;
		}
		else if (Hi == 3) {
			B = HSV[2];
			G = q;
			R = p;
		}
		else if (Hi == 4) {
			B = HSV[2];
			G = p;
			R = t;
		}
		else if (Hi == 5) {
			B = q;
			G = p;
			R = HSV[2];
		}
		else {
			std::cout << "Hi Error!!: " << Hi << "\n";
		}
		//文献通り


	}
	std::vector<double> BGR = {B,G,R};
	return BGR;
}


void hueInvert(cv::Mat &img) {
	int img_rows = img.rows;
	int img_cols = img.cols;

	for (int j = 0; j < img_rows; ++j) {
		cv::Vec3b *img_row_ptr = img.ptr<cv::Vec3b>(j);

		for (int i = 0; i < img_cols; ++i) {
			std::vector<double> HSV = { 0,0,0 };
			double B = img_row_ptr[i][0];
			double G = img_row_ptr[i][1];
			double R = img_row_ptr[i][2];
			//std::cout << B << ":B\n";
			//std::cout << G << ":G\n";
			//std::cout << R << ":R\n";
			HSV = HSVConvertedFromBGR(B,G,R);

			HSV[0] += 180;
			if (HSV[0] >= 360) {
				HSV[0] -= 360;
			}
			//色相の反転

			for (int l = 0; l < 3; ++l) {
				img_row_ptr[i][l] = convertToUchar(BGRConvertedFromHSV(HSV)[l]);
			}

		}
	}

}


int main()
{

	cv::Mat image = cv::imread("..\\images\\color\\Lenna.bmp");
	cv::imshow("original", image);
	cv::waitKey(0);

	//テスト用
	//for (int j = 100; j < 105; ++j) {
	//	cv::Vec3b *img_row_ptr = image.ptr<cv::Vec3b>(j);
	//	for (int k = 100; k < 105; ++k) {
	//		std::cout << "BGR(original): (";
	//		for (int i = 0; i < 3; ++i) { std::cout << (int)img_row_ptr[k][i] << " "; }
	//		std::cout << ")\n";
	//		std::vector<double> HSV;
	//		HSV = HSVConvertedFromBGR(img_row_ptr[k][0], img_row_ptr[k][1], img_row_ptr[k][2]);
	//		std::cout << "HSV(original): (";
	//		std::cout << HSV[0] << ", ";
	//		std::cout << HSV[1] << ", ";
	//		std::cout << HSV[2] << ")\n";
	//		//if (HSV[0] >= 180) {
	//		//	HSV[0] = HSV[0]-180;
	//		//}
	//		//else if (HSV[0] < 180) {
	//		//	HSV[0] = HSV[0]+180;
	//		//}
	//		std::cout << "HSV(conv.): (";
	//		std::cout << HSV[0] << ", ";
	//		std::cout << HSV[1] << ", ";
	//		std::cout << HSV[2] << ")\n";
	//		
	//		for (int l = 0; l < 3; ++l) {
	//			img_row_ptr[k][l] = BGRConvertedFromHSV(HSV)[l];
	//		}
	//		/*for (int i = 0; i < 3; ++i) { std::cout << (int)img_row_ptr[0][i] << "\n"; }*/
	//		std::cout << "BGR(conv): (";
	//		for (int i = 0; i < 3; ++i) { std::cout << BGRConvertedFromHSV(HSV)[i] << " "; }
	//		std::cout << ")\n";
	//		std::cout << "\n";
	//	}
	//}

	//テスト用2
	/*cv::Vec3b *img_row_ptr = image.ptr<cv::Vec3b>(100);
	CvVec3bPointerTest(&img_row_ptr[100]);
	for (int i = 0; i < 3; ++i) { std::cout << (int)img_row_ptr[100][i] << "\n"; }*/


	hueInvert(image);
	cv::imshow("converted", image);
	cv::waitKey(0);

	return 0;
}