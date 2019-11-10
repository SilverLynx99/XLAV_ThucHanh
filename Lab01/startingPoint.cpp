#include<opencv2/opencv.hpp>
#include<iostream>
#include <string>

#include "ColorTransformer.h"
#include "Converter.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

string cmdName[] = { "--rgb2gray", "--gray2rgb", "--rgb2hsv",
					"--hsv2rgb", "--bright", "--contrast", 
					"--hist", "--equalhist", "--drawhist", "--compare"};


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
int identifyCommand(string cmd, string cmdArg,short& mode,short& commandArgument);

int main(int argc, char **argv)
{
	//// Kiểm tra số lượng tham số truyền vào 
	//if (argc <= 1) {
	//	cerr << "Not enough argument passed!" << endl;
	//	return -1;
	//}

	//string cmd, inpPath, cmdArg, secondInpPath;
	//cmd = argv[1];
	//inpPath = argv[2];
	//if (argc == 4)
	//	if (cmd == "--compare")
	//		cmdArg = argv[3];
	//	else
	//		secondInpPath = argv[3];


	// Xác định chế độ để gọi hàm
	int catchError = 1;
	short mode = -1, commandArgument = -1;
	//catchError = identifyCommand(cmd, cmdArg, mode, commandArgument);

	//if (catchError == -1)
	//{
	//	cerr << "Fail to parse arguments" << endl;
	//	return -1;
	//}

	Mat src, des;
	mode = 9;
	commandArgument = -200;
	if (mode != 9) {
		// Mở file và kiểm tra
		src = imread("lena.jpg", 1);
		
		if (!src.data)
		{
			cerr << "Could not open or find the image" << endl;
			return -1;
		}

		// Hiển thị ảnh đầu vào
		namedWindow("Inputimage");
		imshow("Inputimage", src);
	}
	else {
		src = imread("lena.jpg", 1);
		//src = imread(inpPath, 1);

		if (!src.data)
		{
			cerr << "Could not open or find the image" << endl;
			return -1;
		}

		// Hiển thị ảnh đầu vào
		namedWindow("image1");
		imshow("image1", src);

		// des = imread(secondInpPath, 1);
		des = imread("lena_hsv.jpg" , 1);

		if (!des.data)
		{
			cerr << "Could not open or find the image" << endl;
			return -1;
		}

		// Hiển thị ảnh đầu vào
		namedWindow("image2");
		imshow("image2", des);
	}
	

	// Khởi tạo 2 đối tượng để thực hiện các phép biến đổi màu
	Converter Cvert;
	ColorTransformer cTrans;

	double Intersection;
	switch (mode)
	{
	case 1:
		cout << "Chuc nang 1" << endl;
		catchError = Cvert.Convert(src, des, 0);
		break;
	case 2:
		cout << "Chuc nang 2" << endl;
		//catchError = Cvert.Convert(src, des, 1);
		break;
	case 3:
		cout << "Chuc nang 3" << endl;
		// catchError = Cvert.Convert(src, des, 2);
		cvtColor(src, des, COLOR_BGR2HSV);
		break;
	case 4:
		cout << "Chuc nang 4" << endl;
		catchError = Cvert.Convert(src, des, 3);
		break;
	case 5:
		cout << "Chuc nang 5" << endl;
		catchError = cTrans.ChangeBrighness(src, des, commandArgument);
		break;
	case 6:
		//catchError = cTrans.ChangeContrast(src, des, commandArgument);
		cout << "Chuc nang 6" << endl;
		break;
	case 7:
		//catchError = cTrans.HistogramEqualization(src, des);
		cout << "Chuc nang 7" << endl;
		break;
	case 8:
		//catchError = cTrans.HistogramEqualization1(src, des);
		cout << "Chuc nang 8" << endl;
		break;
	case 9:
		cout << "Chuc nang 9" << endl;
		Intersection = cTrans.CompareImage(src, des);
		cout << "The Intersection metric of two image : " << Intersection << endl;
		waitKey(0);
		return 0;
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
	imwrite("lena_hsv1.jpg", des);

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