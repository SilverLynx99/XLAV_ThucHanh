// affine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <math.h>

using namespace cv;
using namespace std;
uchar Interpolate(float tx, float ty, uchar* pSrc, int srcWidthStep, int nChannels)
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
		int c11 = *(pSrc + srcWidthStep+ nChannels);
		//lấy tọa độ điểm
		float x00 = floor(tx), y00 = floor(ty);
		float x11 = x00 + 1, y11 = y00 + 1;
		float R1 = c00 * (x11-tx) + c10 * (tx - x00);
		float R2 = c01 * (x11 - tx) + c11 * (tx - x00);
		float R3 =R1 * (y11 - ty) + R2 * (ty - y00);
		return (uchar)(R3);
	}
}
uchar NeiInterpolate(float tx, float ty, uchar* pSrc, int srcWidthStep, int nChannels) {
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
		jieguo =  *(pSrc + srcWidthStep);
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
int main()
{
   Mat sourceImage= imread("D:\\Lena.jpg",CV_LOAD_IMAGE_COLOR);
   cout << sourceImage.step;
   float sx=2.6, sy=2.6;
   Mat destinationImage(sourceImage.rows*sx*1.0, sourceImage.cols*sy*1.0, CV_8UC3);
   Mat example;
   resize(sourceImage, example, Size(sourceImage.cols * sx, sourceImage.rows * sy), 0, 0, CV_INTER_LINEAR);
   float rows = sourceImage.rows * sx * 1.0;
   float cols = sourceImage.cols * sy * 1.0;
   int i, j,c;
   for (i = 0; i < rows; i++)/*rộng 300x2=600 dòng xuống dòng*/ {
	   for (j = 0; j < cols; j++)/*cao 400x2=800 cột đi từng ô*/ {
		   for (c = 0; c < sourceImage.channels(); c++) {
			   float x1 = i * 1.0 / sx;
			   float y1 = j * 1.0 / sy;
			   uchar* pointer = sourceImage.data;
			   int channels = sourceImage.channels();
			   int vitrianhgoc = int(floor(x1)) * sourceImage.step + int(floor(y1)) * channels + c;
			   pointer = pointer + vitrianhgoc;
			   uchar ketqua = Interpolate(x1, y1, pointer, sourceImage.step, sourceImage.channels());
			   int vitrianhsau = i * destinationImage.step + c + j * channels;
			   destinationImage.data[vitrianhsau] = ketqua;
		   }
	   }
   }
   Vec3b c00 = sourceImage.at<Vec3b>(149, 199);
   cout << c00.val[0] << "," << c00.val[1] << "," << c00.val[2] << endl;
   Vec3b c01 = sourceImage.at<Vec3b>(149, 200);
   cout << c01.val[0] << "," << c01.val[1] << "," << c01.val[2] << endl;
   Vec3b c10 = sourceImage.at<Vec3b>(150, 199);
   cout << c10.val[0] << "," << c10.val[1] << "," << c10.val[2] << endl;
   Vec3b c11 = sourceImage.at<Vec3b>(150, 200);
   cout << c11.val[0] << "," << c11.val[1] << "," << c11.val[2] << endl;
   cout << endl;
   cout << int(c00.val[0]) << "," << int(c00.val[1]) << "," << int(c00.val[2]) << endl;
   cout << int(c01.val[0]) << "," << int(c01.val[1]) << "," << int(c01.val[2]) << endl;
   cout << int(c10.val[0]) << "," << int(c10.val[1]) << "," << int(c10.val[2]) << endl;
   cout << int(c11.val[0]) << "," << int(c11.val[1]) << "," << int(c11.val[2]) << endl;

   namedWindow("showsource");
   imshow("showsource", sourceImage);
   namedWindow("showdes");
   imshow("showdes", destinationImage);
   namedWindow("showex");
   imshow("showex", example);
   waitKey(0);
   return 1;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
