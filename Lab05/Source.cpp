#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "Threshold.h"
#include <iostream>
#include <numeric> 
#include "Kmean.h"
#include <time.h>
using namespace cv;
using namespace std;

int StaticThreshold::Apply(const Mat& srcImage, Mat& dstImage)
{
	if (srcImage.data) {

		int i, j;
		int height = srcImage.rows, width = srcImage.cols;
		Mat des(height, width, CV_8UC1);
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				int srcpixel = srcImage.at<uchar>(i, j);
				if (srcpixel > _lowThreshold&& srcpixel < _highThreshold)
				{
					des.at<uchar>(i, j) = 255;
				}
				else des.at<uchar>(i, j) = 0;
			}
		}
		dstImage = des.clone();
		if (!dstImage.data)return 0;
		return 1;
	}
	return 0;
}

int AverageLocalThreshold::Apply(const Mat& srcImage, Mat& dstImage, Size winSize)
{
	if (srcImage.data)
	{
		int height = srcImage.rows, width = srcImage.cols;
		//khởi tạo dst để xử lý và clone vào dstImage
		Mat dst(height, width, CV_8UC1);

		int area1 = winSize.height * winSize.width;// tính diện tích cũng như mẫu số
		//khởi tạo biến chạy cho cửa sổ
		int h_ave = (winSize.height - 1) / 2;
		int w_ave = (winSize.width - 1) / 2;

		int sum, area;
		int i, j, a, b;

		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				//chạy cửa sổ
				sum = 0;//cho sum quay lại =0
				area = area1;//cho area quay lại là diện tích của cửa sổ	

				for (a = i - h_ave; a <= i + h_ave; a++)
				{
					for (b = j - w_ave; b <= j + w_ave; b++)
					{
						if (a >= 0 && b >= 0 && a < height && b < width)
						{
							//tính tổng các điểm nằm trong cửa sổ
							sum += srcImage.at<uchar>(a, b);
						}
						else
							area = area - 1;
					}
				}

				float mean = sum * 1.0 / area - _C;//tính trung bình trừ đi hằng số C

				if (srcImage.at<uchar>(i, j) > mean)
				{
					dst.at<uchar>(i, j) = 255;
				}
				else
				{
					dst.at<uchar>(i, j) = 0;
				}
			}
		}
		dstImage = dst.clone();
		if (!dstImage.data)return 0;
		return 1;
	}
	return 0;
	
}

int MedianLocalThreshold::Apply(const Mat& srcImage, Mat& dstImage, Size winSize)
{
	if (srcImage.data)
	{
		int height = srcImage.rows, width = srcImage.cols;
		//Khởi tạo ảnh dst để xử lý và clone vào dstImage
		Mat dst(height, width, CV_8UC1);
		//Khởi tạo vector lưu các điểm ảnh lân cận để sort và tính trung vị
		vector<int> pointvec;

		int i, j, a, b;
		//khởi tạo biến chạy cho cửa sổ
		int h_ave = (winSize.height - 1) / 2;
		int w_ave = (winSize.width - 1) / 2;

		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				//làm mới vector pointvec sau mỗi lần lặp
				pointvec.clear();

				for (a = i - h_ave; a <= i + h_ave; a++)
				{
					for (b = j - w_ave; b <= j + w_ave; b++)
					{
						if (a >= 0 && b >= 0 && a < height && b < width)
						{
							int convert = srcImage.at<uchar>(a, b);
							pointvec.push_back(convert);
						}
					}
				}
				//sort các điểm ảnh đã lưu trong vector
				sort(pointvec.begin(), pointvec.end());
				int size = pointvec.size();
				float mean = 0;
				
				if (size / 2 == 0)//nếu lẻ thì trung vị là điểm chính giữa
				{
					int vitri = (size - 1) / 2;
					mean = pointvec[vitri];
				}
				else//nếu chẵn thì trung vị tính bằng trung bình cộng 2 điểm giữa
				{
					int vitri = size / 2;
					mean = (pointvec[vitri] + pointvec[vitri - 1]) * 1.0 / 2;
				}


				mean = mean - _C;

				if (srcImage.at<uchar>(i, j) > mean)
				{
					dst.at<uchar>(i, j) = 255;
				}
				else
				{
					dst.at<uchar>(i, j) = 0;
				}
			}
		}

		dstImage = dst.clone();
		if (!dstImage.data)return 0;
		return 1;
	}
	return 0;
}

int SauvolaLocalThreshold::Apply(const Mat& srcImage, Mat& dstImage, Size winSize) {
	if (srcImage.data)
	{
		int height = srcImage.rows, width = srcImage.cols;
		//khởi tạo dst để xử lý và clone vào dstImage
		Mat dst(height, width, CV_8UC1);

		int area1 = winSize.height * winSize.width;// tính diện tích cũng như mẫu số
		//khởi tạo biến chạy cho cửa sổ
		int h_ave = (winSize.height - 1) / 2;
		int w_ave = (winSize.width - 1) / 2;

		vector<int> pointvec;

		int sum, area;
		int i, j, a, b;
		
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++) {
				//làm mới vector pointvec sau mỗi lần lặp
				pointvec.clear();
				//quét cửa sổ đưa các giá trị vào vector
				for (a = i - h_ave; a <= i + h_ave; a++)
				{
					for (b = j - w_ave; b <= j + w_ave; b++)
					{
						if (a >= 0 && b >= 0 && a < height && b < width)
						{
							int convert = srcImage.at<uchar>(a, b);
							pointvec.push_back(convert);
						}						
					}
				}
				int sum = accumulate(pointvec.begin(), pointvec.end(), 0);
				float mean = sum * 1.0 / pointvec.size();
				float dev=0;
				for (int c = 0; c < pointvec.size(); c++)
				{
					dev += pow((pointvec[c] - mean),2);
				}
				dev = sqrt(dev / (pointvec.size() - 1) * 1.0);
				float results = mean * (1 + _k * (dev / _r - 1) * 1.0);
				if (srcImage.at<uchar>(i, j) > results)
					dst.at<uchar>(i, j) = 255;
				else
					dst.at<uchar>(i, j) = 0;

			}
		}

		dstImage = dst.clone();
		if (!dstImage.data)return 0;
		return 1;
	}
	return 0;
}

