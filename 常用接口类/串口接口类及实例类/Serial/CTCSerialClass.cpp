// CTCSerialClass.cpp: implementation of the CCTCSerialClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CTCSerialClass.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define  WM_RECEIVE_LOCK_DATA  WM_USER+100

CCTCSerialClass::CCTCSerialClass(CWnd *pWnd,BYTE bFile)
{
	m_bSucess = false;
	m_pTargetWnd = pWnd;
	m_ListComAckBuf.clear();
	m_ListComInfoBuf.clear();
}

CCTCSerialClass::~CCTCSerialClass()
{
	TRACE(_T("CCTCSerialClass::~CCTCSerialClass()\n"));
}

const unsigned char /*code */auchCRCHi[] = {
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};
const unsigned char /*code*/ auchCRCLo[] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
	0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
	0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
	0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
	0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
	0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
	0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
	0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
	0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
	0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
	0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
	0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
	0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
	0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
	0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
	0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
	0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
	0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
	0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
	0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
	0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
	0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

///////////////////////////////////////////////////////////
//函数名：Modbus_crc16     
//功能： modbus代码校验      
//参数： 字符型指针或数组首地址，数据个数     
//返回值： crc数据     
//备注：在收到和发送的过程中都需要调用。  
///////////////////////////////////////////////////////////
unsigned int CCTCSerialClass::modbus_crc16(unsigned char *puchMsg,unsigned  int usDataLen)
{
	unsigned int uchCRCHi=0xFF;  /* 高CRC字节初始化 */ 
	unsigned int uchCRCLo=0xFF;  /* 低CRC 字节初始化 */ 
	unsigned long uIndex;         /* CRC循环中的索引 */ 
	while(usDataLen--)
	{
		uIndex=uchCRCHi^*puchMsg++;
		uchCRCHi=uchCRCLo^auchCRCHi[uIndex];
		uchCRCLo=auchCRCLo[uIndex];
	}
	return (unsigned int)(((unsigned int)uchCRCHi<<8) | uchCRCLo);
}

BOOL CCTCSerialClass::ProcessReceiveData( DWORD &dwDataLen )
{	
	int iPos = 0;
	BOOL bFindHead = FALSE;

	for( int i=0; i< dwDataLen; i++)
	{
		int iCount = 1;
		// 找到帧头
		//TRACE(_T("%x\t"),m_pbyReceiveBuffer[i]);
		if (m_pbyReceiveBuffer[i] == DATA_HEADER )
		{
			for (int j=i+1; j< dwDataLen; j++)
			{
				iCount++;
				//
				//TRACE(_T("%x\t"),m_pbyReceiveBuffer[j-1]);
				// 找到帧尾
				if (m_pbyReceiveBuffer[j] == DATA_TRAIL )
				{
					//
					//TRACE(_T("%x\t"),m_pbyReceiveBuffer[j]);
					LPBYTE pFrame = new BYTE[iCount];
	
					memcpy(pFrame, m_pbyReceiveBuffer + i, iCount);

					Parse(pFrame, iCount);

					delete[] pFrame;
					pFrame = NULL;
					iPos = j + 1;
					break;
				}
			}
			// 重置找到帧头标志
			bFindHead = TRUE;
			break;
		}
	}

	// 无帧头的错误数据
	if (!bFindHead)
	{
		return FALSE;
	}
	else
	{ 
		dwDataLen = dwDataLen - iPos;
		if ( dwDataLen < 0 )
		{
			dwDataLen = 0;
		}
		memmove(m_pbyReceiveBuffer, m_pbyReceiveBuffer + iPos, dwDataLen);

		if ( dwDataLen >= DATA_MIN_LENGTH )
		{
			//ProcessReceiveData( dwDataLen );
		}
		return TRUE;
	}
}

UINT SendAckThread(LPVOID pData)
{
	CCTCSerialClass* pSerial = (CCTCSerialClass*)pData;
	if (!pSerial->m_bSucess)
	{
		CAutoLock lock(pSerial->m_Sect);
/*		pSerial->SendAck();*/
	}
	return 0;
}

