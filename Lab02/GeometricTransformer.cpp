#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "GeometricTransformer.h"
const double pi = 3.141592654;

uchar BilinearInterpolate::Interpolate(float tx, float ty, uchar* pSrc, int srcWidthStep, int nChannels)
{

	if ((round(tx) - tx) == 0 && (round(ty) - ty) == 0)
	{
		return *pSrc;
	}
	else
	{
		//lấy màu
		int c00 = *pSrc;
		int c01 = *(pSrc + nChannels);
		int c10 = *(pSrc + srcWidthStep);
		int c11 = *(pSrc + srcWidthStep + nChannels);
		//lấy tọa độ điểm
		float x00 = floor(tx), y00 = floor(ty);
		float x11 = x00 + 1, y11 = y00 + 1;
		float R1 = c00 * (x11 - tx) + c10 * (tx - x00);
		float R2 = c01 * (x11 - tx) + c11 * (tx - x00);
		float R3 = R1 * (y11 - ty) + R2 * (ty - y00);
		return (uchar)(R3);
	}
}
uchar NearestNeighborInterpolate::Interpolate(float tx, float ty, uchar* pSrc, int srcWidthStep, int nChannels)
{
	float roundx = round(tx), roundy = round(ty);
	float floorx = floor(tx), floory = floor(ty);
	float jieguo;
	if (roundx - floorx == 0 && roundy - floory == 0)
	{
		jieguo = *pSrc;
		return (uchar)jieguo;
	}
	else if (roundx - floorx > 0 && roundy - floory == 0)
	{
		jieguo = *(pSrc + srcWidthStep);
		return (uchar)jieguo;
	}
	else if (roundx - floorx == 0 && roundy - floory > 0)
	{
		jieguo = *(pSrc + nChannels);
		return (uchar)jieguo;
	}
	else if (roundx - floorx > 0 && roundy - floory > 0)
	{
		jieguo = *(pSrc + srcWidthStep + nChannels);
		return (uchar)jieguo;
	}
}
int GeometricTransformer::Transform(const Mat& beforeImage, Mat& afterImage, AffineTransform* transformer, PixelInterpolate* interpolator)
{
	int col = beforeImage.cols;
	int row = beforeImage.rows;

	return 0;
}
int GeometricTransformer::RotateKeepImage(const Mat& srcImage, Mat& dstImage, float angle, PixelInterpolate* interpolator)
{
	AffineTransform T, T2;
	float x, y, x2, y2;
	float Newlocation;
	int col = srcImage.cols;
	int row = srcImage.rows;
	int channel = srcImage.channels();


	float temp2[4][2]; // Tọa dộ 4 góc của hình
	float xmax = 0;
	float ymax = 0;
	float xmin = 0;
	float ymin = 0;
	temp2[0][0] = 0;
	temp2[0][1] = 0;
	temp2[1][0] = 0;
	temp2[1][1] = row - 1;
	temp2[2][0] = col - 1;
	temp2[2][1] = 0;
	temp2[3][0] = col - 1;
	temp2[3][1] = row - 1;
	T.Rotate(angle * pi / 180);
	for (int i = 0; i < 4; i++)// Tính tọa độ 4 góc sau khi biến đổi Affine
	{
		T.TransformPoint(temp2[i][0], temp2[i][1]);
		if (temp2[i][0] > xmax)
		{
			xmax = temp2[i][0];
		}
		if (temp2[i][1] > ymax)
		{
			ymax = temp2[i][1];
		}
		if (temp2[i][0] < xmin)
		{
			xmin = temp2[i][0];
		}
		if (temp2[i][1] < ymin)
		{
			ymin = temp2[i][1];
		}
	}

	uchar* p1 = NULL;// Con trỏ trỏ vào từng pixel trong ảnh gốc
	uchar* p2 = NULL; // Con trỏ trỏ vào từng pixel trong ảnh kết quả
	int col2 = round(xmax - xmin) + 1; // Kích thích ảnh mới sau khi thực hiện phép biến đổi Affine  
	int row2 = round(ymax - ymin) + 1;
	dstImage = Mat(row2, col2, CV_8UC3, Scalar(0));
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			p1 = (uchar*)srcImage.ptr(i, j);
			x = j;
			y = i;

			T.TransformPoint(x, y); //Tính tọa độ điểm mới bằng tọa độ điểm gốc qua phép biến đổi Affine 
			x = round(x + abs(xmin));
			y = round(y + abs(ymin));


			p2 = (uchar*)dstImage.ptr(y, x);
			for (int u = 0; u < channel; u++)
			{
				p2[u] = p1[u];

			}
		}
	}
	T2 = T;
	T2.Inverse();
	int step = srcImage.step;
	for (int i = 0; i < row2; i++)
	{
		for (int j = 0; j < col2; j++)
		{
			x = j;
			y = i;
			x = round(x - abs(xmin));
			y = round(y - abs(ymin));
			T2.TransformPoint(x, y);

			if (x > 0 && y > 0 && x < col && y < row)
			{
				uchar* pointer = srcImage.data;
				uchar* pointer2 = (uchar*)dstImage.ptr(y, x);
				pointer = pointer + int(floor(y)) * srcImage.step + int(floor(x)) * channel;

				if (!(pointer2[0] + pointer2[1] + pointer2[2]))
				{
					for (int c = 0; c < channel; c++)
					{
						pointer += 1;
						pointer2[c] = interpolator->Interpolate(x, y, pointer, step, channel);
					}
				}
			}

		}
	}

	waitKey(0);
	return 1;

}
int GeometricTransformer::Scale(const Mat& srcImage, Mat& dstImage, float sx, float sy, PixelInterpolate* interpolator)
{
	Mat destinationImage(srcImage.rows * sx * 1.0, srcImage.cols * sy * 1.0, CV_8UC3);
	Mat example;
	resize(srcImage, example, Size(srcImage.cols * sx, srcImage.rows * sy), 0, 0, CV_INTER_LINEAR);
	float rows = srcImage.rows * sx * 1.0;
	float cols = srcImage.cols * sy * 1.0;
	int i, j, c;
	for (i = 0; i < rows-round(sx); i++)/*rộng 300x2=600 dòng xuống dòng*/ {
		for (j = 0; j < cols-round(sy); j++)/*cao 400x2=800 cột đi từng ô*/ {
			for (c = 0; c < srcImage.channels(); c++) {
				float x1 = i * 1.0 / sx;
				float y1 = j * 1.0 / sy;
				uchar* pointer = srcImage.data;
				int channels = srcImage.channels();
				int vitrianhgoc = int(floor(x1)) * srcImage.step + int(floor(y1)) * channels + c;
				pointer = pointer + vitrianhgoc;
				uchar ketqua = interpolator->Interpolate(x1, y1, pointer, srcImage.step, srcImage.channels());
				int vitrianhsau = i * destinationImage.step + c + j * channels;
				destinationImage.data[vitrianhsau] = ketqua;
			}
		}
	}
	dstImage = destinationImage.clone();
	return 1;
}

