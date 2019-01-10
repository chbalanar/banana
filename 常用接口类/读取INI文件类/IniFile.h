// IniFile.h: interface for the CIniFile class.
// �¿Ƶ��ӣ� INI�ļ���д������
// ���ߣ��¿Ƶ���
// ע�ͣ�INI�ļ���д����
// ���ڣ�2011-8-22
//////////////////////////////////////////////////////////////////////

#ifndef _INIFILE_H_
#define _INIFILE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAXSIZE 1024

class  CIniFile
{
public:
	CIniFile();
	CIniFile(CString csFile);
	virtual ~CIniFile();

public:
	// ��ȡ CString, bool, int, double �������͵�ֵ
	CString ReadString(const CString& csSection, const CString& csItem,
		const CString& csDefault);
	bool ReadBool(const CString& csSection, const CString& csItem,
		bool bDefault);
	UINT ReadInt(const CString& csSection, const CString& csItem,
		UINT iDefault);
	BYTE ReadByte(const CString& csSection, const CString& csItem,
		BYTE iDefault);
	double ReadFloat(const CString& csSection, const CString& csItem,
		double fDefault);

	// д�� CString, bool, int, double �������͵�ֵ
	void WriteString(const CString& csSection, const CString& csItem,
		const CString& csValue);
	void WriteBool(const CString& csSection, const CString& csItem,
		bool bValue);
	void WriteInt(const CString& csSection, const CString& csItem, UINT iValue);
	void WriteFloat(const CString& csSection, const CString& csItem,
		double fValue);
	///
	BOOL IsExistFile();			//�ж��ļ��Ƿ���� 
	BOOL HasWritePermission();	//�ж��ļ��Ƿ��д

	int GetAllSections(CStringArray &strArry);		//��ȡ���еĽ���
	int GetAllItems(LPCTSTR lpSection, CStringArray &strArry);			//��ȡ���������������
	

	//add xjc 2011.5.27
	BOOL ReadSection(const char*  Section, CStringList & cstLst); 

	//ɾ��section�µ�ĳ��KEY ��
	void DelSection_Key(const CString & csSection, const CString & csItem);
	//ɾ��section�µ�����KEY��
	void DelSection_AllKey(const CString & csSection );




private:
	CString m_csFile;//�ļ���
};

#endif // !defined(AFX_INIFILE_H__D8D95DE9_E81F_493D_91FB_22CF8B47A330__INCLUDED_)
