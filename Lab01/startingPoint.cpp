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

//int main(int argc, char **argv)
//{
//	// Kiểm tra số lượng tham số truyền vào 
//	if (argc <= 1) {
//		cerr << "Not enough argument passed!" << endl;
//		return -1;
//	}
//
//	string cmd, inpPath, cmdArg;
//	cmd = argv[1];
//	inpPath = argv[2];
//	if (argc == 4)
//		cmdArg = argv[3];
//
//	// Xác định chế độ để gọi hàm
//	int catchError;
//	short mode = -1, commandArgument = -1;
//	catchError = identifyCommand(cmd, cmdArg, mode, commandArgument);
//	if (catchError == -1)
//	{
//		cerr << "Fail to parse arguments" << endl;
//		return -1;
//	}
//		
//	// Mở file và kiểm tra
//	Mat src = imread(inpPath);
//	if (!src.data)
//	{
//		cerr << "Could not open or find the image" << endl;
//		return -1;
//	}
//
//	// Hiển thị ảnh đầu vào
//	namedWindow("Input image");
//	imshow("Input image", src);
//
//	// Tạo ảnh kết quả
//	Mat des;
//
//	// Khởi tạo 2 đối tượng để thực hiện các phép biến đổi màu
//	Converter Cvert;
//	ColorTransformer cTrans;
//
//	switch (mode)
//	{
//	case 1:
//		//catchError = Cvert.Convert(src, des, 0);
//		cout << "Chuc nang 1" << endl;
//		break;
//	case 2:
//		//catchError = Cvert.Convert(src, des, 1);
//		cout << "Chuc nang 2" << endl;
//		break;
//	case 3:
//		//catchError = Cvert.Convert(src, des, 2);
//		cout << "Chuc nang 3" << endl;
//		break;
//	case 4:
//		//catchError = Cvert.Convert(src, des, 3);
//		cout << "Chuc nang 4" << endl;
//		break;
//	case 5:
//		//catchError = cTrans.ChangeBrighness(src, des, commandArgument);
//		cout << "Chuc nang 5" << endl;
//		break;
//	case 6:
//		//catchError = cTrans.ChangeContrast(src, des, commandArgument);
//		cout << "Chuc nang 6" << endl;
//		break;
//	case 7:
//		//catchError = cTrans.HistogramEqualization(src, des);
//		cout << "Chuc nang 7" << endl;
//		break;
//	case 8:
//		//catchError = cTrans.HistogramEqualization1(src, des);
//		cout << "Chuc nang 8" << endl;
//		break;
//	case 9:
//		//catchError = cTrans.CompareImage(src, des);
//		cout << "Chuc nang 9" << endl;
//		break;
//	default:
//		break;
//	}
//
//	if (catchError == -1)
//	{
//		cerr << "Fail to transform image" << endl;
//		return -1;
//	}
//
//	// Hiển thị ảnh kết quả
//	namedWindow("Output image");
//	imshow("Output image", des);
//	
//	waitKey(0);
//	return 0;
//}

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