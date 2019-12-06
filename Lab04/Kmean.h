#include <iostream>
#include <opencv2/opencv.hpp>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace std;

class Kmean
{
	//số cụm K
	int _numClusters;
public:
	/*
	Hàm áp dụng thuật toán Kmeans để phân đoạn ảnh
	- srcImage: ảnh input
	- dstImage: ảnh kết quả
	Hàm trả về
	1: nếu phân đoạn thành công
	0: nếu phân đoạn không thành công
	*/

	int Apply(const Mat& srcImage, Mat &dstImage);

	Kmean() { _numClusters = 9; }
	~Kmean(){}
};

float dodai(int x1, int y1, int x2, int y2);
int location(vector<float> d);