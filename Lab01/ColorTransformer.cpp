#include "ColorTransformer.h"

int ColorTransformer::ChangeBrighness(const Mat& sourceImage, Mat& destinationImage, short b)
{
	return 0;
}

int ColorTransformer::ChangeContrast(const Mat& sourceImage, Mat& destinationImage, float c)
{
	return 0;
}

int ColorTransformer::HistogramEqualization(const Mat& sourceImage, Mat& destinationImage)
{
	return 0;
}

// Vẽ ảnh màu
int ColorTransformer::HistogramEqualization1(const Mat& sourceImage, Mat& destinationImage)
{

	return 0;
}

float ColorTransformer::CompareImage(const Mat& image1, Mat& image2)
{

	return 0.0f;
}

ColorTransformer::ColorTransformer()
{
}

ColorTransformer::~ColorTransformer()
{
}

int CalculateHistogram(const Mat& sourceImage, Mat& destinationImage)
{
	int histSize = 256;
	Mat histImg;
	
	// Ảnh xám
	if (sourceImage.channels() == 1)
	{
		// Khởi tạo ảnh chứa histogram
		Mat temp(histSize, 1, CV_32FC1, Scalar(0));
		
		// Thống kê số lượng điểm ảnh ở mỗi bins
		int nRows = sourceImage.rows, nCols = sourceImage.cols;
		for (int y = 0; y < nRows; y++)
		{
			const uchar* pRow = sourceImage.ptr<uchar>(y);
			for (int x = 0; x < nCols; x++)
			{
				int index = int(pRow[x]);
				temp.ptr<float>(index)[0]++;
			}
		}

		// Sao chép ảnh temp sang ảnh histImg
		temp.copyTo(histImg);
	}
	else if (sourceImage.channels() == 3) // Ảnh màu
	{
		// Khởi tạo ảnh chứa histogram
		Mat temp(histSize, 1, CV_32FC3, Scalar(0, 0, 0));

		// Thống kê số lượng điểm ảnh ở mỗi bins
		int nRows = sourceImage.rows, nCols = sourceImage.cols, nChannels = sourceImage.channels();
		for (int y = 0; y < nRows; y++)
		{
			const uchar* pRow = sourceImage.ptr<uchar>(y);
			for (int x = 0; x < nCols; x++, pRow += nChannels)
			{
				temp.ptr<float>(int(pRow[0]))[0]++;
				temp.ptr<float>(int(pRow[1]))[1]++;
				temp.ptr<float>(int(pRow[2]))[2]++;
			}
		}

		// Sao chép ảnh temp sang ảnh histImg
		temp.copyTo(histImg);
	}
	else {
		return 0;
	}
	
	// Sao chép dữ liệu ảnh histogram sang ảnh output.
	histImg.copyTo(destinationImage);
	return 1;
}

int drawHistogram(const Mat& sourceImage, Mat& destinationImage)
{
	// Thiết lập các thông số cho ảnh histogram
	int histSize = 256;
	int hist_w = 512, hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	Mat tempHistImg(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
	Mat calculatedHist;
	// Tính phân bố histogram
	CalculateHistogram(sourceImage, calculatedHist);

	if (sourceImage.channels() == 1)
	{
		// Chuẩn hóa để histogram vừa vặn với ảnh
		normalize(calculatedHist, calculatedHist, 0, tempHistImg.rows, NORM_MINMAX, -1, Mat());

		// Vẽ histogram lên ảnh.
		for (int i = 1; i < histSize; i++)
		{
			line(tempHistImg, Point(bin_w * (i - 1), hist_h - cvRound(calculatedHist.at<float>(i - 1))),
				Point(bin_w * (i), hist_h - cvRound(calculatedHist.at<float>(i))),
				Scalar(255, 0, 0), 2, 8, 0);
		}
	}
	else if (sourceImage.channels() == 3)
	{
		vector<Mat> bgr_planes;
		split(calculatedHist, bgr_planes);

		/// Normalize the result to [ 0, histImage.rows ]
		normalize(bgr_planes[0], bgr_planes[0], 0, tempHistImg.rows, NORM_MINMAX, -1, Mat());
		normalize(bgr_planes[1], bgr_planes[1], 0, tempHistImg.rows, NORM_MINMAX, -1, Mat());
		normalize(bgr_planes[2], bgr_planes[2], 0, tempHistImg.rows, NORM_MINMAX, -1, Mat());
		
		/// Draw for each channel
		for (int i = 1; i < histSize; i++)
		{
			line(tempHistImg, Point(bin_w * (i - 1), hist_h - cvRound(bgr_planes[0].at<float>(i - 1))),
				Point(bin_w * (i), hist_h - cvRound(bgr_planes[0].at<float>(i))),
				Scalar(255, 0, 0), 2, 8, 0);
			line(tempHistImg, Point(bin_w * (i - 1), hist_h - cvRound(bgr_planes[1].at<float>(i - 1))),
				Point(bin_w * (i), hist_h - cvRound(bgr_planes[1].at<float>(i))),
				Scalar(0, 255, 0), 2, 8, 0);
			line(tempHistImg, Point(bin_w * (i - 1), hist_h - cvRound(bgr_planes[2].at<float>(i - 1))),
				Point(bin_w * (i), hist_h - cvRound(bgr_planes[2].at<float>(i))),
				Scalar(0, 0, 255), 2, 8, 0);
		}
	}

	tempHistImg.copyTo(destinationImage);

	return 1;
}
