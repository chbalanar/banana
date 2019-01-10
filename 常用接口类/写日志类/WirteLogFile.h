#ifndef _RF_WRITELOG_LOG_FILE_H__
#define _RF_WRITELOG_LOG_FILE_H__


//使用说明
/*
	可通过类 CWirteLogFile 建立全局对象;
	
	初始化调用
	Initialize 初始化函数,传入写日志文件名称(含路径)
	
	结束写日志时调用
	Close()
	
	实时写内容时调用
	WriteLogFileString 写入格式为字符串格式.
	
*/

class CWirteLogFile : public CFile
{
public:
	CWirteLogFile();
	CWirteLogFile(CString csLogFileName);
	~CWirteLogFile();
public:
	virtual void Close();
	BOOL Initialize( CString csLogFileName );

	virtual void WriteLogFileString( CString csLog );

protected:
	BOOL m_bOpenFlag;
};

#endif