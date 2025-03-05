
#include "process.h"
using namespace std;


bool  TryOpenDevice(IMC_HANDLE & handle, int num)
{
    // ѭ�����Դ򿪿����豸
    for (int i = 0; i < num; i++)
    {
        for (int id = 0; id < 4; id++)
        {
            int Status = IMC_Open(&handle, i, id);
            if (Status == IMC_OK) {
                // �����ɹ���Ϣ
                return true; // ���سɹ�״̬
            }
        }
    }
    return false; // ����ʧ��״̬
}


void NETConfig1(int axis, IMC_CONTROL& imc_Control_handle_temp)
{//�˺������ÿ��ƿ����������ã����ƿ���һ���ϵ硢����������������иı�����Щ�Ĵ�����ֵ���������������
 // TODO: Add your control notification handler code here
    IMC_HANDLE handle=NULL;
    IMC_STATUS status;
    if (handle == NULL) return;
    status = IMC_SetParam16(handle, clearLoc, -1, axis, SEL_IFIFO);		//��������λ��ֵ��״̬,����clear����������ڵ�һ
    status = IMC_SetParam32(handle, accellimLoc, 50000, axis, SEL_IFIFO);	//���ü��ٶȼ���
    status = IMC_SetParam32(handle, vellimLoc, 5000000, axis, SEL_IFIFO);	//�����ٶȼ���
    status = IMC_SetParam32(handle, mcsmaxvelLoc, 1000000, axis, SEL_IFIFO);//������ϵ�㵽���˶��Ĺ滮�ٶ�
    status = IMC_SetParam32(handle, mcsaccelLoc, 1000, axis, SEL_IFIFO);	//������ϵ���ٶ�
    status = IMC_SetParam32(handle, mcsdecelLoc, 1000, axis, SEL_IFIFO);	//������ϵ���ٶ�

    status = IMC_SetParam16(handle, steptimeLoc, 300, axis, SEL_IFIFO);
    status = IMC_SetParam16(handle, enaLoc, -1, axis, SEL_IFIFO);			//ʹ�ܸ��ᣬͬʱҲ�����ƽʹ�����������͵�ƽ��
    status = IMC_SetParam16(handle, aioctrLoc, 0x0000, axis, SEL_IFIFO);	//���ø���IO�Ĺ���, AIO0��AIO1Ϊ��λ�������루Ӧ������λ���أ��ҵ͵�ƽ��Ч��������λ���أ���ȥ������䣩
    status = IMC_SetParam16(handle, homeseqLoc, 0x0014, axis, SEL_IFIFO);	//��Ѱԭ�㷽ʽ����ʹ��ԭ�㿪�أ����������
    status = IMC_SetParam32(handle, lowvelLoc, 83646, axis, SEL_IFIFO);	//������Ѱԭ����ٶ�
    status = IMC_SetParam32(handle, highvelLoc, 83646, axis, SEL_IFIFO);	//������Ѱԭ����ٶ�
    status = IMC_SetParam16(handle, exitfiltLoc, 0, axis, SEL_IFIFO);		//���ʹ���ʱ(error�Ĵ�����λʱ)���˳�����������
    status = IMC_SetParam16(handle, stopfiltLoc, -1, axis, SEL_IFIFO);		//���ʹ���ʱ(error�Ĵ�����λʱ)ֹͣ�˶�
    status = IMC_SetParam16(handle, gout1Loc, -1, axis, SEL_IFIFO);		//
    status = IMC_SetParam16(handle, runLoc, -1, axis, SEL_IFIFO);			//���и���
    imc_Control_handle_temp.handle = handle;

}


