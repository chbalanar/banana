// CTCSerialClass.h: interface for the CCTCSerialClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTCSERIALCLASS_H__60B35AC5_CD57_4142_B8E6_57A87C500261__INCLUDED_)
#define AFX_CTCSERIALCLASS_H__60B35AC5_CD57_4142_B8E6_57A87C500261__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AsynSerialDevice.h"
/*#include "./TCpack.h"*/
#include "../include//BusSocketPacket.h"
#include "../include/ThreadEx2.h"
/*#include "../include/LockData.h"*/
#include <list>
#define OUT_BUFFER_SIZE	1024
#define DATA_MIN_LENGTH 26
#define DATA_HEADER		0xEB        //帧头
#define DATA_TRAIL   	0xEB        //帧尾
////Frame Direction
#define		FROM_STATION	0x80
#define		TO_STATION		0x00
//Supervision Frame (收到不同的应答帧回复的方式相应变化)
#define		FRAME_POLL		0x01
#define		FRAME_NAK		0x09
#define		FRAME_ACK		0x05
//Infomation Frame
#define		FRAME_INFO		0x00
//数据区长度
#define		Data_Length		19
//////////Command Type
#define		Cmd_Activate			0x01
#define		Cmd_Deactivate			0x02
#define		Cmd_Enable				0x03
#define		Cmd_Disable				0x04
#define		Cmd_Restore				0x0C
#define		Cmd_Test				0x0A
#define		Cmd_Walk_Test			0x0B
#define		Cmd_Manual				0x05
#define		Cmd_Auto				0x06
#define		Cmd_Silence				0x07
#define		Cmd_Reset				0x08
#define		Cmd_Acknowledge_Event	0x30
#define		Cmd_Report_All_Events	0x31
#define		Cmd_Synchronize_Time	0x32
///////////Device Status
#define	Status_Alarm					0x0001
#define	Status_Trouble_Short			0x000C
#define	Status_Trouble_Open				0x0014
#define	Status_Trouble_Grounding		0x001C
#define	Status_Trouble_Communication	0x0024
#define	Status_Trouble_Overload			0x002C
#define	Status_Trouble_other			0x0034
#define	Status_Trouble_Parameter		0x0044
#define	Status_Active					0x0100
#define	Status_Active_And_Confirm		0x0300
#define	Status_Active_No_Confirm		0x0500
#define	Status_Confirm					0x0200
#define	Status_Disable					0x1000
#define	Status_Supervision_Active		0x2000
#define	Status_Normal					0x0000
//////////Device Mode
#define	Mode_Normal					0x0000
#define	Mode_Manual					0x0001
#define	Mode_Exchange_Level1		0x0002
#define	Mode_Exchange_Level2		0x0004
#define	Mode_Exchange_Level3		0x0006
#define	Mode_Test					0x0008
#define	Mode_Walk_test				0x0010
#define	Mode_Un_configure			0x0040
#define	Unknown						0x0080
#define	Mode_Type_Mismatch			0x0100
#define	Mode_Power_on				0x0200
#define	Mode_Un_configure_Normal	0x2000
#define	Mode_Mismatch_Normal		0x4000
#define	Mode_Exchange_Normal		0x8000

class CCTCSerialClass : public CAsynSerialDevice  
{
public:
	
	BOOL SendDataNew();
	BOOL TransPortMsg(CBusSocketPacket netPacket,unsigned short nLen);
	bool SendAck(CBusSocketPacket netPacket,unsigned short nLen);
	bool SendInfo(CBusSocketPacket netPacket,unsigned short nLen);
	void SetPreData(CBusSocketPacket netPacket);
	bool SendCmdToFcSystem(BYTE nCmd);
	CCTCSerialClass(CWnd *pWnd,BYTE bFile);
	virtual ~CCTCSerialClass();
	virtual BOOL ProcessReceiveData( DWORD &dwDataLen );
	unsigned int modbus_crc16(unsigned char *puchMsg,unsigned  int usDataLen);
protected:
	//PBYTE Change(PBYTE pSourseData, WORD &wLen);
	//PBYTE UnChange(PBYTE pSourseData, WORD wLen);
	//CTCpack m_CRC;
	CWnd * m_pTargetWnd;
	
	UINT Change(BYTE* pBuf, UINT nLen);
    UINT UnChange(BYTE* pBuf, UINT nLen);
	// 判断是否是通信控制帧
	bool Is_Com_Packet(BYTE pkt_header_type);

	void Parse(BYTE* pInBuf, UINT nInLen);

	bool Is_Validate_Packet( BYTE pkt_type );

public:
	typedef std::list<CBusSocketPacket>	LIST_COMBUF;

	LIST_COMBUF m_ListComAckBuf;

	LIST_COMBUF m_ListComInfoBuf;

	CCriticalSection  m_cs_SendCom;
	//CSectLock	m_lockRcvbuf;//

	BYTE m_nHighValue;
	BYTE m_nLowValue;

	CSectLock	m_Sect;

	bool m_bSucess;//握手成功
	CBusSocketPacket m_PreAckdata;//前一个数据包
	CBusSocketPacket m_PreInfoData;
};

#endif // !defined(AFX_CTCSERIALCLASS_H__60B35AC5_CD57_4142_B8E6_57A87C500261__INCLUDED_)
