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


int identifyCommand(string cmd, string cmdArg,short& mode,short& commandArgument);

int main(int argc, char **argv)
{
	// Lấy tham số 
	if (argc <= 0) {
		return -1;
	}

	string cmd, inpPath, cmdArg;
	cmd = argv[1];
	inpPath = argv[2];
	if (argc == 4)
		cmdArg = argv[3];

	// Mo file
	Mat src = imread(inpPath);
	// Kiem tra anh

	//// Tao mot anh moi
	//Mat des;

	// Xac dinh cau lenh
	int catchError;
	short mode = -1, commandArgument = -1;
	catchError = identifyCommand(cmd, cmdArg, mode, commandArgument);
	if (catchError == -1)
		return -1;

	// Khoi tao 2 doi tuong de chuyen doi anh
	Converter Cvert;
	ColorTransformer cTrans;

	switch (mode)
	{
	case 1:
		//catchError = Cvert.Convert(src, des, 0);
		cout << "Chuc nang 1" << endl;
		break;
	case 2:
		//catchError = Cvert.Convert(src, des, 1);
		cout << "Chuc nang 2" << endl;
		break;
	case 3:
		//catchError = Cvert.Convert(src, des, 2);
		cout << "Chuc nang 3" << endl;
		break;
	case 4:
		//catchError = Cvert.Convert(src, des, 3);
		cout << "Chuc nang 4" << endl;
		break;
	case 5:
		//catchError = cTrans.ChangeBrighness(src, des, commandArgument);
		cout << "Chuc nang 5" << endl;
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
		//catchError = cTrans.CompareImage(src, des);
		cout << "Chuc nang 9" << endl;
		break;
	default:
		break;
	}

	namedWindow("calcHist Demo");
	imshow("calcHist Demo", src);
	
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