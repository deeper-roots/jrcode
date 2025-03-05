
#include "process.h"
using namespace std;


bool  TryOpenDevice(IMC_HANDLE & handle, int num)
{
    // 循环尝试打开可行设备
    for (int i = 0; i < num; i++)
    {
        for (int id = 0; id < 4; id++)
        {
            int Status = IMC_Open(&handle, i, id);
            if (Status == IMC_OK) {
                // 弹出成功信息
                return true; // 返回成功状态
            }
        }
    }
    return false; // 返回失败状态
}


void NETConfig1(int axis, IMC_CONTROL& imc_Control_handle_temp)
{//此函数设置控制卡基本的配置，控制卡第一次上电、在其他函数或程序中改变了这些寄存器的值，则必须重新配置
 // TODO: Add your control notification handler code here
    IMC_HANDLE handle=NULL;
    IMC_STATUS status;
    if (handle == NULL) return;
    status = IMC_SetParam16(handle, clearLoc, -1, axis, SEL_IFIFO);		//清除各轴的位置值及状态,配置clear参数必须放在第一
    status = IMC_SetParam32(handle, accellimLoc, 50000, axis, SEL_IFIFO);	//设置加速度极限
    status = IMC_SetParam32(handle, vellimLoc, 5000000, axis, SEL_IFIFO);	//设置速度极限
    status = IMC_SetParam32(handle, mcsmaxvelLoc, 1000000, axis, SEL_IFIFO);//主坐标系点到点运动的规划速度
    status = IMC_SetParam32(handle, mcsaccelLoc, 1000, axis, SEL_IFIFO);	//主坐标系加速度
    status = IMC_SetParam32(handle, mcsdecelLoc, 1000, axis, SEL_IFIFO);	//主坐标系减速度

    status = IMC_SetParam16(handle, steptimeLoc, 300, axis, SEL_IFIFO);
    status = IMC_SetParam16(handle, enaLoc, -1, axis, SEL_IFIFO);			//使能该轴，同时也输出电平使能驱动器（低电平）
    status = IMC_SetParam16(handle, aioctrLoc, 0x0000, axis, SEL_IFIFO);	//设置各轴IO的功能, AIO0、AIO1为限位开关输入（应接入限位开关，且低电平有效；若无限位开关，请去掉该语句）
    status = IMC_SetParam16(handle, homeseqLoc, 0x0014, axis, SEL_IFIFO);	//搜寻原点方式：仅使用原点开关，正方向出发
    status = IMC_SetParam32(handle, lowvelLoc, 83646, axis, SEL_IFIFO);	//低速搜寻原点的速度
    status = IMC_SetParam32(handle, highvelLoc, 83646, axis, SEL_IFIFO);	//高速搜寻原点的速度
    status = IMC_SetParam16(handle, exitfiltLoc, 0, axis, SEL_IFIFO);		//发送错误时(error寄存器置位时)不退出控制器运行
    status = IMC_SetParam16(handle, stopfiltLoc, -1, axis, SEL_IFIFO);		//发送错误时(error寄存器置位时)停止运动
    status = IMC_SetParam16(handle, gout1Loc, -1, axis, SEL_IFIFO);		//
    status = IMC_SetParam16(handle, runLoc, -1, axis, SEL_IFIFO);			//运行该轴
    imc_Control_handle_temp.handle = handle;

}


