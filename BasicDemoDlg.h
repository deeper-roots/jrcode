#ifndef PCH_H
#define PCH_H


// BasicDemoDlg.h : header file
#pragma once
#include "afxwin.h" 
#include "MvCamera.h"
#include "process.h"
#include <string>
#include  <vector>

//����AOI�ṹ��
struct AOI
{   //�Ƿ��Զ�AOI�����Զ��ع�
    bool is_AOI_temp;
    int64_t x_Off_AOI, y_Off_AOI, width_AOI, height_AOI;
    float map_scale;
};




// CBasicDemoDlg dialog
class CBasicDemoDlg : public CDialog
{
// Construction
public:
    double m_zoomFactor; // ����ϵ��
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

/*ch:�ؼ���Ӧ���� | en:Control corresponding variable*/
private:
    BOOL                    m_bSoftWareTriggerCheck;
    double                  m_dExposureEdit;
    double                  m_dGainEdit;
    double                  m_dFrameRateEdit;
    char                    m_chPixelFormat[MV_MAX_SYMBOLIC_LEN];

    CComboBox               m_ctrlDeviceCombo;                // ch:ö�ٵ����豸 | en:Enumerated device
    int                     m_nDeviceCombo;

private:
    /*ch:�ʼʱ�Ĵ��ڳ�ʼ�� | en:Window initialization*/
    void DisplayWindowInitial();

    void EnableControls(BOOL bIsCameraReady);
    void ShowErrorMsg(CString csMessage, int nErrorNum);

    int SetTriggerMode();                // ch:���ô���ģʽ | en:Set Trigger Mode
    int GetTriggerMode();
    int GetExposureTime();               // ch:�����ع�ʱ�� | en:Set Exposure Time
    int SetExposureTime(); 
    int GetGain();                       // ch:�������� | en:Set Gain
    int SetGain();
    int GetFrameRate();                  // ch:����֡�� | en:Set Frame Rate
    int SetFrameRate();
    int GetTriggerSource();              // ch:���ô���Դ | en:Set Trigger Source
    int SetTriggerSource();
    int GetPixelFormat();                // ch:��ȡ���ظ�ʽ | en:Get Pixel Format

    int CloseDevice();                   // ch:�ر��豸 | en:Close Device
    int SaveImage(MV_SAVE_IAMGE_TYPE enSaveImageType);                     // ch:����ͼƬ | en:Save Image
    //ch:�������� | en:Network Configuration
    //void NETConfig(int axis, IMC_CONTROL & imc_Control_handle_temp);
    int SavePicture(MV_SAVE_IAMGE_TYPE enSaveImageType, std::string path, std::vector<float>siteTemp, char* picture_Name);
    //roi����
    int SetAOIRegion(int64_t nOffsetX, int64_t nOffsetY, int64_t nWidth, int64_t nHeight);

private:
    BOOL                    m_bOpenDevice;                        // ch:�Ƿ���豸 | en:Whether to open device
    BOOL                    m_bStartGrabbing;                     // ch:�Ƿ�ʼץͼ | en:Whether to start grabbing
    int                     m_nTriggerMode;                       // ch:����ģʽ | en:Trigger Mode
    int                     m_nTriggerSource;                     // ch:����Դ | en:Trigger Source

    CMvCamera*              m_pcMyCamera;               // ch:CMyCamera��װ�˳��ýӿ� | en:CMyCamera packed commonly used interface
    HWND                    m_hwndDisplay;                        // ch:��ʾ��� | en:Display Handle
    MV_CC_DEVICE_INFO_LIST  m_stDevList;         

    CRITICAL_SECTION        m_hSaveImageMux;
    unsigned char*          m_pSaveImageBuf;
    unsigned int            m_nSaveImageBufSize;
    MV_FRAME_OUT_INFO_EX    m_stImageInfo;

    void*                   m_hGrabThread;              // ch:ȡ���߳̾�� | en:Grab thread handle
    BOOL                    m_bThreadState;

public:
    /*ch:��ʼ�� | en:Initialization*/
    afx_msg void OnBnClickedEnumButton();               // ch:�����豸 | en:Find Devices
    afx_msg void OnBnClickedOpenButton();               // ch:���豸 | en:Open Devices
    afx_msg void OnBnClickedCloseButton();              // ch:�ر��豸 | en:Close Devices
   
    /*ch:ͼ��ɼ� | en:Image Acquisition*/
    //afx_msg void OnBnClickedContinusModeRadio();        // ch:����ģʽ | en:Continus Mode
    //afx_msg void OnBnClickedTriggerModeRadio();         // ch:����ģʽ | en:Trigger Mode
    afx_msg void OnBnClickedStartGrabbingButton();      // ch:��ʼ�ɼ� | en:Start Grabbing
    afx_msg void OnBnClickedStopGrabbingButton();       // ch:�����ɼ� | en:Stop Grabbing
    //afx_msg void OnBnClickedSoftwareTriggerCheck();     // ch:���� | en:Software Trigger
    afx_msg void OnBnClickedSoftwareOnceButton();       // ch:����һ�� | en:Software Trigger Execute Once
  
    /*ch:ͼ�񱣴� | en:Image Save*/
    afx_msg void OnBnClickedSaveBmpButton();            // ch:����bmp | en:Save bmp
    afx_msg void OnBnClickedSaveJpgButton();            // ch:����jpg | en:Save jpg
    afx_msg void OnBnClickedSaveTiffButton();
    afx_msg void OnBnClickedSavePngButton();
  
    /*ch:�������û�ȡ | en:Parameters Get and Set*/
    afx_msg void OnBnClickedGetParameterButton();       // ch:��ȡ���� | en:Get Parameter
    afx_msg void OnBnClickedSetParameterButton();       // ch:���ò��� | en:Exit from upper right corner

    /*ch:�����߻��� | en:Auxiliary Line Drawing*/
    afx_msg void OnBnClickedCircleAuxiliaryButton();
    afx_msg void OnBnClickedLinesAuxiliaryButton();

    afx_msg void OnClose();

    virtual BOOL PreTranslateMessage(MSG* pMsg);
    int GrabThreadProcess();
    afx_msg void OnBnClickedAuxiliaryLineDrawingStatic();


    //��������
    
    afx_msg void OnClickedResetLocation();
    afx_msg void OnStnClickedExposureStatic2();
    float IDC_scaleFactor;
    afx_msg void OnEnChangeExposureEdit2();
    // �޸�step
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