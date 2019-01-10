#ifndef _RF_WRITELOG_LOG_FILE_H__
#define _RF_WRITELOG_LOG_FILE_H__


//ʹ��˵��
/*
	��ͨ���� CWirteLogFile ����ȫ�ֶ���;
	
	��ʼ������
	Initialize ��ʼ������,����д��־�ļ�����(��·��)
	
	����д��־ʱ����
	Close()
	
	ʵʱд����ʱ����
	WriteLogFileString д���ʽΪ�ַ�����ʽ.
	
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