#include <opencv2/opencv.hpp>
#include<iostream>
#include<vector>

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

cv::Vec3b convertToUchar(std::vector<double> vec) {
	return cv::Vec3b {convertToUchar(vec[0]),convertToUchar(vec[1]),convertToUchar(vec[2])};
}

std::vector<double> HSVConvertedFromBGR(double B,double G,double R) {
	double hue, saturation, value;
	double max = 0, min = 0;

	if (B == G && G == R) {
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
	value = max;

	if (hue < 0) {
		hue += 360;
	}
	else if (hue >= 360) {
		hue -= 360;
	}
	return std::vector<double> {hue, saturation, value};
}


std::vector<double> BGRConvertedFromHSV(double H,double S,double V) {
	double B = 0, G = 0, R = 0;
	double Hd = 0;
	double C = 0, X = 0;
	if (S == 0) {
		B = V;
		G = V;
		R = V;
	}
	else {
		Hd = H / 60;
		C = S * V;

		int f = (int)Hd % 2 - 1;
		if (f >= 0) {
			X = C * (1 - f);
		}
		else {
			X = C * (1 + f);
		}

		B = (V-C);
		G = (V-C);
		R = (V-C);

		if (0<=Hd && Hd<1) {
			B += 0;
			G += X;
			R += C;
		}
		else if (1 <= Hd && Hd < 2) {
			B += 0;
			G += C;
			R += X;
		}
		else if (2 <= Hd && Hd < 3) {
			B += X;
			G += C;
			R += 0;
		}
		else if (3 <= Hd && Hd < 4) {
			B += C;
			G += X;
			R += 0;
		}
		else if (4 <= Hd && Hd < 5) {
			B += C;
			G += 0;
			R += X;
		}
		else if (5 <= Hd && Hd < 6) {
			B += X;
			G += 0;
			R += C;
		}
	}
	std::vector<double> BGR = { B,G,R };
	return BGR;
}


void hueInvert(cv::Mat &img) {
	int img_rows = img.rows;
	int img_cols = img.cols;

	for (int j = 0; j < img_rows; ++j) {
		cv::Vec3b *img_row_ptr = img.ptr<cv::Vec3b>(j);

		for (int i = 0; i < img_cols; ++i) {
			std::vector<double> HSV = { 0,0,0 };
			HSV = HSVConvertedFromBGR(img_row_ptr[i][0], img_row_ptr[i][1], img_row_ptr[i][2]);

			HSV[0] += 180;
			if (HSV[0] >= 360) {
				HSV[0] -= 360;
			}

			std::vector<double> BGR = { 0,0,0 };
			BGR = BGRConvertedFromHSV(HSV[0],HSV[1],HSV[2]);
			img_row_ptr[i] = convertToUchar(BGR);

		}
	}

}


int main()
{
	cv::Mat image = cv::imread("..\\images\\color\\Lenna.bmp");
	cv::imshow("original", image);
	cv::waitKey(0);

	hueInvert(image);
	cv::imshow("converted", image);
	cv::waitKey(0);

	system("pause");
	return 0;
}