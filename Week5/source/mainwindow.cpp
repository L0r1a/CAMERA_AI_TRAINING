#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QButtonGroup* mode = new QButtonGroup(this);
    mode->setExclusive(TRUE);
    mode->addButton(ui->Continuous);
    mode->addButton(ui->Trigger);
    ui->Continuous->setChecked(TRUE);
    Initialize();
    qDebug()<<"SDK VERSION:"<<MV_CC_GetSDKVersion();

    QIntValidator* intValidator = new QIntValidator(0, 100);
    ui->Time_delay->setValidator(intValidator);

    intValidator = new QIntValidator(0, 2000);
    ui->Height->setValidator(intValidator);

    intValidator = new QIntValidator(0, 2000);
    ui->Width->setValidator(intValidator);

    QDoubleValidator* DoubleValidator = new QDoubleValidator(0, 24,2);
    ui->FrameRate->setValidator(DoubleValidator);

    DoubleValidator = new QDoubleValidator(0, 1000,2);
    ui->Expotime->setValidator(DoubleValidator);

    DoubleValidator = new QDoubleValidator(0, 10000,2);
    ui->Expotime->setValidator(DoubleValidator);

    DoubleValidator = new QDoubleValidator(0, 20,2);
    ui->Gain->setValidator(DoubleValidator);
}

MainWindow::~MainWindow()
{
    Finalize();
    delete ui;
}

unsigned int __stdcall GrabThread(void* pUser)
{
    if(pUser)
    {
        MainWindow* pCam = (MainWindow*)pUser;
        pCam ->GrabThreadProcess();
        return 0;
    }
    return -1;
}

void MainWindow::Initialize()
{
    int nRet = MV_CC_Initialize(); // Initialize SDK
    if (nRet != MV_OK)
    {
        QString error = "Cannot Initialize SDK";
        QMessageBox::critical(this, "Error", error);
        return;
    }
}

void MainWindow::Finalize()
{
    MV_CC_Finalize(); // Close SDK
}

int MainWindow::CloseDevice()
{
    bThreadState = FALSE;
    if (CameraThread)
    {
        CameraThread->wait();
        CameraThread->quit();
        CameraThread = NULL;
    }

    if (pcMyCamera)
    {
        pcMyCamera->Close();
        delete pcMyCamera;
        pcMyCamera = NULL;
    }
    bStartGrabbing = FALSE;
    bCameraOpened = FALSE;

    if (pSaveImageBuf)
    {
        free(pSaveImageBuf);
        pSaveImageBuf = NULL;
    }
    nSaveImageBufSize = 0;

    return MV_OK;
}

int MainWindow::SetTriggerMode()
{
    return pcMyCamera->SetEnumValue("TriggerMode", bTriggerMode);
}

int MainWindow::SetGain()
{
    pcMyCamera->SetEnumValue("GainAuto", 0);
    return pcMyCamera->SetFloatValue("Gain", (float)Gain);
}

int MainWindow::GetGain()
{
    MVCC_FLOATVALUE stFloatValue = {0};

    int nRet = pcMyCamera->GetFloatValue("Gain", &stFloatValue);
    if (MV_OK != nRet)
    {
        return nRet;
    }
    Gain = stFloatValue.fCurValue;

    return MV_OK;
}

int MainWindow::SetFrameRate()
{
    int nRet = pcMyCamera->SetBoolValue("AcquisitionFrameRateEnable", true);
    if (MV_OK != nRet)
    {
        return nRet;
    }
    return pcMyCamera->SetFloatValue("AcquisitionFrameRate", (float)FrameRate);
}

int MainWindow::GetFrameRate()
{
    MVCC_FLOATVALUE stFloatValue = {0};

    int nRet = pcMyCamera->GetFloatValue("ResultingFrameRate", &stFloatValue);
    if (MV_OK != nRet)
    {
        return nRet;
    }
    FrameRate = stFloatValue.fCurValue;

    return MV_OK;
}

int MainWindow::SetExposureTime()
{
    pcMyCamera->SetEnumValue("ExposureAuto", MV_EXPOSURE_AUTO_MODE_OFF);
    return pcMyCamera->SetFloatValue("ExposureTime", (float)Exposure);
}

int MainWindow::GetExposureTime()
{
    MVCC_FLOATVALUE stFloatValue = {0};

    int nRet = pcMyCamera->GetFloatValue("ExposureTime", &stFloatValue);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    Exposure = stFloatValue.fCurValue;

    return MV_OK;
}

