#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWindow.h>
#include <QMessageBox.h>
#include <QButtonGroup.h>
#include <QThread.h>
#include <QImage>
#include <qdebug.h>
#include <windows.h>
#include <opencv2/opencv.hpp>
#include <QTimer>
#include <CMvCamera.h>

#define MV_NULL    0
#define TRIGGER_MODE 1
#define CONTINUOUS_MODE 0

QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;


private:
    void Initialize(); // Ham khoi dong SDK
    void Finalize();
    int RGB2BGR( unsigned char* pRgbData, unsigned int nWidth, unsigned int nHeight); //Doi anh RGB sang BGR (do opencv)
    cv::Mat Convert2Mat(MV_FRAME_OUT_INFO_EX* pstImageInfo, unsigned char * pData); // Doi data frame thanh data Mat
    void ConvBayer2RGB(MV_FRAME_OUT pstImageInfo, unsigned char* pConvertData, unsigned int nConvertDataSize);

private :
    int SetTriggerMode();
    int SaveImage();
    int GetGain();                       //Set Gain
    int SetGain();
    int GetFrameRate();                  //Set Frame Rate
    int SetFrameRate();
    int SetTriggerSource();
    int GetExposureTime();
    int SetExposureTime();
    int CloseDevice();                   //Close Device

public:

    QThread* CameraThread = NULL;
    CMvCamera* pcMyCamera = NULL;
    cv::Mat dstImga;


    MV_CC_DEVICE_INFO_LIST  stDevList;
    CRITICAL_SECTION        hSaveImageMux;
    MV_CC_IMAGE             imgParam;
    MV_CC_SAVE_IMAGE_PARAM  saveParam;

    unsigned char*          pSaveImageBuf = NULL;
    unsigned int            nSaveImageBufSize = 0;
    MV_FRAME_OUT_INFO_EX    stImageInfo;

    int ImgNum = 0;

public:
    bool bCameraOpened = FALSE;
    bool bStartGrabbing = FALSE;
    bool bTriggerMode = FALSE;
    bool bDeviceFound = FALSE;
    bool bThreadState = FALSE;


    double FrameRate;
    double Gain;
    double Exposure;
    int TriggerModeDelay = 5;
    int ImgWidth = 1280;
    int ImgHeight = 720;

private slots:
    void on_Open_clicked();
    void on_Close_clicked();
    void on_SoftwareTrigger_clicked();
    void on_Start_clicked();
    void on_Stop_clicked();
    void on_GetParam_clicked();
    void on_SetParam_clicked();
    void on_Capture_pressed();
    void on_Enumerate_clicked();
    void on_Trigger_clicked();
    void on_Continuous_clicked();


public:
    void GrabThreadProcess();
};

#endif // MAINWINDOW_H
