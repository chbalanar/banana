//////////////////////////////////////////////////////////////////////////
// 新科电子：临界区管理类
// 作者：新科电子
// 注释：自释放自锁
// 日期：2011-8-22
//////////////////////////////////////////////////////////////////////
#ifndef _LOCKDATA_H_ 
#define _LOCKDATA_H_ 

//临界区可复用泛型类 2010年4月23日 1.1版
/*
您可以在任意线程中很方面的使用它，就像下面一样。
使用时为了防止用户传入类类型，并在不同的线程中访问其方法
而不是进行值拷贝造成的语义分歧故，屏蔽类类型模板参数。见 	union{ T data; } m_data;	//限制于只能使用内置类型
CLockData<int> a;
CLockData<int> b = 10;
CLockData<int> c = b;
CLockData<int> d(c);
a = 20;
int e = a;
c = a;
TRACE(_T("%d %d %d %d %d\n"), (int)a, (int)b, (int)c, (int)d, (int)e);

*/ 

//CLockValue 只能用于值类型

template <class T>
class CLockValue
{
public:
	template <class T >
	class _Auto{
	public:	
		_Auto(CLockValue<T> & data) : m_data(data){ m_data.Lock(); }
		~_Auto(){ m_data.Unlock(); }
	private:
		CLockValue<T> & m_data;
	};
	friend class _Auto<T>;
public:

	typedef _Auto<T>	AUTO;		//自动锁
	typedef T			DATA_TYPE;	//数据类型

	//默认构造函数
	CLockValue(){ ::InitializeCriticalSection(&m_sect); }
	//带参数的构造函数
	CLockValue(const T & data):  m_data(data){ ::InitializeCriticalSection(&m_sect); }
	//拷贝构造函数
	CLockValue( CLockValue<T> & other) { ::InitializeCriticalSection(&m_sect);
		AUTO lock(other); m_data = other.m_data; }
	//析构函数
	~CLockValue(){ ::DeleteCriticalSection(&m_sect); }

	//赋值函数
	CLockValue<T> & operator=(const T & data)
	{
		if (&m_data == &data) 
			return *this;
		AUTO lock(*this); 
		m_data = data;
		return *this;
	}
	CLockValue<T> & operator=(CLockValue<T> & other)
	{
		if (this == &other)
			return *this;
		AUTO lockMe(*this);
		m_data = (T)other;
		return *this;
	}
	//取值函数
	operator T ()
	{ 
		AUTO lock(*this); 
		return m_data; 
	}
	//被保护的数据
	//union{ T data; } m_data;	//限制于只能使用内置类型
protected:
	//加锁
	BOOL Lock(){ ::EnterCriticalSection(&m_sect); return TRUE; }
	//加锁 参考的CCriticalSectionr的实现，函数将忽略形参 dwTimeout
	BOOL Lock(DWORD dwTimeout ){ return Lock(); };
	//解锁
	BOOL Unlock(){ ::LeaveCriticalSection(&m_sect); return TRUE; }
private:
	T m_data;
	CRITICAL_SECTION m_sect;
};

//自动临界区锁
class CSectLock
{
public:
	CSectLock(){::InitializeCriticalSection(&m_cs); };
	~CSectLock(){::DeleteCriticalSection(&m_cs); };

	void Lock(){::EnterCriticalSection(&m_cs); };
	void Unlock(){::LeaveCriticalSection(&m_cs); };
private:
	CRITICAL_SECTION m_cs;
};

typedef  CSectLock SECT;

class CAutoLock
{
public:
	CAutoLock( CSectLock & cs) : m_cs(cs) { m_cs.Lock(); };
	~CAutoLock(){m_cs.Unlock() ; };
private:
	CSectLock & m_cs;
};
class  CLock
{
public:
	CLock(LPCRITICAL_SECTION _m_pCS):m_pCS(_m_pCS){	::EnterCriticalSection(m_pCS);}
	virtual ~CLock(){ LeaveCriticalSection(m_pCS);}

private:

	LPCRITICAL_SECTION m_pCS;
};

typedef  CAutoLock LOCK;

//事件锁
class CEventLock
{
	friend class CAutoEvent;
	friend class CRemoveEvent;
public:
	CEventLock(){
		m_event = CreateEvent(NULL,TRUE,TRUE,NULL);
		m_event1 = CreateEvent(NULL,TRUE,TRUE,NULL);
	}
	~CEventLock()
	{
	}
private:
	void ResetEvent(){ 
		::WaitForSingleObject(m_event1,INFINITE);
		::ResetEvent(m_event);}
	void SetEvent(){ ::SetEvent(m_event);}
	void RemoveEvent(){ 
		::SetEvent(m_event1);
	}
private:
	bool m_bEvent;
	HANDLE m_event;
	HANDLE m_event1;
};
class CAutoEvent
{
public:
	CAutoEvent(CEventLock& ev)
		:m_ev(ev)
	{
		m_ev.ResetEvent();
	}
	CAutoEvent(CEventLock* ev)
		:m_ev(*ev)
	{
		m_ev.ResetEvent();
	}
	~CAutoEvent(){ m_ev.SetEvent();}
private:
	CEventLock& m_ev;
};
class CRemoveEvent
{
public:
	CRemoveEvent(CEventLock& evt)
		:m_ev(evt)
	{
		::ResetEvent(m_ev.m_event1);
		::WaitForSingleObject(m_ev.m_event,INFINITE);
	}
	CRemoveEvent(CEventLock* evt)
		:m_ev(*evt)
	{
		::ResetEvent(m_ev.m_event1);
		::WaitForSingleObject(m_ev.m_event,INFINITE);
	}
	~CRemoveEvent(){
		::SetEvent(m_ev.m_event1);
	}
private:
	CEventLock& m_ev;
};
#define AUTO_EVENT(EVT) CAutoEvent _autoevt_(EVT);
#define REMOVE_EVENT(EVT) CRemoveEvent _autoevt_(EVT);
#endif
