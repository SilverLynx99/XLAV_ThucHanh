#include "ColorTransformer.h"

int ColorTransformer::ChangeBrighness(const Mat& sourceImage, Mat& destinationImage, short b)
{
	if (sourceImage.channels() == 3)
		destinationImage.create(sourceImage.size(), CV_8UC3);
	else 
		destinationImage.create(sourceImage.size(), CV_8UC1);

	if (sourceImage.data)
	{
		int rows = sourceImage.rows;
		int cols = sourceImage.cols;
		int i, j, z;
		for (i = 0; i < rows; i++)
		{
			for (j = 0; j < cols; j++)
			{
				for (z = 0; z < sourceImage.channels(); z++)
				{
					int intensity = sourceImage.at<Vec3b>(i, j)[z];
					destinationImage.at<Vec3b>(i, j)[z] = saturate_cast<uchar>(1.0 * sourceImage.at<Vec3b>(i, j)[z] + b);
				}
			}
		}
		return 1;
	}
	else return 0;
}

int ChangeContrast(const Mat& sourceImage, Mat& destinationImage, float c)
{
	if (Sourceimage.empty())
	{
		return 0;
	}
	int row = Destinationimage.rows;
	int col = Destinationimage.cols;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			for (int o = 0; o < 3; o++)
			{
				int check = Destinationimage.at<Vec3b>(j, i)[o] * c;
				Destinationimage.at<Vec3b>(j, i)[o] *= c; // Áp dụng công thức tính độ phân giải (f'(x,y) = f(x,y) * c 
				if (check > 255) // Kiểm tra xem giá trị màu có vượt quá 255 hay không
				{
					Destinationimage.at<Vec3b>(j, i)[o] = 255;
				}
			}
		}

	}
	return 1;

}

void HistogramEqualization(Mat Srcimage, Mat &Dstimage)
{

	int **hist;
	int row = Srcimage.rows;
	int col = Srcimage.cols;
	int nChannel = Srcimage.channels();
	int temp;

	Srcimage.copyTo(Dstimage);
	Dstimage.setTo(0);

	hist = (int**)calloc(nChannel, sizeof(int*));
	for (int i = 0; i < nChannel; i++)
	{
		hist[i] = (int*)calloc(256, sizeof(int));

		memset(hist[i], 0, 256 * sizeof(int));
	} // Tạo mảng chứa ma trận Histogram với chiều rộng là 256


	for (int i = 0; i < row; i++)
	{
		uchar *pRow = Srcimage.ptr<uchar>(i);
		for (int j = 0; j < col; j++, pRow += nChannel)
		{
			for (int o = 0; o < nChannel; o++)
			{
				for (int u = 1; u <= 255; u++)
				{
					if (pRow[o] < u) // Kiểm tra xem giá trị màu tại vị trí (i,j) của kênh màu thứ o có nằm trong khoảng bin của histogram không 
					{
						hist[o][u - 1]++; // Tăng số lần xuất hiện nếu giá trị màu nằm trong khoảng bin nhất định của histogram
						break;
					}
				}
			}

		}
	}
	// Tính histogram của ảnh

	double **p = NULL;
	p = (double**)calloc(nChannel, sizeof(double*));
	for (int i = 0; i < nChannel; i++)
	{
		p[i] = (double*)calloc(256, sizeof(double));
	} // Khai báo ma mảng chứa xác suất của từng bin trong histogram


	for (int i = 0; i < nChannel; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			p[i][j] = double(hist[i][j]) / double((col*row));
		}
	} // Tính xác suất của các bin trong histogram

	for (int i = 0; i < row; i++)
	{
		uchar *pRow = Srcimage.ptr<uchar>(i);
		for (int j = 0; j < col; j++, pRow += nChannel)
		{
			for (int o = 0; o < nChannel; o++)
			{
				uchar *p2 = Dstimage.ptr<uchar>(i, j);
				for (int u = 0; u <= 255; u++)
				{
					temp = p2[o] + floor(p[o][u] * 255.0);
					p2[o] += floor(p[o][u] * 255.0);
					if (temp > 255)
					{
						p2[o] = 255;
						break;
					}

					if (pRow[o] < u)
					{
						break;
					}
				}
			}

		}
	}

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
