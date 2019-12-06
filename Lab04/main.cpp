#include <iostream>
#include <opencv2/opencv.hpp>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <functional>
#include <numeric> 
#include "Threshold.h"
#include "Kmean.h"
#include <time.h>
using namespace std;
using namespace cv;

#define cuaso 3
bool comp(int a, int b)
{
	return (a < b);
}
int main()
{

	Kmean test;

	Mat srcImage = imread("D:\\Cat.jpg", CV_LOAD_IMAGE_COLOR);
	Mat dstImage;
	test.Apply(srcImage, dstImage);

	namedWindow("showimage");
	imshow("showimage", srcImage);
	namedWindow("showdes");
	imshow("showdes", dstImage);
	

	waitKey(0);
	return 0;
}