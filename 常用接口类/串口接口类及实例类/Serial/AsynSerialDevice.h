#if !defined(AFX_ASYNSERIALDEVICE_H__A89860A6_6453_11D5_81DE_0080C8F19FD1__INCLUDED_)
#define AFX_ASYNSERIALDEVICE_H__A89860A6_6453_11D5_81DE_0080C8F19FD1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AsynSerialDevice.h : header file
//
#include <afxmt.h>

#define RF_SERIAL_PORT_DEFAULT_RECEIVE_BUF_SIZE		8192

/////////////////////////////////////////////////////////////////////////////
// CAsynSerialDevice thread

class CAsynSerialDevice : public CWinThread
{
	DECLARE_DYNCREATE(CAsynSerialDevice)
public:
	BOOL m_bIsRun;
	HANDLE m_hOkInfo;
	HANDLE m_hDealDataFun;
    LPBYTE m_pRecvSerialBuf;     //用于接收小块的内存区
	CMutex m_pRecvSerialBufKey;  //用于锁住小块的内存区
 	DWORD m_OfferSet ;           //接收小块的内存的空间寻址
    int m_TransMode;             //传输模式，0整个一起发送，1固定块的发送，2数据包的发送
	BOOL WriteCommBlock(PBYTE lpByte, DWORD dwBytesToWrite);
	int WriteCommBlock1(BYTE *pBlock,int nBlockLen);
	BOOL WriteBuffer(PBYTE lpBuf, DWORD dwToWrite);
protected:
	CAsynSerialDevice();           // protected constructor used by dynamic creation


protected:
	/*专门处理数据的线程****/

	/***********************/
	OVERLAPPED m_OsWrite;
	OVERLAPPED m_OsRead;
	HANDLE m_hPortHandle;

	CString m_csErrorMessage;
	BOOL m_bShowMessage;
	BOOL m_Over;
	BYTE m_byPortNo;

	// serial port parameters 
	DCB m_Dcb;
	COMMTIMEOUTS m_CommTimeOuts;
	DWORD m_dwWatchEvents;

	BYTE *m_pbyReceiveBuffer;               //接收时使用的缓冲区
	DWORD m_dwReceiveBufSize;

	BOOL OpenPortFile(BYTE PortNo);
	void DisplayErrMsg(DWORD Index);


	BOOL EnableEVDSR(BOOL Enable);
	BOOL EnableEVCTS(BOOL Enable);
	BOOL EnableEVBREAK(BOOL Enable);
	BOOL EnableEVRXCHAR(BOOL Enable);
	BOOL EnableRX80FULL(BOOL Enable);

	BOOL SetRecChar(BOOL fNull = 0, char EofChar = 0, char EvtChar = 0);
	BOOL SetError(BOOL Parity = TRUE, BOOL fErrorChar = 0, BOOL AbortOnError = 0, char ErrorChar = 0);
	BOOL SetWireFlowControl(BOOL OutxCtsFlow = FALSE, BOOL OutxDsrFlow = 0, DWORD DtrControl = 1, BOOL DsrSensitivity = 0, DWORD RtsControl = RTS_CONTROL_DISABLE);
	BOOL SetCharFlowControl(BOOL OutX = 0, BOOL InX = 0, BOOL TXContinueOnXoff = 0, char XonChar = 0x11, char XoffChar = 0x13, WORD XonLim = 1280, WORD XoffLim = 2048);
	BOOL SetTimeOut(DWORD ReadIntervalTimeout = 20, DWORD ReadTotalTimeoutMultiplier = 0, DWORD ReadTotalTimeoutConstant = 1000, DWORD WriteTotalTimeoutMultiplier = 0, DWORD WriteTotalTimeoutConstant = 2000);
	BOOL SetInOutBufSize(DWORD ReceiveSize, DWORD SendSize);

	DWORD Offset;
// Attributes
public:
	BOOL SetDcbBaByPaSt(DWORD BaudRate = 9600, BYTE ByteSize = 8, BYTE StopBits = ONESTOPBIT,  BYTE Parity = 0);
	BOOL CloseComPort(BOOL Over);
	BOOL OpenComPort(BYTE PortNo);
	BOOL StartThread();
    BOOL StopThread();
	BOOL ResumeThisThread();
    DWORD ReadCommBuf(PBYTE Buf, DWORD BufSize);
	void ReadSerialData();                 //不分块的接收
	void ReadSerialData1();                //分块方式的接收
    void ReadSerialData2();                //按数据包分块方式的接收
	//virtual BOOL SendCommand(PVOID pCommand,int Length);
	virtual BOOL ProcessReceiveData( DWORD &dwDataLen );    //Old 

// Operations
public:
	virtual void Delete();
	BOOL SavePortParameters();
	BOOL LodPortParameters();
	BOOL SetReceiveBufferSize( DWORD dwBufSize );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAsynSerialDevice)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAsynSerialDevice();

	// Generated message map functions
	//{{AFX_MSG(CAsynSerialDevice)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ASYNSERIALDEVICE_H__A89860A6_6453_11D5_81DE_0080C8F19FD1__INCLUDED_)
