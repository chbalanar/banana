// IniFile.h: interface for the CIniFile class.
// 新科电子： INI文件读写操作类
// 作者：新科电子
// 注释：INI文件读写操作
// 日期：2011-8-22
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
	// 读取 CString, bool, int, double 五种类型的值
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

	// 写入 CString, bool, int, double 四种类型的值
	void WriteString(const CString& csSection, const CString& csItem,
		const CString& csValue);
	void WriteBool(const CString& csSection, const CString& csItem,
		bool bValue);
	void WriteInt(const CString& csSection, const CString& csItem, UINT iValue);
	void WriteFloat(const CString& csSection, const CString& csItem,
		double fValue);
	///
	BOOL IsExistFile();			//判读文件是否存在 
	BOOL HasWritePermission();	//判断文件是否可写

	int GetAllSections(CStringArray &strArry);		//获取所有的节名
	int GetAllItems(LPCTSTR lpSection, CStringArray &strArry);			//获取节下面的所有项名
	

	//add xjc 2011.5.27
	BOOL ReadSection(const char*  Section, CStringList & cstLst); 

	//删除section下的某个KEY ；
	void DelSection_Key(const CString & csSection, const CString & csItem);
	//删除section下的所有KEY；
	void DelSection_AllKey(const CString & csSection );




private:
	CString m_csFile;//文件名
};

#endif // !defined(AFX_INIFILE_H__D8D95DE9_E81F_493D_91FB_22CF8B47A330__INCLUDED_)
