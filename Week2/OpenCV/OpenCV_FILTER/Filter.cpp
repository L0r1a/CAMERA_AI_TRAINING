#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;

int main(int argc, char** argv)
{
	// Declare Mat objects
	Mat src, dst;
	Mat kernel;

	Point anchor;
	double delta;
	int ddepth;
	int kernel_size;
	const char* window_name = "Image Filter_OpenCV VAS Training";

	const char* imageName = "lenna.png";

	// Load image data onto src
	src = imread(samples::findFile(imageName), IMREAD_COLOR);

	// Check if load is successful
	if (src.empty())
	{
		printf("Can't open image!!!!\n");
		return EXIT_FAILURE;
	}
	// Initialize argument for the filter 
	anchor = Point(-1, -1);
	delta = 0;
	ddepth = -1;

	// Filter the image with different kernel sizes (increasing every 0.5 sec)
	int ind = 0;
	for (;;)
	{
		kernel_size = 3 + 2 * (ind % 5);
		kernel = Mat::ones(kernel_size, kernel_size, CV_32F) / (float)(kernel_size * kernel_size); // Mean filter

		// Appkly Filter 
		filter2D(src, dst, ddepth, kernel, anchor, delta, BORDER_DEFAULT);
		imshow(window_name, dst);

		char c = (char)waitKey(500); // Wait for input for 500 ms
		// if ESC is pressed, exit
		if (c == 27)
		{
			break;
		}
		ind++;
	}


	return EXIT_SUCCESS;
}