// 加载数据到track中，返回true表示成功，false表示失败
// 注意：track_Exposure的大小需要根据实际情况设置
//track_Exposure包含位置信息(x，y)、曝光时间
bool LoadData(vector <vector<float>>& track_Exposure) {


    //打开文件，读取信息
    
    ifstream infile(".\\data\\data.txt");
    if (!infile.is_open()) {
        //MessageBox(NULL,L"无法打开文件", L"错误", MB_OK);
        return false;
    }


    string line;
    // 读取每行数据
    while (getline(infile, line)) {
        vector<float> temp;
        istringstream iss(line);  // 使用 istringstream 来解析每行数据
        float value;

        // 将每行中的所有数据读取到 temp 向量中
        while (iss >> value) {
            temp.push_back(value);
        }

        // 将该行的数据存入 track_Exposure
        track_Exposure.push_back(temp);
    }





    ////读取数据
    //double x, y, t;
    //while (infile >> x >> y >> t) {
    //    vector<double> temp;
    //    temp.push_back(x);
    //    temp.push_back(y);
    //    temp.push_back(t);
    //    track_Exposure.push_back(temp);
    //}

    infile.close();

    return true;
}










//位置的单位是mm

bool MoveAbsolute(vector<double> nowsite, vector<double> targetsite, IMC_HANDLE& handleTemp) {
    IMC_STATUS status;
    bool sucessStatus = false;
    //判断坐标是否相同，相同则不移动
    for (int axis = 0; axis < 2; axis++) {
        if (nowsite[axis]!= targetsite[axis]) {
           //开始移动
            //阻塞,尝试十次
            int k = 0;
            do {
                   //将坐标转换成脉冲单位
                
                status = IMC_MoveAbsolute(handleTemp, (int)(4000*targetsite[axis]), axis, SEL_QFIFO);//调用使电机移动到绝对位置的函数

                k++;
            } while (status != IMC_OK && k<10);


        }
        else {
            //坐标相同，不移动
            status = IMC_OK;
            continue;
        }
    }
    //判断是否成功
    if (status == IMC_OK) {
        return true;
    
    }
    else {
        return  false;
    }


}

//能够控制多个轴运动,扩展因子
bool MoveAbsolute(IMC_CONTROL imc_Cotrol_handle_temp) {


    IMC_STATUS status;
    bool sucessStatus = false;
    int step_now=imc_Cotrol_handle_temp.step;
    float scaleFactor_temp=imc_Cotrol_handle_temp.scaleFactor;
    vector<float> targetsite=imc_Cotrol_handle_temp.track_Exposure[step_now];
    int axis_Num=imc_Cotrol_handle_temp.axis_Num;
    //判断坐标是否相同，相同则不移动
    for (int axis = 0; axis < axis_Num; axis++) {
    bool sucessStatus = false;
        if (imc_Cotrol_handle_temp.nowsite[axis] != targetsite[axis]) {
            //开始移动
             //阻塞,尝试十次
            int k = 0;
            do {
                //将坐标转换成脉冲单位


                status = IMC_MoveAbsolute(imc_Cotrol_handle_temp.handle, round(scaleFactor_temp*4000 * targetsite[axis]), axis, SEL_QFIFO);//调用使电机移动到绝对位置的函数

                k++;
            } while (status != IMC_OK && k < 10);


        }
        else {
            //坐标相同，不移动
            status = IMC_OK;
            continue;
        }
    }
    //判断是否成功
    if (status == IMC_OK) {
        return true;

    }
    else {
        return  false;
    }






}

//能够控制多个轴运动,扩展因子
bool Load_Config_data(IMC_CONTROL imc_Cotrol_handle_temp) {

    //加载配置文件，读取txt文件
    int a = 0;


   return true;


}


// 保存日志


void SaveLog(IMC_CONTROL imc_Cotrol_handle_temp)
{
    //输出相关信息
    //打开文件，再最后写入信息
    ofstream outFile(imc_Cotrol_handle_temp.log_Path, ios::app);


    //写入信息
    outFile << "from" << imc_Cotrol_handle_temp.nowsite[0] << " " 
        << imc_Cotrol_handle_temp.nowsite[1] << "moved to  " << 
        imc_Cotrol_handle_temp.track_Exposure[imc_Cotrol_handle_temp.step][0] 
        << " " << imc_Cotrol_handle_temp.track_Exposure[imc_Cotrol_handle_temp.step][1]
        << endl;

    //关闭文件
    outFile.close();


}
