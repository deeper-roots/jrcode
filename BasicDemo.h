
// BasicDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
// 

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "IMCnet.h"
#include "ParamDef.h"
#include "process.h"
#include <opencv2/opencv.hpp>

// CBasicDemoApp:
// �йش����ʵ�֣������ BasicDemo.cpp
// 

class CBasicDemoApp : public CWinAppEx
{
public:
	CBasicDemoApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CBasicDemoApp theApp;