// AsynSerialDevice.cpp : implementation file
//

#include "stdafx.h"
#include "AsynSerialDevice.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define    RECV_SERIAL_MAX_BUF        4096

/////////////////////////////////////////////////////////////////////////////
// CAsynSerialDevice

IMPLEMENT_DYNCREATE(CAsynSerialDevice, CWinThread)

CAsynSerialDevice::CAsynSerialDevice()
{
	m_TransMode = 0;
	m_bIsRun = FALSE;
	m_dwWatchEvents = 0;
	m_hPortHandle = INVALID_HANDLE_VALUE;
	m_bShowMessage = true;
	m_Over = FALSE;
	m_byPortNo = 0;
	m_pbyReceiveBuffer = NULL;
	m_dwReceiveBufSize = 0;

	m_OsRead.Offset      = 0;
	m_OsRead.OffsetHigh  = 0;
	m_OsWrite.Offset     = 0;
	m_OsWrite.OffsetHigh = 0;
	m_OsRead.hEvent      = CreateEvent(NULL,
                                   TRUE,
                                  FALSE,
                                   NULL );
    m_OsWrite.hEvent     = CreateEvent(NULL,
                                     TRUE,
                                     FALSE,
                                     NULL );

	SetReceiveBufferSize(RF_SERIAL_PORT_DEFAULT_RECEIVE_BUF_SIZE);
	
    //m_hOkInfo = CreateEvent(0,TRUE,FALSE,NULL);    //�����ֶ����ã���ʼ���źŵ��¼�
	

	m_OfferSet = 0;                                  //�����ڴ�Ŀռ�Ѱַ
	Offset = 0;           
	////������յ��ڴ�ռ�
	m_pRecvSerialBuf = (LPBYTE)new char[RECV_SERIAL_MAX_BUF];
	if(!m_pRecvSerialBuf)
	{
         AfxMessageBox("�������մ��ڵ��ڴ�ռ�ʧ��,�����ڴ�ռ��Ƿ��㹻!");
	}
    /****************************END*********************************************/
}

CAsynSerialDevice::~CAsynSerialDevice()
{
//	SuspendThread();
//	CWinThread(TRUE);
	StopThread();
	DWORD exit;
	GetExitCodeThread(m_hThread,&exit);	
	//TerminateThread(m_hThread,exit);
	TRACE(_T("End 1=====m_SerialDevice\n"));
	if (::WaitForSingleObject(m_hThread, 1000) == WAIT_FAILED)
	{
		TRACE("���� �̷߳������˳�!\n");
		::TerminateThread(m_hThread, 100);
	}	
    CloseHandle(m_OsRead.hEvent );
    CloseHandle(m_OsWrite.hEvent );
	//CloseHandle(m_hPortHandle);

    //CloseHandle(m_hOkInfo);
    if(m_pRecvSerialBuf)
	{
		delete []m_pRecvSerialBuf;
		m_pRecvSerialBuf = NULL;
	}
	if(m_pbyReceiveBuffer )
	{
		delete [] m_pbyReceiveBuffer;
		m_pbyReceiveBuffer=NULL;
	}
	m_Over = TRUE;
	TRACE(_T("CAsynSerialDevice::~CAsynSerialDevice()\n"));
    //ɾ���Զ����߳�
//	DWORD exitCode;
//	::GetExitCodeThread(m_hDealDataFun,&exitCode);
//	::TerminateThread(m_hDealDataFun,exitCode);
}

