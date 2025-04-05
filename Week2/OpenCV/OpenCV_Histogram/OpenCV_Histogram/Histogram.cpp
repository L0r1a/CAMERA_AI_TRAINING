// Chương trình plot ra biểu đồ Histogram của ảnh grayscale được chuyển từ ảnh RGB
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	const char* imageName = "cat.png";
	const char* windowName = "HISTOGRAM - VAS OPENCV TRAINING";
	Mat src;
	src	= imread(samples::findFile(imageName), IMREAD_COLOR);

	if (src.empty())
	{
		cout << "Can not find image file" << endl;
		return -1;
	}
    cvtColor(src,src,COLOR_BGR2GRAY);

	int histSize = 256;
	float range[] = { 0, 256 }; //Xác định khoảng giá trị của histogram
	const float* histRange[] = { range };
	bool uniform = true, accumulate = false;

	Mat histogram; // Mat object chứa dữ liệu biểu đồ Histogram
	calcHist(&src, 1, 0, Mat(), histogram, 1, &histSize, histRange, uniform, accumulate); // Tính toán Biều đồ hist

	int hist_w = 512, hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
	normalize(histogram,  histogram, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	for (int i = 1; i < histSize; i++) // Vẽ biểu đồ histogram
	{
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(histogram.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(histogram.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
	}
	// Hiện ảnh và biểu đồ Histogram của ảnh (grayscale)
    imshow(windowName, src);
	imshow("calcHist Demo", histImage);

	waitKey();

	return 0;
}