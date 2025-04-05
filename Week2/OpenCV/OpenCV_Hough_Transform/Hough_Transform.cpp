// Chương trình phát hiện đường sử dụng Hough-transform và Canny Edge Detection
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    // Khai báo giá trị đầu ra
    Mat dst, cdst, cdstP;
    const char* filename = "sudoku.jpg";
    const char* windowName = "HOUGH_VAS_OPENCV_TRAINING";

    // Tãi dữ liệu ảnh
    Mat src = imread(samples::findFile(filename), IMREAD_GRAYSCALE);

    // Kiểm tra xem nếu ảnh đã tải lên thành công
    if (src.empty()) {
        printf(" Error opening image\n");
        return -1;
    }

    //  Áp dụng thuật toán Canny phát hiện cạnh
    Canny(src, dst, 50, 200, 3);

    // Chuyển thông tin edge thành hình ảnh có màu
    cvtColor(dst, cdst, COLOR_GRAY2BGR);
    cdstP = cdst.clone();

    // Sử dùng Hough-transform tiêu chuẩn
    vector<Vec2f> lines; // vector Lines sẽ giữ đầu ra của Hough-transform
    HoughLines(dst, lines, 1, CV_PI / 180, 150, 0, 0); // chạy thuật toán
    // Vẽ đường 
    for (size_t i = 0; i < lines.size(); i++)
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a * rho, y0 = b * rho;
        pt1.x = cvRound(x0 + 1000 * (-b));
        pt1.y = cvRound(y0 + 1000 * (a));
        pt2.x = cvRound(x0 - 1000 * (-b));
        pt2.y = cvRound(y0 - 1000 * (a));
        line(cdst, pt1, pt2, Scalar(0, 0, 255), 3, LINE_AA);
    }

  
    // Hiển thị kết quả
    imshow(windowName, src);
    imshow(windowName, cdst);

    waitKey();
    return 0;
}