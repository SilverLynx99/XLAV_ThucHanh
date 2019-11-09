#include "Converter.h"

int Converter::RGB2GrayScale(const Mat& sourceImage, Mat& destinationImage)
{
	destinationImage.create(sourceImage.size(), CV_8UC1);

	if (sourceImage.data)
	{
		int rows = sourceImage.rows;
		int cols = sourceImage.cols;
		int i, j;
		for (i = 0; i < rows; i++)
		{
			for (j = 0; j < cols; j++)
			{
				Vec3b intensity = sourceImage.at<Vec3b>(i, j);
				int blue = intensity.val[0];
				int green = intensity.val[1];
				int red = intensity.val[2];
				
				destinationImage.at<uchar>(i, j) = blue * 0.0722 + green * 0.7152 + red * 0.2126;
			}
		}
		return 1;
	}
	else
		return 0;
}

int Converter::GrayScale2RGB(const Mat& sourceImage, Mat& destinationImage)
{
	if (sourceImage.data)
	{
		// cvtColor(sourceImage, destinationImage, CV_GRAY2BGR);
		return 1;
	}
	else
		return 0;
}

int Converter::RGB2HSV(const Mat& sourceImage, Mat& destinationImage)
{
	destinationImage.create(sourceImage.size(), CV_8UC3);

	if (sourceImage.data) {
		int rows = sourceImage.rows;
		int cols = sourceImage.cols;
		int i, j, V, mini;
		float S, H;
		for (i = 0; i < rows; i++)
		{
			for (j = 0; j < cols; j++)
			{
				Vec3b intensity = sourceImage.at<Vec3b>(i, j);
				int blue = intensity.val[0];
				int green = intensity.val[1];
				int red = intensity.val[2];
				//V <- max(R,G,B)
				V = max(blue, max(green, red));
				mini = min(blue, min(green, red));
				// S <--
				if (V == 0)
				{
					S == 0;
					H == 0;
				}
				else
				{
					int delta = V - mini;
					S = 1.0 * (V - mini) / V;
					//H <--
					if (V == red)
						H = 1.0 * (60 * (green - blue)) / delta;
					else if (V == green)
						H = 120 + 1.0 * 60 * (blue - red) / delta;
					else if (V == blue)
						H = 240 + 1.0 * 60 * (red - green) / delta;

					if (H < 0)
						H += 360;

				}
				destinationImage.at<Vec3b>(i, j) = Vec3b(H / 2, S * 255, V);
			}
		}
		return 1;
	}
	else
		return 0;
}

int Converter::HSV2RGB(const Mat& sourceImage, Mat& destinationImage)
{
	destinationImage.create(sourceImage.size(), CV_8UC3);

	if (sourceImage.data) {
		return 1;
		int rows = sourceImage.rows;
		int cols = sourceImage.cols;
		int i, j, V;
		float C, HPrime, X, M;
		float R, G, B;
		for (i = 0; i < rows; i++)
		{
			for (j = 0; j < cols; j++)
			{
				Vec3b intensity = sourceImage.at<Vec3b>(i, j);
				float H = intensity.val[0] * 2.0;
				float S = intensity.val[1] / 255.0;
				float V = intensity.val[2] / 255.0;
				C = V * S;
				HPrime = fmod(H / 60.0, 6);
				X = C * (1 - fabs(fmod(HPrime, 2) - 1));
				M = V - C;
				if (0 <= HPrime && HPrime < 1)
				{
					R = C;
					G = X;
					B = 0;
				}
				else if (1 <= HPrime && HPrime < 2) {
					R = X;
					G = C;
					B = 0;
				}
				else if (2 <= HPrime && HPrime < 3) {
					R = 0;
					G = C;
					B = X;
				}
				else if (3 <= HPrime && HPrime < 4) {
					R = 0;
					G = X;
					B = C;
				}
				else if (4 <= HPrime && HPrime < 5) {
					R = X;
					G = 0;
					B = C;
				}
				else if (5 <= HPrime && HPrime < 6) {
					R = C;
					G = 0;
					B = X;
				}
				else
				{
					R = 0;
					G = 0;
					B = 0;
				}

				destinationImage.at<Vec3b>(i, j) = Vec3b((B + M) * 255.0, (G + M) * 255.0, (R + M) * 255.0);
			}
		}
	}
	else
		return 0;
}

int Converter::Convert(Mat& sourceImage, Mat& destinationImage, int type)
{
	int returnValue = 1;
	switch (type)
	{
	case 0:
		returnValue = RGB2GrayScale(sourceImage, destinationImage);
		break;
	case 1:
		returnValue = GrayScale2RGB(sourceImage, destinationImage);
		break;
	case 2:
		returnValue = RGB2HSV(sourceImage, destinationImage);
		break;
	case 3:
		returnValue = HSV2RGB(sourceImage, destinationImage);
		break;
	default:
		break;
	}
	return returnValue;
}

Converter::Converter()
{
}

Converter::~Converter()
{
}
