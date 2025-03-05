
// BasicDemoDlg.cpp : implementation file
#include "stdafx.h"
//#include"process.h"
#include "BasicDemo.h"
#include "BasicDemoDlg.h"
#include <iomanip>   // for std::setprecision

#ifdef _DEBUG
#define new DEBUG_NEW
#endif




IMC_CONTROL IMC_Control_handle ;



// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CBasicDemoDlg dialog
CBasicDemoDlg::CBasicDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBasicDemoDlg::IDD, pParent)
    , m_pcMyCamera(NULL)
    , m_nDeviceCombo(0)
    , m_bOpenDevice(FALSE)
    , m_bStartGrabbing(FALSE)
    , m_hGrabThread(NULL)
    , m_bThreadState(FALSE)
    , m_nTriggerMode(MV_TRIGGER_MODE_OFF)
    , m_dExposureEdit(0)
    , m_dGainEdit(0)
    , m_dFrameRateEdit(0)
    , m_bSoftWareTriggerCheck(FALSE)
    , m_nTriggerSource(MV_TRIGGER_SOURCE_SOFTWARE)
    , m_pSaveImageBuf(NULL)
    , m_nSaveImageBufSize(0)
    , IDC_scaleFactor(1)
    , goto_step(0)
    , xoff_ROI(0)
    , yoff_ROI(0)
    , width_ROI(0)
    , height_ROI(0)
    , is_AOI(false)
    , map_scale(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    memset(m_chPixelFormat, 0, MV_MAX_SYMBOLIC_LEN);
    memset(&m_stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
}

void CBasicDemoDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_DEVICE_COMBO, m_ctrlDeviceCombo);
    DDX_CBIndex(pDX, IDC_DEVICE_COMBO, m_nDeviceCombo);
    DDX_Text(pDX, IDC_EXPOSURE_EDIT, m_dExposureEdit);
    DDX_Text(pDX, IDC_GAIN_EDIT, m_dGainEdit);
    DDX_Text(pDX, IDC_FRAME_RATE_EDIT, m_dFrameRateEdit);
    DDX_Text(pDX, IDC_PIXEL_FORMAT_EDIT, (CString)(m_chPixelFormat));
    //DDX_Check(pDX, IDC_SOFTWARE_TRIGGER_CHECK, m_bSoftWareTriggerCheck);
    DDX_Text(pDX, IDC_EXPOSURE_EDIT2, IDC_scaleFactor);
    DDX_Text(pDX, IDC_EXPOSURE_EDIT3, goto_step);
    DDX_Text(pDX, IDC_EDIT1, xoff_ROI);
    DDX_Text(pDX, IDC_EDIT2, yoff_ROI);
    DDX_Text(pDX, IDC_EDIT3, width_ROI);
    DDX_Text(pDX, IDC_EDIT4, height_ROI);
    DDX_Text(pDX, IDC_EXPOSURE_EDIT5, is_AOI);
    DDX_Text(pDX, IDC_EDIT5, map_scale);
}

BEGIN_MESSAGE_MAP(CBasicDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	// }}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_ENUM_BUTTON, &CBasicDemoDlg::OnBnClickedEnumButton)
    ON_BN_CLICKED(IDC_OPEN_BUTTON, &CBasicDemoDlg::OnBnClickedOpenButton)
    ON_BN_CLICKED(IDC_CLOSE_BUTTON, &CBasicDemoDlg::OnBnClickedCloseButton)
    //ON_BN_CLICKED(IDC_CONTINUS_MODE_RADIO, &CBasicDemoDlg::OnBnClickedContinusModeRadio)
    //ON_BN_CLICKED(IDC_TRIGGER_MODE_RADIO, &CBasicDemoDlg::OnBnClickedTriggerModeRadio)
    ON_BN_CLICKED(IDC_START_GRABBING_BUTTON, &CBasicDemoDlg::OnBnClickedStartGrabbingButton)
    ON_BN_CLICKED(IDC_STOP_GRABBING_BUTTON, &CBasicDemoDlg::OnBnClickedStopGrabbingButton)
    ON_BN_CLICKED(IDC_GET_PARAMETER_BUTTON, &CBasicDemoDlg::OnBnClickedGetParameterButton)
    ON_BN_CLICKED(IDC_SET_PARAMETER_BUTTON, &CBasicDemoDlg::OnBnClickedSetParameterButton)
    //ON_BN_CLICKED(IDC_SOFTWARE_TRIGGER_CHECK, &CBasicDemoDlg::OnBnClickedSoftwareTriggerCheck)
    //ON_BN_CLICKED(IDC_SOFTWARE_ONCE_BUTTON, &CBasicDemoDlg::OnBnClickedSoftwareOnceButton)
    ON_BN_CLICKED(IDC_SAVE_BMP_BUTTON, &CBasicDemoDlg::OnBnClickedSaveBmpButton)
    ON_BN_CLICKED(IDC_SAVE_JPG_BUTTON, &CBasicDemoDlg::OnBnClickedSaveJpgButton)
    ON_BN_CLICKED(IDC_SAVE_TIFF_BUTTON, &CBasicDemoDlg::OnBnClickedSaveTiffButton)
    ON_BN_CLICKED(IDC_SAVE_PNG_BUTTON, &CBasicDemoDlg::OnBnClickedSavePngButton)
    ON_BN_CLICKED(IDC_CIRCLE_AUXILIARY_BUTTON, &CBasicDemoDlg::OnBnClickedCircleAuxiliaryButton)
    ON_BN_CLICKED(IDC_LINES_AUXILIARY_BUTTON, &CBasicDemoDlg::OnBnClickedLinesAuxiliaryButton)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_AUXILIARY_LINE_DRAWING_STATIC, &CBasicDemoDlg::OnBnClickedAuxiliaryLineDrawingStatic)
    ON_BN_CLICKED(IDC_Reset_Locaztion, & CBasicDemoDlg::OnClickedResetLocation)
    ON_STN_CLICKED(IDC_EXPOSURE_STATIC2, &CBasicDemoDlg::OnStnClickedExposureStatic2)
    ON_EN_CHANGE(IDC_EXPOSURE_EDIT2, &CBasicDemoDlg::OnEnChangeExposureEdit2)
    ON_BN_CLICKED(IDC_BUTTON2, &CBasicDemoDlg::OnClickedGoToStep)
    ON_BN_CLICKED(IDC_BUTTON3, &CBasicDemoDlg::OnBnClickedButton3)
    ON_WM_MOUSEHWHEEL()
    ON_BN_CLICKED(IDC_BUTTON4, &CBasicDemoDlg::OnClickedButton4)
    ON_BN_CLICKED(IDC_BUTTON5, &CBasicDemoDlg::OnBnClickedButton5)
END_MESSAGE_MAP()

// ch:取流线程 | en:Grabbing thread
unsigned int __stdcall GrabThread(void* pUser)
{
    if (pUser)
    {
        CBasicDemoDlg* pCam = (CBasicDemoDlg*)pUser;

        pCam->GrabThreadProcess();
        
        return 0;
    }

    return -1;
}