int Kmean::Apply(const Mat& srcImage, Mat& dstImage)
{
	if (srcImage.data)
	{
		//khởi tạo các giá trị cần thiết
		vector<float> distance;
		int i, j, z;
		int h, w;
		//Khởi tạo vector lưu số lượng cluster
		vector<int> Size;
		//tạo vector để chứa các tọa độ pixel vào các phân vùng
		vector<vector<int>> clusterW(_numClusters);
		vector<vector<int>> clusterH(_numClusters);
		//lấy height width của srcImage 
		int height = srcImage.rows;
		int width = srcImage.cols;
		Mat dst(height, width, CV_8UC3);
		//tạo mảng lưu trữ các center cũ
		int* width_p = new int[_numClusters];
		if (width_p == NULL)return 0;

		int* height_p = new int[_numClusters];
		if (height_p == NULL)return 0;

		//khởi tạo giá trị các center bằng random
		srand(time(NULL));
		for (i = 0; i < _numClusters; i++)
		{
			*(width_p + i) = rand() % width;
			*(height_p + i) = rand() % height;
		}
		//int kiểm tra nếu điểm mới trùng với điểm cũ để dừng vòng lặp while
		int check = 0;
		int lap = 1;
		//chạy vòng lặp cho tới khi 2 vòng lặp liên tiếp có tọa độ center không đổi
		while (check != _numClusters) {
			//check số vòng lặp cần chạy
			cout << "lap lan thu: " << lap << endl;
			//reset check lại =0
			check = 0;
			//chia các pixel vào các center
			for (h = 0; h < height; h++) {
				for (w = 0; w < width; w++) {
					distance.clear();
					//tính khoảng cách từ 1 pixel đến các điểm center
					for (z = 0; z < _numClusters; z++)
					{
						float d = dodai(w, h, *(width_p + z), *(height_p + z));
						distance.push_back(d);
					}
					//tìm khoảng cách min
					int khoangcach = location(distance);
					//push pixel vào các phân vùng tương ứng
					clusterW[khoangcach].push_back(w);
					clusterH[khoangcach].push_back(h);
				}
			}
			//tính center point mới sau khi ra khỏi vòng lặp hình ảnh
			for (i = 0; i < _numClusters; i++)
			{
				//check nếu center không có điểm nào nối tới
				if (clusterW[i].size() == 0)
				{
					check += 1;
					continue;					
				}
				//tính tọa độ center mới
				float meanW = accumulate(clusterW[i].begin(), clusterW[i].end(), 0) / clusterW[i].size();
				float meanH = accumulate(clusterH[i].begin(), clusterH[i].end(), 0) / clusterH[i].size();
				if (meanW == *(width_p + i) && meanH == *(height_p + i))
				{
					check += 1;
				}
				else {
					//lưu các center mới vào vị trí các center cũ
					*(width_p + i) = (int)meanW;
					*(height_p + i) = (int)meanH;
					check = 0;
				}

			}
			//kiếm tra xem check bằng _numCluster có nghĩa là các điểm không đổi, nếu không thì clear vector
			if (check != _numClusters)
				for (i = 0; i < _numClusters; i++)
				{
					
					clusterW[i].clear();
					clusterH[i].clear();
				}
			lap++;
		}
	
		//vòng lặp thoát ra khi check= _numCluster gán các pixel ở các vùng bằng màu của cluster
		for (i = 0; i < _numClusters; i++)
		{
			int blue = srcImage.at<Vec3b>( *(height_p + i), *(width_p + i)).val[0];
			int green = srcImage.at<Vec3b>( *(height_p + i), *(width_p + i)).val[1];
			int red = srcImage.at<Vec3b>(*(height_p + i), *(width_p + i)).val[2];
			for (j = 0; j < clusterW[i].size(); j++)
			{
				dst.at<Vec3b>(clusterH[i][j], clusterW[i][j]) = Vec3b(blue, green, red);
			}
		}
		//highlight giá trị các điểm center
		for (i = 0; i < _numClusters; i++)
		{
			int wid = *(width_p + i) - 5, hei = *(height_p + i)-5;
			for (h = hei; h < hei + 10; h++)
				for (w = wid; w < wid + 10; w++)
					dst.at<Vec3b>(h,w) = Vec3b(210, 230,78);
		}
			
		//gán xong xóa hết giá trị trong vector và xóa pointer new int
		clusterW.clear();
		clusterH.clear();
		delete[] width_p;
		delete[] height_p;
		dstImage = dst.clone();
		if (!dstImage.data)return 0;

		return 1;
	}
	return 0;
}

float dodai(int x1, int y1, int x2, int y2)
{
	return (pow(x1 - x2, 2) + pow(y1 - y2, 2));
}
int location(vector<float> d)
{
	int local = 0;
	float min = d[local];
	for (int i = 1; i < d.size(); i++)
	{
		if (d[i] < min)
		{
			min = d[i];
			local = i;
		}
	}
	return local;
}