BOOL CAsynSerialDevice::InitInstance()
{
	//DWORD EvtMask;
	if (m_OsRead.hEvent == NULL)
	{
		m_csErrorMessage = "�����첽�������¼�ʧ�ܣ��߳̽�����";
		if (m_bShowMessage)
			AfxMessageBox(m_csErrorMessage);
		return FALSE;
	}
	if (m_OsWrite.hEvent == NULL)
	{
		m_csErrorMessage = "�����첽д�����¼�ʧ�ܣ��߳̽�����";
		if (m_bShowMessage)
			AfxMessageBox(m_csErrorMessage);
		return FALSE;
	}
	/*****************��������е�����****************************/
	if (!PurgeComm(m_hPortHandle,PURGE_RXCLEAR ))
	{
		DWORD dwError = GetLastError();
		m_csErrorMessage.Format( "�������ʧ��:%d", dwError );
		if (m_bShowMessage)
			AfxMessageBox(m_csErrorMessage);
	}
    if(m_TransMode == 0)
	{
		ReadSerialData();                           //һ����ʱ�Ľ��ն�����
	}
	else if(m_TransMode == 1)
	{
		ReadSerialData1();                          //���̶���ʱ�Ľ���
	}
	else if(m_TransMode ==2)
	{
		ReadSerialData2();                           //�����ݿ�ʱ�Ľ���
	}		
	else
	 {
		 AfxMessageBox("���ڴ���ģʽ����ʧ��");
	 }

	TRACE(_T("CAsynSerialDevice::InitInstance()------------------------1\n"));
	

	//CAsynSerialDevice::~CAsynSerialDevice();
	DWORD exit;	
	GetExitCodeThread(m_hThread,&exit);	
	//AfxEndThread(exit);
	ExitThread(exit);
	//delete this;
	//exit(0);
	
	TRACE(_T("CAsynSerialDevice::InitInstance()------------------------2\n"));
	return FALSE;
}
/***************************************************
*
*               ���ֿ鴮�ڵĽ���
*
****************************************************/
void CAsynSerialDevice::ReadSerialData()
{
    DWORD EvtMask =0;     
//	DWORD Offset=0;
	DWORD dwReceivedLen;
	DWORD BufSize;
	DWORD DataLen;
	

	while (!m_Over)
	{
		//		TRACE(_T("Start CAsynSerialDevice::ReadSerialData() === run \n"));
		if (m_hPortHandle != INVALID_HANDLE_VALUE)
		{
			if (WaitCommEvent(m_hPortHandle, &EvtMask, NULL))
			{
				//AfxMessageBox("�յ���������\n");
				if ((EvtMask & EV_RXCHAR) == EV_RXCHAR)
				{
					BufSize = m_dwReceiveBufSize - Offset;
					dwReceivedLen = ReadCommBuf(m_pbyReceiveBuffer + Offset, BufSize);

					//��������
					if (dwReceivedLen > 0)
					{
						// ��ǰ�������е����ݳ���
						Offset = Offset + dwReceivedLen;

						// ����ԭ�����ݳ���
						DataLen = Offset;
						// ���ݴ������û������е�����

  						if ( !ProcessReceiveData( Offset ))
						{
							// ����������뻺��������������
							TRACE("���û���������\n");
							if (!PurgeComm( m_hPortHandle,PURGE_RXCLEAR ))
							{
								DWORD dwError = GetLastError();
								m_csErrorMessage.Format( "�������ʧ��:%d", dwError );
								if (m_bShowMessage)
								{
									AfxMessageBox(m_csErrorMessage);
								}
							}
							
							// �������ݽ��ջ�����
							Offset = 0;
						    memset(m_pbyReceiveBuffer, 0, m_dwReceiveBufSize);
						}
					}
				}
				else if ((EvtMask & EV_RX80FULL) == EV_RX80FULL)
				{
				}
				else if ((EvtMask & EV_BREAK) == EV_BREAK)
				{
				}
				else if ((EvtMask & EV_CTS) == EV_CTS)
				{
				}
				else if ((EvtMask & EV_DSR) == EV_DSR)
				{
				}
				else if ((EvtMask & EV_RLSD) == EV_RLSD)
				{
				}
				else if ((EvtMask & EV_RXFLAG) == EV_RXFLAG)
				{
				}
				else if ((EvtMask & EV_TXEMPTY) == EV_TXEMPTY)
				{
				}
				else if ((EvtMask & EV_ERR) == EV_ERR)
				{
				}
				else if ((EvtMask & EV_RLSD) == EV_RLSD)
				{
				}
				if (m_Over)
				{
					break;
				}
				
			}
			//TRACE( _T("Run CAsynSerialDevice::ReadSerialData() m_Over = %d\n"), m_Over);
		}
		else
		{
			if( !m_Over )
			{
				while ((!m_Over) && (m_hPortHandle == INVALID_HANDLE_VALUE) && (SuspendThread() <= 0));
			}
			else
			{
				TRACE( _T("�˳� CAsynSerialDevice::ReadSerialData() \n"));
				break;
			}
		}
		
		if (m_Over)
		{
			TRACE( _T("�˳� CAsynSerialDevice::ReadSerialData() \n"));
			break;
		}
//		TRACE(_T("End CAsynSerialDevice::ReadSerialData() === run \n"));


	}
	TRACE( _T("�˳� CAsynSerialDevice::ReadSerialData() \n"));
}
/***************************************************
*
*            �̶ֹ���ĵĽ���
*
****************************************************/