// CBasicDemoDlg message handlers
BOOL CBasicDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

    //设置缩放系数
    m_zoomFactor = 1.0; // 初始化缩放系数
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	// when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	DisplayWindowInitial();             // ch:显示框初始化 | en:Display Window Initialization

    InitializeCriticalSection(&m_hSaveImageMux);
    //  重置按钮不能用
    GetDlgItem(IDC_Reset_Locaztion)->EnableWindow(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBasicDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CBasicDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBasicDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ch:按钮使能 | en:Enable control
void CBasicDemoDlg::EnableControls(BOOL bIsCameraReady)
{
    GetDlgItem(IDC_OPEN_BUTTON)->EnableWindow(m_bOpenDevice ? FALSE : (bIsCameraReady ? TRUE : FALSE));
    GetDlgItem(IDC_CLOSE_BUTTON)->EnableWindow((m_bOpenDevice && bIsCameraReady) ? TRUE : FALSE);
    GetDlgItem(IDC_START_GRABBING_BUTTON)->EnableWindow((m_bStartGrabbing && bIsCameraReady) ? FALSE : (m_bOpenDevice ? TRUE : FALSE));
    GetDlgItem(IDC_STOP_GRABBING_BUTTON)->EnableWindow(m_bStartGrabbing ? TRUE : FALSE);
    //GetDlgItem(IDC_SOFTWARE_TRIGGER_CHECK)->EnableWindow(m_bOpenDevice ? TRUE : FALSE);
    //GetDlgItem(IDC_SOFTWARE_ONCE_BUTTON)->EnableWindow((m_bStartGrabbing && m_bSoftWareTriggerCheck && ((CButton *)GetDlgItem(IDC_TRIGGER_MODE_RADIO))->GetCheck())? TRUE : FALSE);
    GetDlgItem(IDC_SAVE_BMP_BUTTON)->EnableWindow(m_bStartGrabbing ? TRUE : FALSE);
    GetDlgItem(IDC_SAVE_TIFF_BUTTON)->EnableWindow(m_bStartGrabbing ? TRUE : FALSE);
    GetDlgItem(IDC_SAVE_PNG_BUTTON)->EnableWindow(m_bStartGrabbing ? TRUE : FALSE);
    GetDlgItem(IDC_SAVE_JPG_BUTTON)->EnableWindow(m_bStartGrabbing ? TRUE : FALSE);
    GetDlgItem(IDC_EXPOSURE_EDIT)->EnableWindow(m_bOpenDevice ? TRUE : FALSE);
    GetDlgItem(IDC_GAIN_EDIT)->EnableWindow(m_bOpenDevice ? TRUE : FALSE);
    GetDlgItem(IDC_FRAME_RATE_EDIT)->EnableWindow(m_bOpenDevice ? TRUE : FALSE);
    GetDlgItem(IDC_PIXEL_FORMAT_EDIT)->EnableWindow(FALSE);
    GetDlgItem(IDC_GET_PARAMETER_BUTTON)->EnableWindow(m_bOpenDevice ? TRUE : FALSE);
    GetDlgItem(IDC_SET_PARAMETER_BUTTON)->EnableWindow(m_bOpenDevice ? TRUE : FALSE);
    //GetDlgItem(IDC_CONTINUS_MODE_RADIO)->EnableWindow(m_bOpenDevice ? TRUE : FALSE);
    //GetDlgItem(IDC_TRIGGER_MODE_RADIO)->EnableWindow(m_bOpenDevice ? TRUE : FALSE);
    GetDlgItem(IDC_CIRCLE_AUXILIARY_BUTTON)->EnableWindow(m_bStartGrabbing ? TRUE : FALSE);
    GetDlgItem(IDC_LINES_AUXILIARY_BUTTON)->EnableWindow(m_bStartGrabbing ? TRUE : FALSE);
}

// ch:最开始时的窗口初始化 | en:Initial window initialization
void CBasicDemoDlg::DisplayWindowInitial()
{
    CWnd *pWnd = GetDlgItem(IDC_DISPLAY_STATIC);
    if (pWnd)
    {
        m_hwndDisplay = pWnd->GetSafeHwnd();
        if (m_hwndDisplay)
        {
            EnableControls(FALSE);
        }
    }
}

// ch:显示错误信息 | en:Show error message
void CBasicDemoDlg::ShowErrorMsg(CString csMessage, int nErrorNum)
{
    CString errorMsg;
    if (nErrorNum == 0)
    {
        errorMsg.Format(_T("%s"), csMessage);
    }
    else
    {
        errorMsg.Format(_T("%s: Error = %x: "), csMessage, nErrorNum);
    }

    switch(nErrorNum)
    {
    case MV_E_HANDLE:           errorMsg += "Error or invalid handle ";                                         break;
    case MV_E_SUPPORT:          errorMsg += "Not supported function ";                                          break;
    case MV_E_BUFOVER:          errorMsg += "Cache is full ";                                                   break;
    case MV_E_CALLORDER:        errorMsg += "Function calling order error ";                                    break;
    case MV_E_PARAMETER:        errorMsg += "Incorrect parameter ";                                             break;
    case MV_E_RESOURCE:         errorMsg += "Applying resource failed ";                                        break;
    case MV_E_NODATA:           errorMsg += "No data ";                                                         break;
    case MV_E_PRECONDITION:     errorMsg += "Precondition error, or running environment changed ";              break;
    case MV_E_VERSION:          errorMsg += "Version mismatches ";                                              break;
    case MV_E_NOENOUGH_BUF:     errorMsg += "Insufficient memory ";                                             break;
    case MV_E_ABNORMAL_IMAGE:   errorMsg += "Abnormal image, maybe incomplete image because of lost packet ";   break;
    case MV_E_UNKNOW:           errorMsg += "Unknown error ";                                                   break;
    case MV_E_GC_GENERIC:       errorMsg += "General error ";                                                   break;
    case MV_E_GC_ACCESS:        errorMsg += "Node accessing condition error ";                                  break;
    case MV_E_ACCESS_DENIED:	errorMsg += "No permission ";                                                   break;
    case MV_E_BUSY:             errorMsg += "Device is busy, or network disconnected ";                         break;
    case MV_E_NETER:            errorMsg += "Network error ";                                                   break;
    }

    MessageBox(errorMsg, TEXT("PROMPT"), MB_OK | MB_ICONWARNING);
}

// ch:关闭设备 | en:Close Device
int CBasicDemoDlg::CloseDevice()
{
    m_bThreadState = FALSE;
    if (m_hGrabThread)
    {
        WaitForSingleObject(m_hGrabThread, INFINITE);
        CloseHandle(m_hGrabThread);
        m_hGrabThread = NULL;
    }

    if (m_pcMyCamera)
    {
        m_pcMyCamera->Close();
        delete m_pcMyCamera;
        m_pcMyCamera = NULL;
    }

    m_bStartGrabbing = FALSE;
    m_bOpenDevice = FALSE;

    if (m_pSaveImageBuf)
    {
        free(m_pSaveImageBuf);
        m_pSaveImageBuf = NULL;
    }
    m_nSaveImageBufSize = 0;

    return MV_OK;
}

// ch:获取触发模式 | en:Get Trigger Mode
int CBasicDemoDlg::GetTriggerMode()
{
    MVCC_ENUMVALUE stEnumValue = {0};

    int nRet = m_pcMyCamera->GetEnumValue("TriggerMode", &stEnumValue);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    m_nTriggerMode = stEnumValue.nCurValue;
    if (MV_TRIGGER_MODE_ON ==  m_nTriggerMode)
    {
        //OnBnClickedTriggerModeRadio();
        int a = 0;
    }
    else
    {
        m_nTriggerMode = MV_TRIGGER_MODE_OFF;
        //OnBnClickedContinusModeRadio();
    }

    return MV_OK;
}

// ch:设置触发模式 | en:Set Trigger Mode
int CBasicDemoDlg::SetTriggerMode()
{
    return m_pcMyCamera->SetEnumValue("TriggerMode", m_nTriggerMode);
}

// ch:获取曝光时间 | en:Get Exposure Time
int CBasicDemoDlg::GetExposureTime()
{
    MVCC_FLOATVALUE stFloatValue = {0};

    int nRet = m_pcMyCamera->GetFloatValue("ExposureTime", &stFloatValue);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    m_dExposureEdit = stFloatValue.fCurValue;

    return MV_OK;
}

// ch:设置曝光时间 | en:Set Exposure Time
int CBasicDemoDlg::SetExposureTime()
{
    m_pcMyCamera->SetEnumValue("ExposureAuto", MV_EXPOSURE_AUTO_MODE_OFF);

    return m_pcMyCamera->SetFloatValue("ExposureTime", (float)m_dExposureEdit);
}

// ch:获取增益 | en:Get Gain
int CBasicDemoDlg::GetGain()
{
    MVCC_FLOATVALUE stFloatValue = {0};

    int nRet = m_pcMyCamera->GetFloatValue("Gain", &stFloatValue);
    if (MV_OK != nRet)
    {
        return nRet;
    }
    m_dGainEdit = stFloatValue.fCurValue;

    return MV_OK;
}

// ch:设置增益 | en:Set Gain
int CBasicDemoDlg::SetGain()
{
    // ch:设置增益前先把自动增益关闭，失败无需返回
    //en:Set Gain after Auto Gain is turned off, this failure does not need to return
    m_pcMyCamera->SetEnumValue("GainAuto", 0);

    return m_pcMyCamera->SetFloatValue("Gain", (float)m_dGainEdit);
}

// ch:获取帧率 | en:Get Frame Rate
int CBasicDemoDlg::GetFrameRate()
{
    MVCC_FLOATVALUE stFloatValue = {0};

    int nRet = m_pcMyCamera->GetFloatValue("ResultingFrameRate", &stFloatValue);
    if (MV_OK != nRet)
    {
        return nRet;
    }
    m_dFrameRateEdit = stFloatValue.fCurValue;

    return MV_OK;
}

// ch:设置帧率 | en:Set Frame Rate
int CBasicDemoDlg::SetFrameRate()
{
    int nRet = m_pcMyCamera->SetBoolValue("AcquisitionFrameRateEnable", true);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    return m_pcMyCamera->SetFloatValue("AcquisitionFrameRate", (float)m_dFrameRateEdit);
}

// ch:获取触发源 | en:Get Trigger Source
int CBasicDemoDlg::GetTriggerSource()
{
    MVCC_ENUMVALUE stEnumValue = {0};

    int nRet = m_pcMyCamera->GetEnumValue("TriggerSource", &stEnumValue);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    if ((unsigned int)MV_TRIGGER_SOURCE_SOFTWARE == stEnumValue.nCurValue)
    {
        m_bSoftWareTriggerCheck = TRUE;
    }
    else
    {
        m_bSoftWareTriggerCheck = FALSE;
    }

    return MV_OK;
}

// ch:设置触发源 | en:Set Trigger Source
int CBasicDemoDlg::SetTriggerSource()
{
    int nRet = MV_OK;
    if (m_bSoftWareTriggerCheck)
    {
        m_nTriggerSource = MV_TRIGGER_SOURCE_SOFTWARE;
        nRet = m_pcMyCamera->SetEnumValue("TriggerSource", m_nTriggerSource);
        if (MV_OK != nRet)
        {
            ShowErrorMsg(TEXT("Set Software Trigger Fail"), nRet);
            return nRet;
        }
        //GetDlgItem(IDC_SOFTWARE_ONCE_BUTTON )->EnableWindow(TRUE);
    }
    else
    {
        m_nTriggerSource = MV_TRIGGER_SOURCE_LINE0;
        nRet = m_pcMyCamera->SetEnumValue("TriggerSource", m_nTriggerSource);
        if (MV_OK != nRet)
        {
            ShowErrorMsg(TEXT("Set Hardware Trigger Fail"), nRet);
            return nRet;
        }
        //GetDlgItem(IDC_SOFTWARE_ONCE_BUTTON )->EnableWindow(FALSE);
    }

    return nRet;
}

// 该接口只展示GetEnumEntrySymbolic接口的使用方法
int CBasicDemoDlg::GetPixelFormat()
{
    MVCC_ENUMVALUE stEnumValue = {0};
    MVCC_ENUMENTRY stPixelFormatInfo = {0};

    int nRet = m_pcMyCamera->GetEnumValue("PixelFormat", &stEnumValue);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    stPixelFormatInfo.nValue = stEnumValue.nCurValue;
    nRet = m_pcMyCamera->GetEnumEntrySymbolic("PixelFormat", &stPixelFormatInfo);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    strcpy_s(m_chPixelFormat, MV_MAX_SYMBOLIC_LEN, stPixelFormatInfo.chSymbolic);

    return MV_OK;
}

int CBasicDemoDlg::SetAOIRegion(int64_t nOffsetX, int64_t nOffsetY, int64_t nWidth, int64_t nHeight)
{

    //关闭aoi自动曝光
    int nRet= m_pcMyCamera->SetBoolValue("AutoFunctionAOIUsageIntensity", false);
    // 设置AOI的偏移量和大小
     nRet = m_pcMyCamera->SetIntValue("AutoFunctionAOIWidth", nWidth);
    if (MV_OK != nRet)
    {
        return nRet;
    }
    nRet = m_pcMyCamera->SetIntValue("AutoFunctionAOIHeight", nHeight);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    nRet = m_pcMyCamera->SetIntValue("AutoFunctionAOIOffsetX", nOffsetX);
    if (MV_OK != nRet)
    {
        return nRet;
    }

    nRet = m_pcMyCamera->SetIntValue("AutoFunctionAOIOffsetY", nOffsetY);
    if (MV_OK != nRet)
    {
        return nRet;
    }


    //开始区域AOI曝光
    nRet= m_pcMyCamera->SetBoolValue("AutoFunctionAOIUsageIntensity", true);




    return MV_OK;
}

// ch:保存图片 | en:Save Image
int CBasicDemoDlg::SaveImage(MV_SAVE_IAMGE_TYPE enSaveImageType)
{
    vector<string> root_path = { "./picture/0",".//picture//1" };
    //char* root_path[] = { "./picture/0", "./picture/1", "./picture/2" };
    MV_SAVE_IMAGE_TO_FILE_PARAM_EX stSaveFileParam;
    memset(&stSaveFileParam, 0, sizeof(MV_SAVE_IMAGE_TO_FILE_PARAM_EX));

    EnterCriticalSection(&m_hSaveImageMux);
    if (m_pSaveImageBuf == NULL || m_stImageInfo.enPixelType == 0)
    {
        LeaveCriticalSection(&m_hSaveImageMux);
        return MV_E_NODATA;
    }

    stSaveFileParam.enImageType = enSaveImageType; // ch:需要保存的图像类型 | en:Image format to save
    stSaveFileParam.enPixelType = m_stImageInfo.enPixelType;  // ch:相机对应的像素格式 | en:Camera pixel type
    stSaveFileParam.nWidth      = m_stImageInfo.nWidth;         // ch:相机对应的宽 | en:Width
    stSaveFileParam.nHeight     = m_stImageInfo.nHeight;          // ch:相机对应的高 | en:Height
    stSaveFileParam.nDataLen    = m_stImageInfo.nFrameLen;
    stSaveFileParam.pData       = m_pSaveImageBuf;
    stSaveFileParam.iMethodValue = 0;
	stSaveFileParam.pcImagePath=(char*)malloc(256);
	memset(stSaveFileParam.pcImagePath,0,256);

    // ch:jpg图像质量范围为(50-99], png图像质量范围为[0-9] | en:jpg image nQuality range is (50-99], png image nQuality range is [0-9]
    if (MV_Image_Bmp == stSaveFileParam.enImageType)
    {
        sprintf_s(stSaveFileParam.pcImagePath, 256, "Image_w%d_h%d_fn%03d.bmp", stSaveFileParam.nWidth, stSaveFileParam.nHeight, m_stImageInfo.nFrameNum);
    }
    else if (MV_Image_Jpeg == stSaveFileParam.enImageType)
    {
        stSaveFileParam.nQuality = 80;
        sprintf_s(stSaveFileParam.pcImagePath, 256, "Image_w%d_h%d_fn%03d.jpg" , stSaveFileParam.nWidth, stSaveFileParam.nHeight, m_stImageInfo.nFrameNum);
    }
    else if (MV_Image_Tif == stSaveFileParam.enImageType)
    {
        sprintf_s(stSaveFileParam.pcImagePath, 256, "Image_w%d_h%d_fn%03d.tif", stSaveFileParam.nWidth, stSaveFileParam.nHeight, m_stImageInfo.nFrameNum);
    }
    else if (MV_Image_Png == stSaveFileParam.enImageType)
    {
        stSaveFileParam.nQuality = 8;
        sprintf_s(stSaveFileParam.pcImagePath, 256, "Image_w%d_h%d_fn%03d.png", stSaveFileParam.nWidth, stSaveFileParam.nHeight, m_stImageInfo.nFrameNum);
    }

    int nRet = m_pcMyCamera->SaveImageToFile(&stSaveFileParam);
    LeaveCriticalSection(&m_hSaveImageMux);
	free(stSaveFileParam.pcImagePath);

    return nRet;
}



// ch:保存图片 | en:Save Image
int CBasicDemoDlg::SavePicture(MV_SAVE_IAMGE_TYPE enSaveImageType,string path,vector<float>siteTemp ,char *picture_Name)
{
    MV_SAVE_IMAGE_TO_FILE_PARAM_EX stSaveFileParam;
    memset(&stSaveFileParam, 0, sizeof(MV_SAVE_IMAGE_TO_FILE_PARAM_EX));

    EnterCriticalSection(&m_hSaveImageMux);
    if (m_pSaveImageBuf == NULL || m_stImageInfo.enPixelType == 0)
    {
        LeaveCriticalSection(&m_hSaveImageMux);
        return MV_E_NODATA;
    }

    stSaveFileParam.enImageType = enSaveImageType; // ch:需要保存的图像类型 | en:Image format to save
    stSaveFileParam.enPixelType = m_stImageInfo.enPixelType;  // ch:相机对应的像素格式 | en:Camera pixel type
    stSaveFileParam.nWidth = m_stImageInfo.nWidth;         // ch:相机对应的宽 | en:Width
    stSaveFileParam.nHeight = m_stImageInfo.nHeight;          // ch:相机对应的高 | en:Height
    stSaveFileParam.nDataLen = m_stImageInfo.nFrameLen;
    stSaveFileParam.pData = m_pSaveImageBuf;
    stSaveFileParam.iMethodValue = 0;
    stSaveFileParam.pcImagePath = (char*)malloc(256);
    memset(stSaveFileParam.pcImagePath, 0, 256);




    //根据个数自动添加所有数据
    string path_file = path + "//(";  
    //最后一个数字是曝光值
    //for (int i = 0; i < siteTemp.size() - 1; i++) {
    //
    //    path_file =path_file+"_" + picture_Name[i] +to_string(siteTemp[i]);
    //
    //
    //}

    for (int i = 0; i < siteTemp.size() - 1; i++) {
        // 使用 stringstream 来控制小数位数
        std::stringstream ss;
        ss << std::fixed << std::setprecision(0) << siteTemp[i];  // 保留1位小数

        // 构造文件路径
        //path_file = path_file + "_" + picture_Name[i]+ ss.str();
        path_file = path_file + ss.str() + ",";
    }
    path_file.pop_back();
    path_file += ")";
    // ch:jpg图像质量范围为(50-99], png图像质量范围为[0-9] | en:jpg image nQuality range is (50-99], png image nQuality range is [0-9]
    if (MV_Image_Bmp == stSaveFileParam.enImageType)
    {
        sprintf_s(stSaveFileParam.pcImagePath, 256,"%s.bmp", path_file.c_str());
    }
    else if (MV_Image_Jpeg == stSaveFileParam.enImageType)
    {
        stSaveFileParam.nQuality = 80;
        //sprintf_s(stSaveFileParam.pcImagePath, 256, "%s//Image_x%d_y%d.jpg", path.c_str(), siteTemp[0], siteTemp[1]);
        sprintf_s(stSaveFileParam.pcImagePath, 256, "%s.jpg", path_file.c_str());
    }
    else if (MV_Image_Tif == stSaveFileParam.enImageType)
    {
        sprintf_s(stSaveFileParam.pcImagePath, 256, "%s.tif", path_file.c_str());
        //sprintf_s(stSaveFileParam.pcImagePath, 256, "%s//Image_x%d_y%d.tif", path.c_str(), siteTemp[0], siteTemp[1]);
    }
    else if (MV_Image_Png == stSaveFileParam.enImageType)
    {
        stSaveFileParam.nQuality = 8;
        //sprintf_s(stSaveFileParam.pcImagePath, 256, "%s//Image_x%d_y%d.png", path.c_str(), siteTemp[0], siteTemp[1]);
        sprintf_s(stSaveFileParam.pcImagePath, 256, "%s.png", path_file.c_str());
    }

    int nRet = m_pcMyCamera->SaveImageToFile(&stSaveFileParam);
    LeaveCriticalSection(&m_hSaveImageMux);
    free(stSaveFileParam.pcImagePath);

    return nRet;
}

int CBasicDemoDlg::GrabThreadProcess()
{
    MV_FRAME_OUT stImageInfo = {0};
    MV_DISPLAY_FRAME_INFO stDisplayInfo = {0};
    int nRet = MV_OK;

    while(m_bThreadState)
    {
		if (!m_bStartGrabbing)
		{
			Sleep(10);
			continue;
		}

        nRet = m_pcMyCamera->GetImageBuffer(&stImageInfo, 1000);
        if (nRet == MV_OK)
        {
            //用于保存图片
            EnterCriticalSection(&m_hSaveImageMux);
            if (NULL == m_pSaveImageBuf || stImageInfo.stFrameInfo.nFrameLen > m_nSaveImageBufSize)
            {
                if (m_pSaveImageBuf)
                {
                    free(m_pSaveImageBuf);
                    m_pSaveImageBuf = NULL;
                }

                m_pSaveImageBuf = (unsigned char *)malloc(sizeof(unsigned char) * stImageInfo.stFrameInfo.nFrameLen);
                if (m_pSaveImageBuf == NULL)
                {
                    LeaveCriticalSection(&m_hSaveImageMux);
                    return 0;
                }
                m_nSaveImageBufSize = stImageInfo.stFrameInfo.nFrameLen;
            }
            memcpy(m_pSaveImageBuf, stImageInfo.pBufAddr, stImageInfo.stFrameInfo.nFrameLen);
            memcpy(&m_stImageInfo, &(stImageInfo.stFrameInfo), sizeof(MV_FRAME_OUT_INFO_EX));
            LeaveCriticalSection(&m_hSaveImageMux);

            stDisplayInfo.hWnd = m_hwndDisplay;
            stDisplayInfo.pData = stImageInfo.pBufAddr;
            stDisplayInfo.nDataLen = stImageInfo.stFrameInfo.nFrameLen;
            stDisplayInfo.nWidth = stImageInfo.stFrameInfo.nWidth;
            stDisplayInfo.nHeight = stImageInfo.stFrameInfo.nHeight;
            stDisplayInfo.enPixelType = stImageInfo.stFrameInfo.enPixelType;
            m_pcMyCamera->DisplayOneFrame(&stDisplayInfo);

            m_pcMyCamera->FreeImageBuffer(&stImageInfo);
        }
        else
        {
            if (MV_TRIGGER_MODE_ON ==  m_nTriggerMode)
            {
                Sleep(5);
            }
        }
    }

    return MV_OK;
}
// ch:按下查找设备按钮:枚举 | en:Click Find Device button:Enumeration 
void CBasicDemoDlg::OnBnClickedEnumButton()
{
    CString strMsg;

    m_ctrlDeviceCombo.ResetContent();
    memset(&m_stDevList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

    // ch:枚举子网内所有设备 | en:Enumerate all devices within subnet
    int nRet = CMvCamera::EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &m_stDevList);
    if (MV_OK != nRet)
    {
        return;
    }

    // ch:将值加入到信息列表框中并显示出来 | en:Add value to the information list box and display
    for (unsigned int i = 0; i < m_stDevList.nDeviceNum; i++)
    {
        MV_CC_DEVICE_INFO* pDeviceInfo = m_stDevList.pDeviceInfo[i];
        if (NULL == pDeviceInfo)
        {
            continue;
        }

        wchar_t* pUserName = NULL;
        if (pDeviceInfo->nTLayerType == MV_GIGE_DEVICE)
        {
            int nIp1 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
            int nIp2 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
            int nIp3 = ((m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
            int nIp4 = (m_stDevList.pDeviceInfo[i]->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);

            if (strcmp("", (LPCSTR)(pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName)) != 0)
            {
               char strUserName[256] = {0};
				sprintf_s(strUserName, 256, "%s (%s)", pDeviceInfo->SpecialInfo.stGigEInfo.chUserDefinedName,
					pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber);
                DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, NULL, 0);
                pUserName = new wchar_t[dwLenUserName];
                MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, pUserName, dwLenUserName);
            }
            else
            {
                  char strUserName[256] = {0};
                sprintf_s(strUserName, 256, "%s (%s)", pDeviceInfo->SpecialInfo.stGigEInfo.chModelName,
                    pDeviceInfo->SpecialInfo.stGigEInfo.chSerialNumber);
                DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, NULL, 0);
                pUserName = new wchar_t[dwLenUserName];
                MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, pUserName, dwLenUserName);
            }
            strMsg.Format(_T("[%d]GigE:    %s  (%d.%d.%d.%d)"), i, pUserName, nIp1, nIp2, nIp3, nIp4);
        }
        else if (pDeviceInfo->nTLayerType == MV_USB_DEVICE)
        {
            if (strcmp("", (char*)pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName) != 0)
            {
                char strUserName[256] = {0};
				sprintf_s(strUserName, 256, "%s (%s)", pDeviceInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName,
					pDeviceInfo->SpecialInfo.stUsb3VInfo.chSerialNumber);
                DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, NULL, 0);
                pUserName = new wchar_t[dwLenUserName];
                MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, pUserName, dwLenUserName);
            }
            else
            {
                char strUserName[256] = {0};
                sprintf_s(strUserName, 256, "%s (%s)", pDeviceInfo->SpecialInfo.stUsb3VInfo.chModelName,
                    pDeviceInfo->SpecialInfo.stUsb3VInfo.chSerialNumber);
                DWORD dwLenUserName = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, NULL, 0);
                pUserName = new wchar_t[dwLenUserName];
                MultiByteToWideChar(CP_ACP, 0, (LPCSTR)(strUserName), -1, pUserName, dwLenUserName);
            }
            strMsg.Format(_T("[%d]UsbV3:  %s"), i, pUserName);
        }
        else
        {
            ShowErrorMsg(TEXT("Unknown device enumerated"), 0);
        }
        m_ctrlDeviceCombo.AddString(strMsg);

        if (pUserName)
        {
            delete[] pUserName;
            pUserName = NULL;
        }
    }

    if (0 == m_stDevList.nDeviceNum)
    {
        ShowErrorMsg(TEXT("No device"), 0);
        return;
    }
    m_ctrlDeviceCombo.SetCurSel(0);

    EnableControls(TRUE);
}

