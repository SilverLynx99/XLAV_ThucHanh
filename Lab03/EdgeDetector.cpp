#include "EdgeDetector.h"


int EdgeDetector::DetectEdge(const Mat& sourceImage, Mat& destinationImage, int kWidth, int kHeight, int method)
{
	switch (method)
	{
	case 1:
	{
		int threshold = 80;
		Mat imgX, imgY;
		Convolution cCon_x, cCon_y;
		float kernel_x[] = { -1,-2, -1, 0, 0, 0, 1, 2, 1 };
		vector<float> tempVector(kernel_x, kernel_x + sizeof(kernel_x) / sizeof(kernel_x[0]));
		cCon_x.SetKernel(tempVector, 3, 3);
		cCon_x.DoConvolution(sourceImage, imgX);

		float kernel_y[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1 };
		tempVector = vector<float>(kernel_y, kernel_y + sizeof(kernel_y) / sizeof(kernel_y[0]));
		cCon_y.SetKernel(tempVector, 3, 3);
		cCon_y.DoConvolution(sourceImage, imgY);

		// tinh e(x, y). Do dai vector gradient
		int nCol = sourceImage.cols, nRow = sourceImage.rows;
		destinationImage = sourceImage.clone();
		for (int i = 0; i < nRow; i++) {
			float* pX = imgX.ptr<float>(i);
			float* pY = imgY.ptr<float>(i);

			uchar* pDes = destinationImage.ptr<uchar>(i);

			for (int j = 0; j < nCol; j++) {

				int value = (uchar) (abs(pX[j]) + abs(pY[j]));
				if (value >= threshold)
					pDes[j] = 255;
				else
					pDes[j] = 0;
			}
		}
	}
		break;
	case 2:
	{
		int threshold = 80;
		Mat imgX, imgY;
		Convolution cCon_x, cCon_y;
		float kernel_x[] = { -1,-1, -1, 0, 0, 0, 1, 1, 1 };
		vector<float> tempVector(kernel_x, kernel_x + sizeof(kernel_x) / sizeof(kernel_x[0]));
		cCon_x.SetKernel(tempVector, 3, 3);
		cCon_x.DoConvolution(sourceImage, imgX);

		float kernel_y[] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };
		tempVector = vector<float>(kernel_y, kernel_y + sizeof(kernel_y) / sizeof(kernel_y[0]));
		cCon_y.SetKernel(tempVector, 3, 3);
		cCon_y.DoConvolution(sourceImage, imgY);

		// tinh e(x, y). Do dai vector gradient
		int nCol = sourceImage.cols, nRow = sourceImage.rows;
		destinationImage = sourceImage.clone();
		for (int i = 0; i < nRow; i++) {
			float* pX = imgX.ptr<float>(i);
			float* pY = imgY.ptr<float>(i);

			uchar* pDes = destinationImage.ptr<uchar>(i);

			for (int j = 0; j < nCol; j++) {

				int value = (uchar)(abs(pX[j]) + abs(pY[j]));
				if (value >= threshold)
					pDes[j] = 255;
				else
					pDes[j] = 0;
			}
		}
	}
		break;
	case 3:
	{
		int threshold = 25;
		Mat img;
		Convolution cCon;
		float kernel_laplace[] = { 0, 1, 0, 1, -4, 1, 0, 1, 0 };
		vector<float> tempVector(kernel_laplace, kernel_laplace + sizeof(kernel_laplace) / sizeof(kernel_laplace[0]));
		cCon.SetKernel(tempVector, 3, 3);
		cCon.DoConvolution(sourceImage, img);

		int nCol = sourceImage.cols, nRow = sourceImage.rows;
		destinationImage = sourceImage.clone();
		// Zero - crossing
		for (int i = 1; i < nRow - 1; i++) {

			for (int j = 1; j < nCol - 1; j++) {
				float upvalue = img.at<float>(i - 1, j);
				float downvalue = img.at<float>(i + 1, j);
				float leftValue = img.at<float>(i, j - 1);
				float rightValue = img.at<float>(i, j + 1);	

				if ((upvalue * downvalue) < 0 && abs(upvalue - downvalue) >= threshold) { // nó là biên cạnh
					destinationImage.at<uchar>(i, j) = 255;
				}
				else if ((leftValue * rightValue) < 0 && abs(leftValue - rightValue) >= threshold) {
					destinationImage.at<uchar>(i, j) = 255;
				}
				else{
					destinationImage.at<uchar>(i, j) = 0;
				}
				
			}
		}
	}
		break;

	default:
		break;
	}
	return 0;
}

EdgeDetector::EdgeDetector()
{
}

EdgeDetector::~EdgeDetector()
{
}