void CCTCSerialClass::Parse(BYTE* pInBuf, UINT nInLen)
{
	// 反转义帧数据
	nInLen = UnChange(pInBuf, nInLen);
	if (nInLen<26)
	{
		return ;
	}
	//先校验
	unsigned int modbus_crc_recevie; 
	modbus_crc_recevie = modbus_crc16(pInBuf ,nInLen-3);  //将生成的数据与接收到的数据进行对比来判断接收的数据是否正确
	WORD wCRC = 0;
	wCRC = MAKEWORD(pInBuf[nInLen-2],pInBuf[nInLen-3]);
	if(wCRC != modbus_crc_recevie)
	{
		TRACE("校验码错误!!!!\n");
		return;
	}
	else
	{
		CBusSocketPacket ComPacket;
		TRACE("校验码正确!!!!\n");
		//Frame Direction
		ComPacket.SetType(pInBuf[1]);
		//Control Code
		if (pInBuf[2] & 1)
		{
			//应答消息
// 			if (!m_bSucess)
// 			{
			//如果是车站发来的Info,则直接返回不处理
			if (pInBuf[1] == FROM_STATION)
			{
				return ;
			}
				if (pInBuf[2] == FRAME_POLL||pInBuf[2] == FRAME_ACK)
				{
					//poll Frame
					TRACE("SuperVision Frame(poll Frame)\n");
					ComPacket.SetComInfoType(FRAME_POLL);
					ComPacket.SetData((const char*)pInBuf,nInLen,ComPacket.GetType());

					CAutoLock lock(m_Sect);

					m_nHighValue = pInBuf[4];
					m_nLowValue = pInBuf[5];
					if (SendAck(ComPacket,nInLen))
					{
						m_bSucess = true;
					}

					if (m_PreAckdata==ComPacket)
					{
						ComPacket.Empty();
						TRACE("删除重复发送SuperVision Frame数据\n");
						return ;
					}
					m_PreAckdata.SetData(ComPacket.GetBuffer(), ComPacket.GetBufLen(), ComPacket.GetType());

//					TRACE("收到数据：	");
// 					for (int n = 0;n<nInLen ;n++)
// 					{
// 						TRACE(_T("%x\t"),pInBuf[n]);
// 					}
//					TRACE("\n");
				}
				else if (pInBuf[2] == FRAME_NAK)
				{
					//NAK
					ComPacket.SetComInfoType(FRAME_NAK);
					TRACE("SuperVision Frame(NAK Frame)\n");
				}
//			}
		}
		else
		{
			//如果是车站发来的Info,则直接返回不处理
			if (pInBuf[1] == FROM_STATION)
			{
				return ;
			}
			ComPacket.SetComInfoType(FRAME_INFO);
			TRACE("Info Frame\n");
			//信息
			int nTempLength = nInLen - 7;
			BYTE* TempBuf = new BYTE[nTempLength];
			memset(TempBuf,0,nTempLength);
			memcpy(TempBuf,pInBuf + 4,nTempLength);
			//只保留数据区
 			ComPacket.SetData((const char*)TempBuf,nTempLength,ComPacket.GetType());

			//delete
			delete []TempBuf;
			TempBuf = NULL;

			m_nHighValue = pInBuf[4];
			m_nLowValue = pInBuf[5];
			SendInfo(ComPacket,ComPacket.GetBufLen());

			if (m_PreInfoData==ComPacket)
			{
				ComPacket.Empty();
				TRACE("删除重复发送Info Frame数据\n");
				return ;
			}

			//m_PreInfoData.SetData(ComPacket.GetBuffer(), ComPacket.GetBufLen(), ComPacket.GetType());
			CAutoLock lock(m_Sect);
			m_ListComInfoBuf.push_back(ComPacket);
			
		}
	}
}