// ch:按下打开设备按钮：打开设备 | en:Click Open button: Open Device
void CBasicDemoDlg::OnBnClickedOpenButton()
{
    if (TRUE == m_bOpenDevice || NULL != m_pcMyCamera)
    {
        return;
    }
    UpdateData(TRUE);

    int nIndex = m_nDeviceCombo;
    if ((nIndex < 0) | (nIndex >= MV_MAX_DEVICE_NUM))
    {
        ShowErrorMsg(TEXT("Please select device"), 0);
        return;
    }

    // ch:由设备信息创建设备实例 | en:Device instance created by device information
    if (NULL == m_stDevList.pDeviceInfo[nIndex])
    {
        ShowErrorMsg(TEXT("Device does not exist"), 0);
        return;
    }

    m_pcMyCamera = new CMvCamera;
    if (NULL == m_pcMyCamera)
    {
        return;
    }

    int nRet = m_pcMyCamera->Open(m_stDevList.pDeviceInfo[nIndex]);
    if (MV_OK != nRet)
    {
        delete m_pcMyCamera;
        m_pcMyCamera = NULL;
        ShowErrorMsg(TEXT("Open Fail"), nRet);
        return;
    }

    // ch:探测网络最佳包大小(只对GigE相机有效) | en:Detection network optimal package size(It only works for the GigE camera)
    if (m_stDevList.pDeviceInfo[nIndex]->nTLayerType == MV_GIGE_DEVICE)
    {
        unsigned int nPacketSize = 0;
        nRet = m_pcMyCamera->GetOptimalPacketSize(&nPacketSize);
        if (nRet == MV_OK)
        {
            nRet = m_pcMyCamera->SetIntValue("GevSCPSPacketSize",nPacketSize);
            if(nRet != MV_OK)
            {
                ShowErrorMsg(TEXT("Warning: Set Packet Size fail!"), nRet);
            }
        }
        else
        {
            ShowErrorMsg(TEXT("Warning: Get Packet Size fail!"), nRet);
        }
    }

    m_bOpenDevice = TRUE;
    EnableControls(TRUE);
    OnBnClickedGetParameterButton(); // ch:获取参数 | en:Get Parameter
}