void CAsynSerialDevice::ReadSerialData1()
{
    DWORD EvtMask;     
	   
	//�Լ����Ŀ��ƽ��ջ������ı���
	DWORD RecBufLen;	
	while (TRUE)
	{
		if (m_hPortHandle != INVALID_HANDLE_VALUE)
		{
			if (WaitCommEvent(m_hPortHandle, &EvtMask, NULL))
			{
				if ((EvtMask & EV_RXCHAR) == EV_RXCHAR)
				{
					//��������					
					//RecBufLen = ReadCommBuf(RecBuf + Offset1,RecBufLenght);
					Sleep(100);
                    m_pRecvSerialBufKey.Lock();

					RecBufLen = ReadCommBuf(m_pRecvSerialBuf + m_OfferSet,RECV_SERIAL_MAX_BUF);
					m_OfferSet += RecBufLen;

					m_pRecvSerialBufKey.Unlock();                    
				}
				if (m_Over)
					break;
				
			}
		}
		else if (m_Over)
			break;
		else
		{
			while ((!m_Over) && (m_hPortHandle == INVALID_HANDLE_VALUE) && (SuspendThread() <= 0));
		}
	}
//	delete RecBuf;
}
/************************************
*
*   //�����ݰ��ֿ鷽ʽ�Ľ���
*
*************************************/
void CAsynSerialDevice::ReadSerialData2()
{
    DWORD EvtMask;     
	   
	//�Լ����Ŀ��ƽ��ջ������ı���
	DWORD RecBufLen;	
	while (TRUE)
	{
		if (m_hPortHandle != INVALID_HANDLE_VALUE)
		{
			if (WaitCommEvent(m_hPortHandle, &EvtMask, NULL))
			{
				if ((EvtMask & EV_RXCHAR) == EV_RXCHAR)
				{
					//��������					
					//RecBufLen = ReadCommBuf(RecBuf + Offset1,RecBufLenght);
                    m_pRecvSerialBufKey.Lock();

					RecBufLen = ReadCommBuf(m_pRecvSerialBuf + m_OfferSet,RECV_SERIAL_MAX_BUF);
					m_OfferSet += RecBufLen;

					m_pRecvSerialBufKey.Unlock();                    
				}
				if (m_Over)
					break;
			}
		}
		else if (m_Over)
			break;
		else
		{
			while ((!m_Over) && (m_hPortHandle == INVALID_HANDLE_VALUE) && (SuspendThread() <= 0));
		}
	}
}

int CAsynSerialDevice::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CAsynSerialDevice, CWinThread)
	//{{AFX_MSG_MAP(CAsynSerialDevice)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAsynSerialDevice message handlers
BOOL CAsynSerialDevice::EnableEVCTS(BOOL Enable)
{
	if ((m_dwWatchEvents & EV_CTS == EV_CTS) && !Enable)
	{
		m_dwWatchEvents ^= EV_CTS;
		if (m_hPortHandle != (HANDLE) - 1)
			if (SetCommMask(m_hPortHandle, m_dwWatchEvents))
				return TRUE;
			else
			{
				m_csErrorMessage = "ȡ��CTS�¼����Ӵ���";
				if (m_bShowMessage)
					AfxMessageBox(m_csErrorMessage);
				return FALSE;
			}
		else
			return TRUE;
	}
	if (!(m_dwWatchEvents & EV_CTS) && Enable)
	{
		m_dwWatchEvents |= EV_CTS;
		if (m_hPortHandle != (HANDLE) - 1)
			if (SetCommMask(m_hPortHandle, m_dwWatchEvents))
				return TRUE;
			else
			{
				m_csErrorMessage = "����CTS�¼����Ӵ���";
				if (m_bShowMessage)
					AfxMessageBox(m_csErrorMessage);
				return FALSE;
			}
		else
			return TRUE;
	}
	return TRUE;
}
BOOL CAsynSerialDevice::EnableEVDSR(BOOL Enable)
{
	if ((m_dwWatchEvents & EV_DSR == EV_DSR) && !Enable)
	{
		m_dwWatchEvents ^= EV_DSR;
		if (m_hPortHandle != (HANDLE) - 1)
			if (SetCommMask(m_hPortHandle, m_dwWatchEvents))
				return TRUE;
			else
			{
				m_csErrorMessage = "ȡ��DSR�¼�����";
				if (m_bShowMessage)
					AfxMessageBox(m_csErrorMessage);
				return FALSE;
			}
		else
			return TRUE;
	}
	if (!(m_dwWatchEvents & EV_DSR) && Enable)
	{
		m_dwWatchEvents |= EV_DSR;
		if (m_hPortHandle != (HANDLE) - 1)
			if (SetCommMask(m_hPortHandle, m_dwWatchEvents))
				return TRUE;
			else
			{
				m_csErrorMessage = "����DSR�¼�����";
				if (m_bShowMessage)
					AfxMessageBox(m_csErrorMessage);
				return FALSE;
			}
		else
			return TRUE;
	}
	return TRUE;
}
BOOL CAsynSerialDevice::EnableEVRXCHAR(BOOL Enable)
{
	if ((m_dwWatchEvents & EV_RXCHAR == EV_RXCHAR) && !Enable)
	{
		m_dwWatchEvents ^= EV_RXCHAR;
		if (m_hPortHandle != (HANDLE) - 1)
			if (SetCommMask(m_hPortHandle, m_dwWatchEvents))
				return TRUE;
			else
			{
				m_csErrorMessage = "ȡ��RXCHAR�¼�����";
				if (m_bShowMessage)
					AfxMessageBox(m_csErrorMessage);
				return FALSE;
			}
		else
			return TRUE;
	}
	if (!(m_dwWatchEvents & EV_RXCHAR) && Enable)
	{
		m_dwWatchEvents |= EV_RXCHAR;
		if (m_hPortHandle != (HANDLE) - 1)
			if (SetCommMask(m_hPortHandle, m_dwWatchEvents))
				return TRUE;
			else
			{
				m_csErrorMessage = "����RXCHAR�¼�����";
				if (m_bShowMessage)
					AfxMessageBox(m_csErrorMessage);
				return FALSE;
			}
		else
			return TRUE;
	}
	return TRUE;
}
BOOL CAsynSerialDevice::EnableEVBREAK(BOOL Enable)
{
	if ((m_dwWatchEvents & EV_BREAK == EV_BREAK) && !Enable)
	{
		m_dwWatchEvents ^= EV_BREAK;
		if (m_hPortHandle != (HANDLE) - 1)
			if (SetCommMask(m_hPortHandle, m_dwWatchEvents))
				return TRUE;
			else
			{
				m_csErrorMessage = "ȡ��BREAK�¼�����";
				if (m_bShowMessage)
					AfxMessageBox(m_csErrorMessage);
				return FALSE;
			}
		else
			return TRUE;
	}
	if (!(m_dwWatchEvents & EV_BREAK) && Enable)
	{
		m_dwWatchEvents |= EV_BREAK;
		if (m_hPortHandle != (HANDLE) - 1)
			if (SetCommMask(m_hPortHandle, m_dwWatchEvents))
				return TRUE;
			else
			{
				m_csErrorMessage = "����BREAK�¼�����";
				if (m_bShowMessage)
					AfxMessageBox(m_csErrorMessage);
				return FALSE;
			}
		else
			return TRUE;
	}
	return TRUE;
}

