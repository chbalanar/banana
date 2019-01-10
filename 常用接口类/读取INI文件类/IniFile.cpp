// IniFile.cpp: implementation of the CIniFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IniFile.h"
#include <io.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIniFile::CIniFile():m_csFile(_T(""))
{
}

CIniFile::CIniFile(CString csFile) 
    : m_csFile(csFile)
{
	ASSERT( csFile );
}

CIniFile::~CIniFile()
{
}

CString CIniFile::ReadString(const CString& csSection, const CString& csItem,
	const CString& csDefault)
{
	char szBuffer[1025];
	::GetPrivateProfileString(csSection, csItem, csDefault, szBuffer, 1024,
		m_csFile);
	return szBuffer;
}

bool CIniFile::ReadBool(const CString& csSection, const CString& csItem,
	bool bDefault)
{
	UINT nRet;
	nRet = GetPrivateProfileInt(csSection, csItem, (bDefault) ? 1 : 0,
			m_csFile);
	return (nRet == 1);
}

UINT CIniFile::ReadInt(const CString& csSection, const CString& csItem,
	UINT iDefault)
{
	UINT nRet;
	nRet = GetPrivateProfileInt(csSection, csItem, iDefault, m_csFile);
	return nRet;
}

BYTE CIniFile::ReadByte(const CString& csSection, const CString& csItem, BYTE iDefault)
{
	UINT nRet;
	nRet = GetPrivateProfileInt(csSection, csItem, (UINT)iDefault, m_csFile);
	return (BYTE)nRet;
}
double CIniFile::ReadFloat(const CString& csSection, const CString& csItem,
	double fDefault)
{
	char szBuffer[25];
	char szValue[25];
	sprintf_s(szValue,sizeof(szValue), "%f", fDefault);
	GetPrivateProfileString(csSection, csItem, szValue, szBuffer, 24, m_csFile);
	return atof(szBuffer);
}

void CIniFile::WriteString(const CString& csSection, const CString& csItem,
	const CString& csValue)
{
	WritePrivateProfileString(csSection, csItem, csValue, m_csFile);
}

void CIniFile::WriteBool(const CString& csSection, const CString& csItem,
	bool  bValue)
{
	WriteString(csSection, csItem, (bValue) ? "1" : "0");
}

void CIniFile::WriteInt(const CString& csSection, const CString& csItem,
	UINT iValue)
{
	CString csValue;
	csValue.Format("%d", iValue);
	WriteString(csSection, csItem, csValue);
}

void CIniFile::WriteFloat(const CString& csSection, const CString& csItem,
	double fValue)
{
	CString csValue;
	csValue.Format("%f", fValue);
	WriteString(csSection, csItem, csValue);
}

void CIniFile::DelSection_Key(const CString & csSection, const CString & csItem)
{
	WritePrivateProfileStruct(csSection, csItem, NULL, 0, m_csFile);
}

void CIniFile::DelSection_AllKey(const CString & csSection)
{
	WritePrivateProfileSection(csSection, "", m_csFile);
}

BOOL CIniFile::ReadSection (const char* Section, CStringList &  cstLst) 
{ 
	char buff[MAXSIZE]; 
	char* p=NULL; 
	cstLst.RemoveAll(); 

	TRACE(Section );	
	int ReadSize=GetPrivateProfileString(Section, NULL, NULL, buff, MAXSIZE, m_csFile); 
	if (!ReadSize)   
	{ 
		TRACE(_T( "Can not read specific Section from Configuration file!\n")); 
		return false;   
	} 
	else 
	{ 
		p=buff; 
		while ((*p) != '\0')  
		{   
			//cstLst.AddHead(p); 
			cstLst.AddTail(p);
			//TRACE("p=%s\n",  p);
			p += strlen(p) + 1;
			if((*p) == '\0') 
				return true;
		} 
		return true; 
	}  
	return true;
} 


BOOL CIniFile::IsExistFile()
{
	if( (_taccess( m_csFile, 0 )) != -1 )
		return TRUE;
	else
		return FALSE;
}

BOOL CIniFile::HasWritePermission()
{
	if( (_taccess( m_csFile, 0 )) != -1 )
	{
		if( (_taccess( "crt_ACCESS.C", 2 )) != -1 )
			return TRUE;
	}
	return FALSE;
}

int CIniFile::GetAllSections(CStringArray &strArry)
{
	TCHAR      chSectionNames[2048]={0};      //���н�����ɵ��ַ�����
	char * pSectionName; //�����ҵ���ĳ�������ַ������׵�ַ
	int i;      //iָ������chSectionNames��ĳ��λ�ã���0��ʼ��˳�����
	int j=0;  //j����������һ�������ַ������׵�ַ����ڵ�ǰi��λ��ƫ����
	int count=0;     //ͳ�ƽڵĸ��� 
	::GetPrivateProfileSectionNames(chSectionNames,2048,m_csFile); 

	for(i=0;i<2048;i++,j++)
	{
		if(chSectionNames[0]=='\0')
			break;      //�����һ���ַ�����0����˵��ini��һ����Ҳû��
		if(chSectionNames[i]=='\0')
		{
			pSectionName=&chSectionNames[i-j]; 
			//�ҵ�һ��0����˵��������ַ���ǰ������j��ƫ����������һ���������׵�ַ
			j=-1; 
			++count;
			strArry.Add(pSectionName);
			if(chSectionNames[i+1]==0)
			{
				break;  //���������ڵ��ַ�����0ʱ�������еĽ��������ҵ���ѭ����ֹ
			}
		}     
	}
	return count;
}

int CIniFile::GetAllItems(LPCTSTR lpSection,CStringArray &strArry)
{
	int nNum = 0;
	try
	{
		CStdioFile file(m_csFile, CFile::modeRead);
		CString strLine;
		CString strDes;
		strDes.Format(_T("[%s]"), lpSection);
		while(file.ReadString(strLine))
		{
			strLine.TrimLeft();
			strLine.TrimRight();
			if (strLine == strDes)
			{
				while(file.ReadString(strLine))
				{
					strLine.TrimLeft();
					if(strLine.IsEmpty() || strLine[0] == ';')
						continue;
					if (strLine[0] == '[')//˵����������һ������
						break;
					CString strItem = strLine.Left(strLine.Find('='));
					strItem.TrimRight();
					++nNum;
					strArry.Add(strItem);
				}
				break;
			}
		}
	}
	catch(CFileException &e)
	{
		TCHAR   szCause[255] = {0};
		e.GetErrorMessage(szCause, 255);
		::OutputDebugString(szCause);
		return -1;
	}
	return nNum;
}