#include "CMvCamera.h"

CMvCamera::CMvCamera()
{
    m_hDevHandle = MV_NULL;
}

CMvCamera::~CMvCamera()
{
    if (m_hDevHandle)
    {
        MV_CC_DestroyHandle(m_hDevHandle);
        m_hDevHandle = MV_NULL;
    }
}

//GET SDK VERSION
int CMvCamera::GetSDKVersion()
{
    return MV_CC_GetSDKVersion();
}

//ENUMERATE Device
int CMvCamera::EnumDevices(unsigned int nTLayerType, MV_CC_DEVICE_INFO_LIST* pstDevList)
{
    return MV_CC_EnumDevices(nTLayerType, pstDevList);
}

//  is The device accessible
bool CMvCamera::IsDeviceAccessible(MV_CC_DEVICE_INFO* pstDevInfo, unsigned int nAccessMode)
{
    return MV_CC_IsDeviceAccessible(pstDevInfo, nAccessMode);
}

//  Open Device
int CMvCamera::Open(MV_CC_DEVICE_INFO* pstDeviceInfo)
{
    if (MV_NULL == pstDeviceInfo)
    {
        return MV_E_PARAMETER;
    }

    if (m_hDevHandle)
    {
        return MV_E_CALLORDER;
    }

    int nRet = MV_CC_CreateHandle(&m_hDevHandle, pstDeviceInfo);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    nRet = MV_CC_OpenDevice(m_hDevHandle);
    if (MV_OK != nRet)
    {
        MV_CC_DestroyHandle(m_hDevHandle);
        m_hDevHandle = MV_NULL;
    }

    return nRet;
}

// Close Device
int CMvCamera::Close()
{
    if (MV_NULL == m_hDevHandle)
    {
        return MV_E_HANDLE;
    }

    MV_CC_CloseDevice(m_hDevHandle);

    int nRet = MV_CC_DestroyHandle(m_hDevHandle);
    m_hDevHandle = MV_NULL;

    return nRet;
}

//  is The device connect CONNECTED
bool CMvCamera::IsDeviceConnected()
{
    return MV_CC_IsDeviceConnected(m_hDevHandle);
}

//  Register Image Data Callback
int CMvCamera::RegisterImageCallBack(void(__stdcall* cbOutput)(unsigned char* pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser), void* pUser)
{
    return MV_CC_RegisterImageCallBackEx(m_hDevHandle, cbOutput, pUser);
}

//  START Grabbing
int CMvCamera::StartGrabbing()
{
    return MV_CC_StartGrabbing(m_hDevHandle);
}

// STOP Grabbing
int CMvCamera::StopGrabbing()
{
    return MV_CC_StopGrabbing(m_hDevHandle);
}

// Get One Frame Initiratively
int CMvCamera::GetImageBuffer(MV_FRAME_OUT* pFrame, int nMsec)
{
    return MV_CC_GetImageBuffer(m_hDevHandle, pFrame, nMsec);
}

//  Free Image Buffer
int CMvCamera::FreeImageBuffer(MV_FRAME_OUT* pFrame)
{
    return MV_CC_FreeImageBuffer(m_hDevHandle, pFrame);
}

//  Get One Frame Initiratively
int     CMvCamera::GetOneFrameTimeout(unsigned char* pData, unsigned int* pnDataLen, unsigned int nDataSize, MV_FRAME_OUT_INFO_EX* pFrameInfo, int nMsec)
{
    if (NULL == pnDataLen)
    {
        return MV_E_PARAMETER;
    }

    int nRet = MV_OK;

    *pnDataLen = 0;

    nRet = MV_CC_GetOneFrameTimeout(m_hDevHandle, pData, nDataSize, pFrameInfo, nMsec);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    *pnDataLen = pFrameInfo->nFrameLen;

    return nRet;
}


// SET Display Window Handle
int CMvCamera::DisplayOneFrame(MV_DISPLAY_FRAME_INFO* pDisplayInfo)
{
    return MV_CC_DisplayOneFrame(m_hDevHandle, pDisplayInfo);
}

// SET the NUMBER of the Internal Image Cache Nodes in SDK
int CMvCamera::SetImageNodeNum(unsigned int nNum)
{
    return MV_CC_SetImageNodeNum(m_hDevHandle, nNum);
}

