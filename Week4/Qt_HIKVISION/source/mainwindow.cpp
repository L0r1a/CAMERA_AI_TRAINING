#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int SDKver =  MV_CC_GetSDKVersion();

    qDebug() << "SDK Version: " << SDKver;
    MainWindow::Initialize();

    g_nPayloadSize = 0;
    stImageInfo = {0};
    pcMyCamera = NULL;

    pConvertData = NULL;
    nConvertDataSize = 0;
}

MainWindow::~MainWindow()
{
    timer->stop();
    delete ui;
    delete timer;
    MV_CC_Finalize();
}

void MainWindow::GrabImage()
{
    MVCC_INTVALUE stParameter;
    memset(&stParameter, 0, sizeof(MVCC_INTVALUE));
    int nRet = MV_CC_GetIntValue(pcMyCamera, "PayloadSize", &stParameter);
    if (nRet != MV_OK)
    {
        QMessageBox::warning(this,"Warning","Get Payload Failed");
        return;
    }
    g_nPayloadSize = stParameter.nCurValue;
    // Start grabbing
    nRet = MV_CC_StartGrabbing(pcMyCamera);


    if(nRet != MV_OK)
    {
        QMessageBox :: critical (this, "Error", "Start Grabbing Failed");
        return;
    }
    pData = (unsigned char*)malloc(sizeof (unsigned char)* (g_nPayloadSize));
    if (pData == NULL)
    {
        QMessageBox::critical(this,"error","Image Data Initialization Failed");
        return;
    }
    // Get a picture get one frame from Camera with timeout = 1000ms
    nRet = MV_CC_GetOneFrameTimeout(pcMyCamera, pData, g_nPayloadSize, &stImageInfo, 100);
    if (nRet == MV_OK)
    {
        QString text = QString("Get One Frame: Width[%1], Height[%2], nFrameNum[%3]\n").arg(stImageInfo.nWidth).arg(stImageInfo.nHeight).arg(stImageInfo.nFrameNum);
        ui->label->text();
    }
    else
    {
        QString text = QString("No image data! NRET [% 1]").arg(nRet);
        QMessageBox::warning(this,"Warning",text);
        free(pData);
        pData = NULL;
        return;
    }
}

int MainWindow::RGB2BGR( unsigned char* pRgbData, unsigned int nWidth, unsigned int nHeight )
{
    if ( NULL == pRgbData )
    {
        return MV_E_PARAMETER;
    }

    for (unsigned int j = 0; j < nHeight; j++)
    {
        for (unsigned int i = 0; i < nWidth; i++)
        {
            unsigned char red = pRgbData[j * (nWidth * 3) + i * 3];
            pRgbData[j * (nWidth * 3) + i * 3]     = pRgbData[j * (nWidth * 3) + i * 3 + 2];
            pRgbData[j * (nWidth * 3) + i * 3 + 2] = red;
        }
    }

    return MV_OK;
}

void MainWindow::ConvBayer2RGB()
{
    int nRet = MV_CC_SetBayerCvtQuality(pcMyCamera, 1);
    if (nRet != MV_OK)
    {
        QMessageBox::warning(this, "Warning", "Pixel Format Conversion Failed");
    }

    nConvertDataSize = stImageInfo.nExtendWidth * stImageInfo.nExtendHeight* 3;
    pConvertData = (unsigned char*)malloc(nConvertDataSize);

    if (NULL == pData)
    {
        qDebug() << "pData is Empty";
        return;
    }

    MV_CC_PIXEL_CONVERT_PARAM_EX stConvertParam = {0};
    stConvertParam.nWidth = stImageInfo.nExtendWidth;
    stConvertParam.nHeight = stImageInfo.nExtendHeight;
    stConvertParam.pSrcData = pData;
    stConvertParam.nSrcDataLen = stImageInfo.nFrameLenEx;
    stConvertParam.enSrcPixelType = stImageInfo.enPixelType;
    stConvertParam.enDstPixelType = PixelType_Gvsp_RGB8_Packed;
    stConvertParam.pDstBuffer = pConvertData ;
    stConvertParam.nDstBufferSize = nConvertDataSize ;

    nRet = MV_CC_ConvertPixelTypeEx(pcMyCamera, &stConvertParam);
    if (nRet != MV_OK)
    {
        QMessageBox::critical(this, "Error", "Pixel Format Conversion Failed");
        return;
    }

    stImageInfo.enPixelType = PixelType_Gvsp_RGB8_Packed;
}

void MainWindow::GetImage()// Collect a picture
{
    int nRet = MV_CC_GetOneFrameTimeout(pcMyCamera, pData, g_nPayloadSize, &stImageInfo, 100);
    if (nRet == MV_OK)
    {
        ConvBayer2RGB();

        cv::Mat dstImga;
        dstImga = Convert2Mat(&stImageInfo, pConvertData);
        //cv::imshow("image",dstImga);
        cv::cvtColor(dstImga,dstImga,cv::COLOR_BGR2RGB);

        QImage Qtemp = QImage((const unsigned char*)(dstImga.data), dstImga.cols, dstImga.rows, dstImga.step, QImage::Format_RGB888);
        ui->Camera_View->setScaledContents(true);
        ui->Camera_View->setPixmap(QPixmap::fromImage(Qtemp));
    }
    else
    {
        QString text = QString("No image data! NRET [% 1]").arg(nRet);
        QMessageBox::warning(this,"Warning",text);
        free(pData);
        pData = NULL;
        return;
    }
}

