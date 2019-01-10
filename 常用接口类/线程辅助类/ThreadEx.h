//////////////////////////////////////////////////////////////////////////
// 新科电子：线程辅助类
// 作者：王巧
// 注释：
// 日期：2011-8-22
//////////////////////////////////////////////////////////////////////
#pragma once
#ifndef _CTHREADEX_H_
#define _CTHREADEX_H_
#include "LockData.h"
#include <list>


class DLL_EXPORT CThreadEx
{
public:
	static enum{
		THREAD_PASH = 1,
		THREAD_START,
		THREAD_STOP,
	};
public:
	CThreadEx(void);
	~CThreadEx(void);
public:
	void CloseThread();
	void CreateThread(
		 LPTHREAD_START_ROUTINE lpStartAddress,
		 LPVOID lpParameter,
		 DWORD dwCreationFlags=THREAD_PRIORITY_NORMAL,
		 LPDWORD lpThreadId=NULL,
		 BYTE nType=THREAD_START
		);
	void ResetEvent(){ ::ResetEvent(m_event);}
	void SetEvent(){ ::SetEvent(m_event);}
	void WriteType(BYTE type){ 
		if(IsType(THREAD_STOP)) return ;
		m_type = type;
	}
	bool IsType(BYTE type){return m_type==type;}
private:
	HANDLE m_thread;
	HANDLE m_event;
	BYTE   m_type;
};
class DLL_EXPORT CThreadManage
{
	friend class CThreadPool;
public:
	CThreadManage(LPTHREAD_START_ROUTINE lpStartAddress,
		LPVOID lpParameter,
		DWORD dwCreationFlags=THREAD_PRIORITY_NORMAL,
		LPDWORD lpThreadId=NULL,
		BYTE nType=CThreadEx::THREAD_START);
	~CThreadManage();
protected:
	static DWORD WorkThread(LPVOID lpsrc);
private:
	LPTHREAD_START_ROUTINE m_lpRoutine;
	LPVOID m_lpParameter;
	CThreadEx m_thread;
	CThreadPool* m_lpThreadPool;
	CSectLock m_sect;
};
class  DLL_EXPORT CThreadPool
{
	typedef std::list<CThreadManage*> lstThreadManage;

public:
	CThreadPool();
	~CThreadPool();
public:
	void InitThreadPool(
		UINT nVal,
		LPTHREAD_START_ROUTINE lpStartAddress,
		LPVOID lpParameter,
		DWORD dwCreationFlags=THREAD_PRIORITY_NORMAL
		);
	void UnThreadPool();
	
	void RemovEvent();
	void WriteEvent(UINT nVal);
	bool DecreEvent();
	bool IsEvent();
	bool IsStop();
private:
	UINT m_nEvent;
	bool m_bStop;
	UINT m_nCount;//

	lstThreadManage m_vThreadMange;
	CSectLock m_sect;
	CSectLock m_sc_event;

};
#endif