// ch:按下关闭设备按钮：关闭设备 | en:Click Close button: Close Device
void CBasicDemoDlg::OnBnClickedCloseButton()
{
    CloseDevice();
    EnableControls(TRUE);
}

//// ch:按下连续模式按钮 | en:Click Continues button
//void CBasicDemoDlg::OnBnClickedContinusModeRadio()
//{
//    ((CButton *)GetDlgItem(IDC_CONTINUS_MODE_RADIO))->SetCheck(TRUE);
//    ((CButton *)GetDlgItem(IDC_TRIGGER_MODE_RADIO))->SetCheck(FALSE);
//    //((CButton *)GetDlgItem(IDC_SOFTWARE_TRIGGER_CHECK))->EnableWindow(FALSE);
//    m_nTriggerMode = MV_TRIGGER_MODE_OFF;
//    int nRet = SetTriggerMode();
//    if (MV_OK != nRet)
//    {
//        return;
//    }
//    //GetDlgItem(IDC_SOFTWARE_ONCE_BUTTON)->EnableWindow(FALSE);
//}

//// ch:按下触发模式按钮 | en:Click Trigger Mode button
//void CBasicDemoDlg::OnBnClickedTriggerModeRadio()
//{
//    UpdateData(TRUE);
//    //((CButton *)GetDlgItem(IDC_CONTINUS_MODE_RADIO))->SetCheck(FALSE);
//    //((CButton *)GetDlgItem(IDC_TRIGGER_MODE_RADIO))->SetCheck(TRUE);
//    //((CButton *)GetDlgItem(IDC_SOFTWARE_TRIGGER_CHECK))->EnableWindow(TRUE);
//    m_nTriggerMode = MV_TRIGGER_MODE_ON;
//    int nRet = SetTriggerMode();
//    if (MV_OK != nRet)
//    {
//        ShowErrorMsg(TEXT("Set Trigger Mode Fail"), nRet);
//        return;
//    }
//
//    if (m_bStartGrabbing == TRUE)
//    {
//        if (TRUE == m_bSoftWareTriggerCheck)
//        {
//           // GetDlgItem(IDC_SOFTWARE_ONCE_BUTTON )->EnableWindow(TRUE);
//            int a = 0;
//        }
//    }
//}