cv::Mat MainWindow::Convert2Mat(MV_FRAME_OUT_INFO_EX* pstImageInfo, unsigned char * pData)
{
    cv::Mat srcImage;

    if ( pstImageInfo->enPixelType == PixelType_Gvsp_Mono8 )
    {
        srcImage = cv::Mat(pstImageInfo->nHeight, pstImageInfo->nWidth, CV_8UC1, pData);
        return srcImage;
    }
    else if ( pstImageInfo->enPixelType == PixelType_Gvsp_RGB8_Packed)
    {

        RGB2BGR(pData, pstImageInfo->nWidth, pstImageInfo->nHeight);
        srcImage = cv::Mat(pstImageInfo->nHeight, pstImageInfo->nWidth, CV_8UC3, pData);
        return srcImage;
    }
    else
    {
        QMessageBox::critical(this,"Error","Does not support this type of conversion");
        return srcImage;
    }

    if ( NULL == srcImage.data )
    {
        QMessageBox::critical(this,"Error","Convert image is empty");
        return srcImage;
    }
}



void MainWindow::Initialize()
{
    int nRet = MV_CC_Initialize();
    if (nRet != MV_OK)
    {
        QMessageBox::critical(this, "Error", "Cannot Initialize SDK");
    }
     qDebug()<< "SDK INITIALIZED";
}

int MainWindow::CloseDevice()
{

    int nRet = MV_CC_StopGrabbing(pcMyCamera);
    if (MV_OK != nRet)
    {
        QMessageBox::critical(this, "Error", "Stop Grabbing Fail");
        return nRet;
    }

    // Turn off the device
    nRet = MV_CC_CloseDevice(pcMyCamera);
    if (MV_OK != nRet)
    {
        QMessageBox::critical(this, "Error", "Clode device fail");
        return nRet;
    }

    // Destroy handle
    nRet = MV_CC_DestroyHandle(pcMyCamera);
    if (MV_OK != nRet)
    {
        QMessageBox::critical(this, "Error", "Cannot Destroy Handle");
        return nRet;
    }

    return MV_OK;
}

void MainWindow::on_Enumerate_clicked()
{
    int nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE, &stDeviceList);
    if(nRet != MV_OK)
    {
        QMessageBox::critical(this, "Error", "Cannot Enumerate Device");
        return;
    }


    else if (stDeviceList.nDeviceNum > 0)
    {
        for (unsigned int i = 0; i <stDeviceList.nDeviceNum ; i++)
        {
            MV_CC_DEVICE_INFO* pDeviceInfo = stDeviceList.pDeviceInfo[0];

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

    if (stDeviceList.nDeviceNum == 0)
    {
        QMessageBox::critical(this, "Error", "Cannot Find Any Device");
        return;
    }
}

void MainWindow::on_Start_clicked()
{
    // Choose Device
     qDebug()<< "Start Device";
    unsigned int nIndex = ui->comboBox->currentIndex();

    int nRet = MV_CC_CreateHandle(&pcMyCamera,stDeviceList.pDeviceInfo[nIndex]);

    if (nRet != MV_OK)
    {
        qDebug()<<"nRet:" <<nRet;
        QMessageBox::critical(this,"Error","Cannot create Device");
        return;
    }

    nRet = MV_CC_OpenDevice(pcMyCamera);
    if (nRet != MV_OK)
    {
        qDebug()<< "OpenDevice Fail";
        QMessageBox::critical(this, "Error", "Open device failed");
        return;
    }

    if (stDeviceList.pDeviceInfo[nIndex]->nTLayerType == MV_GIGE_DEVICE)
    {
        int nPacketSize = MV_CC_GetOptimalPacketSize(pcMyCamera);
        if (nPacketSize > 0)
        {
            nRet = MV_CC_SetIntValue(pcMyCamera, "GevSCPSPacketSize", nPacketSize);
            if (nRet != MV_OK)
            {
                QMessageBox::critical(this, "Error", "Set Packet Size Failed");
            }
        }
        else
        {
            QMessageBox::critical(this,"Error","Get packet size failed");
        }

         qDebug()<<"Get packet size: " << nPacketSize;
    }



    // Set Trigger mode off
    nRet = MV_CC_SetEnumValue(pcMyCamera, "TriggerMode", 0);
    if (nRet != MV_OK)
    {
        QMessageBox::critical(this,"Error","Set Trigger Mode fail");
        return;
    }
    // Start grabbing image
    GrabImage();

    timer = new QTimer;
    timer->start(50);
    connect(timer,&QTimer::timeout,this,&MainWindow::GetImage);

}

void MainWindow::on_Stop_clicked()
{
    timer->stop();
    int nRet = MainWindow::CloseDevice();
    if (nRet != MV_OK)
    {
        QString text = QString("Cannot CLose Device! NRET [% 1]").arg(nRet);
        QMessageBox::warning(this,"Warning",text);
    }
}