void CCTCSerialClass::SetPreData(CBusSocketPacket netPacket)
{
	m_PreInfoData.SetData(netPacket.GetBuffer(),netPacket.GetBufLen(),netPacket.GetType());
}
BOOL CCTCSerialClass::TransPortMsg(CBusSocketPacket netPacket,unsigned short nLen)
{
	BYTE pBuf[OUT_BUFFER_SIZE] = {0};
	memcpy(pBuf + 4,(BYTE*)netPacket.GetBuffer(),nLen);
	pBuf[0] = DATA_HEADER;
	pBuf[1] = TO_STATION;
	pBuf[2] = FRAME_INFO;
	pBuf[3] = 0xDD;

	unsigned int modbus_crc_send;
	modbus_crc_send = modbus_crc16(pBuf ,nLen + 4);             //生成数据后，将数据放在最后两个字节

	pBuf[nLen + 4] = HIBYTE(modbus_crc_send);
	pBuf[nLen + 5] = LOBYTE(modbus_crc_send);
	pBuf[nLen + 6] = DATA_TRAIL;

	nLen = Change(pBuf,nLen+7);

// 	for (int n = 0;n<nLen ;n++)
// 	{
// 		TRACE(_T("%x\t"),pBuf[n]);
// 	}
// 	TRACE("\n");
	CAutoLock lock(m_Sect);
	if (WriteCommBlock(pBuf,nLen))
	{
		TRACE("转发信息至车站--发送成功~！\n");
		
		return true;
	}
	else
	{
		TRACE("转发信息至车站--发送失败~！\n");
		
		return false;
	}
	return false;
}


bool CCTCSerialClass::SendAck(CBusSocketPacket netPacket,unsigned short nLen)
{
	if (netPacket.GetBufLen() > 0)
	{
		//ACK nlen是所有数据长度
		if (netPacket.GetType() == TO_STATION)
		{
			//多创建10个字节，为change做准备

			BYTE pBuf[OUT_BUFFER_SIZE] = {0};

			memcpy(pBuf,(BYTE*)netPacket.GetBuffer(),nLen);

			if (pBuf[1] == FROM_STATION)
			{
				pBuf[1] = TO_STATION;
			}
			else if(pBuf[1] == TO_STATION)
			{
				pBuf[1] = FROM_STATION;
			}

			//应答方式选择
			if (pBuf[2] == FRAME_POLL||pBuf[2] == FRAME_NAK)
			{
				pBuf[2] = FRAME_ACK;
			}
			pBuf[nLen - 3] = 0;
			pBuf[nLen - 2] = 0;
			pBuf[nLen - 1] = 0;
			unsigned int modbus_crc_send;
			modbus_crc_send = modbus_crc16(pBuf ,nLen - 3);             //生成数据后，将数据放在最后两个字节
			pBuf[nLen - 3] = HIBYTE(modbus_crc_send);
			pBuf[nLen - 2] = LOBYTE(modbus_crc_send);
			pBuf[nLen - 1] = DATA_TRAIL;

			nLen = Change(pBuf,nLen);
// 			TRACE("发送数据：	");
// 			for (int n = 0;n<nLen ;n++)
// 			{
// 				TRACE(_T("%x\t"),pBuf[n]);
// 			}
// 			TRACE("\n");
			CAutoLock lock(m_Sect);
			if (WriteCommBlock(pBuf,nLen))
			{
				TRACE("ACK应答--发送成功~！\n");
				return true;
			}
			else
			{
				TRACE("ACK应答--发送失败~！\n");
				return false;
			}
		}
		else if (netPacket.GetType() == FROM_STATION)
		{
			//不处理
		}
	}
	return true;
}

bool CCTCSerialClass::SendCmdToFcSystem(BYTE nCmd)
{
// 	if (m_bSucess)
// 	{
		int nLen = 26;
		CAutoLock lock(m_Sect);
		BYTE pBuf[OUT_BUFFER_SIZE] = {0};
		pBuf[0] = DATA_HEADER;
		pBuf[1] = FROM_STATION;
		pBuf[2] = FRAME_INFO;
		pBuf[3] = 0xDD;
		pBuf[4] = m_nHighValue;
		pBuf[5] = m_nLowValue;
		pBuf[6] = 0x03;
		pBuf[15] = 0x00;
		pBuf[16] = nCmd;
		pBuf[nLen - 3] = 0;
		pBuf[nLen - 2] = 0;
		pBuf[nLen - 1] = 0;
		unsigned int modbus_crc_send;
		modbus_crc_send = modbus_crc16(pBuf ,nLen - 3);             //生成数据后，将数据放在最后两个字节
		pBuf[nLen - 3] = HIBYTE(modbus_crc_send);
		pBuf[nLen - 2] = LOBYTE(modbus_crc_send);
		pBuf[nLen - 1] = DATA_TRAIL;

		nLen = Change(pBuf,nLen);

// 		for (int n = 0;n<nLen ;n++)
// 		{
// 			TRACE(_T("%x\t"),pBuf[n]);
// 		}
// 		TRACE("\n");
		if (WriteCommBlock((PBYTE)pBuf,nLen))
		{
			TRACE("请求事件--发送成功！\n");
			return true;
		}
		else
		{
			TRACE("请求事件--发送失败~！\n");
			return false;
		}
		return false;
//	}
}