BOOL CAsynSerialDevice::EnableRX80FULL(BOOL Enable)
{
	if ((m_dwWatchEvents & EV_RX80FULL == EV_RX80FULL) && !Enable)
	{
		m_dwWatchEvents ^= EV_RX80FULL;
		if (m_hPortHandle != (HANDLE) - 1)
			if (SetCommMask(m_hPortHandle, m_dwWatchEvents))
				return TRUE;
			else
			{
				m_csErrorMessage = "ȡ��RX80FULL�¼�����";
				if (m_bShowMessage)
					AfxMessageBox(m_csErrorMessage);
				return FALSE;
			}
		else
			return TRUE;
	}
	if (!(m_dwWatchEvents & EV_RX80FULL) && Enable)
	{
		m_dwWatchEvents |= EV_RX80FULL;
		if (m_hPortHandle != (HANDLE) - 1)
			if (SetCommMask(m_hPortHandle, m_dwWatchEvents))
				return TRUE;
			else
			{
				m_csErrorMessage = "����RX80FULL�¼�����";
				if (m_bShowMessage)
					AfxMessageBox(m_csErrorMessage);
				return FALSE;
			}
		else
			return TRUE;
	}
	return TRUE;
}


BOOL CAsynSerialDevice::CloseComPort(BOOL Over)
{
	BOOL PreOver = m_Over;
	m_Over = Over;

    if (m_hPortHandle != INVALID_HANDLE_VALUE)
	{
		if (!PurgeComm(m_hPortHandle,PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR ))
		{
			m_csErrorMessage = "�������ʧ��";
			if (m_bShowMessage)
				AfxMessageBox(m_csErrorMessage);
		}
		if (!CloseHandle(m_hPortHandle))
		{
			m_csErrorMessage = "�رմ��ھ��ʧ��";
			if (m_bShowMessage)
				AfxMessageBox(m_csErrorMessage);
			return FALSE;
		}
		else
		{
			TRACE(_T("CAsynSerialDevice::CloseComPort CloseHandle m_hPortHandle\n"));
		}
		m_hPortHandle =  INVALID_HANDLE_VALUE;
		m_byPortNo = 0;
		m_dwWatchEvents = 0;
	}

	//�߳�û��������ֱ�ӷ���
	if(!m_bIsRun)
		return TRUE;
	if (Over)
	{
		m_hPortHandle =  INVALID_HANDLE_VALUE;
		if (!PreOver)
		{
			while (ResumeThread() > 1);
		}
	}
	else
	{
		while (SuspendThread() <= 0);
	}

	Sleep(1000);
	

	TRACE(_T("CAsynSerialDevice::CloseComPort(BOOL Over)\n"));	
	return TRUE;
}


BOOL CAsynSerialDevice::SetDcbBaByPaSt(DWORD BaudRate, BYTE ByteSize, BYTE StopBits, BYTE Parity)
{
	m_Dcb.DCBlength = sizeof(DCB);
  if (!GetCommState(m_hPortHandle, &m_Dcb ))		//���ж��IOCTL��Ӧ
	{

		m_csErrorMessage = "������DCB���ô���";
		if (m_bShowMessage)
			AfxMessageBox(m_csErrorMessage);
		return FALSE;
	}
  m_Dcb.fBinary = TRUE;
	m_Dcb.BaudRate = BaudRate;
	m_Dcb.ByteSize = ByteSize;
	m_Dcb.StopBits = StopBits;
	m_Dcb.Parity = Parity;
	if (Parity > 0)
		m_Dcb.fParity = TRUE;
	else
		m_Dcb.fParity = FALSE;
	if (!SetCommState(m_hPortHandle, &m_Dcb))	//����֮ǰ��Ҫ��ȡ
	{
		DWORD dwError = GetLastError();
		m_csErrorMessage.Format("���ô���DCB����! GetLastError() return %d", dwError );
		if (m_bShowMessage)
			AfxMessageBox(m_csErrorMessage);
		return FALSE;
	}
	return TRUE;
}

