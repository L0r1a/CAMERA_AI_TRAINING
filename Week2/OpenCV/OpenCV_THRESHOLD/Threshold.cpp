// Chương trình chuyển ảnh RGB sang Grayscale và ảnh trắng đen
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

#define MAX_VALUE 255
#define MAX_BINARY_VALUE 225

using namespace cv;
using std::cout;

const char* imageName = "lenna.png";
const char* windowName = "THRESHOLD_OPERATION_VAS_TRAINING";
int threshold_value = 125;
int threshold_type = 1;
// Tạo Mat Object chứa dữ liệu ảnh nguồn, ảnh grayscale và ảnh trắng đen
Mat src, src_gray, dst_mat;

int display_dst(int delay, Mat dst); 
int display_caption(const char* caption); 

int main(int argc, char** argv){
    src = imread(samples::findFile(imageName), IMREAD_COLOR);

    if (src.empty())
    {
        cout << "Cannot Read Image" << std::endl;
        return -1;
    }

    for (;;)
    {
        if (display_caption("Original Image") != 0)
        {
            return 0;
        }


        display_dst(1500,src);
        cvtColor(src, src_gray, COLOR_BGR2GRAY);

        if (display_caption("GrayScale Image") != 0)
        {
            return 0;
        }

        display_dst(1500, src_gray);
        threshold(src_gray, dst_mat, threshold_value, MAX_BINARY_VALUE, threshold_type);

        if (display_caption("Threshold") != 0)
        {
        
            return 0;
        }

        display_dst(1500, dst_mat);

        char c = (char)waitKey(50);
        // Nhấn ESC để thoát khỏi chương trình
        if (c == 27)
        {
            break;
        }
    }
    return 0;
}


int display_caption(const char* caption) // Tạo caption
{
    Mat dst;
    dst = Mat::zeros(src.size(), src.type());
    putText(dst, caption,
        Point(src.cols / 4, src.rows / 2),
        FONT_HERSHEY_COMPLEX, 1, Scalar(255, 255, 255));

    return display_dst(1500, dst);
}


int display_dst(int delay, Mat dst) // Hiện hình ảnh và delay theo khoản tùy chỉnh
{
    imshow(windowName, dst);
    int c = waitKey(delay);
    if (c >= 0) { return -1; }
    return 0;
}