int MainWindow::SetTriggerSource()
{
    int nRet = MV_OK;
    nRet = pcMyCamera->SetEnumValue("TriggerSource", MV_TRIGGER_SOURCE_SOFTWARE);
    if (MV_OK != nRet)
    {
        QString error = "Set Software Trigger Fail";
        QMessageBox::critical(this, "Error", error);
        return nRet;
    }
    return nRet;
}

void MainWindow::GrabThreadProcess()
{


    while(bThreadState)
    {

        cv::Mat* getImage = new cv::Mat();
        cv::Mat dstImgB;
        unsigned int nRecvBufSize = 0;
        MVCC_INTVALUE stParam;
        memset(&stParam, 0, sizeof(MVCC_INTVALUE));
        int nRet = MV_CC_GetIntValue(pcMyCamera->m_hDevHandle, "PayloadSize", &stParam);
        unsigned int n_nSaveImageBufSize = 0;
        unsigned char* n_pSaveImageBuf;

        MV_FRAME_OUT_INFO_EX n_stImageInfo = {0};

        if (nRet != 0)
        {
            return;
        }
        nRecvBufSize = stParam.nCurValue;
        unsigned char* pDate;
        pDate = (unsigned char*)malloc(nRecvBufSize);

        if (!bStartGrabbing)
        {
            Sleep(10);
            continue;
        }

        nRet = MV_CC_GetOneFrameTimeout(pcMyCamera->m_hDevHandle, pDate, nRecvBufSize, &n_stImageInfo, 1000);


        if (nRet == MV_OK)
        {
            n_nSaveImageBufSize = n_stImageInfo.nWidth * n_stImageInfo.nHeight * 3 + 2048;
            n_pSaveImageBuf = (unsigned char*)malloc(n_nSaveImageBufSize);

            bool isMono;
            switch (n_stImageInfo.enPixelType)
            {
            case PixelType_Gvsp_Mono8:
            case PixelType_Gvsp_Mono10:
            case PixelType_Gvsp_Mono10_Packed:
            case PixelType_Gvsp_Mono12:
            case PixelType_Gvsp_Mono12_Packed:
                isMono = true;
                break;
            default:
                isMono = false;
                break;
            }
            if (isMono)
            {
                *getImage = cv::Mat(n_stImageInfo.nHeight, n_stImageInfo.nWidth, CV_8UC1, pDate);
                // imwrite ("D: \\ Test OpenCV_MONO.TIF", Image);
            }
            else
            {
                // Conversion image format is BGR8
                MV_CC_SetBayerCvtQuality(pcMyCamera->m_hDevHandle, 1);

                MV_CC_PIXEL_CONVERT_PARAM stConvertParam = { 0 };
                memset(&stConvertParam, 0, sizeof(MV_CC_PIXEL_CONVERT_PARAM));
                stConvertParam.nWidth =  n_stImageInfo.nWidth;                 // CH: Image wide | EN: image width
                stConvertParam.nHeight = n_stImageInfo.nHeight;               // CH: Image High | EN: Image Heart
                stConvertParam.pSrcData = pDate;                  // CH: Enter data cache | EN: Input data buffer
                stConvertParam.nSrcDataLen = n_stImageInfo.nFrameLen;         // CH: Enter data size | EN: INPUT DATA SIZE
                stConvertParam.enSrcPixelType = n_stImageInfo.enPixelType;    // CH: Enter a pixel format | EN: INPUT PIXEL FORMAT
                stConvertParam.enDstPixelType = PixelType_Gvsp_BGR8_Packed; // CH: Output pixel format | EN: Output Pixel Format Applicable to OpenCV Image Format
                stConvertParam.pDstBuffer = n_pSaveImageBuf;                    //  EN: OUTPUT DATA BUFFER
                stConvertParam.nDstBufferSize = n_nSaveImageBufSize;            //  EN: OUTPUT BUFFER SIZE
                MV_CC_ConvertPixelType(pcMyCamera->m_hDevHandle, &stConvertParam);

                *getImage = cv::Mat(n_stImageInfo.nHeight, n_stImageInfo.nWidth, CV_8UC3, n_pSaveImageBuf);

            }
            (*getImage).copyTo(dstImga);
            cv::cvtColor(dstImga, dstImgB, COLOR_BGR2RGB);

            QImage Qtemp = QImage((const unsigned char*)(dstImgB.data), dstImgB.cols, dstImgB.rows, dstImgB.step, QImage::Format_RGB888);
            ui->Camera_View->setScaledContents(true);
            ui->Camera_View->setPixmap(QPixmap::fromImage(Qtemp));


            (*getImage).release();
            free(pDate);
            free(n_pSaveImageBuf);

        }
        else
        {
            if (MV_TRIGGER_MODE_ON ==  bTriggerMode)
            {
                Sleep(TriggerModeDelay);
            }
        }
    }

}