// ch:按下开始采集按钮 | en:Click Start button
void CBasicDemoDlg::OnBnClickedStartGrabbingButton()
{
    if (FALSE == m_bOpenDevice || TRUE == m_bStartGrabbing || NULL == m_pcMyCamera)
    {
        return;
    }  

    memset(&m_stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
    m_bThreadState = TRUE;
    unsigned int nThreadID = 0;
    m_hGrabThread = (void*)_beginthreadex( NULL , 0 , GrabThread , this, 0 , &nThreadID );
    if (NULL == m_hGrabThread)
    {
        m_bThreadState = FALSE;
        ShowErrorMsg(TEXT("Create thread fail"), 0);
        return;
    }

	int nRet = m_pcMyCamera->StartGrabbing();
	if (MV_OK != nRet)
	{
		m_bThreadState = FALSE;
		ShowErrorMsg(TEXT("Start grabbing fail"), nRet);
		return;
	}
    m_bStartGrabbing = TRUE;
    EnableControls(TRUE);
}

// ch:按下结束采集按钮 | en:Click Stop button
void CBasicDemoDlg::OnBnClickedStopGrabbingButton()
{
    if (FALSE == m_bOpenDevice || FALSE == m_bStartGrabbing || NULL == m_pcMyCamera)
    {
        return;
    }

    m_bThreadState = FALSE;
    if (m_hGrabThread)
    {
        WaitForSingleObject(m_hGrabThread, INFINITE);
        CloseHandle(m_hGrabThread);
        m_hGrabThread = NULL;
    }

    int nRet = m_pcMyCamera->StopGrabbing();
    if (MV_OK != nRet)
    {
        ShowErrorMsg(TEXT("Stop grabbing fail"), nRet);
        return;
    }
    m_bStartGrabbing = FALSE;
    EnableControls(TRUE);
}

// ch:按下获取参数按钮 | en:Click Get Parameter button
void CBasicDemoDlg::OnBnClickedGetParameterButton()
{
    int nRet = GetTriggerMode();
    if (nRet != MV_OK)
    {
        ShowErrorMsg(TEXT("Get Trigger Mode Fail"), nRet);
    }

    nRet = GetExposureTime();
    if (nRet != MV_OK)
    {
        ShowErrorMsg(TEXT("Get Exposure Time Fail"), nRet);
    }

    nRet = GetGain();
    if (nRet != MV_OK)
    {
        ShowErrorMsg(TEXT("Get Gain Fail"), nRet);
    }

    nRet = GetFrameRate();
    if (nRet != MV_OK)
    {
        ShowErrorMsg(TEXT("Get Frame Rate Fail"), nRet);
    }

    nRet = GetTriggerSource();
    if (nRet != MV_OK)
    {
        ShowErrorMsg(TEXT("Get Trigger Source Fail"), nRet);
    }

    nRet = GetPixelFormat();
    if (nRet != MV_OK)
    {
        ShowErrorMsg(TEXT("Get Pixel Format Fail"), nRet);
    }

    UpdateData(FALSE);
}

// ch:按下设置参数按钮 | en:Click Set Parameter button
void CBasicDemoDlg::OnBnClickedSetParameterButton()
{
    UpdateData(TRUE);

    bool bIsSetSucceed = true;
    int nRet = SetExposureTime();
    if (nRet != MV_OK)
    {
        bIsSetSucceed = false;
        ShowErrorMsg(TEXT("Set Exposure Time Fail"), nRet);
    }
    nRet = SetGain();
    if (nRet != MV_OK)
    {
        bIsSetSucceed = false;
        ShowErrorMsg(TEXT("Set Gain Fail"), nRet);
    }
    nRet = SetFrameRate();
    if (nRet != MV_OK)
    {
        bIsSetSucceed = false;
        ShowErrorMsg(TEXT("Set Frame Rate Fail"), nRet);
    }
    
    if (true == bIsSetSucceed)
    {
        ShowErrorMsg(TEXT("Set Parameter Succeed"), nRet);
    }
}

//// ch:按下软触发按钮 | en:Click Software button
//void CBasicDemoDlg::OnBnClickedSoftwareTriggerCheck()
//{
//    UpdateData(TRUE);
//
//    int nRet = SetTriggerSource();
//    if (nRet != MV_OK)
//    {
//        return;
//    }
//}

// ch:按下软触发一次按钮 | en:Click Execute button
void CBasicDemoDlg::OnBnClickedSoftwareOnceButton()
{
    if (TRUE != m_bStartGrabbing)
    {
        return;
    }

    m_pcMyCamera->CommandExecute("TriggerSoftware");
}

// ch:按下保存bmp图片按钮 | en:Click Save BMP button
void CBasicDemoDlg::OnBnClickedSaveBmpButton()
{
    //判断step-1符合规则
    if (IMC_Control_handle.step -1<0 ||IMC_Control_handle.step>IMC_Control_handle.track_Exposure.size())
    {
        //不符合条件，直接返回
        return;
    }


    //只有成功才递进,并开始保存图片
    int root_Sel = (IMC_Control_handle.step-1) % IMC_Control_handle.picture_class;

    //显示信息,


    AfxMessageBox(TEXT("曝光设置移动导轨成功,点击确定开始保存图片"));
    //保存图片,函数可以优化
    int nRet=SavePicture(MV_Image_Bmp, IMC_Control_handle.root_Path[root_Sel], IMC_Control_handle.track_Exposure[IMC_Control_handle.step-1], IMC_Control_handle.picture_Name);



    //int nRet = SaveImage(MV_Image_Bmp);
    if (MV_OK != nRet)
    {
        ShowErrorMsg(TEXT("Save bmp fail"), nRet);
        return;
    }
    ShowErrorMsg(TEXT("Save bmp succeed"), nRet);
}

// ch:按下保存jpg图片按钮 | en:Click Save JPG button
void CBasicDemoDlg::OnBnClickedSaveJpgButton()
{
    int nRet = SaveImage(MV_Image_Jpeg);
    if (MV_OK != nRet)
    {
        ShowErrorMsg(TEXT("Save jpg fail"), nRet);
        return;
    }
    ShowErrorMsg(TEXT("Save jpg succeed"), nRet);
}

//void CBasicDemoDlg::NETConfig(int axis, IMC_CONTROL & imc_Control_handle_temp)
//{//此函数设置控制卡基本的配置，控制卡第一次上电、在其他函数或程序中改变了这些寄存器的值，则必须重新配置
// // TODO: Add your control notification handler code here
//    IMC_HANDLE handle;
//    IMC_STATUS status;
//    if (handle == NULL) return;
//    status = IMC_SetParam16(handle, clearLoc, -1, axis, SEL_IFIFO);		//清除各轴的位置值及状态,配置clear参数必须放在第一
//    status = IMC_SetParam32(handle, accellimLoc, 50000, axis, SEL_IFIFO);	//设置加速度极限
//    status = IMC_SetParam32(handle, vellimLoc, 5000000, axis, SEL_IFIFO);	//设置速度极限
//    status = IMC_SetParam32(handle, mcsmaxvelLoc, 1000000, axis, SEL_IFIFO);//主坐标系点到点运动的规划速度
//    status = IMC_SetParam32(handle, mcsaccelLoc, 1000, axis, SEL_IFIFO);	//主坐标系加速度
//    status = IMC_SetParam32(handle, mcsdecelLoc, 1000, axis, SEL_IFIFO);	//主坐标系减速度
//
//    status = IMC_SetParam16(handle, steptimeLoc, 300, axis, SEL_IFIFO);
//    status = IMC_SetParam16(handle, enaLoc, -1, axis, SEL_IFIFO);			//使能该轴，同时也输出电平使能驱动器（低电平）
//    status = IMC_SetParam16(handle, aioctrLoc, 0x0000, axis, SEL_IFIFO);	//设置各轴IO的功能, AIO0、AIO1为限位开关输入（应接入限位开关，且低电平有效；若无限位开关，请去掉该语句）
//    status = IMC_SetParam16(handle, homeseqLoc, 0x0014, axis, SEL_IFIFO);	//搜寻原点方式：仅使用原点开关，正方向出发
//    status = IMC_SetParam32(handle, lowvelLoc, 83646, axis, SEL_IFIFO);	//低速搜寻原点的速度
//    status = IMC_SetParam32(handle, highvelLoc, 83646, axis, SEL_IFIFO);	//高速搜寻原点的速度
//    status = IMC_SetParam16(handle, exitfiltLoc, 0, axis, SEL_IFIFO);		//发送错误时(error寄存器置位时)不退出控制器运行
//    status = IMC_SetParam16(handle, stopfiltLoc, -1, axis, SEL_IFIFO);		//发送错误时(error寄存器置位时)停止运动
//    status = IMC_SetParam16(handle, gout1Loc, -1, axis, SEL_IFIFO);		//
//    status = IMC_SetParam16(handle, runLoc, -1, axis, SEL_IFIFO);			//运行该轴
//    imc_Control_handle_temp.handle = handle;
//
//}








// ch:按下保存tiff图片按钮 | en:Click Save Tiff button
//改成了打开导轨设备，配置导轨信息
void CBasicDemoDlg::OnBnClickedSaveTiffButton()
{
    //打开并连接导轨

    //搜索网卡
    NIC_INFO info;
    IMC_STATUS status;
    int i, num;
    char n[10];
    
    status = IMC_FindNetCard(&info, &num);
    //初始化 handle，类型IMC_HANDLE

   
    
    //循环尝试打开可行设备，用IMC_Open打开
    bool statuesTemp = TryOpenDevice(IMC_Control_handle.handle, num);
    if (statuesTemp == false) {
        MessageBox(NULL, L"导轨设备打开失败,搜索到设备个数"+CString(to_string(num).c_str()));
        return;
    }
    else {
        //开始配置导轨信息

        WORD axis, Naxis;
        BeginWaitCursor();
        //获取支持的轴数
        status = IMC_GetParam16(IMC_Control_handle.handle, naxisLoc, &Naxis, 0);
        if (status != IMC_OK)
            Naxis = 16;
        for (axis = 0; axis < Naxis; axis++)
            NETConfig1(axis, IMC_Control_handle   );
        if (status == IMC_OK)
            AfxMessageBox(L"配置成功！");

        //存在重复配置
        IMC_Control_handle.nowsite.clear();
        //第一次加载，需要清空，再加载
        // 
        //重置数据
        for (int i = 0; i < IMC_Control_handle.axis_Num; i++)
        {

            IMC_Control_handle.nowsite.push_back(0);

        }
        IMC_Control_handle.step = 0;
   /*     IMC_Control_handle.nowsite[0]=0;
        IMC_Control_handle.nowsite[1]=0;*/
        EndWaitCursor();
        //使能按钮
        GetDlgItem(IDC_Reset_Locaztion)->EnableWindow(TRUE);
    }
    


            

























}
//
// ch:按下保存png图片按钮 | en:Click Save PNG button
//     改为加载数据
void CBasicDemoDlg::OnBnClickedSavePngButton()
{
    //step置零
    IMC_Control_handle.step = 0;
    vector <vector<float>> track_Exposure;
    //调用函数，显示加载数据
    bool statuesTemp = LoadData(track_Exposure);
    IMC_Control_handle.track_Exposure= track_Exposure;
    if (statuesTemp == false) {
        MessageBox(NULL, L"加载数据失败");
        return;
    }
    else {
        IMC_Control_handle.nowsite.clear();
        //IMC_Control_handle.axis_Num = IMC_Control_handle.track_Exposure[0].size()-1;
        //加载文件，重置位置,清零
        for (int i = 0; i < IMC_Control_handle.axis_Num; i++) {
            IMC_Control_handle.nowsite.push_back( 0);
        
        }
        UpdateData(TRUE);
        //读取扩展因子
        IMC_Control_handle.scaleFactor=IDC_scaleFactor;

        UpdateData(FALSE);
        //提示加载数据成功
        MessageBox(NULL, L"加载数据成功");


    }

}

// ch:辅助线绘制 | en:Auxiliary Line Drawing
//改成设置曝光、移动
void CBasicDemoDlg::OnBnClickedCircleAuxiliaryButton()
{      
    vector<string> path;
    //确保不会超出范围
    int exposure_index=IMC_Control_handle.track_Exposure[0].size() -1;
   // int IMC_Control_handle_axis_Num=IMC_Control_handle.track_Exposure.size();

    //判断是否AOI区域自动曝光
 
    if (IMC_Control_handle.step < IMC_Control_handle.track_Exposure.size()) {
        //没必要每次设置模式，可以优化
        int nRet= MV_OK;
        //判断是否自动曝光
        //float i = IMC_Control_handle.step % 2;
        //if ((IMC_Control_handle.step % 2) == 1) {
            //aoi_temp.is_AOI_temp = false;
        //}
        //else {
            //aoi_temp.is_AOI_temp = true;
       // }
        if(aoi_temp.is_AOI_temp==false){
            //关闭自动曝光
            m_pcMyCamera->SetEnumValue("ExposureAuto", MV_EXPOSURE_AUTO_MODE_OFF);
            //判断曝光时间是否和上次相同，如果相同则不设置
            if (IMC_Control_handle.step > 0 && IMC_Control_handle.track_Exposure[IMC_Control_handle.step][exposure_index] 
                != IMC_Control_handle.track_Exposure[IMC_Control_handle.step - 1][exposure_index]) {
            
                 nRet = m_pcMyCamera->SetFloatValue("ExposureTime", (float)IMC_Control_handle.track_Exposure[IMC_Control_handle.step][exposure_index]);
            }
            else if(IMC_Control_handle.step==0) {
                //第一步需要设置
                nRet = m_pcMyCamera->SetFloatValue("ExposureTime", (float)IMC_Control_handle.track_Exposure[IMC_Control_handle.step][exposure_index]);
        
            }
        }
        else {
        //设置AOI区域自动曝光
            int64_t x_off_temp, y_off_temp, width_temp, height_temp;
            int step_now = IMC_Control_handle.step;
            float scaleFactor_temp = IMC_Control_handle.scaleFactor;
            vector<float> targetsite = IMC_Control_handle.track_Exposure[step_now];
            int axis_Num = IMC_Control_handle.axis_Num;
            int nRet = m_pcMyCamera->SetEnumValue("ExposureAuto", MV_EXPOSURE_AUTO_MODE_OFF);
            //根据坐标计算AOI
            //具体是加还是减，需要看实际情况

            x_off_temp = aoi_temp.x_Off_AOI - aoi_temp.map_scale*targetsite[0];
            y_off_temp =aoi_temp.y_Off_AOI - aoi_temp.map_scale* targetsite[1];
            width_temp = aoi_temp.width_AOI;
            height_temp = aoi_temp.height_AOI;
            nRet = SetAOIRegion(x_off_temp, y_off_temp, width_temp, height_temp);
             
            ////nRet = SetAOIRegion(0, 0, 300, 300);
            //if (MV_OK != nRet)
            //{
            //    ShowErrorMsg(TEXT("Set AOI Region Fail"), nRet);
            //    return;
            //}


            nRet = m_pcMyCamera->SetEnumValue("ExposureAuto", MV_EXPOSURE_AUTO_MODE_CONTINUOUS);

           
            //if (MV_OK != nRet)
            //{
            //    ShowErrorMsg(TEXT("曝光Fail"), nRet);
            //    return;
            //}

            //ShowErrorMsg(TEXT("成功"), nRet);
           //计算当前AOI区域
        

            //设置自动曝光
        
        }

        //int nRet = m_pcMyCamera->SetFloatValue("ExposureTime", (float)track_Exposure[step][2]);
        if (MV_OK != nRet) {
            ShowErrorMsg(TEXT("设置曝光失败,请重试"), nRet);
            return;
        }
        else {
            //开始移动导轨
            

            bool status=MoveAbsolute( IMC_Control_handle);
            
            if (!status) {
                //移动不成功，重新,显示信息
                ShowErrorMsg(TEXT("移动导轨失败,请重试"), nRet);
                return;
            }
            else {
                //延迟几秒，确保，上述过程正常
                Sleep(1000);
                //只有成功才递进,并开始保存图片
                int root_Sel = IMC_Control_handle.step % IMC_Control_handle.picture_class;

                //显示信息,
                //显示坐标信息

                AfxMessageBox(TEXT("移动前坐标为：") + 
                    CString(to_string(IMC_Control_handle.nowsite[0]).c_str()) 
                    + TEXT(",") +
                    CString(to_string(IMC_Control_handle.nowsite[1]).c_str())+ 
                    TEXT("移动后坐标为：")+ CString(to_string(IMC_Control_handle.track_Exposure[IMC_Control_handle.step][0]).c_str())
                    + TEXT(",") +
                    CString(to_string(IMC_Control_handle.track_Exposure[IMC_Control_handle.step][1]).c_str())
                
                );

                AfxMessageBox(TEXT("曝光设置移动导轨成功,点击确定开始保存图片"));
                //保存图片,函数可以优化
                SavePicture(MV_Image_Bmp, IMC_Control_handle.root_Path[root_Sel], IMC_Control_handle.track_Exposure[IMC_Control_handle.step], IMC_Control_handle.picture_Name);
                SaveLog(IMC_Control_handle);
                //更新数据
                // 循环更新数据
                for (int i = 0; i < IMC_Control_handle.axis_Num; i++)
                {

                    IMC_Control_handle.nowsite[i] = IMC_Control_handle.track_Exposure[IMC_Control_handle.step][i];

                }
               
                IMC_Control_handle.step += 1;

            }

        
        
        }
        
        
    }
    
    
}
//修改成  关闭导轨
void CBasicDemoDlg::OnBnClickedLinesAuxiliaryButton()
{
    // TODO: 在此添加控件通知处理程序代码
    if (IMC_Control_handle.handle != NULL)
    {
        IMC_Close(IMC_Control_handle.handle);
        IMC_Control_handle.handle = NULL;
        MessageBox(L"设备已经成功关闭");
    }


}

// ch:右上角退出 | en:Exit from upper right corner
void CBasicDemoDlg::OnClose()
{
    PostQuitMessage(0);
    CloseDevice();

    DeleteCriticalSection(&m_hSaveImageMux);
    CDialog::OnClose();
}

BOOL CBasicDemoDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN&&pMsg->wParam == VK_ESCAPE)
    {
        return TRUE;
    }

    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
    {
        return TRUE;
    }

    return CDialog::PreTranslateMessage(pMsg);
}



