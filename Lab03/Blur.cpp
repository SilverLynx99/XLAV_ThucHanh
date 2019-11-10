#include "Blur.h"

int Blur::BlurImage(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight, int method)
{
	Convolution cCon;
	vector<float> abc;
	Mat tempImg;

	if (method == 0) {
		
		// Find kernel
		for (int i = 0; i < kWidth * kHeight; i++) {
			abc.push_back(float(1) / float(kWidth * kHeight));
		}

		destinationImage = sourceImage.clone();

		cCon.SetKernel(abc, kWidth, kHeight);
		cCon.DoConvolution(sourceImage, tempImg);
		for (int i = 0; i < sourceImage.rows; i++)
			for (int j = 0; j < sourceImage.cols; j++)
				destinationImage.at<uchar>(i, j) = (uchar)tempImg.at<float>(i, j);

	}
	else if (method == 1) {

	}
	else if (method == 2) {
		float PI = 3.141592;
		float sigma = 1.0;
		float value;
		
		for (int cnt =0; cnt < kWidth * kHeight; cnt++) {
			int i = cnt % kWidth;
			int j = cnt / kHeight;

			value = expf(-(powf((i - kWidth / 2), 2) + powf((j - kHeight / 2), 2)) / (2 * sigma * sigma)) / (2 * PI * sigma * sigma);
			abc.push_back(value);
		}

		
		destinationImage = sourceImage.clone();

		cCon.SetKernel(abc, kWidth, kHeight);
		cCon.DoConvolution(sourceImage, tempImg);
		for (int i = 0; i < sourceImage.rows; i++)
			for (int j = 0; j < sourceImage.cols; j++)
				destinationImage.at<uchar>(i, j) = (uchar)tempImg.at<float>(i, j);
	}
	else
		return 1;


	return 0;
}

Blur::Blur()
{
}

Blur::~Blur()
{
}
