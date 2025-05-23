#include "MvCamera.h"

CMvCamera::CMvCamera()
{
    hDevHandle = MV_NULL;
}

CMvCamera::~CMvCamera()
{
    if (hDevHandle)
    {
        MV_CC_DestroyHandle(hDevHandle);
        hDevHandle    = MV_NULL;
    }
}

int CMvCamera::InitSDK()
{
    return MV_CC_Initialize();
}

int CMvCamera::FinalizeSDK()
{
    return MV_CC_Finalize();
}

int CMvCamera::EnumDevices(unsigned int nTLayerType, MV_CC_DEVICE_INFO_LIST* pstDevList)
{
    return MV_CC_EnumDevices(nTLayerType, pstDevList);
}

int CMvCamera::SetIntValue(IN const char* strKey, IN int64_t nValue)
{
    return MV_CC_SetIntValueEx(hDevHandle, strKey, nValue);
}



int CMvCamera::Open(MV_CC_DEVICE_INFO* pstDeviceInfo)
{
    if (MV_NULL == pstDeviceInfo)
    {
        return MV_E_PARAMETER;
    }

    if (hDevHandle)
    {
        return MV_E_CALLORDER;
    }

    int nRet  = MV_CC_CreateHandle(&hDevHandle, pstDeviceInfo);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    nRet = MV_CC_OpenDevice(hDevHandle);
    if (MV_OK != nRet)
    {
        MV_CC_DestroyHandle(hDevHandle);
        hDevHandle = MV_NULL;
    }

    return nRet;
}

// ch:关闭设备 | en:Close Device
int CMvCamera::Close()
{
    if (MV_NULL == hDevHandle)
    {
        return MV_E_HANDLE;
    }

    int nRet = MV_CC_StopGrabbing(hDevHandle);
    if (nRet != MV_OK)
    {
        return MV_E_HANDLE;
    }
    nRet = MV_CC_CloseDevice(hDevHandle);
    if (nRet != MV_OK)
    {
        return MV_E_HANDLE;
    }
    nRet = MV_CC_DestroyHandle(hDevHandle);

    if (nRet != MV_OK)
    {
        return MV_E_HANDLE;
    }

    hDevHandle = MV_NULL;


    return nRet;
}

int CMvCamera::StartGrabbing()
{
    return MV_CC_StartGrabbing(hDevHandle);
}

// ch:停止抓图 | en:Stop Grabbing
int CMvCamera::StopGrabbing()
{
    return MV_CC_StopGrabbing(hDevHandle);
}

// ch:设置显示窗口句柄 | en:Set Display Window Handle
int CMvCamera::DisplayOneFrame(void* hwndDisplay,MV_CC_IMAGE* pImageInfo)
{
    return MV_CC_DisplayOneFrameEx2(hDevHandle, hwndDisplay, pImageInfo,0);

}

// ch:获取设备信息 | en:Get device information
int CMvCamera::GetDeviceInfo(MV_CC_DEVICE_INFO* pstDevInfo)
{
    return MV_CC_GetDeviceInfo(hDevHandle, pstDevInfo);
}

// ch:获取GEV相机的统计信息 | en:Get detect info of GEV camera
int CMvCamera::GetGevAllMatchInfo(MV_MATCH_INFO_NET_DETECT* pMatchInfoNetDetect)
{
    if (MV_NULL == pMatchInfoNetDetect)
    {
        return MV_E_PARAMETER;
    }

    MV_CC_DEVICE_INFO stDevInfo = {0};
    GetDeviceInfo(&stDevInfo);
    if (stDevInfo.nTLayerType != MV_GIGE_DEVICE)
    {
        return MV_E_SUPPORT;
    }

    MV_ALL_MATCH_INFO struMatchInfo = {0};

    struMatchInfo.nType = MV_MATCH_TYPE_NET_DETECT;
    struMatchInfo.pInfo = pMatchInfoNetDetect;
    struMatchInfo.nInfoSize = sizeof(MV_MATCH_INFO_NET_DETECT);
    memset(struMatchInfo.pInfo, 0, sizeof(MV_MATCH_INFO_NET_DETECT));

    return MV_CC_GetAllMatchInfo(hDevHandle, &struMatchInfo);
}



// ch:探测网络最佳包大小(只对GigE相机有效) | en:Detection network optimal package size(It only works for the GigE camera)
int CMvCamera::GetOptimalPacketSize(unsigned int* pOptimalPacketSize)
{
    if (MV_NULL == pOptimalPacketSize)
    {
        return MV_E_PARAMETER;
    }

    int nRet = MV_CC_GetOptimalPacketSize(hDevHandle);
    if (nRet < MV_OK)
    {
        return nRet;
    }

    *pOptimalPacketSize = (unsigned int)nRet;

    return MV_OK;
}

// ch:像素格式转换 | en:Pixel format conversion
int CMvCamera::ConvertPixelType(MV_CC_PIXEL_CONVERT_PARAM_EX* pstCvtParam)
{
    return MV_CC_ConvertPixelTypeEx(hDevHandle, pstCvtParam);
}

int CMvCamera::FreeImageBuffer_n(MV_FRAME_OUT* pFrame)
{
    return MV_CC_FreeImageBuffer(hDevHandle, pFrame);
}

int CMvCamera::GetImageBuffer_n(MV_FRAME_OUT* pFrame, int nMsec)
{
    return MV_CC_GetImageBuffer(hDevHandle, pFrame, nMsec);
}

bool CMvCamera::IsDeviceConnected()
{
    return MV_CC_IsDeviceConnected(hDevHandle);
}

int CMvCamera::GetIntValue(IN const char* strKey, OUT MVCC_INTVALUE_EX *pIntValue)
{
    return MV_CC_GetIntValueEx(hDevHandle, strKey, pIntValue);
}

