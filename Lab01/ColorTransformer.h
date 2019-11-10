#pragma once
#include<opencv2/opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;

class ColorTransformer
{
public:
	/*
	Hàm nhận vào một ảnh, thay đổi độ sáng của ảnh này và lưu kết quả vào ảnh mới
	Tham so:
		sourceImage	: ảnh ban đầu
		destinationImage: ảnh kết quả
		b	: giá trị số nguyên dùng để thay đổi độ sáng của ảnh
	Hàm trả về:
		1: Nếu thành công thì trả về ảnh kết quả (ảnh gốc vẫn giữ nguyên giá trị)
		0: Nếu không tạo được ảnh kết quả hoặc ảnh input không tồn tại
	*/
	int ChangeBrighness(const Mat& sourceImage, Mat& destinationImage, short b);

	/*
	Hàm nhận vào một ảnh, thay đổi độ tương phản của ảnh này và lưu kết quả vào ảnh mới
	Tham so :
		sourceImage : ảnh ban đầu
		destinationImage : ảnh kết quả
		c	: giá trị số thực dùng để thay đổi độ tương phản của ảnh
	Hàm trả về:
		1: Nếu thành công thì trả về ảnh kết quả (ảnh gốc vẫn giữ nguyên giá trị)
		0: Nếu không tạo được ảnh kết quả hoặc ảnh input không tồn tại
	*/	
	int ChangeContrast(const Mat& sourceImage, Mat& destinationImage, float c);
	
	
	/*
	Hàm cân bằng lược đồ màu tổng quát cho ảnh bất kỳ
	Tham so :
		sourceImage : ảnh ban đầu
		destinationImage : ảnh kết quả
	Hàm trả về:
		1: Nếu thành công thì trả về ảnh kết quả (ảnh gốc vẫn giữ nguyên giá trị)
		0: Nếu không tạo được ảnh kết quả hoặc ảnh input không tồn tại
	*/
	int HistogramEqualization(const Mat& sourceImage, Mat& destinationImage);
	
	/*
	Hàm cân bằng lược đồ màu tổng quát cho ảnh bất kỳ
	Tham so :
		image : ảnh dùng để tính histogram
		histImage : ảnh histogram
	Hàm trả về:
		1: Nếu thành công vẽ được histogram
		0: Nếu không vẽ được histogram
	*/
	int HistogramEqualization1(const Mat& sourceImage, Mat& destinationImage);
	
	
	/*
	Hàm so sánh hai ảnh
	Tham so :
		image1 : ảnh thứ nhất
		image2 : ảnh thứ hai
	Hàm trả về:
		độ đo sự tương đồng giữa hai ảnh
		Chọn độ đo Chi-Squared
	*/
	float CompareImage(const Mat& image1, Mat& image2);
	
	
	ColorTransformer();
	~ColorTransformer();
};

/*
Hàm thống kê phân bố của ảnh sourceImage và lưu lại trong ảnh destinationImage
Tham số:
	sourceImage : Ảnh dùng để tìm histogram. Có thể là ảnh màu hoặc ảnh xám
	destinationImage: Ảnh lưu lại phân bố của ảnh gốc.
		Nếu là ảnh xám, lưu lại bằng một kênh màu
		Nếu là ảnh màu, lưu lại bằng ba kênh theo thứ tự BGR.
Hàm trả về:
	1: nếu thực thi thành công
	0: Nếu không thành công
*/
int CalculateHistogram(const Mat& sourceImage, Mat& destinationImage);

int drawHistogram(const Mat& sourceImage, Mat& destinationImage);