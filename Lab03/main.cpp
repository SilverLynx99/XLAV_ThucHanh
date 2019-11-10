#include<opencv2/opencv.hpp>
#include<iostream>
#include <string>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "EdgeDetector.h"
#include "Blur.h"
#include "Convolution.h"

using namespace std;
using namespace cv;

string cmdName[] = { "--mean", "--median", "--gauss",
					"--sobel", "--prewitt", "--laplace" };


// Note: Giả định người dùng luôn nhập đúng định dạng đã quy định.

/*
Hàm dùng để chuyển lệnh người dùng nhập thành các chế độ để thực thi chương trình
Tham số
	cmd : lệnh cần thực thi
	cmd : tham số của lệnh cần thực thi
	mode : chế độ để thực thi các hàm tương ứng.
	commandArgument: tham số để truyền vào chế độ tương ứng
Hàm trả về:
	0 : nếu phân tích thành công
	-1 : nếu phân tích thất bại
*/
int identifyCommand(string cmd, string cmdArg, short& mode, short& commandArgument);

int main(int argc, char** argv)
{
	//// Kiểm tra số lượng tham số truyền vào 
	//if (argc <= 1) {
	//	cerr << "Not enough argument passed!" << endl;
	//	return -1;
	//}

	//string cmd, inpPath, cmdArg;
	//cmd = argv[1];
	//inpPath = argv[2];
	//if (argc == 4)
	//	cmdArg = argv[3];
	//	

	// Xác định chế độ để gọi hàm
	int catchError = 1;
	short mode = -1, commandArgument = -1;
	// catchError = identifyCommand(cmd, cmdArg, mode, commandArgument);

	//if (catchError == -1)
	//{
	//	cerr << "Fail to parse arguments" << endl;
	//	return -1;
	//}

	Mat src, des;

	// Mở file và kiểm tra
	//src = imread("nhan3.jpg", 1);
	//src = imread("sample2.jpg", 1);
	src = imread("lena.jpg", 1);

	if (!src.data)
	{
		cerr << "Could not open or find the image" << endl;
		return -1;
	}

	// Hiển thị ảnh đầu vào
	namedWindow("Inputimage");
	imshow("Inputimage", src);

	// Khởi tạo 3 đối tượng
	EdgeDetector eDect;
	Blur blur;
	cvtColor(src, src, COLOR_RGB2GRAY);
	cout << src.channels() << endl;
	mode = 6;
	commandArgument = 3;
	switch (mode)
	{
	case 1:
		catchError = blur.BlurImage(src, des, commandArgument, commandArgument, 0);
		break;
	case 2:
		catchError = blur.BlurImage(src, des, commandArgument, commandArgument, 1);
		break;
	case 3:
		catchError = blur.BlurImage(src, des, commandArgument, commandArgument, 2);
		break;
	case 4:
		catchError = eDect.DetectEdge(src, des, 3, 3, 1);
		break;
	case 5:
		catchError = eDect.DetectEdge(src, des, 3, 3, 2);
		break;
	case 6:
		catchError = eDect.DetectEdge(src, des, 3, 3, 3);
		break;

	default:
		break;
	}

	if (catchError == -1)
	{
		cerr << "Fail to transform image" << endl;
		return -1;
	}

	// Hiển thị ảnh kết quả
	namedWindow("Output image");
	imshow("Output image", des);

	waitKey(0);
	return 0;
}

int identifyCommand(string cmd, string cmdArg, short& mode, short& commandArgument)
{
	int lenOfCmdName = sizeof(cmdName) / sizeof(cmdName[0]);
	mode = -1;
	for (int i = 0; i < lenOfCmdName; i++) {
		if (cmdName[i] == cmd) {
			mode = i + 1;
			break;
		}
	}
	if (mode == -1)
		return -1;

	if (cmdArg.size() != 0)
		commandArgument = stoi(cmdArg);
	return 0;
}