BOOL CAsynSerialDevice::SetInOutBufSize(DWORD ReceiveSize, DWORD SendSize)
{
	if (SetupComm(m_hPortHandle, ReceiveSize, SendSize))
		return TRUE;
	else
	{
		m_csErrorMessage = "���ô��ڻ�������������";
		if (m_bShowMessage)
			AfxMessageBox(m_csErrorMessage);
		return FALSE;
	}
}
BOOL CAsynSerialDevice::SetTimeOut(DWORD ReadIntervalTimeout, DWORD ReadTotalTimeoutMultiplier, DWORD ReadTotalTimeoutConstant, DWORD WriteTotalTimeoutMultiplier, DWORD WriteTotalTimeoutConstant)
{
	m_CommTimeOuts.ReadIntervalTimeout = ReadIntervalTimeout;
	m_CommTimeOuts.ReadTotalTimeoutMultiplier = ReadTotalTimeoutMultiplier;
	m_CommTimeOuts.ReadTotalTimeoutConstant = ReadTotalTimeoutConstant;
	m_CommTimeOuts.WriteTotalTimeoutMultiplier = WriteTotalTimeoutMultiplier;
	m_CommTimeOuts.WriteTotalTimeoutConstant = WriteTotalTimeoutConstant;
	if (SetCommTimeouts(m_hPortHandle, &m_CommTimeOuts))
		return TRUE;
	else
	{
		m_csErrorMessage = "���ô���ʱ���������";
		if (m_bShowMessage)
			AfxMessageBox(m_csErrorMessage);
		return FALSE;
	}
	
}

BOOL CAsynSerialDevice::SetCharFlowControl(BOOL OutX, BOOL InX, BOOL TXContinueOnXoff, char XonChar, char XoffChar, WORD XonLim, WORD XoffLim)
{
	m_Dcb.DCBlength = sizeof(DCB);
    if (!GetCommState(m_hPortHandle, &m_Dcb ))
	{
		m_csErrorMessage = "������DCB���ô���";
		if (m_bShowMessage)
			AfxMessageBox(m_csErrorMessage);
		return FALSE;
	}
	m_Dcb.fOutX = OutX;
	m_Dcb.fInX = InX;
	m_Dcb.fTXContinueOnXoff = TXContinueOnXoff;
	m_Dcb.XonChar = XonChar;
	m_Dcb.XoffChar = XoffChar;
	m_Dcb.XonLim = XonLim;
	m_Dcb.XoffLim = XoffLim;
	if (!SetCommState(m_hPortHandle, &m_Dcb))
	{
		m_csErrorMessage = "���ô���DCB����";
		if (m_bShowMessage)
			AfxMessageBox(m_csErrorMessage);
		return FALSE;
	}
	return TRUE;
}


BOOL CAsynSerialDevice::SetWireFlowControl(BOOL OutxCtsFlow, BOOL OutxDsrFlow, DWORD DtrControl, BOOL DsrSensitivity, DWORD RtsControl)
{
	m_Dcb.DCBlength = sizeof(DCB);
    if (!GetCommState(m_hPortHandle, &m_Dcb ))
	{
		m_csErrorMessage = "������DCB���ô���";
		if (m_bShowMessage)
			AfxMessageBox(m_csErrorMessage);
		return FALSE;
	}
	m_Dcb.fOutxCtsFlow = OutxCtsFlow;
	m_Dcb.fOutxDsrFlow = OutxDsrFlow;
	m_Dcb.fDtrControl = DtrControl;
	m_Dcb.fDsrSensitivity = DsrSensitivity;
	m_Dcb.fRtsControl = RtsControl;
	if (!SetCommState(m_hPortHandle, &m_Dcb))
	{
		m_csErrorMessage = "���ô���DCB����";
		if (m_bShowMessage)
			AfxMessageBox(m_csErrorMessage);
		return FALSE;
	}
	return TRUE;
}
BOOL CAsynSerialDevice::SetError(BOOL Parity, BOOL fErrorChar, BOOL AbortOnError, char ErrorChar)
{
	m_Dcb.DCBlength = sizeof(DCB);
    if (!GetCommState(m_hPortHandle, &m_Dcb ))
	{
		m_csErrorMessage = "������DCB���ô���";
		if (m_bShowMessage)
			AfxMessageBox(m_csErrorMessage);
		return FALSE;
	}
	m_Dcb.fParity = Parity;
	m_Dcb.fErrorChar = fErrorChar;
	m_Dcb.fAbortOnError = AbortOnError;
	m_Dcb.ErrorChar = ErrorChar;
	if (!SetCommState(m_hPortHandle, &m_Dcb))
	{
		m_csErrorMessage = "���ô���DCB����";
		if (m_bShowMessage)
			AfxMessageBox(m_csErrorMessage);
		return FALSE;
	}
	return TRUE;
}
BOOL CAsynSerialDevice::SetRecChar(BOOL fNull, char EofChar, char EvtChar)
{
	m_Dcb.DCBlength = sizeof(DCB);
    if (!GetCommState(m_hPortHandle, &m_Dcb ))
	{
		m_csErrorMessage = "������DCB���ô���";
		if (m_bShowMessage)
			AfxMessageBox(m_csErrorMessage);
		return FALSE;
	}
	m_Dcb.fNull = fNull;
	m_Dcb.EofChar = EofChar;
	m_Dcb.EvtChar = EvtChar;
	if (!SetCommState(m_hPortHandle, &m_Dcb))
	{
		m_csErrorMessage = "���ô���DCB����";
		if (m_bShowMessage)
			AfxMessageBox(m_csErrorMessage);
		return FALSE;
	}
	return TRUE;
}