void CBasicDemoDlg::OnBnClickedAuxiliaryLineDrawingStatic()
{
    // TODO: 在此添加控件通知处理程序代码
}


void CBasicDemoDlg::OnClickedResetLocation()
{
    // TODO: 在此添加控件通知处理程序代码
    IMC_STATUS status;
    bool sucessStatus = false;
    //判断坐标是否相同，相同则不移动
    for (int axis = 0; axis < IMC_Control_handle.axis_Num; axis++) {
        
             //阻塞,尝试十次
            int k = 0;
            do {
                //将坐标转换成脉冲单位

                status = IMC_MoveAbsolute( IMC_Control_handle.handle, 0, axis, SEL_QFIFO);//调用使电机移动到绝对位置的函数
                //使用命令搜索原点，避免位置误差
                //status = IMC_SetParam16(IMC_Control_handle.handle, gohomeLoc, 0xffff, axis, SEL_IFIFO);
                k++;
            } while (status != IMC_OK && k < 10);
            //显示信息
            if (!status) {
                //重置原点,则step 可能也需要更新
                AfxMessageBox(TEXT("重置坐标成功！"));
            }
            else {
            
                AfxMessageBox(TEXT("重置坐标失败！"));
            }
            

        }



    //// TODO: 在此添加控件通知处理程序代码
    //IMC_HANDLE handle = NULL;
    //IMC_STATUS status;
    ////直接采用全局变量

    //if (IMC_Control_handle.handle == NULL) {
    //    AfxMessageBox(TEXT("导轨未打开，请先打开导轨"));
    //    return;
    //}
    //for (int axis = 0; axis < IMC_Control_handle.axis_Num; axis++) {
    //    //给导轨设备发送命令
    //    //PVOID *data = 0xffff;
    //    status = IMC_SetParam16(IMC_Control_handle.handle, sethomeLoc, 0xffff, axis, SEL_IFIFO);

    //    //
    //    if (status == IMC_OK) {
    //        AfxMessageBox(TEXT("原点设置成功"));
    //    }

    //}



}