void MainWindow::on_Open_clicked()
{
    if(bCameraOpened == TRUE || pcMyCamera != NULL) return;

    int nIndex = ui->comboBox->currentIndex();
    pcMyCamera = new CMvCamera;
    if(NULL == pcMyCamera)
    {
        return;
    }

    int nRet = pcMyCamera->Open(stDevList.pDeviceInfo[nIndex]);
    if (nRet != MV_OK)
    {
        delete pcMyCamera;
        pcMyCamera = NULL;
        QString error = "Open Fail";
        QMessageBox::critical(this, "Error", error);
        return;
    }
    // Choose Optimized packet size
    unsigned int nPacketSize = 0;
    nRet = pcMyCamera->GetOptimalPacketSize(&nPacketSize);
    if (nRet == MV_OK)
    {
        nRet = pcMyCamera->SetIntValue("GevSCPSPacketSize",nPacketSize);
        if (nRet != MV_OK)
        {
            QString warning = "Set packet size fail";
            QMessageBox::warning(this, "Warning", warning);
        }
    }
    else
    {
        QString warning = "Get packet size fail";
        QMessageBox::warning(this, "Warning", warning);
    }

    bCameraOpened = TRUE;
    SetTriggerMode();
    qDebug()<<"Camera Device Created";
    on_GetParam_clicked();
}


void MainWindow::on_Close_clicked()
{
    if (!bCameraOpened) return;
    CloseDevice();
}


void MainWindow::on_SoftwareTrigger_clicked()
{
    if (!bStartGrabbing) return;
    pcMyCamera->CommandExecute("TriggerSoftware");
}


