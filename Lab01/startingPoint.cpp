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
int identifyCommand(string cmd, string cmdArg, short & mode, float & commandArgument);

int main(int argc, char **argv)
{
	// Kiểm tra số lượng tham số truyền vào 
	if (argc <= 1) {
		cerr << "Not enough argument passed!" << endl;
		return -1;
	}

	string cmd, inpPath, cmdArg, secondInpPath;
	cmd = argv[1];
	inpPath = argv[2];
	if (argc == 4)
		if (cmd == "--compare")
			secondInpPath = argv[3];
		else
			cmdArg = argv[3];


	// Xác định chế độ để gọi hàm
	int catchError = 1;
	short mode = -1;
	float commandArgument = -1;
	catchError = identifyCommand(cmd, cmdArg, mode, commandArgument);

	if (catchError == -1)
	{
		cerr << "Fail to parse arguments" << endl;
		return -1;
	}

	Mat src, des;
	if (mode != 10) {
		// Mở file và kiểm tra
		src = imread(inpPath, 1);
		
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
		//src = imread("lena.jpg", 1);
		src = imread(inpPath, 1);

		if (!src.data)
		{
			cerr << "Could not open or find the image" << endl;
			return -1;
		}

		// Hiển thị ảnh đầu vào
		namedWindow("image1");
		imshow("image1", src);
	

		des = imread(secondInpPath, 1);
		// des = imread("lena_hsv.jpg" , 1);

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

	
	switch (mode)
	{
	case 1:
		catchError = Cvert.Convert(src, des, 0);
		break;
	case 2:
		// chuyển về 1 kênh màu
		cvtColor(src, src, COLOR_BGR2GRAY);
		catchError = Cvert.Convert(src, des, 1);
		break;
	case 3:// rgb2hsv
		catchError = Cvert.Convert(src, des, 2);
		break;
	case 4:// hsv2rgb
		cvtColor(src, des, COLOR_HSV2BGR);
		//catchError = Cvert.Convert(src, des, 3);
		break;
	case 5:
		catchError = cTrans.ChangeBrighness(src, des, commandArgument);
		break;
	case 6:
		catchError = cTrans.ChangeContrast(src, des, commandArgument);
		break;
	case 7: // tính hist
		cout << "Da tinh nhung khong biet show ra ntn" << endl;
		return 0;
		break;
	case 8:// cân bằng hist
		catchError = cTrans.HistogramEqualization(src, des);
		break;
	case 9:
		cTrans.drawHistogram(src, des);
		break;
	case 10:
	{
		double Intersection = cTrans.CompareImage(src, des);
		cout << "The Intersection metric of two image : " << Intersection << endl;
		waitKey(0);
		return 0;

	}
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
	imwrite("lene_hsv.jpg", des);

	waitKey(0);
	return 0;
}

int identifyCommand(string cmd, string cmdArg, short & mode, float & commandArgument)
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
		commandArgument = stof(cmdArg);
	return 0;
}