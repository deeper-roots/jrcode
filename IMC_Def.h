
#ifndef _IMC_DEF_HEADER_
#define _IMC_DEF_HEADER_


#pragma once

typedef PVOID IMC_HANDLE;

typedef float IMCF32; //
typedef double IMCF64; //
//typedef float IMCFLOAT; //��ͬ��ϵͳ���岻ͬ���ȵĸ������ͣ�
typedef double IMCFLOAT; //��ͬ��ϵͳ���岻ͬ���ȵĸ������ͣ�
typedef char IMC8; //
typedef unsigned char IMCU8; //
typedef short IMC16; //
typedef unsigned short IMCU16; //
typedef long int IMC32; //
typedef unsigned long int IMCU32; //


//G����������
#define	GIMC_SUCCESS					0
#define	GIMC_INVALID_HANDLE				1	//��Ч�ľ��
#define	GIMC_INVALID_STRING_ADDRESS		2	//������ַ�����ַ��Ч
#define	GIMC_INVALID_AXIS				3	//��Ч�����
#define	GIMC_LACK_PARAM					4	//ȱ�ٲ���
#define	GIMC_ERROR_PARAM				5	//��������
#define	GIMC_ERROR_LINE					6	//�кŴ���
#define	GIMC_ERROR_FILE					7	//�ļ�����
#define	GIMC_NO_INIT					8	//û�г�ʼ��

enum IMC_STATUS {
	IMC_OK=0,				//�ɹ�
	IMC_SEND_FAIL,			//���ݷ���ʧ��
	IMC_READ_FAIL,			//���ݽ���ʧ��
	IMC_TIME_OUT,			//���ݷ��ͽ��ճ�ʱ
	IMC_DEVICE_NOT_OPEN,	//�豸û�д�
	IMC_DEVICE_NOT_FOUND,	//�豸û���ҵ�,���ڴ��豸ʱ
	IMC_INVALID_HANDLE,		//��Ч���豸���
	IMC_INVALID_PARAM,		//��Ч�Ĳ���
	IMC_INVALID_AXIS,		//��Ч�����
	IMC_INVALID_FIFO,		//��Ч��FIFO
	IMC_FIFO_FULL,			//FIFO��
	IMC_FIFO_NULL,			//FIFO��
	IMC_PARAM_READ_ONLY,	//ֻ������
	IMC_OUT_OF_RANGE,		//���ݽ����ĺ�������ֵ������Χ
	IMC_CHECK_ERROR,		//У�����
	IMC_VERSION_ERROR,		//������汾��Ӳ���汾��ƥ��
	IMC_OTHER_ERROR,		//��������ֵ
	IMC_PASSWORD_ERROR,		//�������
	IMC_RBFIFO_EMPTY,		//RBFIFO��
};

//
#define IMC_BIT(bit)	(1 << (bit))
//error�Ĵ�����bit��Ӧ�Ĵ�����Ϣ
#define IMC_ERR_PLIM 		IMC_BIT(0)	//(0x0001)����Ӳ����Խ��
#define IMC_ERR_NLIM 		IMC_BIT(1)	//(0x0002)����Ӳ����Խ��
#define IMC_ERR_PSOF 		IMC_BIT(2)	//(0x0004)��������Խ��
#define IMC_ERR_NSOF 		IMC_BIT(3)	//(0x0008)��������Խ��
#define IMC_ERR_STOP 		IMC_BIT(4)	//(0x0010)��ͣ����
#define IMC_ERR_ALM 		IMC_BIT(6)	//(0x0040)�ŷ����󱨾�
#define IMC_ERR_POSERRLIM 	IMC_BIT(7)	//(0x0080)λ������
#define IMC_ERR_TGPOSOV 	IMC_BIT(9)	//(0x0200)Ŀ��λ��Խ��
#define IMC_ERR_POSOV 		IMC_BIT(10)	//(0x0400)ָ��λ��Խ��
#define IMC_ERR_INSERR 		IMC_BIT(12)	//(0x1000)ָ�����
#define IMC_ERR_VELLIM 		IMC_BIT(13)	//(0x2000)�ٶȳ���
#define IMC_ERR_ACCLIM 		IMC_BIT(14)	//(0x4000)���ٶȳ���
#define IMC_ERR_DELPOS 		IMC_BIT(15)	//(0x8000)ָ��λ���쳣
//*******************************************************************************************************************
//������Ϣ
typedef struct {
	char description[16][256];	//��������
}NIC_INFO,*PNIC_INFO;

//*******************************************************************************************************************
typedef struct _WR_MUL_DES_{
	WORD addr;	//������ַ
	WORD axis;	//���
	WORD len;	//�������ȣ���λ���֣�16λ��, ��ֵΪ1��2��3�� 1:��ʾһ���֣�short�� 2:��ʾ2���֣�long) 3:��ʾ3���֣�����
	WORD data[4];//д����ص�����
}WR_MUL_DES, *pWR_MUL_DES;
//*******************************************************************************************************************
enum FIFO_SEL{
	SEL_IFIFO,
	SEL_QFIFO,
	SEL_PFIFO1,
	SEL_PFIFO2,
	SEL_CFIFO,
};

//�¼�����
enum IMC_EventType{
	IMC_Allways,		//��������ִ�С�

