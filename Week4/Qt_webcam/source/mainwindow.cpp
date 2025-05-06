#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace cv;
using namespace std;

QPixmap Mat2QPixmp(Mat matImage)
{
    if ( matImage.type() == CV_8UC3) // kiem tra neu hinh co 4 channel
    {
        QImage image(matImage.data,  // Load data hinh
                     matImage.cols, // Chieu rong
                     matImage.rows, // Chieu cao
                     static_cast<int>(matImage.step), // Image matrix element type
                     QImage::Format_RGB888); // Format

        return QPixmap::fromImage(image.rgbSwapped()); // chuyen BGR sang RGB
    }
    else if (matImage.type() == CV_8UC1) // kiem tra neu hinh la grayscale hoac 1 channel
    {
        QImage image(matImage.data,  // Load data hinh
                     matImage.cols, // Chieu rong
                     matImage.rows, // Chieu cao
                     static_cast<int>(matImage.step), // Image matrix element type
                     QImage::Format_Grayscale8); // Format

        return QPixmap::fromImage(image);
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->Reset->setChecked(true);

    QButtonGroup *group = new QButtonGroup(this);
    group->setExclusive(true);
    group->addButton(ui->Mean);
    group->addButton(ui->Grayscale);
    group->addButton(ui->Reset);
    group->addButton(ui->Threshold);


    // Bao cao version cua OpenCV
    qDebug() << "OpenCV ver:" << CV_VERSION;
    qDebug() << "OpenCV major ver:" << CV_VERSION_MAJOR;
    qDebug() << "OpenCV minor ver:" << CV_VERSION_MINOR;


    std::thread capRun(captureRun, ui->label, ui->label_2, ui->Reset, ui->Grayscale, ui->Threshold, ui->Mean);
    capRun.detach();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::captureRun(QLabel *label1, QLabel *label2,QCheckBox *Reset,  QCheckBox *Grayscale,  QCheckBox *Threshold, QCheckBox *Mean)
{
    VideoCapture cap;

    Mat input_frame;
    Mat output_frame;
    int deviceID = 0;
    int apiID = cv::CAP_ANY;

    cap.open(deviceID, apiID);

    for (;;)
    {
        cap.read(input_frame);

        QPixmap pix = Mat2QPixmp(input_frame);
        label1->setPixmap(pix);

        output_frame = input_frame.clone();

        if(!Reset->isChecked())
        {
            if(Grayscale->isChecked())
            {
                cvtColor( output_frame, output_frame, COLOR_BGR2GRAY ); // Convert the image to Grayscale
            }
            else if(Threshold->isChecked())
            {
                cvtColor( output_frame, output_frame, COLOR_BGR2GRAY );
                threshold( output_frame, output_frame, 120, 255, 0 );
            }
            else if(Mean->isChecked())
            {
                Point anchor_point = {-1, -1};
                Size ksize = {5,5};
                boxFilter(output_frame,output_frame, -1, ksize ,anchor_point, true);
            }
        }

        pix = Mat2QPixmp(output_frame);
        label2->setPixmap(pix);
    }
}
