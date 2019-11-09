#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "GeometricTransformer.h"

uchar BilinearInterpolate::Interpolate(float tx, float ty, uchar * pSrc, int srcWidthStep, int nChannels)
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
uchar NearestNeighborInterpolate::Interpolate(float tx, float ty, uchar * pSrc, int srcWidthStep, int nChannels)
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
int GeometricTransformer::Scale(const Mat& srcImage, Mat& dstImage, float sx, float sy, PixelInterpolate* interpolator)
{
	Mat destinationImage(srcImage.rows * sx * 1.0, srcImage.cols * sy * 1.0, CV_8UC3);
	Mat example;
	resize(srcImage, example, Size(srcImage.cols * sx, srcImage.rows * sy), 0, 0, CV_INTER_LINEAR);
	float rows = srcImage.rows * sx * 1.0;
	float cols = srcImage.cols * sy * 1.0;
	int i, j, c;
	for (i = 0; i < rows; i++)/*rộng 300x2=600 dòng xuống dòng*/ {
		for (j = 0; j < cols; j++)/*cao 400x2=800 cột đi từng ô*/ {
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
