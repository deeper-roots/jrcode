#ifndef PROCESS_H
#define PROCESS_H

#include "stdafx.h"
#include "BasicDemo.h"
#include "BasicDemoDlg.h"
#include "resource.h"		// ������
#include "IMCnet.h"
#include "ParamDef.h"
#include "PackageFun.h"
#include "IMC_Def.h"

#include <vector>
#include<iostream>
#include <fstream>
#include <sstream>

using namespace std;

////����AOI�ṹ��
//struct AOI
//{
//    int x_Off_AOI, y_Off_AOI, width_AOI, height_AOI;
//};


class IMC_CONTROL {
public:
    // ���캯������ʼ����Ա����
    IMC_CONTROL()
        : exposure_Set(true),
        root_Path_Circle(1),
        step(0),
        axis_Num(2),
        picture_class(2),
        scaleFactor(1.0),
        picture_Name{ 'x','y','z','a','b','c' },
        log_Path(".//data//log.txt"),
        track_Exposure(),
        nowsite{ 0, 0 },
        log_Output_status{ true, true, true, true },
        root_Path{ ".//picture//0", ".//picture//1", ".//pictue//2" }
    {
       




    }

    // �ع�����Ƿ�����
    bool exposure_Set;

    // Ĭ�ϱ��浽һ���ļ������棬������ظ���δʹ��
    int root_Path_Circle;

    // ��ʾ���д���
    int step;

    // Ĭ��֧���������˶���Ҳ�����޸���������Ҫ���������ļ�ͬʱ�޸�
    int axis_Num;

    //
    //ͼ���ļ������
    int picture_class;

    // ��������, Ĭ��Ϊ1 ����ʾ��λ����
    double scaleFactor;

    // ͼƬ�ļ����ַ���ʾ�����Ա����ļ�����, ǰ�����ַ�Ϊ x, y, z
    char picture_Name[100];

    // ���浼��������ƶ�������Ϣ������ͼƬ·����Ϣ���ع�������Ϣ���Ѿ�ʱ��ڵ�
    string log_Path;

    // �켣�ع�ʱ��
    vector<vector<float>> track_Exposure;

    // ��ǰ��λ��
    vector<float> nowsite;

    // ��һ����ʾ�ع��Ƿ�������ڶ�����ʾ��0�Ƿ�������Դ�����
    vector<bool> log_Output_status;

    // ·���ɶ���Ĭ��Ϊ 0
    vector<string> root_Path;

    // �����豸���������켣���ع�ʱ�����
    IMC_HANDLE handle;

    // ��غ���
    // void SaveLog();
};

//class IMC_CONTROL {
//public:
//	//�ع�����Ƿ�����
//	bool exposure_Set = false;
//	
//	//Ĭ�ϱ��浽һ���ļ�������
//	int root_Path_Circle = 1;
//	//��ʾ���д���
//	int step = 0;
//	//mĬ��֧���������˶���Ҳ�����޸���������Ҫ���������ļ�ͬʱ�޸�
//	int axis_Num = 2;
//
//	//��������,Ĭ��Ϊ1 ����ʾ��λ���룬���Ϊ1�����ʾ��λ����Ϊ1mm���ɸ���
//	//���ļ�����ͼ�λ���������
//	double scaleFactor = 1;
//	//ͼƬ�ļ����ַ���ʾ�����Ա����ļ�����,ǰ�����ַ�Ϊx,y,z
//
//	char picture_Name[100] = {'x','y','z','a','b','c'};
//	//���浼��������ƶ�������Ϣ������ͼƬ·����Ϣ���ع�������Ϣ���Ѿ�ʱ��ڵ�
//		string log_Path = ".//data//log.txt";
//	//�켣�ع�ʱ��
//	vector <vector<double>> track_Exposure;
//	vector<double> nowsite = { 0,0 };
//	//��һ����ʾ�ع��Ƿ�������ڶ�����ʾ��0�Ƿ�������Դ����ƣ����ǿ��������ᣬ���һ����ʾ�Ƿ����ͼ��,
//	// ÿ���ƶ���Ҫ����״̬�ж���������Ϣ
//	vector<bool> log_Output_status = {true,true,true,true};
//	//·���ɶ���Ĭ��Ϊ0
//	vector<string> root_Path = { ".//picture//0" ,".//picture//1",".//pictue//2"};
//	
//	//�����豸���������켣���ع�ʱ�����
//	IMC_HANDLE  handle;
//
//
//
//	//��غ���
//	//����ÿ����Ϣ
//	//void SaveLog();
//
//};
//ֱ��Ԥ����ṹ��
// IMC_Control_handle;






//ѭ�����Դ򿪿����豸���Ƚ���
bool TryOpenDevice(IMC_HANDLE& handle, int num);

void NETConfig1(int axis, IMC_CONTROL& imc_Control_handle_temp);

//���ص��� �켣������ع����
bool LoadData(vector <vector<float>>& track);
//�ƶ����쵽ָ��λ�ã������ж�λ���Ƿ�һ�������һ�����ƶ���ԭʼ����
bool MoveAbsolute(vector<double> nowsite, vector<double> targetsite, IMC_HANDLE& handleTemp);

bool MoveAbsolute(IMC_CONTROL imc_Cotrol_handle_temp);

bool Load_Config_data(IMC_CONTROL imc_Cotrol_handle_temp);

void SaveLog(IMC_CONTROL imc_Cotrol_handle_temp);










#endif 