// �������ݵ�track�У�����true��ʾ�ɹ���false��ʾʧ��
// ע�⣺track_Exposure�Ĵ�С��Ҫ����ʵ���������
//track_Exposure����λ����Ϣ(x��y)���ع�ʱ��
bool LoadData(vector <vector<float>>& track_Exposure) {


    //���ļ�����ȡ��Ϣ
    
    ifstream infile(".\\data\\data.txt");
    if (!infile.is_open()) {
        //MessageBox(NULL,L"�޷����ļ�", L"����", MB_OK);
        return false;
    }


    string line;
    // ��ȡÿ������
    while (getline(infile, line)) {
        vector<float> temp;
        istringstream iss(line);  // ʹ�� istringstream ������ÿ������
        float value;

        // ��ÿ���е��������ݶ�ȡ�� temp ������
        while (iss >> value) {
            temp.push_back(value);
        }

        // �����е����ݴ��� track_Exposure
        track_Exposure.push_back(temp);
    }





    ////��ȡ����
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










//λ�õĵ�λ��mm

bool MoveAbsolute(vector<double> nowsite, vector<double> targetsite, IMC_HANDLE& handleTemp) {
    IMC_STATUS status;
    bool sucessStatus = false;
    //�ж������Ƿ���ͬ����ͬ���ƶ�
    for (int axis = 0; axis < 2; axis++) {
        if (nowsite[axis]!= targetsite[axis]) {
           //��ʼ�ƶ�
            //����,����ʮ��
            int k = 0;
            do {
                   //������ת�������嵥λ
                
                status = IMC_MoveAbsolute(handleTemp, (int)(4000*targetsite[axis]), axis, SEL_QFIFO);//����ʹ����ƶ�������λ�õĺ���

                k++;
            } while (status != IMC_OK && k<10);


        }
        else {
            //������ͬ�����ƶ�
            status = IMC_OK;
            continue;
        }
    }
    //�ж��Ƿ�ɹ�
    if (status == IMC_OK) {
        return true;
    
    }
    else {
        return  false;
    }


}

//�ܹ����ƶ�����˶�,��չ����
bool MoveAbsolute(IMC_CONTROL imc_Cotrol_handle_temp) {


    IMC_STATUS status;
    bool sucessStatus = false;
    int step_now=imc_Cotrol_handle_temp.step;
    float scaleFactor_temp=imc_Cotrol_handle_temp.scaleFactor;
    vector<float> targetsite=imc_Cotrol_handle_temp.track_Exposure[step_now];
    int axis_Num=imc_Cotrol_handle_temp.axis_Num;
    //�ж������Ƿ���ͬ����ͬ���ƶ�
    for (int axis = 0; axis < axis_Num; axis++) {
    bool sucessStatus = false;
        if (imc_Cotrol_handle_temp.nowsite[axis] != targetsite[axis]) {
            //��ʼ�ƶ�
             //����,����ʮ��
            int k = 0;
            do {
                //������ת�������嵥λ


                status = IMC_MoveAbsolute(imc_Cotrol_handle_temp.handle, round(scaleFactor_temp*4000 * targetsite[axis]), axis, SEL_QFIFO);//����ʹ����ƶ�������λ�õĺ���

                k++;
            } while (status != IMC_OK && k < 10);


        }
        else {
            //������ͬ�����ƶ�
            status = IMC_OK;
            continue;
        }
    }
    //�ж��Ƿ�ɹ�
    if (status == IMC_OK) {
        return true;

    }
    else {
        return  false;
    }






}

//�ܹ����ƶ�����˶�,��չ����
bool Load_Config_data(IMC_CONTROL imc_Cotrol_handle_temp) {

    //���������ļ�����ȡtxt�ļ�
    int a = 0;


   return true;


}


// ������־


void SaveLog(IMC_CONTROL imc_Cotrol_handle_temp)
{
    //��������Ϣ
    //���ļ��������д����Ϣ
    ofstream outFile(imc_Cotrol_handle_temp.log_Path, ios::app);


    //д����Ϣ
    outFile << "from" << imc_Cotrol_handle_temp.nowsite[0] << " " 
        << imc_Cotrol_handle_temp.nowsite[1] << "moved to  " << 
        imc_Cotrol_handle_temp.track_Exposure[imc_Cotrol_handle_temp.step][0] 
        << " " << imc_Cotrol_handle_temp.track_Exposure[imc_Cotrol_handle_temp.step][1]
        << endl;

    //�ر��ļ�
    outFile.close();


}