void CBasicDemoDlg::OnStnClickedExposureStatic2()
{
    // TODO: 在此添加控件通知处理程序代码
}


void CBasicDemoDlg::OnEnChangeExposureEdit2()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CDialog::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
    //将数据写入到自定义的类中
   // IMC_Control_handle.scaleFactor = atof(m_ExposureEdit2.GetWindowText());
}


void CBasicDemoDlg::OnClickedGoToStep()
{
    // TODO: 在此添加控件通知处理程序代码

    //获取控件数据
    UpdateData(TRUE);
    //判断输入数据是否正常
    if(goto_step < 0 || goto_step > IMC_Control_handle.track_Exposure.size()){
     //数据不正常，
        //跳出弹窗
        AfxMessageBox(TEXT("数据输入存在问题！"));
        return;
    }
    //下标从零开始，实际从1开始
    IMC_Control_handle.step =goto_step-1 ;
    UpdateData(FALSE);
}

//设置原点，将当前位置设置为原点
void CBasicDemoDlg::OnBnClickedButton3()
{
    // TODO: 在此添加控件通知处理程序代码
    IMC_HANDLE handle=NULL;
    IMC_STATUS status;
    //直接采用全局变量

    if (IMC_Control_handle.handle == NULL) {
        AfxMessageBox(TEXT("导轨未打开，请先打开导轨"));
        return;
    }
    for (int axis = 0; axis < IMC_Control_handle.axis_Num; axis++) {
        //给导轨设备发送命令
        //PVOID *data = 0xffff;
        status=IMC_SetParam16(IMC_Control_handle.handle, sethomeLoc, 0xffff, axis, SEL_IFIFO);
        //机械原点与原点位置可能存在偏移，需要修改使嘚机械原点与原点位置重合
        status = IMC_SetParam32(IMC_Control_handle.handle, homeposLoc, 0, axis, SEL_IFIFO);
        
        //
        if (status == IMC_OK) {
            AfxMessageBox(TEXT("原点设置成功"));
        }
        
    }
    




}