BOOL CAsynSerialDevice::OpenComPort(BYTE PortNo)
{
	if (m_byPortNo == PortNo && m_hPortHandle != INVALID_HANDLE_VALUE)
		return TRUE;
	if (!CloseComPort(FALSE))
		return FALSE;

	if (!OpenPortFile(PortNo))
		return FALSE;
	if (!EnableEVCTS(TRUE))		//IOCTL_SERIAL_SET_WAIT_MASK
	{
		while (ResumeThread() > 1);
		return FALSE;
	}
	if (!EnableEVRXCHAR(TRUE))  //IOCTL_SERIAL_SET_WAIT_MASK
	{
		while (ResumeThread() > 1);
		return FALSE;
	}
	/*
	if (!SetInOutBufSize(8 * 1024, 8 * 1024))	//18(�ڲ����ܺͷ��ͳߴ�)
	{
		while (ResumeThread() > 1);
		return FALSE;
	}
	*/
//	if (!SetTimeOut())			//5
//	{
//		while (ResumeThread() > 1);
//		return FALSE;
//	}
	if (!SetDcbBaByPaSt())
	{
		while (ResumeThread() > 1);
		return FALSE;
	}
//	if (!SetWireFlowControl())
//	{
//		while (ResumeThread() > 1);
//		return FALSE;
//	}
//	if (!SetCharFlowControl())
//	{
//		while (ResumeThread() > 1);
//		return FALSE;
//	}
//	if (!SetError())
//	{
//		while (ResumeThread() > 1);
//		return FALSE;
//	}
//	if (!SetRecChar())
//	{
//		while (ResumeThread() > 1);
//		return FALSE;
//	}
	m_byPortNo = PortNo;
	//while (ResumeThread() > 1);
	return TRUE;
}


BOOL CAsynSerialDevice::OpenPortFile(BYTE PortNo)
{
	char          Port[ 15 ];
    wsprintf(Port, "\\\\.\\COM%d", PortNo);
	m_hPortHandle = CreateFile(Port,
	 				    GENERIC_READ | GENERIC_WRITE,
						0,
		                NULL,
		                OPEN_EXISTING,
		                FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
						NULL);
	DWORD dw = GetLastError();
	if (m_hPortHandle == INVALID_HANDLE_VALUE)
	{
		m_csErrorMessage = "���ڱ�ռ�û��޴˴���";
		if (m_bShowMessage)
			AfxMessageBox(m_csErrorMessage);
		return FALSE;
    }
	return TRUE;
}


DWORD CAsynSerialDevice::ReadCommBuf(PBYTE Buf, DWORD BufSize)
{
	BOOL       FreadStat ;
	COMSTAT    ComStat ;
	DWORD      dwErrorFlags;
	DWORD      dwError;
	DWORD      dwLength;

	ClearCommError(m_hPortHandle, &dwErrorFlags, &ComStat ) ;
	dwLength = min(BufSize, ComStat.cbInQue);
		                                 

	if (dwLength > 0)		   
	{
		FreadStat = ReadFile(m_hPortHandle, Buf,
		                      dwLength, &dwLength, &m_OsRead ) ;
		if (!FreadStat)
		{
			DWORD  RET = GetLastError();
			if (RET == ERROR_IO_PENDING)
			{

				while(!GetOverlappedResult(m_hPortHandle,
					&m_OsRead, &dwLength, TRUE))
				{
					dwError = GetLastError();
					if(dwError == ERROR_IO_INCOMPLETE)
					{
						continue;
					}
					else
					{
						ClearCommError(m_hPortHandle, &dwErrorFlags, &ComStat ) ;
						break;
					}						
				}					
			}
			else
			{
			    dwLength = 0 ;
				ClearCommError(m_hPortHandle, &dwErrorFlags, &ComStat ) ;
			}

		}
		if (!FreadStat)
		{
            dwLength = DWORD(-1);
		}
	}
	return (dwLength);
}



