#include <opencv2/opencv.hpp>
#include<vector>
#include <iostream>

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

//Vec3 のポインタ渡しのテスト用関数
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

//hue invertの関数，修正前
std::vector<double> BGRConvertedFromHSV(double H, double S, double V) {
	double B = 0.0, G = 0.0, R = 0.0;
	int Hi = 0;
	double f = 0.0, p = 0.0, q = 0.0, t = 0.0;

	if (H >= 360.0) {
		H -= 360.0;
	}
	else if (H < 0.0) {
		H += 360.0;
	}

	if (S == 0.0) {
		B = V;
		G = V;
		R = V;
	}
	else {

		//修正！！

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
			std::cout << "Hi Error!!: " << Hi << "\n";
		}

	}
	std::vector<double> BGR = { 255.0*B,255.0*G,255.0*R };
	return BGR;
}

std::vector<double> HSVConvertedFromBGR(double B, double G, double R) {
	double hue = 0.0, saturation = 0.0, value = 0.0;
	double max = 0.0, min = 0.0;
	max = std::max({ B,G,R });
	min = std::min({ B,G,R });
	if (max == min) {
		hue = 0;
		saturation = 0;
		value = B / 255.0;
	}
	else if (max == B) {

		hue = 60.0* (R - G) / (max - min) + 240.0;
	}
	else if (max == G) {
		hue = 60.0 *(B - R) / (max - min) + 120.0;
	}
	else if (max == R) {
		hue = 60 * (G - B) / (max - min);
	}
	saturation = (max - min) / max;
	value = max / 255.0;

	if (hue < 0) {
		hue += 360.0;
	}
	else if (hue > 359) {
		hue -= 360.0;
	}
	return std::vector<double> {hue, saturation, value};
}

//wikiに従った　HSV->BGR
//std::vector<double> BGRConvertedFromHSV(double H, double S, double V) {
//	double B = 0.0, G = 0.0, R = 0.0;
//	double Hd = 0.0;
//	double C = 0.0, X = 0.0;
//
//	if (H >= 360.0) {
//		H -= 360.0;
//	}
//	else if (H < 0.0) {
//		H += 360.0;
//	}
//
//	if (S == 0.0) {
//		B = V;
//		G = V;
//		R = V;
//	}
//	else {
//		Hd = H / 60.0;
//		C = S * V;
//
//		int f = (int)Hd % 2 - 1;
//		if (f >= 0) {
//			X = C * (1 - f);
//		}
//		else {
//			X = C * (1 + f);
//		}
//
//		B = (V - C);
//		G = (V - C);
//		R = (V - C);
//
//		if (0 <= Hd && Hd < 1) {
//			B += 0;
//			G += X;
//			R += C;
//		}
//		else if (1 <= Hd && Hd < 2) {
//			B += 0;
//			G += C;
//			R += X;
//		}
//		else if (2 <= Hd && Hd < 3) {
//			B += X;
//			G += C;
//			R += 0;
//		}
//		else if (3 <= Hd && Hd < 4) {
//			B += C;
//			G += X;
//			R += 0;
//		}
//		else if (4 <= Hd && Hd < 5) {
//			B += C;
//			G += 0;
//			R += X;
//		}
//		else if (5 <= Hd && Hd < 6) {
//			B += X;
//			G += 0;
//			R += C;
//		}
//	}
//	std::vector<double> BGR = { 255 * B,255 * G,255 * R };
//	return BGR;
//}


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
int indexOfMedian(std::vector<uchar> vec) {
	int vec_size = vec.size();
	std::vector <std::pair<double, int>> value_index_pairs(vec_size);
	for (int i=0; i < vec_size; ++i) {
		value_index_pairs[i] = std::make_pair(vec[i], i);
	}

	//memo:
	//pair同士の比較はfirstの大小、secondの大小の順で行われる
	//よって、sortすると原則valueの昇順で並び、valueが同じ場合indexの昇順となる

	std::sort(value_index_pairs.begin(), value_index_pairs.end());

	//valueの順で並んだ中から、真ん中の要素のsecond(ソート前のindex)を返す
	return value_index_pairs[vec_size / 2].second;
}
//
//int indexOfMedian(std::vector<uchar> vec) {
//	std::vector<uchar> sorted_vec = vec;
//
//	insertionSort(sorted_vec, sorted_vec.size());
//	uchar median = sorted_vec[(sorted_vec.size() - 1) / 2];
//
//	std::vector<uchar>::iterator itr_median = std::find(vec.begin(), vec.end(), median);
//	size_t median_index = std::distance(vec.begin(), itr_median);
//
//	return (int)median_index;
//}