// Get Device Information
int CMvCamera::GetDeviceInfo(MV_CC_DEVICE_INFO* pstDevInfo)
{
    return MV_CC_GetDeviceInfo(m_hDevHandle, pstDevInfo);
}

//  Get Detect Info of Gev Camera
int CMvCamera::GetGevAllMatchInfo(MV_MATCH_INFO_NET_DETECT* pMatchInfoNetDetect)
{
    if (MV_NULL == pMatchInfoNetDetect)
    {
        return MV_E_PARAMETER;
    }

    MV_CC_DEVICE_INFO stDevInfo = { 0 };
    GetDeviceInfo(&stDevInfo);
    if (stDevInfo.nTLayerType != MV_GIGE_DEVICE)
    {
        return MV_E_SUPPORT;
    }

    MV_ALL_MATCH_INFO struMatchInfo = { 0 };

    struMatchInfo.nType = MV_MATCH_TYPE_NET_DETECT;
    struMatchInfo.pInfo = pMatchInfoNetDetect;
    struMatchInfo.nInfoSize = sizeof(MV_MATCH_INFO_NET_DETECT);
    memset(struMatchInfo.pInfo, 0, sizeof(MV_MATCH_INFO_NET_DETECT));

    return MV_CC_GetAllMatchInfo(m_hDevHandle, &struMatchInfo);
}

// Get Detect Info of U3v Camera
int CMvCamera::GetU3VAllMatchInfo(MV_MATCH_INFO_USB_DETECT* pMatchInfoUSBDetect)
{
    if (MV_NULL == pMatchInfoUSBDetect)
    {
        return MV_E_PARAMETER;
    }

    MV_CC_DEVICE_INFO stDevInfo = { 0 };
    GetDeviceInfo(&stDevInfo);
    if (stDevInfo.nTLayerType != MV_USB_DEVICE)
    {
        return MV_E_SUPPORT;
    }

    MV_ALL_MATCH_INFO struMatchInfo = { 0 };

    struMatchInfo.nType = MV_MATCH_TYPE_USB_DETECT;
    struMatchInfo.pInfo = pMatchInfoUSBDetect;
    struMatchInfo.nInfoSize = sizeof(MV_MATCH_INFO_USB_DETECT);
    memset(struMatchInfo.pInfo, 0, sizeof(MV_MATCH_INFO_USB_DETECT));

    return MV_CC_GetAllMatchInfo(m_hDevHandle, &struMatchInfo);
}

// en:Get Int type parameters, such as Width and Height, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int     CMvCamera::GetIntValue(IN const char* strKey, OUT unsigned int* pnValue)
{
    if (NULL == strKey || NULL == pnValue)
    {
        return MV_E_PARAMETER;
    }

    MVCC_INTVALUE stParam;
    memset(&stParam, 0, sizeof(MVCC_INTVALUE));
    int nRet = MV_CC_GetIntValue(m_hDevHandle, strKey, &stParam);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    *pnValue = stParam.nCurValue;

    return MV_OK;
}


int CMvCamera::SetIntValue(IN const char* strKey, IN int64_t nValue)
{
    return MV_CC_SetIntValueEx(m_hDevHandle, strKey, nValue);
}

// Get Enum type parameters, such as PixelFormat, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int CMvCamera::GetEnumValue(IN const char* strKey, OUT MVCC_ENUMVALUE* pEnumValue)
{
    return MV_CC_GetEnumValue(m_hDevHandle, strKey, pEnumValue);
}

int CMvCamera::SetEnumValue(IN const char* strKey, IN unsigned int nValue)
{
    return MV_CC_SetEnumValue(m_hDevHandle, strKey, nValue);
}

int CMvCamera::SetEnumValueByString(IN const char* strKey, IN const char* sValue)
{
    return MV_CC_SetEnumValueByString(m_hDevHandle, strKey, sValue);
}

// Get Float type parameters, such as ExposureTime and Gain, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int CMvCamera::GetFloatValue(IN const char* strKey, OUT MVCC_FLOATVALUE* pFloatValue)
{
    return MV_CC_GetFloatValue(m_hDevHandle, strKey, pFloatValue);
}

int CMvCamera::SetFloatValue(IN const char* strKey, IN float fValue)
{
    return MV_CC_SetFloatValue(m_hDevHandle, strKey, fValue);
}

