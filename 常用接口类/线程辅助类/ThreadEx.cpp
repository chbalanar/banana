#include "StdAfx.h"
#include "ThreadEx.h"


CThreadEx::CThreadEx(void)
{
	m_event = CreateEvent(NULL,TRUE,TRUE,NULL);
	m_thread= INVALID_HANDLE_VALUE;
	m_type = THREAD_PASH;
}


CThreadEx::~CThreadEx(void)
{
	CloseThread();
}
void CThreadEx::CreateThread( LPTHREAD_START_ROUTINE lpStartAddress,LPVOID lpParameter,DWORD dwCreationFlags, LPDWORD lpThreadId, BYTE nType)
{
	if(m_thread==INVALID_HANDLE_VALUE)
	{
		m_type = nType;
		m_thread = ::CreateThread(NULL,0,lpStartAddress,lpParameter,dwCreationFlags,lpThreadId);

		if(m_thread==NULL)
		{
			CString s;
			s.Format("CreateThread Error [0x%0x]",GetLastError());
			MessageBox(NULL,s,"创建线程",MB_OK);
		}
	}
}
void CThreadEx::CloseThread()
{
	WriteType(THREAD_STOP);
	::WaitForSingleObject(m_event,INFINITE);
	if (m_thread!=INVALID_HANDLE_VALUE)
		::CloseHandle(m_thread);
	m_thread=INVALID_HANDLE_VALUE;
}
//////////////////////////////////////////////////////////////////////////
CThreadManage::CThreadManage(LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags/* =THREAD_PRIORITY_NORMAL */, LPDWORD lpThreadId/* =NULL */, BYTE nType/* =THREAD_START */)
	:m_lpRoutine(lpStartAddress)
	,m_lpParameter(lpParameter)
{
	m_thread.CreateThread((LPTHREAD_START_ROUTINE)WorkThread,this,dwCreationFlags,lpThreadId,nType);
}
CThreadManage::~CThreadManage()
{
	m_thread.CloseThread();
}
DWORD CThreadManage::WorkThread(LPVOID lpsrc)
{
	if(!lpsrc) return -1;
	CThreadManage* lp=(CThreadManage*)lpsrc;
	lp->m_thread.ResetEvent();
	while(1)
	{
		Sleep(1);
		if(lp->m_thread.IsType(CThreadEx::THREAD_STOP)) break;
		if(lp->m_thread.IsType(CThreadEx::THREAD_PASH)) continue;
		if(lp->m_lpThreadPool && lp->m_lpThreadPool->DecreEvent())
		{
			DWORD val=(*(lp->m_lpRoutine))(lp->m_lpParameter);
		}
	}
	lp->m_thread.SetEvent();

	return 0;
}
//////////////////////////////////////////////////////////////////////////
CThreadPool::CThreadPool()
{
	m_nCount = 0;
	m_nEvent = 0;
	m_bStop = false;
}
CThreadPool::~CThreadPool()
{
	UnThreadPool();
// 	CAutoLock lock(m_sect);
// 	m_bStop = true;
// 
// 	for(lstThreadManage::iterator it=m_vThreadMange.begin(); it!=m_vThreadMange.end(); )
// 	{
// 		if(*it)
// 		{
// 			delete *it;
// 		}
// 		it=m_vThreadMange.erase(it);
// 	}
// 	m_nCount=0;
}
void CThreadPool::InitThreadPool( UINT nVal, LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter, DWORD dwCreationFlags/* =THREAD_PRIORITY_NORMAL */ )
{
	UnThreadPool();
	CAutoLock lock(m_sect);
	m_bStop = false;
	while(m_nCount<nVal)
	{
		CThreadManage* lp=new CThreadManage(lpStartAddress,lpParameter,dwCreationFlags);
		lp->m_lpThreadPool = this;
		m_vThreadMange.push_back(lp);
		m_nCount++;
	}
}
void CThreadPool::UnThreadPool()
{
	RemovEvent();
	CAutoLock lock(m_sect);
	m_bStop = true;

	for(lstThreadManage::iterator it=m_vThreadMange.begin(); it!=m_vThreadMange.end(); )
	{
		if(*it)
		{
			delete *it;
		}
		it=m_vThreadMange.erase(it);
	}
	m_nCount=0;
}
void CThreadPool::RemovEvent()
{
	CAutoLock lock(m_sc_event);
	m_nEvent = 0;
}
void CThreadPool::WriteEvent(UINT nVal)
{
	CAutoLock lock(m_sc_event);
	if(m_bStop) return ;

	m_nEvent+=nVal;
}
bool CThreadPool::DecreEvent()
{
	CAutoLock lock(m_sc_event);
	if(m_bStop) return false;

	if(m_nEvent==(UINT)-1) return true;

	if(m_nEvent>0)
	{
		m_nEvent--;
		return true;
	}
	return false;
}
bool CThreadPool::IsEvent()
{
	CAutoLock lock(m_sc_event);
	
	return m_nEvent!=0;
}
bool CThreadPool::IsStop()
{
	return m_bStop;
}