int main()
{


	//hue_invertの BGRConvertedFromHSV, HSVConvertedFromBGRのテスト用
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

	/*CvVec3bPointerTestのテスト用
	cv::Vec3b *img_row_ptr = image.ptr<cv::Vec3b>(100);
	CvVec3bPointerTest(&img_row_ptr[100]);
	for (int i = 0; i < 3; ++i) { std::cout << (int)img_row_ptr[100][i] << "\n"; }*/



	//cv::Mat img = cv::imread("..\\..\\images\\color\\Lenna.bmp");
	//cv::imshow("origin", img);
	//cv::waitKey(0);

	//int img_rows = img.rows;
	//int img_cols = img.cols;
	//int window_h = 10;

	////ミラーリングでパディング
	//cv::Mat img_zero_pdd = cv::Mat::zeros(img_cols + (2 * window_h), img_rows + (2 * window_h), CV_8UC3);

	//for (int j = -window_h; j < img_rows + window_h; ++j) {
	//	cv::Vec3b *pdd_row_ptr = img_zero_pdd.ptr<cv::Vec3b>(j + window_h);
	//	int k = j;
	//	if (j < 0) {
	//		k = -j - 1;
	//	}
	//	else if (j >= img_rows) {
	//		k = 2 * img_rows - 1 - j;
	//	}
	//	cv::Vec3b *img_row_ptr = img.ptr<cv::Vec3b>(k);
	//	for (int i = -window_h; i < img_cols + window_h; ++i) {
	//		int l = i;
	//		if (i < 0) {
	//			l = -i - 1;
	//		}
	//		else if (i >= img_cols) {
	//			l = 2 * img_cols - 1 - i;
	//		}

	//		pdd_row_ptr[i + window_h] = cv::Vec3b(img_row_ptr[l][0], img_row_ptr[l][1], img_row_ptr[l][2]);
	//	}
	//}

	//cv::imshow("millor", img_zero_pdd);
	//cv::waitKey(0);


	//double Hd = 190 / 60.0;
	//double C = 0.8;
	//double X = 0;

	//int f = (int)Hd % 2 - 1;
	//if (f >= 0) {
	//	X = C * (1 - f);
	//}
	//else {
	//	X = C * (1 + f);
	//}

	//std::cout << X << "\n";

	//hue invert テスト用
	/*double B,G,R;
	std::cout << "B: ";
	std::cin >> B;
	std::cout << "\n";
	std::cout << "G: ";
	std::cin >> G;
	std::cout << "\n";
	std::cout << "R: ";
	std::cin >> R;
	std::cout << "\n";

	std::vector<double> HSV = HSVConvertedFromBGR(B, G, R);
	for (int i=0; i < HSV.size(); ++i) { std::cout << HSV[i] << "\n"; }

	std::vector<double> re_BGR = BGRConvertedFromHSV(HSV[0],HSV[1],HSV[2]);
	for (int i = 0; i < re_BGR.size(); ++i) { std::cout << re_BGR[i] << "\n"; }*/



	std::vector<uchar> vec = { 3,5,4,1,7,6,2 };
	std::cout << indexOfMedian(vec) << "\n";

	system("pause");
	return 0;
}