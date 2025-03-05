

#ifndef _IMC_FUN_H_
#define _IMC_FUN_H_

IMC_API IMC_STATUS WINAPI IMC_MoveAbsolute (IMC_HANDLE Handle, IMC32 pos, IMC32 axis,int UseQ);
IMC_API IMC_STATUS WINAPI IMC_MoveRelative (IMC_HANDLE Handle, IMC32 dist, IMC32 axis,int UseQ);
IMC_API IMC_STATUS WINAPI IMC_MoveSuperimposed (IMC_HANDLE Handle, IMC32 dist, IMC32 axis,int UseQ);
IMC_API IMC_STATUS WINAPI IMC_Phasing (IMC_HANDLE Handle, IMC32 dist, IMC32 axis,int UseQ);
IMC_API IMC_STATUS WINAPI IMC_MoveVelocity (IMC_HANDLE Handle, IMC32 vel, IMC32 axis,int UseQ);
IMC_API IMC_STATUS WINAPI IMC_HomeSwitch1  (IMC_HANDLE Handle,IMC16 direction, IMC16 RiseEdge, IMC16 stop, IMC32 axis,int UseQ);
IMC_API IMC_STATUS WINAPI IMC_HomeSwitch2  (IMC_HANDLE Handle,IMC16 direction, IMC16 RiseEdge, IMC16 stop, IMC32 axis,int UseQ);
IMC_API IMC_STATUS WINAPI IMC_HomeSwitch3  (IMC_HANDLE Handle,IMC16 direction, IMC16 RiseEdge, IMC16 stop, IMC32 axis,int UseQ);
IMC_API IMC_STATUS WINAPI IMC_HomeSwitchIndex1 (IMC_HANDLE Handle,IMC16 direction, IMC16 RiseEdge, IMC16 stop, IMC32 axis,int UseQ);
IMC_API IMC_STATUS WINAPI IMC_HomeSwitchIndex2 (IMC_HANDLE Handle,IMC16 direction, IMC16 RiseEdge, IMC16 stop, IMC32 axis,int UseQ);
IMC_API IMC_STATUS WINAPI IMC_HomeSwitchIndex3 (IMC_HANDLE Handle,IMC16 direction, IMC16 RiseEdge, IMC16 stop, IMC32 axis,int UseQ);
IMC_API IMC_STATUS WINAPI IMC_HomeIndex  (IMC_HANDLE Handle,IMC16 direction, IMC16 stop, IMC32 axis,int UseQ);

IMC_API IMC_STATUS WINAPI IMC_GearFree	(IMC_HANDLE Handle, IMCFLOAT ratio, IMC32 master, IMC32 followtype, IMC32 axis,int UseQ);
IMC_API IMC_STATUS WINAPI IMC_GearInDist (IMC_HANDLE Handle, IMCFLOAT ratio, IMC32 master, IMC32 sourcetype, IMC32 synaxis,
										  IMC32 synsourcetype, IMC32 syndist,  IMC32 axis,int UseQ);
IMC_API IMC_STATUS WINAPI IMC_GearInTime (IMC_HANDLE Handle, IMCFLOAT ratio, IMC32 master, IMC32 sourcetype, IMC32 syntime , IMC32 axis,int UseQ);

IMC_API IMC_STATUS WINAPI IMC_HandWheel	(IMC_HANDLE Handle, IMCFLOAT ratio, IMC32 encsaxis, IMC32 axis,int UseQ);

IMC_API IMC_STATUS WINAPI IMC_TgvelInDist(IMC_HANDLE Handle, IMC32 refdist, IMC32 refaxis, IMC32 tgvel, IMC32 axis,int UseQ);
IMC_API IMC_STATUS WINAPI IMC_TgvelInTime(IMC_HANDLE Handle, IMC32 time,IMC32 tgvel, IMC32 axis,int UseQ);
IMC_API IMC_STATUS WINAPI IMC_MoveInDist (IMC_HANDLE Handle, IMC32 refdist, IMC32 refaxis, IMC32 movedist, IMC32 axis,int UseQ);
IMC_API IMC_STATUS WINAPI IMC_MoveInTime (IMC_HANDLE Handle, IMC32 time,IMC32 movedist, IMC32 axis,int UseQ);

#endif