int CAsynSerialDevice::WriteCommBlock1(BYTE *pBlock,int nBlockLen)
{  //return 0 if error
	DWORD dwBytesWritten;
	DWORD dwModemStat;
	int   WaitErr;
	BOOL  fWriteStat;

/*	if (!m_bConnected) return 0;*/
	dwBytesWritten=0;
	fWriteStat=WriteFile(m_hPortHandle,pBlock,nBlockLen,&dwBytesWritten,&m_OsWrite);
	if(!fWriteStat) 
	{   // Writting wait
		if (GetLastError()==ERROR_IO_PENDING)
		{  // if writting processing
			//???        ResetEvent(m_OsWrite.hEvent);//???
			if (WaitErr=WaitForSingleObject(m_OsWrite.hEvent,60000)) // 1 minute
			{   // WaitErr!=0
				if(WaitErr==WAIT_TIMEOUT) 
				{  // time out
					//KillTimer(1);
					// if device error! 
					GetCommModemStatus(m_hPortHandle,&dwModemStat); 
					if(dwModemStat &= MS_CTS_ON) // if (CTS_ON && TIME_OUT) then COMM Hardware Errors.
					{ // 16 "GREEN"
						AfxMessageBox("COMM Device hardware errors.",MB_ICONSTOP);// don't move
						//m_bLoadStop=1;
						dwBytesWritten=0;
					}
					else
					{ // 0 "RED"
						AfxMessageBox("CTS timer 60 seconds out!",MB_ICONSTOP);
						//m_bLoadStop=1;
						dwBytesWritten=0;
					}
				}
				//                if(WaitErr==WAIT_FAILED) =-1
				//                else other waiterr
			}
			else
			{ // WaitErr=0.if you SetEvent(m_OsWrite.hEvent) anywhere else
				GetOverlappedResult(m_hPortHandle,&m_OsWrite,&dwBytesWritten,FALSE);
				m_OsWrite.Offset +=dwBytesWritten;
			}
		}
		//        else  // not IO_PENDING
	}
	return (int)dwBytesWritten;
}


BOOL CAsynSerialDevice::WriteBuffer(PBYTE lpBuf, DWORD dwToWrite)
{
	//OVERLAPPED m_OsWrite = {0};
	DWORD dwWritten;
	DWORD dwRes;
	BOOL fRes;

	// Create this write operation's OVERLAPPED structure's hEvent.
	/*m_OsWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);*/
	if (m_OsWrite.hEvent == NULL)
		// error creating overlapped event handle
		return FALSE;
	// Issue write.
	if (!WriteFile(m_hPortHandle, lpBuf, dwToWrite, &dwWritten, &m_OsWrite))
	{
		if (GetLastError() != ERROR_IO_PENDING)
		{ 
			// WriteFile failed, but isn't delayed. Report error and abort.
			fRes = FALSE;
		}
		else
			// Write is pending.
			dwRes = WaitForSingleObject(m_OsWrite.hEvent, INFINITE);

		switch(dwRes)
		{
			// OVERLAPPED structure's event has been signaled. 
		case WAIT_OBJECT_0:
			if (!GetOverlappedResult(m_hPortHandle, &m_OsWrite, &dwWritten, FALSE))
				fRes = FALSE;
			else
				// Write operation completed successfully.
				fRes = TRUE;
			break;
		default:
			// An error has occurred in WaitForSingleObject.
			// This usually indicates a problem with the
			// OVERLAPPED structure's event handle.
			fRes = FALSE;
			break;
		}
	}
	else
		// WriteFile completed immediately.
		fRes = TRUE;

	CloseHandle(m_OsWrite.hEvent);
	return fRes;
}

BOOL CAsynSerialDevice::WriteCommBlock(PBYTE lpByte, DWORD dwBytesToWrite)
{
	BOOL        fWriteStat;
	DWORD       dwBytesWritten;
	DWORD       dwErrorFlags;
	DWORD     	dwError;
	COMSTAT     ComStat;

	fWriteStat = WriteFile(m_hPortHandle,
						   lpByte,
						   dwBytesToWrite,
	                       &dwBytesWritten,
						   &m_OsWrite);
	if (!fWriteStat) 
	{
		dwError = GetLastError();
		if(dwError == ERROR_IO_PENDING)
		{
			while(!GetOverlappedResult(m_hPortHandle,&m_OsWrite, &dwBytesWritten, TRUE ))
			{																	
				dwError = GetLastError();
				if(dwError == ERROR_IO_INCOMPLETE)
				{
					continue;
				}
				else
				{
					ClearCommError(m_hPortHandle,&dwErrorFlags, &ComStat) ;
					if (dwErrorFlags > 0)
						DisplayErrMsg(dwErrorFlags);
					break;
				}
			}
		}
		else
		{						 	
			ClearCommError(m_hPortHandle, &dwErrorFlags, &ComStat ) ;
			if (dwErrorFlags > 0)
				DisplayErrMsg(dwErrorFlags);
			return  FALSE ;
		}
	}
	if(dwBytesToWrite == dwBytesWritten)
	{
		return TRUE;
	}
	else
		return FALSE;
}


