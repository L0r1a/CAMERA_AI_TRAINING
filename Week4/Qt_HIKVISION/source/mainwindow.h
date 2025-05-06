#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QWindow.h>
#include <QMessageBox.h>
#include <qdebug.h>
#include <windows.h>
#include <opencv2/opencv.hpp>
#include <QTimer>
#include <MvCameraControl.h>

#define MV_NULL    0

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    void GrabImage();  // Khoi dong lay anh
    int RGB2BGR( unsigned char* pRgbData, unsigned int nWidth, unsigned int nHeight); //Doi anh RGB sang BGR (do opencv)
    cv::Mat Convert2Mat(MV_FRAME_OUT_INFO_EX* pstImageInfo, unsigned char * pData); // Doi data frame thanh data Mat

private slots:
    void on_Enumerate_clicked();        // Thuc hien khi nhan Find Device
    void on_Start_clicked();            // Thuc hien khi nhan start
    void on_Stop_clicked();             // Thuc hien khi nhan stop
    void GetImage();                    // Ham thuc thi lay anh

private:
    Ui::MainWindow *ui;

public:
    void Initialize(); // Ham khoi dong SDK
    int CloseDevice();
    void ConvBayer2RGB();

private:
    MV_FRAME_OUT stImageOut;
    MV_FRAME_OUT_INFO_EX    stImageInfo;
    MV_CC_DEVICE_INFO_LIST  stDeviceList;
    MV_CC_IMAGE   stImageData;

    void* pcMyCamera;
    unsigned int g_nPayloadSize; // data image size
    unsigned char* pData; // Image info

    unsigned char *pConvertData;
    unsigned int nConvertDataSize ;

    QTimer* timer;
};

#endif // MAINWINDOW_H