void CBasicDemoDlg::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // 此功能要求 Windows Vista 或更高版本。
    // _WIN32_WINNT 符号必须 >= 0x0600。
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    
  
   


}


void CBasicDemoDlg::OnClickedButton4()
{
    // TODO: 在此添加控件通知处理程序代码
    UpdateData(TRUE);

    //开始获取
    //long  xoff_temp,yoff_temp,width_temp,height_temp;
    //先关闭自动曝光
       //写入数据到结构体
    aoi_temp.x_Off_AOI = xoff_ROI;
    aoi_temp.y_Off_AOI = yoff_ROI;
    aoi_temp.width_AOI = width_ROI;
    aoi_temp.height_AOI = height_ROI;
    aoi_temp.is_AOI_temp = is_AOI;
    aoi_temp.map_scale = map_scale;

 

    int nRet = m_pcMyCamera->SetEnumValue("ExposureAuto", MV_EXPOSURE_AUTO_MODE_OFF);
    // 设置AOI区域 (示例值)
 
    nRet = SetAOIRegion(xoff_ROI, yoff_ROI, width_ROI, height_ROI);
    //nRet = SetAOIRegion(0, 0, 300, 300);
    if (MV_OK != nRet)
    {
        ShowErrorMsg(TEXT("Set AOI Region Fail"), nRet);
        return;
    }


    nRet = m_pcMyCamera->SetEnumValue("ExposureAuto", MV_EXPOSURE_AUTO_MODE_CONTINUOUS);
    if (MV_OK != nRet)
    {
        ShowErrorMsg(TEXT("曝光Fail"), nRet);
        return;
    }

    ShowErrorMsg(TEXT("成功"), nRet);







    //UpdateData(FALSE);
    
}
// 全局变量
cv::Mat g_displayImage; // 存储当前显示的图像
bool stopDisplay = false; // 标志位，用于停止显示

// 鼠标回调函数
void onMouse(int event, int x, int y, int flags, void* userdata) {
    if (event == cv::EVENT_LBUTTONDOWN) {
        if (!g_displayImage.empty()) {
            if (x >= 0 && x < g_displayImage.cols && y >= 0 && y < g_displayImage.rows) {
                std::string pixelValueStr;
                if (g_displayImage.channels() == 1) {
                    // 单通道图像（灰度图）
                    uchar pixelValue = g_displayImage.at<uchar>(y, x);
                    pixelValueStr = "Value: " + std::to_string(static_cast<int>(pixelValue));
                }
                else if (g_displayImage.channels() == 3) {
                    // 三通道图像（彩色图）
                    cv::Vec3b pixel = g_displayImage.at<cv::Vec3b>(y, x);
                    uchar b = pixel[0];
                    uchar g = pixel[1];
                    uchar r = pixel[2];
                    pixelValueStr = "B: " + std::to_string(static_cast<int>(b)) +
                        ", G: " + std::to_string(static_cast<int>(g)) +
                        ", R: " + std::to_string(static_cast<int>(r));
                }

                // 创建一个新图像用于显示像素值
                cv::Mat popupImage = cv::Mat::zeros(150, 300, CV_8UC3); // 创建一个黑色背景的图像
                cv::putText(popupImage, pixelValueStr, cv::Point(10, 75), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);

                // 显示弹窗
                cv::imshow("Pixel Value", popupImage);
            }
        }
    }
    else if (event == cv::EVENT_RBUTTONDOWN) {
        // 设置标志位，停止显示
        stopDisplay = true;
    }
}

void CBasicDemoDlg::OnBnClickedButton5()
{
    // 进入临界区
    EnterCriticalSection(&m_hSaveImageMux);
    stopDisplay = false;
    // 检查图像数据是否有效
    if (m_pSaveImageBuf == nullptr || m_stImageInfo.nWidth <= 0 || m_stImageInfo.nHeight <= 0) {
        // 离开临界区
        LeaveCriticalSection(&m_hSaveImageMux);
        std::cerr << "Invalid image data!" << std::endl;
        return;
    }

    // 根据像素类型创建 cv::Mat 对象
    cv::Mat image;
    switch (m_stImageInfo.enPixelType) {
    case PixelType_Gvsp_Mono8:
        image = cv::Mat(m_stImageInfo.nHeight, m_stImageInfo.nWidth, CV_8UC1, m_pSaveImageBuf);
        break;
    case PixelType_Gvsp_RGB8_Packed:
        image = cv::Mat(m_stImageInfo.nHeight, m_stImageInfo.nWidth, CV_8UC3, m_pSaveImageBuf);
        cv::cvtColor(image, image, cv::COLOR_RGB2BGR); // OpenCV 默认使用 BGR 格式
        break;
    default:
        // 可以根据需要添加更多像素类型的处理
        std::cerr << "Unsupported pixel type!" << std::endl;
        LeaveCriticalSection(&m_hSaveImageMux);
        return;
    }

    // 离开临界区
    LeaveCriticalSection(&m_hSaveImageMux);

    // 显示图像窗口
    cv::namedWindow("Display Image", cv::WINDOW_NORMAL);

    // 设置鼠标回调函数
    cv::setMouseCallback("Display Image", onMouse);

    // 主循环：实时显示图像
    stopDisplay = false; // 重置标志位
    while (!stopDisplay) {
        // 更新显示的图像
        g_displayImage = image.clone();

        // 显示图像
        cv::imshow("Display Image", g_displayImage);

        // 等待 1ms，并检查是否按下 ESC 键
        if (cv::waitKey(1) == 27) { // ESC 键的 ASCII 码为 27
            break;
        }
    }

    // 关闭窗口
    cv::destroyWindow("Display Image");
}