bool CCTCSerialClass::SendInfo(CBusSocketPacket netPacket,unsigned short nLen)
{
	if (netPacket.GetType() == TO_STATION)
	{
		BYTE pBuf[OUT_BUFFER_SIZE] = {0};

		memcpy(pBuf+4,netPacket.GetBuffer(),nLen-7);

		pBuf[0] = DATA_HEADER;
		pBuf[1] = FROM_STATION;
		pBuf[2] = FRAME_ACK;
		pBuf[3] = 0xDD;
		unsigned int modbus_crc_send;
		modbus_crc_send = modbus_crc16(pBuf ,nLen + 4);             //生成数据后，将数据放在最后两个字节
		pBuf[nLen + 4] = HIBYTE(modbus_crc_send);
		pBuf[nLen + 5] = LOBYTE(modbus_crc_send);
		pBuf[nLen + 6] = DATA_TRAIL;

		nLen = Change(pBuf,nLen+7);

// 		for (int n = 0;n<nLen ;n++)
// 		{
// 			TRACE(_T("%x\t"),pBuf[n]);
// 		}
// 		TRACE("\n");
		//m_cs_SendCom.Lock();
		CAutoLock lock(m_Sect);
		if (WriteCommBlock(pBuf,nLen))
		{
			TRACE("消息应答发送成功~！\n");
			
			return true;
		}
		else
		{
			TRACE("发送失败~！");
			
			return false;
		}
	}
	return true;
}
// 字符转义操作
UINT CCTCSerialClass::Change(BYTE* pBuf, UINT nLen)
{
	for (int i = 1;i<nLen - 1;i++)
	{
		switch( pBuf[i] )
		{
		case 0xEB:
			{
				if (nLen>=OUT_BUFFER_SIZE)
				{
					return 0;
				}
				memmove(pBuf+i+1,pBuf+i,nLen-i);
				pBuf[i] = 0xEC;
				pBuf[i+1] = 0xEC;
				nLen++;
				i++;
				break;
			}
		case 0xEC:
			{
				if (nLen>=OUT_BUFFER_SIZE)
				{
					return 0;
				}
				memmove(pBuf+i+1,pBuf+i,nLen-i);
				pBuf[i] = 0xEC;
				pBuf[i+1] = 0xED;
				nLen++;
				i++;
				break;
			}
		default:
			break;
		}
	}
	return nLen;
}

// 字符反转义操作
UINT CCTCSerialClass::UnChange(BYTE* pBuf, UINT nLen)
{
	bool bChange = false;
	for( UINT i=1; i<nLen-1; i++ )
	{
		if( pBuf[i] == 0xEC )
		{
			if (i+1<nLen - 1)
			{
				if (pBuf[i+1] == 0xEC)
				{
					pBuf[i] = 0xEB;
					bChange = true;
				}
				else if (pBuf[i+1] == 0xED)
				{
					pBuf[i] = 0xEC;
					bChange = true;
				}
				memmove(pBuf+i+1,pBuf+i+2,nLen-i-2);
				nLen--;
			}
		}
	}
// 	if (bChange)
// 	{
// 		for (int n = 0;n<nLen ;n++)
// 		{
// 			TRACE(_T("%x\t"),pBuf[n]);
// 		}
// 	}
// 	TRACE("\n");
	return nLen;
}

bool CCTCSerialClass::Is_Validate_Packet( BYTE pkt_type )
{
	const WORD PacketType[15] ={ 0x12,0x13,0x06,0x15,0x10,
		                         0x65,0x6a,0x85,0x8a,0x95,
								 0x9a,0xa5,0xaa,0x75,0x7a};
	for ( size_t i=0; i<15; i++ )
	{
		if ( PacketType[i] == pkt_type )
		{
			return true;
		}
	}
	return false;
}
