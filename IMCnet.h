

#ifndef _IMC_NETDLL_H_
#define _IMC_NETDLL_H_

#ifdef iMCNET_EXPORTS
#define IMC_API __declspec(dllexport)
#else
#define IMC_API __declspec(dllimport)
#endif

#include "IMC_Def.h"

#ifdef __cplusplus
extern "C" {
#endif
	
#include "PackageFun.h"


//基础函数 
	
IMC_API IMC_STATUS WINAPI IMC_FindNetCard(NIC_INFO *info, int* cntOfcard);
IMC_API IMC_STATUS WINAPI IMC_Open(IMC_HANDLE* Handle, int netcardsel, int imcid);
IMC_API IMC_STATUS WINAPI IMC_OpenUsePassword( IMC_HANDLE* Handle, int netcardsel, int imcid, char* password);
IMC_API IMC_STATUS WINAPI IMC_Close(IMC_HANDLE Handle);


//******************************************************************************
IMC_API IMC_STATUS WINAPI IMC_SetMulParam(IMC_HANDLE Handle, pWR_MUL_DES pdes, int ArrNum, int sel);
IMC_API IMC_STATUS WINAPI IMC_SetParam16(IMC_HANDLE Handle, IMC16 paramloc, IMC16 thedata,IMC32 axis,int Sel);
IMC_API IMC_STATUS WINAPI IMC_SetParam32(IMC_HANDLE Handle, IMC16 paramloc, IMC32 thedata,IMC32 axis,int Sel);
IMC_API IMC_STATUS WINAPI IMC_SetParam48(IMC_HANDLE Handle, IMC16 paramloc, __int64 thedata,IMC32 axis,int Sel);

IMC_API IMC_STATUS WINAPI IMC_SetParamBit(IMC_HANDLE Handle, IMC16 paramloc, IMC16 bit, IMC16 val, IMC32 axis,int Sel);
IMC_API IMC_STATUS WINAPI IMC_TurnParamBit(IMC_HANDLE Handle, IMC16 paramloc, IMC16 bit, IMC32 axis,int Sel);
IMC_API IMC_STATUS WINAPI IMC_ORXORParam(IMC_HANDLE Handle, IMC16 paramloc, IMC16 ORdata, IMC16 XORdata, IMC32 axis,int Sel);

IMC_API IMC_STATUS WINAPI IMC_GetMulParam(IMC_HANDLE Handle, pWR_MUL_DES pdes,  WORD NumofArr);
IMC_API IMC_STATUS WINAPI IMC_GetParam16(IMC_HANDLE Handle, IMC16 paramloc, PVOID pdata, IMC32 axis);
IMC_API IMC_STATUS WINAPI IMC_GetParam32(IMC_HANDLE Handle, IMC16 paramloc, PVOID pdata, IMC32 axis);
IMC_API IMC_STATUS WINAPI IMC_GetParam48(IMC_HANDLE Handle, IMC16 paramloc, PVOID pdata, IMC32 axis);
IMC_API IMC_STATUS WINAPI IMC_GetParamBit(IMC_HANDLE Handle, IMC16 paramloc, IMC16* pdata, IMC16 bit, IMC32 axis);


IMC_API IMC_STATUS WINAPI IMC_PutContourData(IMC_HANDLE Handle, IMC16 *pdata, DWORD nWords, int Sel);
IMC_API IMC_STATUS WINAPI IMC_GetCapData(IMC_HANDLE Handle, IMC32 rdnum, IMC32 *pdata, IMC32 *dataNum, IMC32 *lastNum, IMC32 axis);


IMC_API IMC_STATUS WINAPI IMC_AddLineN(IMC_HANDLE Handle, PFIFOSegData* pdata, int sel);
IMC_API IMC_STATUS WINAPI IMC_AddArcLine(IMC_HANDLE Handle, PFIFOSegData* pdata, int sel);
IMC_API IMC_STATUS WINAPI IMC_AddLineNWithVel(IMC_HANDLE Handle, PFIFOSegInfo* pdata, int sel);
IMC_API IMC_STATUS WINAPI IMC_AddArcLineWithVel(IMC_HANDLE Handle, PFIFOSegInfo* pdat, int sel);
IMC_API IMC_STATUS WINAPI IMC_AddArc(IMC_HANDLE Handle, IMC32 Endx, IMC32 Endy, IMC32 centerX, IMC32 centerY, IMC32 dir, int sel);
IMC_API IMC_STATUS WINAPI IMC_AddArcWithVel(IMC_HANDLE Handle, IMC32 TgVel, IMC32 EndVel, 
										IMC32 Endx, IMC32 Endy, IMC32 centerX, IMC32 centerY, IMC32 dir, int sel);
IMC_API IMC_STATUS WINAPI IMC_AddSpiral(IMC_HANDLE Handle, IMC32 Endx, IMC32 Endy,IMC32 EndLine,  
										IMC32 centerX, IMC32 centerY, IMC32 dir, int sel);
IMC_API IMC_STATUS WINAPI IMC_AddSpiralWithVel(IMC_HANDLE Handle, IMC32 TgVel, IMC32 EndVel, 
										IMC32 Endx, IMC32 Endy, IMC32 EndLine, IMC32 centerX, IMC32 centerY, IMC32 dir, int sel);

IMC_API IMC_STATUS WINAPI IMC_InstallEvent(IMC_HANDLE Handle, PEventInfo pEvent, IMCU16 EventNum, int sel);

IMC_API IMC_STATUS WINAPI IMC_WaitParamBit(IMC_HANDLE Handle,IMC16 paramloc, IMC16 bit, IMC16 val, IMC32 timeout, IMC32 axis, int sel);
IMC_API IMC_STATUS WINAPI IMC_WaitTime(IMC_HANDLE Handle,IMC32 time, int sel);
IMC_API IMC_STATUS WINAPI IMC_WaitParam(IMC_HANDLE Handle,IMC16 paramloc, IMC16 data, IMC32 timeout, IMC32 axis, int sel);
IMC_API IMC_STATUS WINAPI IMC_WaitParamMask(IMC_HANDLE Handle,IMC16 paramloc, IMC16 mask, IMC16 data, IMC32 timeout, IMC32 axis, int sel);


//其他函数 
IMC_API IMC_STATUS WINAPI IMC_ClearFIFO(IMC_HANDLE Handle,int sel);


#ifdef __cplusplus
}
#endif








#endif