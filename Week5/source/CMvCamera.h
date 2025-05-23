#ifndef CMVCAMERA_H
#define CMVCAMERA_H

#include <MvCameraControl.h>

#ifndef MV_NULL
#define MV_NULL    0
#endif

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
using namespace cv;

class CMvCamera
{
public:
    CMvCamera();
    ~CMvCamera();

    //GET SDK VERSION
    static int GetSDKVersion();

    //ENUMERATE Device
    static int EnumDevices(unsigned int nTLayerType, MV_CC_DEVICE_INFO_LIST* pstDevList);

    // is The device accessible
    static bool IsDeviceAccessible(MV_CC_DEVICE_INFO* pstDevInfo, unsigned int nAccessMode);

    // Open Device
    int Open(MV_CC_DEVICE_INFO* pstDeviceInfo);

    // Close Device
    int Close();

    // is The device connect CONNECTED
    bool IsDeviceConnected();

    // Register Image Data Callback
    int RegisterImageCallBack(void(__stdcall* cbOutput)(unsigned char* pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser), void* pUser);

    // START Grabbing
    int StartGrabbing();

    // STOP Grabbing
    int StopGrabbing();

    //  Get One Frame Initiratively
    int GetImageBuffer(MV_FRAME_OUT* pFrame, int nMsec);

    // Free Image Buffer
    int FreeImageBuffer(MV_FRAME_OUT* pFrame);

    //  Get One Frame Initiratively
    int GetOneFrameTimeout(unsigned char* pData, unsigned int* pnDataLen, unsigned int nDataSize, MV_FRAME_OUT_INFO_EX* pFrameInfo, int nMsec);

    //  DISPLAY One Frame Image
    int DisplayOneFrame(MV_DISPLAY_FRAME_INFO* pDisplayInfo);

    // SET the NUMBER of the Internal Image Cache Nodes in SDK
    int SetImageNodeNum(unsigned int nNum);

    // Get Device Information
    int GetDeviceInfo(MV_CC_DEVICE_INFO* pstDevInfo);

    //  Get Detect Info of Gev Camera
    int GetGevAllMatchInfo(MV_MATCH_INFO_NET_DETECT* pMatchInfoNetDetect);

    // Get Detect Info of U3v Camera
    int GetU3VAllMatchInfo(MV_MATCH_INFO_USB_DETECT* pMatchInfoUSBDetect);

    // Get Int type parameters, such as Width and Height, for details please refer to MvCameraNode.xlsx file under SDK installation directory
    //int GetIntValue(IN const char* strKey, OUT MVCC_INTVALUE_EX* pIntValue);
    int GetIntValue(IN const char* strKey, OUT unsigned int* pnValue);
    int SetIntValue(IN const char* strKey, IN int64_t nValue);

    // en:Get Enum type parameters, such as PixelFormat, for details please refer to MvCameraNode.xlsx file under SDK installation directory
    int GetEnumValue(IN const char* strKey, OUT MVCC_ENUMVALUE* pEnumValue);
    int SetEnumValue(IN const char* strKey, IN unsigned int nValue);
    int SetEnumValueByString(IN const char* strKey, IN const char* sValue);

    // en:Get Float type parameters, such as ExposureTime and Gain, for details please refer to MvCameraNode.xlsx file under SDK installation directory
    int GetFloatValue(IN const char* strKey, OUT MVCC_FLOATVALUE* pFloatValue);
    int SetFloatValue(IN const char* strKey, IN float fValue);

    // en:Get Bool type parameters, such as ReverseX, for details please refer to MvCameraNode.xlsx file under SDK installation directory
    int GetBoolValue(IN const char* strKey, OUT bool* pbValue);
    int SetBoolValue(IN const char* strKey, IN bool bValue);

    // en:Get String type parameters, such as DeviceUserID, for details please refer to MvCameraNode.xlsx file under SDK installation directory
    int GetStringValue(IN const char* strKey, MVCC_STRINGVALUE* pStringValue);
    int SetStringValue(IN const char* strKey, IN const char* strValue);

    // en:Execute Command once, such as UserSetSave, for details please refer to MvCameraNode.xlsx file under SDK installation directory
    int CommandExecute(IN const char* strKey);

    // Detection Network Optimal Package Size (It Only Works for the Gige Camera)
    int GetOptimalPacketSize(unsigned int* pOptimalPacketSize);

    //  Register Message Exception Callback
    int RegisterExceptionCallBack(void(__stdcall* cbException)(unsigned int nMsgType, void* pUser), void* pUser);

    // Register Event Callback
    int RegisterEventCallBack(const char* pEventName, void(__stdcall* cbEvent)(MV_EVENT_OUT_INFO* pEventInfo, void* pUser), void* pUser);

    // Force IP
    int ForceIp(unsigned int nIP, unsigned int nSubNetMask, unsigned int nDefaultGateWay);

    // IP Configuration Method
    int SetIpConfig(unsigned int nType);

    //  SET NET TRANSFER MODE
    int SetNetTransMode(unsigned int nType);

    //  Pixel format conversion
    int ConvertPixelType(MV_CC_PIXEL_CONVERT_PARAM* pstCvtParam);

    // Save Image
    int SaveImage(MV_SAVE_IMAGE_PARAM_EX* pstParam);

    // SAVE The Image As a file
    int SaveImageToFile(MV_SAVE_IMG_TO_FILE_PARAM* pstParam);

    // Set whether to trigger mode
    int setTriggerMode(unsigned int TriggerModeNum);

    // Set the trigger source
    int setTriggerSource(unsigned int TriggerSourceNum);

    // Soft trigger
    int softTrigger();

    // Read buffer
    int ReadBuffer(cv::Mat& image);

    // Set CVT quality
    int SetBayerCvT(int Quality);

public:
    void* m_hDevHandle;
    unsigned int     m_nTLayerType;

public:
    unsigned char* m_pBufForSaveImage;         // Used to save the cache
    unsigned int    m_nBufSizeForSaveImage;

    unsigned char* m_pBufForDriver;            // Used to get the cache for image from the drive
    unsigned int    m_nBufSizeForDriver;

};

#endif // CMVCAMERA_H
#pragma once
