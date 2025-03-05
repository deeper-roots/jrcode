#ifndef PROCESS_H
#define PROCESS_H

#include "stdafx.h"
#include "BasicDemo.h"
#include "BasicDemoDlg.h"
#include "resource.h"		// 主符号
#include "IMCnet.h"
#include "ParamDef.h"
#include "PackageFun.h"
#include "IMC_Def.h"

#include <vector>
#include<iostream>
#include <fstream>
#include <sstream>

using namespace std;

////定义AOI结构体
//struct AOI
//{
//    int x_Off_AOI, y_Off_AOI, width_AOI, height_AOI;
//};


class IMC_CONTROL {
public:
    // 构造函数，初始化成员变量
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

    // 曝光参数是否设置
    bool exposure_Set;

    // 默认保存到一个文件夹里面，与后面重复，未使用
    int root_Path_Circle;

    // 表示运行次数
    int step;

    // 默认支持两个轴运动，也可以修改轴数，需要根据数据文件同时修改
    int axis_Num;

    //
    //图像文件名类别
    int picture_class;

    // 倍数因子, 默认为1 ，表示单位距离
    double scaleFactor;

    // 图片文件名字符表示，用以保存文件名称, 前三个字符为 x, y, z
    char picture_Name[100];

    // 保存导轨的所有移动过程信息，保存图片路径信息，曝光设置信息，已经时间节点
    string log_Path;

    // 轨迹曝光时间
    vector<vector<float>> track_Exposure;

    // 当前的位置
    vector<float> nowsite;

    // 第一个表示曝光是否输出，第二个表示轴0是否输出，以此类推
    vector<bool> log_Output_status;

    // 路径可读，默认为 0
    vector<string> root_Path;

    // 导轨设备句柄，导轨轨迹，曝光时间参数
    IMC_HANDLE handle;

    // 相关函数
    // void SaveLog();
};

//class IMC_CONTROL {
//public:
//	//曝光参数是否设置
//	bool exposure_Set = false;
//	
//	//默认保存到一个文件夹里面
//	int root_Path_Circle = 1;
//	//表示运行次数
//	int step = 0;
//	//m默认支持两个轴运动，也可以修改轴数，需要根据数据文件同时修改
//	int axis_Num = 2;
//
//	//倍数因子,默认为1 ，表示单位距离，如果为1，则表示单位距离为1mm，可更改
//	//从文件或者图形化界面输入
//	double scaleFactor = 1;
//	//图片文件名字符表示，用以保存文件名称,前三个字符为x,y,z
//
//	char picture_Name[100] = {'x','y','z','a','b','c'};
//	//保存导轨的所有移动过程信息，保存图片路径信息，曝光设置信息，已经时间节点
//		string log_Path = ".//data//log.txt";
//	//轨迹曝光时间
//	vector <vector<double>> track_Exposure;
//	vector<double> nowsite = { 0,0 };
//	//第一个表示曝光是否输出，第二个表示轴0是否输出，以此类推，覆盖控制所有轴，最后一个表示是否输出图像,
//	// 每次移动需要根据状态判断输出相关信息
//	vector<bool> log_Output_status = {true,true,true,true};
//	//路径可读，默认为0
//	vector<string> root_Path = { ".//picture//0" ,".//picture//1",".//pictue//2"};
//	
//	//导轨设备句柄，导轨轨迹，曝光时间参数
//	IMC_HANDLE  handle;
//
//
//
//	//相关函数
//	//保存每次信息
//	//void SaveLog();
//
//};
//直接预定义结构体
// IMC_Control_handle;






//循环尝试打开可用设备，比较慢
bool TryOpenDevice(IMC_HANDLE& handle, int num);

void NETConfig1(int axis, IMC_CONTROL& imc_Control_handle_temp);

//加载导轨 轨迹、相机曝光参数
bool LoadData(vector <vector<float>>& track);
//移动导轨到指定位置，首先判断位置是否一样，如果一样则不移动，原始函数
bool MoveAbsolute(vector<double> nowsite, vector<double> targetsite, IMC_HANDLE& handleTemp);

bool MoveAbsolute(IMC_CONTROL imc_Cotrol_handle_temp);

bool Load_Config_data(IMC_CONTROL imc_Cotrol_handle_temp);

void SaveLog(IMC_CONTROL imc_Cotrol_handle_temp);










#endif 