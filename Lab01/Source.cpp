#include<opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include<iostream>
#include "ColorTransformer.h"

using namespace std;
using namespace cv;

int tao_anh_hist_tu_anh_xam(const Mat& input, Mat& output)
{
	int histSize = 256;

	// Khởi tạo ảnh chứa histogram
	Mat histImg(histSize, 1, CV_32FC1, Scalar(0));

	// Thống kê số lượng điểm ảnh ở mỗi bins
	for (int y = 0; y < input.rows; y++)
	{
		const uchar* pRow = input.ptr<uchar>(y);
		for (int x = 0; x < input.cols; x++)
		{
			int index = int(pRow[x]);
			histImg.ptr<float>(index)[0]++;
		}
	}

	// Sao chép dữ liệu ảnh histogram sang ảnh output.
	histImg.copyTo(output);

	return 0;
}

int tao_anh_hist_tu_anh_mau(const Mat& input, vector<Mat>& output)
{
	// Khởi tạo ảnh để chứa 3 kênh màu.
	vector<Mat> bgr_planes;
	split(input, bgr_planes);

	for (int i = 0; i < input.channels(); i++)
		tao_anh_hist_tu_anh_xam(bgr_planes[i], output[i]);

	return 0;
}

int ve_histogram(const Mat& input)
{	
	vector<Mat> hist(3);
	// Kiểm tra là ảnh xám hay ảnh màu
	if (input.channels() == 1)
		tao_anh_hist_tu_anh_xam(input, hist[0]);
	else
		tao_anh_hist_tu_anh_mau(input, hist);

	/// Establish the number of bins
	int histSize = 256;

	
	// Draw the histograms for B, G and R
	int hist_w = 512; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	///// Normalize the result to [ 0, histImage.rows ]
	//normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	//normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	//normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	/////// Draw for each channel
	//for (int i = 1; i < histSize; i++)
	//{
	//	line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
	//		Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))),
	//		Scalar(255, 0, 0), 2, 8, 0);
	//	line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
	//		Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))),
	//		Scalar(0, 255, 0), 2, 8, 0);
	//	line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
	//		Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))),
	//		Scalar(0, 0, 255), 2, 8, 0);
	//}

	/// Display
	namedWindow("calcHist Demo");
	imshow("calcHist Demo", histImage);

	waitKey(0);
	return -1;
}

//int main()
//{
//	Mat src,src1, dst, dst1;
//
//	/// Load image
//	src = imread("lena.jpg", 1);
//	src1 = imread("lena_hsv1.jpg", 1);
//	//namedWindow("calcHist Demo");
//	//imshow("calcHist Demo", src);
//
//	//waitKey(0);
//
//	if (!src.data)
//	{
//		return -1;
//	}
//
//	cout << src.channels() << endl;
//	drawHistogram(src, dst);
//	drawHistogram(src1, dst1);
//
//	/// Display
//	namedWindow("calcHist Demo");
//	imshow("calcHist Demo", dst);
//	namedWindow("calcHist Demo1");
//	imshow("calcHist Demo1", dst1);
//
//	waitKey(0);
//
//	return 0;
//}

