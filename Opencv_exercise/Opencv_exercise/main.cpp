#include <opencv2/opencv.hpp>
#include <iostream>

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

//Vec3 �̃|�C���^�n���̃e�X�g�p�֐�
void CvVec3bPointerTest(cv::Vec3b *ptr) {
	ptr[0][0] = (uchar)100;
	ptr[0][1] = (uchar)100;
	ptr[0][2] = (uchar)100;
	/*note�F
	Vec3b�̃|�C���^�Q�Ƃ�
	cv::Vec3b *img_row_ptr = img.ptr<cv::Vec3b>(j);
	hogehoge(img_row_ptr[i]);
	�Ƃ�����Ƃ��C�֐�����ptr[x][y]��img��Mat�̉�f(i+x,j)�̑�y�`�����l���̒l������
	->j�s�ڂ̒l�Ȃ炱�̊֐����ŕύX�ł���
	*/
}

//hue invert�̊֐��C�C���O
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

		//�C���I�I

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
		//�����iBGR->HSV->BGR�̕����j���ʂɂ��ƂÂ�

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
		//�����ʂ�


	}
	std::vector<double> BGR = {B,G,R};
	return BGR;
}

int main()
{


	//hue_invert�� BGRConvertedFromHSV, HSVConvertedFromBGR�̃e�X�g�p
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

	/*CvVec3bPointerTest�̃e�X�g�p
	cv::Vec3b *img_row_ptr = image.ptr<cv::Vec3b>(100);
	CvVec3bPointerTest(&img_row_ptr[100]);
	for (int i = 0; i < 3; ++i) { std::cout << (int)img_row_ptr[100][i] << "\n"; }*/



	system("pause");
	return 0;
}