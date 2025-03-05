#ifndef PCH_H
#define PCH_H


// BasicDemoDlg.h : header file
#pragma once
#include "afxwin.h" 
#include "MvCamera.h"
#include "process.h"
#include <string>
#include  <vector>

//定义AOI结构体
struct AOI
{   //是否自动AOI区域自动曝光
    bool is_AOI_temp;
    int64_t x_Off_AOI, y_Off_AOI, width_AOI, height_AOI;
    float map_scale;
};




// CBasicDemoDlg dialog
class CBasicDemoDlg : public CDialog
{
// Construction
public:
    double m_zoomFactor; // 缩放系数
    AOI  aoi_temp;
	CBasicDemoDlg(CWnd* pParent = NULL);	// Standard constructor

    // Dialog Data
	enum { IDD = IDD_BasicDemo_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;
    
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

/*ch:控件对应变量 | en:Control corresponding variable*/
private:
    BOOL                    m_bSoftWareTriggerCheck;
    double                  m_dExposureEdit;
    double                  m_dGainEdit;
    double                  m_dFrameRateEdit;
    char                    m_chPixelFormat[MV_MAX_SYMBOLIC_LEN];

    CComboBox               m_ctrlDeviceCombo;                // ch:枚举到的设备 | en:Enumerated device
    int                     m_nDeviceCombo;

private:
    /*ch:最开始时的窗口初始化 | en:Window initialization*/
    void DisplayWindowInitial();

    void EnableControls(BOOL bIsCameraReady);
    void ShowErrorMsg(CString csMessage, int nErrorNum);

    int SetTriggerMode();                // ch:设置触发模式 | en:Set Trigger Mode
    int GetTriggerMode();
    int GetExposureTime();               // ch:设置曝光时间 | en:Set Exposure Time
    int SetExposureTime(); 
    int GetGain();                       // ch:设置增益 | en:Set Gain
    int SetGain();
    int GetFrameRate();                  // ch:设置帧率 | en:Set Frame Rate
    int SetFrameRate();
    int GetTriggerSource();              // ch:设置触发源 | en:Set Trigger Source
    int SetTriggerSource();
    int GetPixelFormat();                // ch:获取像素格式 | en:Get Pixel Format

    int CloseDevice();                   // ch:关闭设备 | en:Close Device
    int SaveImage(MV_SAVE_IAMGE_TYPE enSaveImageType);                     // ch:保存图片 | en:Save Image
    //ch:网络配置 | en:Network Configuration
    //void NETConfig(int axis, IMC_CONTROL & imc_Control_handle_temp);
    int SavePicture(MV_SAVE_IAMGE_TYPE enSaveImageType, std::string path, std::vector<float>siteTemp, char* picture_Name);
    //roi设置
    int SetAOIRegion(int64_t nOffsetX, int64_t nOffsetY, int64_t nWidth, int64_t nHeight);

private:
    BOOL                    m_bOpenDevice;                        // ch:是否打开设备 | en:Whether to open device
    BOOL                    m_bStartGrabbing;                     // ch:是否开始抓图 | en:Whether to start grabbing
    int                     m_nTriggerMode;                       // ch:触发模式 | en:Trigger Mode
    int                     m_nTriggerSource;                     // ch:触发源 | en:Trigger Source

    CMvCamera*              m_pcMyCamera;               // ch:CMyCamera封装了常用接口 | en:CMyCamera packed commonly used interface
    HWND                    m_hwndDisplay;                        // ch:显示句柄 | en:Display Handle
    MV_CC_DEVICE_INFO_LIST  m_stDevList;         

    CRITICAL_SECTION        m_hSaveImageMux;
    unsigned char*          m_pSaveImageBuf;
    unsigned int            m_nSaveImageBufSize;
    MV_FRAME_OUT_INFO_EX    m_stImageInfo;

    void*                   m_hGrabThread;              // ch:取流线程句柄 | en:Grab thread handle
    BOOL                    m_bThreadState;

public:
    /*ch:初始化 | en:Initialization*/
    afx_msg void OnBnClickedEnumButton();               // ch:查找设备 | en:Find Devices
    afx_msg void OnBnClickedOpenButton();               // ch:打开设备 | en:Open Devices
    afx_msg void OnBnClickedCloseButton();              // ch:关闭设备 | en:Close Devices
   
    /*ch:图像采集 | en:Image Acquisition*/
    //afx_msg void OnBnClickedContinusModeRadio();        // ch:连续模式 | en:Continus Mode
    //afx_msg void OnBnClickedTriggerModeRadio();         // ch:触发模式 | en:Trigger Mode
    afx_msg void OnBnClickedStartGrabbingButton();      // ch:开始采集 | en:Start Grabbing
    afx_msg void OnBnClickedStopGrabbingButton();       // ch:结束采集 | en:Stop Grabbing
    //afx_msg void OnBnClickedSoftwareTriggerCheck();     // ch:软触发 | en:Software Trigger
    afx_msg void OnBnClickedSoftwareOnceButton();       // ch:软触发一次 | en:Software Trigger Execute Once
  
    /*ch:图像保存 | en:Image Save*/
    afx_msg void OnBnClickedSaveBmpButton();            // ch:保存bmp | en:Save bmp
    afx_msg void OnBnClickedSaveJpgButton();            // ch:保存jpg | en:Save jpg
    afx_msg void OnBnClickedSaveTiffButton();
    afx_msg void OnBnClickedSavePngButton();
  
    /*ch:参数设置获取 | en:Parameters Get and Set*/
    afx_msg void OnBnClickedGetParameterButton();       // ch:获取参数 | en:Get Parameter
    afx_msg void OnBnClickedSetParameterButton();       // ch:设置参数 | en:Exit from upper right corner

    /*ch:辅助线绘制 | en:Auxiliary Line Drawing*/
    afx_msg void OnBnClickedCircleAuxiliaryButton();
    afx_msg void OnBnClickedLinesAuxiliaryButton();

    afx_msg void OnClose();

    virtual BOOL PreTranslateMessage(MSG* pMsg);
    int GrabThreadProcess();
    afx_msg void OnBnClickedAuxiliaryLineDrawingStatic();


    //变量声明
    
    afx_msg void OnClickedResetLocation();
    afx_msg void OnStnClickedExposureStatic2();
    float IDC_scaleFactor;
    afx_msg void OnEnChangeExposureEdit2();
    // 修改step
    int goto_step;
    afx_msg void OnClickedGoToStep();
    afx_msg void OnBnClickedButton3();
    afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
    int64_t xoff_ROI;
    int64_t yoff_ROI;
    int64_t width_ROI;
    int64_t height_ROI;
    afx_msg void OnClickedButton4();
    BOOL is_AOI;
    float map_scale;
    afx_msg void OnBnClickedButton5();
};








#endif