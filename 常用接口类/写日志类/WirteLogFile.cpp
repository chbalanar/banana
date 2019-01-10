#include "stdafx.h"
#include "WirteLogFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CWirteLogFile::CWirteLogFile()
{
	m_bOpenFlag = FALSE;
}
CWirteLogFile::CWirteLogFile(CString csLogFileName)
{
	m_bOpenFlag = FALSE;
	Initialize(csLogFileName);
}

CWirteLogFile::~CWirteLogFile()
{
    CString csLog;
	CTime start = CTime::GetCurrentTime();
	csLog.Format( "结束时间：%4d年%02d月%02d日 %02d:%02d:%02d\r\n",
		start.GetYear(), start.GetMonth(), start.GetDay(),
		start.GetHour(), start.GetMinute(), start.GetSecond() );
	WriteLogFileString( csLog );

	Close();
}

BOOL CWirteLogFile::Initialize( CString csLogFileName )
{
	CString csFileName;   
	//csFileName = RF_LOGFILE_PATH + csLogFileName;
    csFileName = csLogFileName;
	CFileException excep;	 
	
	if( !Open( csFileName, 
		 CFile::modeWrite | CFile::shareDenyWrite | CFile::modeNoTruncate |CFile::modeCreate| CFile::modeNoTruncate, 
		&excep ) )
	{

		TCHAR   szCause[255];
		excep.GetErrorMessage(szCause, 255);

		CString msg;
		msg.Format( "打开文件%s出错:%s", csFileName, szCause );
		AfxMessageBox( msg );
		return FALSE;
	}
	m_bOpenFlag = TRUE;

	SeekToEnd();

	CString csLog;
	CTime start = CTime::GetCurrentTime();
	csLog.Format( "初始化时间：%4d年%02d月%02d日 %02d:%02d:%02d",
		start.GetYear(), start.GetMonth(), start.GetDay(),
		start.GetHour(), start.GetMinute(), start.GetSecond() );
	WriteLogFileString( csLog );

	return TRUE;
}



void CWirteLogFile::WriteLogFileString( CString csLog )
{
	if( m_bOpenFlag == FALSE )
		return;
	if( csLog.GetLength() == 0 )
		return;

	CString csMsg;
	csMsg = csLog + "\r\n";
	
	int Len = csMsg.GetLength();
	Write( csMsg, Len );

	Flush();
}

void CWirteLogFile::Close()
{
	if( m_bOpenFlag == TRUE )
	{
		CFile::Close();
		m_bOpenFlag = FALSE;
	}
}