void MainWindow::on_Start_clicked()
{
    if (!bCameraOpened || bStartGrabbing || pcMyCamera == NULL)
    {
        return;
    }
    memset(&stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
    bThreadState = TRUE;

    int nRet = pcMyCamera->StartGrabbing();
    if (MV_OK != nRet)
    {
        bThreadState = FALSE;
        QString error = "Start Grabbing Fail";
        QMessageBox::critical(this, "Error", error);
        return;
    }

    CameraThread = QThread::create(GrabThread, this);
    CameraThread->start();



    bStartGrabbing = TRUE;
    bThreadState = TRUE;
}


void MainWindow::on_Stop_clicked()
{
    if(bCameraOpened == FALSE || bStartGrabbing == FALSE || pcMyCamera == NULL)
    {
        return;
    }

    bThreadState = FALSE;
    if(CameraThread)
    {
        CameraThread->wait();
        CameraThread = NULL ;
    }

    int nRet = pcMyCamera->StopGrabbing();
    if (MV_OK != nRet)
    {
        QString error = "Stop Grabbing Fail";
        QMessageBox::critical(this, "error", error);
        return;
    }
    bStartGrabbing = FALSE;
}


void MainWindow::on_GetParam_clicked()
{
    int nRet = GetGain();
    if (MV_OK != nRet)
    {
        QString error = "Get Gain Unsuccessfull";
        QMessageBox::warning(this, "Warning", error);
    }
    nRet = GetFrameRate();
    if (MV_OK != nRet)
    {
        QString error = "Get FPS Unsuccessfull";
        QMessageBox::warning(this, "Warning", error);
    }
    nRet = GetExposureTime();
    if (MV_OK != nRet)
    {
        QString error = "Get Exposure Time Unsuccessfull";
        QMessageBox::warning(this, "Warning", error);
    }

    ui->FrameRate->setText(QString::number(FrameRate));
    ui->Expotime->setText(QString::number(Exposure));
    ui->Gain->setText(QString::number(Gain));
    ui->Time_delay->setText(QString::number(TriggerModeDelay));
    ui->Width->setText(QString::number(ImgWidth));
    ui->Height->setText(QString::number(ImgHeight));
}


void MainWindow::on_SetParam_clicked()
{
    on_Stop_clicked();

    QString Text = ui->Time_delay->text();
    TriggerModeDelay = Text.toInt();

    Text = ui->Width->text();
    ImgWidth = Text.toInt();

    Text = ui->Height->text();
    ImgHeight = Text.toInt();

    Text = ui->Gain->text();
    Gain = Text.toDouble();
    int nRet = SetGain();
    if (nRet != MV_OK)
    {
        QString error = "Set Gain Unsuccessfull";
        QMessageBox::warning(this, "Warning", error);
    }

    Text = ui->FrameRate->text();
    FrameRate = Text.toDouble();
    nRet = SetFrameRate();
    if (nRet != MV_OK)
    {
        QString error = "Set FrameRate Unsuccessfull";
        QMessageBox::warning(this, "Warning", error);
    }

    Text = ui->Expotime->text();
    Exposure = Text.toDouble();
    nRet = SetExposureTime();
    if (nRet != MV_OK)
    {
        QString error = "Set Exposure Unsuccessfull";
        QMessageBox::warning(this, "Warning", error);
    }

    on_Start_clicked();
}


void MainWindow::on_Capture_pressed()
{
    QString ImagePath = QString("C:/Users/Thinkpad T480S/Desktop/Qt_project/Qt_HIKROBOT/Capture/IMG_%1.png").arg(ImgNum);
    ImgNum++;
    cv::Mat saveImg = dstImga.clone();
    on_Stop_clicked();
    cv::cvtColor(saveImg, dstImga, COLOR_BGR2RGB);
    cv::resize(saveImg, saveImg, cv::Size(ImgWidth, ImgHeight),INTER_LINEAR);
    QImage Qtemp = QImage((const unsigned char*)(saveImg.data), saveImg.cols, saveImg.rows, saveImg.step, QImage::Format_RGB888);
    Qtemp.save(ImagePath,"PNG");
    on_Start_clicked();
}




void MainWindow::on_Enumerate_clicked()
{
    int nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE, &stDevList);
    if(nRet != MV_OK)
    {
        QMessageBox::critical(this, "Error", "Cannot Enumerate Device");
        return;
    }


    else if (stDevList.nDeviceNum > 0)
    {
        for (unsigned int i = 0; i <stDevList.nDeviceNum ; i++)
        {
            MV_CC_DEVICE_INFO* pDeviceInfo = stDevList.pDeviceInfo[0];

            if (pDeviceInfo == NULL)
            {
                QMessageBox::critical(this, "Error", "Find Device Info Fail");
                continue;
            }

            wchar_t* pUserName = NULL;

            if(pDeviceInfo->nTLayerType == MV_GIGE_DEVICE)
            {
                int nIp1 = ((pDeviceInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
                int nIp2 = ((pDeviceInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
                int nIp3 = ((pDeviceInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
                int nIp4 = (pDeviceInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);
                if (std::strcmp("",(LPCSTR)(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName)) != 0)
                {
                    DWORD dwLenUserName = MultiByteToWideChar(CP_ACP,0,(LPCSTR)(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName),-1,NULL,0);
                    pUserName = new wchar_t[dwLenUserName];
                    MultiByteToWideChar(CP_ACP,0,(LPCSTR)(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName),-1,pUserName,dwLenUserName);
                }
                else
                {
                    DWORD dwLenUserName = MultiByteToWideChar(CP_ACP,0,(LPCSTR)(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName),-1,NULL,0);
                    pUserName = new wchar_t[dwLenUserName];
                    MultiByteToWideChar(CP_ACP,0,(LPCSTR)(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName),-1,pUserName,dwLenUserName);
                }
                QString GIGE_Info = QString("Found device IPs: %1.%2.%3.%4").arg(nIp1).arg(nIp2).arg(nIp3).arg(nIp4);
                ui->comboBox->addItem(GIGE_Info);
            }
            else
            {
                QMessageBox::warning(this,"warning","Unknown Decice");
                QString text = QString("Unknown equipment [% 1]").arg(i);
                ui->comboBox->addItem(text);
            }
            if (pUserName)
            {
                delete [] pUserName;
                pUserName = NULL;
            }
        }
    }

    if (stDevList.nDeviceNum == 0)
    {
        QMessageBox::critical(this, "Error", "Cannot Find Any Device");
        return;
    }
}



void MainWindow::on_Trigger_clicked()
{
    bTriggerMode = ui->Trigger->isChecked();
    qDebug()<<"Trigger Mode -- MODE" << bTriggerMode;
    int nRet = SetTriggerMode();
    if (nRet != MV_OK)
    {
        QString error = "Set Trigger Mode Fail";
        QMessageBox::critical(this, "Error", error);
        return;
    }
}


void MainWindow::on_Continuous_clicked()
{
    bTriggerMode = !(ui->Continuous->isChecked());
    qDebug()<<"Continuous Mode -- MODE" << bTriggerMode;
    int nRet = SetTriggerMode();
    if (nRet != MV_OK)
    {
        QString error = "Set Trigger Mode Fail";
        QMessageBox::critical(this, "Error", error);
        return;
    }
}