void CAsynSerialDevice::DisplayErrMsg(DWORD Index)
{
	switch(Index)
	{
	case CE_BREAK:
		m_csErrorMessage = "I/O HW found an interrupt condition"; 
		break;
	case CE_FRAME:
		m_csErrorMessage = "I/O HW found a frame error"; 
		break;
	case CE_IOE:
		m_csErrorMessage = "I/O error during communitcation"; 
		break;
	case CE_MODE:
		m_csErrorMessage = "Not support needed mode,or illegal hCommDev par.";
		break;
	case CE_OOP:
		m_csErrorMessage = "Parallel equip.sent sig.overflowed from paper"; 
		break;
	case CE_OVERRUN:
		m_csErrorMessage = "Serial port read no character from HW before next character\n this character lost"; 
		break;
	case CE_RXOVER:
		m_csErrorMessage = "Serial ;port input buffer overflow";
		break;
	case CE_RXPARITY:
		m_csErrorMessage = "Serial port HW found a parity error"; 
		break;
	case CE_TXFULL:
		m_csErrorMessage = "Serial port try to transfer a character, but the output buffer already full"; 
		break;
	case CE_DNS:
		m_csErrorMessage = "Not select parallel equip"; 
		break;
	case CE_PTO:
		m_csErrorMessage = "Overtime on I/O equip."; 
		break;
	default:
		m_csErrorMessage = "δ֪����";
	}
	if (m_bShowMessage)
		AfxMessageBox(m_csErrorMessage);
}

BOOL CAsynSerialDevice::SetReceiveBufferSize(DWORD dwBufSize)
{
	if( m_dwReceiveBufSize == dwBufSize )
		return TRUE;

	if( m_pbyReceiveBuffer != NULL )
		delete [] m_pbyReceiveBuffer;
	m_pbyReceiveBuffer = new BYTE[dwBufSize];
	if (m_pbyReceiveBuffer==NULL)
	{
		m_csErrorMessage = "���뻺���ڴ�ʧ��";
		if (m_bShowMessage)
			AfxMessageBox(m_csErrorMessage);
		m_dwReceiveBufSize = 0;
		return FALSE;
	}

	m_dwReceiveBufSize = dwBufSize;

	return TRUE;
}


//������ջ��������ݣ����ؽ��ջ�������δ������������ֽ���
//�����δ���������ݣ��ƶ������ջ�����ͷ��
//����ֵ = TRUE: ���ݱ�����dwDataLen ���ؽ��ջ�������δ������������ֽ���
//       = FALSE: ���ݴ��󣨶˿��������ѽ������ݱ������
BOOL CAsynSerialDevice::ProcessReceiveData( DWORD &dwDataLen )
{
// 	TRACE("%d",m_pbyReceiveBuffer);
// 	return true;
	return 0 ;
}

BOOL CAsynSerialDevice::LodPortParameters()
{
	return FALSE;
}

BOOL CAsynSerialDevice::SavePortParameters()
{

	return FALSE;
}

BOOL CAsynSerialDevice::StartThread()
{
//	m_pThreadParams = NULL;
	if (!CreateThread(CREATE_SUSPENDED))
	{
		m_bAutoDelete = FALSE;
		m_csErrorMessage = "��ʼ�߳�ʧ�ܣ��߳�����";
		if (m_bShowMessage)
			AfxMessageBox(m_csErrorMessage);
		Delete();
		return FALSE;
	}
	else
	{
		SetThreadPriority(THREAD_PRIORITY_NORMAL);
		ResumeThread();
		m_bIsRun = TRUE;
	}
	return TRUE;
}

BOOL CAsynSerialDevice::StopThread()
{
//	DWORD ExitCode ;
	
	//����������߳̾��
/*	HANDLE hThread;
	hThread = GetCurrentThread();
	DuplicateHandle(GetCurrentProcess(),
		            GetCurrentProcess(),
					GetCurrentProcess(),
					&hThread,
					0,
					FALSE,
					DUPLICATE_SAME_ACCESS);
					*/
/*	this->SuspendThread();
	GetExitCodeThread(m_hThread,&ExitCode);
	TerminateThread(m_hThread,ExitCode);
	return 1;
*/

	DWORD exit;
	GetExitCodeThread(m_hThread,&exit);	
	VERIFY(SetEvent(m_OsRead.hEvent));
	VERIFY(SetEvent(m_OsWrite.hEvent));
	
	SetThreadPriority(THREAD_PRIORITY_ABOVE_NORMAL);
	CloseComPort(TRUE);

	WaitForSingleObject(m_OsRead.hEvent, INFINITE);
	WaitForSingleObject(m_OsWrite.hEvent, INFINITE);
	//WaitForSingleObject(m_hThread,200);
	if (::WaitForSingleObject(m_hThread, 1000) == WAIT_FAILED)
	{
		//TRACE("���� %d �̷߳������˳�!\n", m_byPortNo);
		::TerminateThread(m_hThread, 100);
	}
	// now delete CWinThread object since no longer necessary

	//delete this;
	//TRACE("���� %d �߳������˳�!\n", m_byPortNo);

	return true;
}

BOOL CAsynSerialDevice::ResumeThisThread()
{
	return ResumeThread();
}

void CAsynSerialDevice::Delete()
{
	VERIFY(SetEvent(m_OsRead.hEvent));
	VERIFY(SetEvent(m_OsWrite.hEvent));

}