// en:Get Bool type parameters, such as ReverseX, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int CMvCamera::GetBoolValue(IN const char* strKey, OUT bool* pbValue)
{
    return MV_CC_GetBoolValue(m_hDevHandle, strKey, pbValue);
}

int CMvCamera::SetBoolValue(IN const char* strKey, IN bool bValue)
{
    return MV_CC_SetBoolValue(m_hDevHandle, strKey, bValue);
}

// en:Get String type parameters, such as DeviceUserID, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int CMvCamera::GetStringValue(IN const char* strKey, MVCC_STRINGVALUE* pStringValue)
{
    return MV_CC_GetStringValue(m_hDevHandle, strKey, pStringValue);
}

int CMvCamera::SetStringValue(IN const char* strKey, IN const char* strValue)
{
    return MV_CC_SetStringValue(m_hDevHandle, strKey, strValue);
}

// en:Execute Command once, such as UserSetSave, for details please refer to MvCameraNode.xlsx file under SDK installation directory
int CMvCamera::CommandExecute(IN const char* strKey)
{
    return MV_CC_SetCommandValue(m_hDevHandle, strKey);
}

// Detection Network Optimal Package Size (It Only Works for the Gige Camera)
int CMvCamera::GetOptimalPacketSize(unsigned int* pOptimalPacketSize)
{
    if (MV_NULL == pOptimalPacketSize)
    {
        return MV_E_PARAMETER;
    }

    int nRet = MV_CC_GetOptimalPacketSize(m_hDevHandle);
    if (nRet < MV_OK)
    {
        return nRet;
    }

    *pOptimalPacketSize = (unsigned int)nRet;

    return MV_OK;
}

//  Register Message Exception Callback
int CMvCamera::RegisterExceptionCallBack(void(__stdcall* cbException)(unsigned int nMsgType, void* pUser), void* pUser)
{
    return MV_CC_RegisterExceptionCallBack(m_hDevHandle, cbException, pUser);
}

// Register Event Callback
int CMvCamera::RegisterEventCallBack(const char* pEventName, void(__stdcall* cbEvent)(MV_EVENT_OUT_INFO* pEventInfo, void* pUser), void* pUser)
{
    return MV_CC_RegisterEventCallBackEx(m_hDevHandle, pEventName, cbEvent, pUser);
}

// Force IP
int CMvCamera::ForceIp(unsigned int nIP, unsigned int nSubNetMask, unsigned int nDefaultGateWay)
{
    return MV_GIGE_ForceIpEx(m_hDevHandle, nIP, nSubNetMask, nDefaultGateWay);
}

// IP Configuration Method
int CMvCamera::SetIpConfig(unsigned int nType)
{
    return MV_GIGE_SetIpConfig(m_hDevHandle, nType);
}

// SET NET TRANSFER MODE
int CMvCamera::SetNetTransMode(unsigned int nType)
{
    return MV_GIGE_SetNetTransMode(m_hDevHandle, nType);
}

// Pixel format conversion
int CMvCamera::ConvertPixelType(MV_CC_PIXEL_CONVERT_PARAM* pstCvtParam)
{
    return MV_CC_ConvertPixelType(m_hDevHandle, pstCvtParam);
}

// Save Image
int CMvCamera::SaveImage(MV_SAVE_IMAGE_PARAM_EX* pstParam)
{
    return MV_CC_SaveImageEx2(m_hDevHandle, pstParam);
}

// SAVE The Image As a file
int CMvCamera::SaveImageToFile(MV_SAVE_IMG_TO_FILE_PARAM* pstSaveFileParam)
{
    return MV_CC_SaveImageToFile(m_hDevHandle, pstSaveFileParam);
}

int CMvCamera::SetBayerCvT(int Quality)
{
    return MV_CC_SetBayerCvtQuality(m_hDevHandle, Quality);
}


// Set whether to trigger mode
int CMvCamera::setTriggerMode(unsigned int TriggerModeNum)
{
    //0：Off  1：On
    int tempValue = MV_CC_SetEnumValue(m_hDevHandle, "TriggerMode", TriggerModeNum);
    if (tempValue != 0){
        return -1;
    }
    else {
        return 0;
    }
}