	IMC_Edge_Zero,		//������������ִ�С�������Ϊ0ʱ
	IMC_Edge_NotZero,	//������������ִ�С�������Ϊ��0ʱ
	IMC_Edge_Great, 	//������������ִ�С�������Ϊ����ʱ
 	IMC_Edge_GreatEqu, 	//������������ִ�С�������Ϊ���ڵ���ʱ
	IMC_Edge_Little,	//������������ִ�С�������ΪС��ʱ
	IMC_Edge_Carry,		//������������ִ�С�������Ϊ���ʱ
	IMC_Edge_NotCarry, 	//������������ִ�С�������Ϊ�����ʱ

	IMC_IF_Zero,		//����ƽ������ִ�С�������Ϊ0
	IMC_IF_NotZero, 	//����ƽ������ִ�С�������Ϊ��0
	IMC_IF_Great,		//����ƽ������ִ�С�����������
	IMC_IF_GreatEqu, 	//����ƽ������ִ�С����������ڵ���
	IMC_IF_Little, 		//����ƽ������ִ�С�������С��
	IMC_IF_Carry,		//����ƽ������ִ�С����������
	IMC_IF_NotCarry		//����ƽ������ִ�С������������
};

//�¼�ָ��
enum IMC_EVENT_CMD{
	CMD_ADD32,		//����32bit������� 
	CMD_ADD32i,		//32bit���� �� 32bit������
	CMD_ADD48,		//����48bit������� 
	CMD_ADD48i,		//48bit���� �� 48bit������
	CMD_CMP32,		//���� 32bit������Ƚ�
	CMD_CMP32i,		//32bit���� �� 32bit������ ��Ƚ�
	CMD_CMP48,		//���� 48bit������Ƚ�
	CMD_CMP48i,		//48bit���� �� 48bit������ ��Ƚ�
	CMD_SCA32,		//32bit�������ţ�����(48bit)Ϊ��һ����
	CMD_SCA32i,		//32bit�������ţ�����(48bit)Ϊ������
	CMD_SCA48,		//48bit�������ţ�����(48bit)Ϊ��һ����
	CMD_SCA48i,		//48bit�������ţ�����(48bit)Ϊ������
	CMD_MUL32L,		//32bit���� ���� 32bit���� ����ȡ�� 32bit
	CMD_MUL32iL,	//32bit���� ���� ������ ����ȡ�� 32bit
	CMD_MUL32A,		//32bit���� ���� 32bit���� ����ȡ�� 48bit
	CMD_MUL32iA,	//32bit���� ���� ������ ����ȡ�� 48bit
	CMD_COP16,		//���� 16bit����
	CMD_COP32,		//���� 32bit����
	CMD_COP48,		//���� 48bit����
	CMD_SET16,		//���� 16bit����
	CMD_SET32,		//���� 32bit����
	CMD_SET48,		//���� 48bit����
	CMD_OR16,		//���� OR ����
	CMD_OR16i,		//���� OR ������
	CMD_OR16B,		//���� OR ����  ����ת��ΪBOOL����
	CMD_OR16iB,		//���� OR ������  ����ת��ΪBOOL����
	CMD_AND16,		//���� AND ����
	CMD_AND16i,		//���� AND ������
	CMD_AND16B,		//���� AND ����  ����ת��ΪBOOL����
	CMD_AND16iB,	//���� AND ������  ����ת��ΪBOOL����
	CMD_XOR16,		//���� OR ����
	CMD_XOR16i,		//���� OR ������
	CMD_XOR16B,		//���� OR ���� ����ת��ΪBOOL����
	CMD_XOR16iB		//���� OR ������ ����ת��ΪBOOL����
};

typedef struct _EVENT_INFO_ {
	IMC16 EventCMD;  //�����룬��ö������IMC_EVENT_CMD�е�ֵ
	IMC16 EventType; //ִ�����ͣ���ö������IMC_EventType�е�ֵ
	IMC16 Src1_loc;	//ָ�������1�Ĳ�����ַ
	IMC16 Src1_axis;	//������1���������
	union{  			//��һ������������֯������2
		struct {
			IMC16 Src2_loc;	//ָ�������2�Ĳ�����ַ
			IMC16 Src2_axis;//������2���������
			IMC32 reserve;	//����
		}param;
		struct {
			IMC16 data;		//16λ��ĳ���
			IMC16 reserve1;	//����
			IMC32 reserve2;	//����
		}data16;
		struct {
			IMC32 data;		//32λ��ĳ���
			IMC32 reserve;	//����
		}data32;
		__int64 data48;		//48λ��ĳ���
	}Src2;
	IMC16 dest_loc;			//ָ��洢Ŀ��Ĳ�����ַ
	IMC16 dest_axis;		//Ŀ��������������
	IMC32 reserve;			//����
}EventInfo, *PEventInfo;

//*******************************************************************************************************************

typedef struct _PFIFO_SEG_DATA_{
	IMC32 datanum;			//SegEndData��������Ч���ݸ���,������·���˶�������
	IMC32 SegEndData[16];
	IMC32 CenterX;		//Բ��·��Բ�ĵĺ�����
	IMC32 CenterY;		//Բ��·��Բ�ĵ�������
	IMC32 dir;	//0:˳ʱ�뷽��  -1����ʱ�뷽��
}PFIFOSegData,*PPFIFOSegData;

typedef struct _PFIFO_SEG_INFO_{
	IMC32 SegTgVel;		//Ŀ���ٶ�
	IMC32 SegEndVel;	//�յ��ٶ�
	PFIFOSegData data;	//������
}PFIFOSegInfo, *PPFIFOSegInfo;
//*******************************************************************************************************************


#endif