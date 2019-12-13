#include <iostream>
#include <opencv2/opencv.hpp>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <functional>
#include <numeric> 
#include "Threshold.h"
#include "Kmean.h"
#include <time.h>
#include <string.h>
using namespace std;
using namespace cv;

string cmdName[] = { "--static", "--mean", "--median",
					"--sauvola", "--kmeans"};
// Note: Giả định người dùng luôn nhập đúng định dạng đã quy định.

/*
Hàm dùng để chuyển lệnh người dùng nhập thành các chế độ để thực thi chương trình
Tham số
	cmd : lệnh cần thực thi
	cmd : tham số của lệnh cần thực thi
	commandArgument: tham số để truyền vào chế độ tương ứng
Hàm trả về:
	0 : nếu phân tích thành công
	-1 : nếu phân tích thất bại
*/

int main(int argc, char** argv)
{

	// Kiểm tra số lượng tham số truyền vào 
	if (argc <= 1) {
		cerr << "Not enough argument passed!" << endl;
		return -1;
	}

	string cmd, inpPath,arg1;
	cmd = argv[1];
	inpPath = argv[2];
	arg1 = argv[3];
	Mat src,srctoBW, des;
	
	int catchError;
	
		//mode chỉ ra vị trí cmd cần thực hiện
		int lenOfCmdName = sizeof(cmdName) / sizeof(cmdName[0]);
		int mode = 0;
		for (int i = 0; i < lenOfCmdName; i++) {
			if (cmdName[i] == cmd) {
				mode = i + 1;
				break;
			}
		}
		//check ảnh đầu vào
		src = imread(inpPath, 1);
		if (!src.data)
		{
			cerr << "Could not open or find the image" << endl;
			return -1;
		}		
		//sau khi doc src thi chuyen sang gray
		cvtColor(src, srctoBW, CV_BGR2GRAY);
			if (mode == 1)//kiểm tra low high nằm giữa 0 và 255 hay không và low cao hơn high ko
			{
				if (stoi(argv[3]) < 0 || stoi(argv[3]) >255||stoi(argv[4])<0||stoi(argv[4])>255)
				{ 
					cerr << "value must between 0 and 255";
					return -1;
				}
				if (argv[3] > argv[4])
				{
					cerr << "low must lower than high";
					return -1;
				}
			}
			//kiểm tra cửa sổ nhập có lẻ hay ko
			if (mode == 2 || mode == 3 || mode == 4)
				if (stoi(argv[3]) % 2 == 0)
				{
					cerr << "Win size must odd";
					return -1;
				}
				
		
		if (mode == 5)// kiểm tra k cluster khác 0 hay ko
		{
			if (argv[3] == 0)
			{
				cerr << "Cluster must greater than 0";
				return -1;
			}
		}
		// Hiển thị ảnh đầu vào
		namedWindow("Inputimage");
		imshow("Inputimage", src);
		
		switch (mode)
		{
		case 1:
		{string arg2 = argv[4];
		StaticThreshold threshold;
		threshold.lowhigh(stoi(arg1), stoi(arg2));
		catchError = threshold.Apply(srctoBW, des);
		}
			break;
		case 2:
		{int size = stoi(argv[3]);
		AverageLocalThreshold fun;
		Size cuaso = Size(size, size);
		fun.getC(stoi(argv[4]));
		catchError = fun.Apply(srctoBW, des, cuaso);
		}
			break;
		case 3:
		{int size = stoi(argv[3]);
		MedianLocalThreshold fun;
		Size cuaso = Size(size, size);
		fun.getC(stoi(argv[4]));
		catchError = fun.Apply(srctoBW, des, cuaso);
		}
			break;
		case 4:
		{int size = stoi(argv[3]);
		SauvolaLocalThreshold fun;
		Size cuaso = Size(size, size);
		catchError = fun.Apply(srctoBW, des, cuaso);
		}
			break;
		case 5:
		{Kmean fun;
		fun.Cluster(stoi(argv[3]));
		catchError = fun.Apply(src, des);
		}
			break;
		default:
			break;
		}
		if (catchError == 0)
		{
			cerr << "Failed to process function";
			return -1;
		}
		// Hiển thị ảnh kết quả
		namedWindow("Output image");
		imshow("Output image", des);
		imwrite("final.jpg", des);

	

	waitKey(0);
	return 0;
}