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
	return cv::Vec3b{ convertToUchar(vec[0]),convertToUchar(vec[1]),convertToUchar(vec[2]) };
}


//memo: pixelごとに関数に入れて処理すべきか，Mat全体を入れて処理させるべきか？
std::vector<double> HSVConvertedFromBGR(double B, double G, double R) {
	double hue, saturation, value;
	double max = 0.0, min = 0.0;

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
		hue = 60.0 * ((R - G) / (max - min)) + 240.0;
	}
	else if (G >= R && G >= B) {
		max = G;
		if (R >= B) {
			min = B;
		}
		else {
			min = R;
		}
		hue = 60.0 * ((B - R) / (max - min)) + 120.0;
	}
	else if (R >= B && R >= G) {
		max = R;
		if (B >= G) {
			min = G;
		}
		else {
			min = B;
		}
		hue = 60 * ((G - B) / (max - min));
	}
	saturation = ((max - min) / max);
	value = max / 255.0;

	if (hue < 0) {
		hue += 360;
	}
	else if (hue >= 360.0) {
		hue -= 360;
	}
	return std::vector<double> {hue, saturation, value};
}


std::vector<double> BGRConvertedFromHSV(double H, double S, double V) {
	double B = 0.0, G = 0.0, R = 0.0;
	int Hi = 0;
	double f = 0.0, p = 0.0, q = 0.0, t = 0.0;

	//H,S,Vの値をそれぞれ[0,360), [0,1], [0,1]の範囲内に収める
	if (H >= 360.0) {
		H -= 360.0;
	}
	else if (H < 0.0) {
		H += 360.0;
	}
	if (S > 1.0) {
		S = 1.0;
	}
	else if (S < 0.0) {
		S = 0.0;
	}
	if (V > 1.0) {
		V = 1.0;
	}
	else if (V < 0.0) {
		V = 0.0;
	}


	if (S == 0.0) {
		B = V;
		G = V;
		R = V;
	}
	else {

		Hi = (int)(H / 60);
		f = (double)(H / 60.0) - Hi;
		p = V * (1 - S);
		q = V * (1 - f * S);
		t = V * (1 - (1 - f)*S);

		if (Hi == 0) {
			B = p;
			G = t;
			R = V;
		}
		else if (Hi == 1) {
			B = p;
			G = V;
			R = q;
		}
		else if (Hi == 2) {
			B = t;
			G = V;
			R = p;
		}
		else if (Hi == 3) {
			B = V;
			G = q;
			R = p;
		}
		else if (Hi == 4) {
			B = V;
			G = p;
			R = t;
		}
		else if (Hi == 5) {
			B = q;
			G = p;
			R = V;
		}
		else {
			std::cout << "Hi Error: " << Hi << "\n";
		}

	}
	std::vector<double> BGR = { 255.0*B,255.0*G,255.0*R };
	return BGR;
}

void hueInvert(cv::Mat &img) {
	int img_rows = img.rows;
	int img_cols = img.cols;

	for (int j = 0; j < img_rows; ++j) {
		cv::Vec3b *img_row_ptr = img.ptr<cv::Vec3b>(j);

		for (int i = 0; i < img_cols; ++i) {
			std::vector<double> HSV = { 0,0,0 };
			HSV = HSVConvertedFromBGR((double)img_row_ptr[i][0], (double)img_row_ptr[i][1], (double)img_row_ptr[i][2]);


			//hueを反転
			HSV[0] += 180.0;
			if (HSV[0] >= 360.0) {
				HSV[0] -= 360.0;
			}

			//HSV[2] =1.0-HSV[2];



			std::vector<double> BGR = { 0,0,0 };
			BGR = BGRConvertedFromHSV(HSV[0], HSV[1], HSV[2]);

			img_row_ptr[i] = convertToUchar(BGR);

		}
	}

}


int main(int argc, char *argv[])
{
	//コマンドライン変数で画像ファイル名を指定
	cv::Mat image = cv::imread(argv[1]);
	cv::imshow("original", image);
	cv::waitKey(0);

	cv::Mat image_ground_truth = ~image;
	hueInvert(image);
	cv::imshow("converted", image);
	cv::waitKey(0);
	cv::imshow("Ground truth", image_ground_truth);
	cv::waitKey(0);

	system("pause");
	return 0;
}