GeometricTransformer::GeometricTransformer()
{
}

GeometricTransformer::~GeometricTransformer()
{
}

AffineTransform::AffineTransform()
{
	float* temp = NULL;
	_matrixTransform = Mat(3, 3, CV_32FC1, Scalar(1));

	temp = (float*)_matrixTransform.ptr(2, 2);
	*temp = 1.0;
	if (_matrixTransform.empty())
	{
		cout << "Failed to create Transform matrix !!!" << endl;
	}

}

AffineTransform::~AffineTransform()
{
}

void AffineTransform::Inverse()
{
	_matrixTransform = _matrixTransform.inv(DECOMP_SVD);
}

void AffineTransform::Translate(float dx, float dy)
{
	float* temp = NULL;
	AffineTransform Transformer;
	for (int i = 0; i < 3; i++)
	{
		temp = (float*)Transformer._matrixTransform.ptr(i);
		for (int j = 0; j < 3; temp++, j++)
		{
			*temp = 0;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		temp = (float*)Transformer._matrixTransform.ptr(i, i);
		*temp = 1;

	}
	temp = (float*)Transformer._matrixTransform.ptr(0, 2);
	*temp = dx;
	temp = (float*)Transformer._matrixTransform.ptr(1, 2);
	*temp = dy;
	(*this)._matrixTransform = (*this)._matrixTransform.mul(Transformer._matrixTransform);
}

void AffineTransform::Rotate(float angle)
{
	float* temp = NULL;
	AffineTransform Transformer;
	for (int i = 0; i < 3; i++)
	{
		temp = (float*)Transformer._matrixTransform.ptr(i);
		for (int j = 0; j < 3; temp++, j++)
		{
			*temp = 0;
		}
	}
	temp = (float*)Transformer._matrixTransform.ptr(0, 0);
	*temp = cos(angle);
	temp++;
	*temp = -sin(angle);

	temp = (float*)Transformer._matrixTransform.ptr(1, 0);
	*temp = sin(angle);
	temp++;
	*temp = cos(angle);


	temp = (float*)Transformer._matrixTransform.ptr(2, 2);
	*temp = 1;

	(*this)._matrixTransform = (*this)._matrixTransform.mul(Transformer._matrixTransform);

}

void AffineTransform::Scale(float sx, float sy)
{
	float* temp = NULL;
	AffineTransform Transformer;
	for (int i = 0; i < 3; i++)
	{
		temp = (float*)Transformer._matrixTransform.ptr(i);
		for (int j = 0; j < 3; temp++, j++)
		{
			*temp = 0;
		}
	}

	temp = (float*)Transformer._matrixTransform.ptr(0, 0);
	*temp = sx;
	temp = (float*)Transformer._matrixTransform.ptr(1, 1);
	*temp = sy;
	temp = (float*)Transformer._matrixTransform.ptr(2, 2);
	*temp = 1;

	(*this)._matrixTransform = (*this)._matrixTransform.mul(Transformer._matrixTransform);

}

void AffineTransform::TransformPoint(float& x, float& y)
{
	float x2, y2;
	float* temp = NULL;

	// Áp dụng công thức phép biến đổi Affine
	temp = (float*)_matrixTransform.ptr(0, 0);
	x2 = temp[0] * x + temp[1] * y + temp[2]; //x' = a0*x + a1*y + a2;

	temp = (float*)_matrixTransform.ptr(1, 0);
	y2 = temp[0] * x + temp[1] * y + temp[2]; //y' = b0*x + b1*y + b2;
	x = x2;
	y = y2;

}

AffineTransform& AffineTransform::operator=(AffineTransform T)
{
	T._matrixTransform.copyTo((*this)._matrixTransform);
	return(*this);
}

PixelInterpolate::PixelInterpolate()
{
}

PixelInterpolate::~PixelInterpolate()
{
}