// Set the trigger source
int CMvCamera::setTriggerSource(unsigned int TriggerSourceNum)
{
    //0：Line0  1：Line1  7：Software
    int tempValue = MV_CC_SetEnumValue(m_hDevHandle, "TriggerSource", TriggerSourceNum);
    if (tempValue != 0) {
        return -1;
    }
    else {
        return 0;
    }
}

// ************************************************************************************************
// Send a soft trigger
int CMvCamera::softTrigger()
{
    int tempValue = MV_CC_SetCommandValue(m_hDevHandle, "TriggerSoftware");
    if (tempValue != 0)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

// Read the image in the camera
//int ReadBuffer(cv::Mat &image);
// Read the image in the camera
int CMvCamera::ReadBuffer(cv::Mat& image)
{
    cv::Mat* getImage = new cv::Mat();
    unsigned int nRecvBufSize = 0;
    MVCC_INTVALUE stParam;
    memset(&stParam, 0, sizeof(MVCC_INTVALUE));
    int tempValue = MV_CC_GetIntValue(m_hDevHandle, "PayloadSize", &stParam);
    if (tempValue != 0)
    {
        return -1;
    }
    nRecvBufSize = stParam.nCurValue;
    unsigned char* pDate;
    pDate = (unsigned char*)malloc(nRecvBufSize);

    MV_FRAME_OUT_INFO_EX stImageInfo = { 0 };
    tempValue = MV_CC_GetOneFrameTimeout(m_hDevHandle, pDate, nRecvBufSize, &stImageInfo, 500);
    if (tempValue != 0)
    {
        return -1;
    }
    m_nBufSizeForSaveImage = stImageInfo.nWidth * stImageInfo.nHeight * 3 + 2048;
    unsigned char* m_pBufForSaveImage;
    m_pBufForSaveImage = (unsigned char*)malloc(m_nBufSizeForSaveImage);


    bool isMono;
    switch (stImageInfo.enPixelType)
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
        *getImage = cv::Mat(stImageInfo.nHeight, stImageInfo.nWidth, CV_8UC1, pDate);
        // imwrite ("D: \\ Test OpenCV_MONO.TIF", Image);
    }
    else
    {
        // Conversion image format is BGR8
        MV_CC_SetBayerCvtQuality(m_hDevHandle, 1);

        MV_CC_PIXEL_CONVERT_PARAM stConvertParam = { 0 };
        memset(&stConvertParam, 0, sizeof(MV_CC_PIXEL_CONVERT_PARAM));
        stConvertParam.nWidth = stImageInfo.nWidth;                 // CH: Image wide | EN: image width
        stConvertParam.nHeight = stImageInfo.nHeight;               // CH: Image High | EN: Image Heart
        //stconvertparam.psrcdata = m_pbuffordriver; // CH: Enter data cache | EN: Input data buffer
        stConvertParam.pSrcData = pDate;                  // CH: Enter data cache | EN: Input data buffer
        stConvertParam.nSrcDataLen = stImageInfo.nFrameLen;         // CH: Enter data size | EN: INPUT DATA SIZE
        stConvertParam.enSrcPixelType = stImageInfo.enPixelType;    // CH: Enter a pixel format | EN: INPUT PIXEL FORMAT
        stConvertParam.enDstPixelType = PixelType_Gvsp_BGR8_Packed; // CH: Output pixel format | EN: Output Pixel Format Applicable to OpenCV Image Format
        //stconvertparam.endstpixeltype = pixeltype_gvsp_rgb8_packed; // ch: Output pixel format | EN: OUTPUT PIXEL FORMAT
        stConvertParam.pDstBuffer = m_pBufForSaveImage;                    // CH: Output Data Cache | EN: OUTPUT DATA BUFFER
        stConvertParam.nDstBufferSize = m_nBufSizeForSaveImage;            // CH: Output Cache Size | EN: OUTPUT BUFFER SIZE
        MV_CC_ConvertPixelType(m_hDevHandle, &stConvertParam);

        *getImage = cv::Mat(stImageInfo.nHeight, stImageInfo.nWidth, CV_8UC3, m_pBufForSaveImage);

    }
    (*getImage).copyTo(image);
    (*getImage).release();
    free(pDate);
    free(m_pBufForSaveImage);
    return 0;
}
