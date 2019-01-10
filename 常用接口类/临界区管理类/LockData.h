//////////////////////////////////////////////////////////////////////////
// �¿Ƶ��ӣ��ٽ���������
// ���ߣ��¿Ƶ���
// ע�ͣ����ͷ�����
// ���ڣ�2011-8-22
//////////////////////////////////////////////////////////////////////
#ifndef _LOCKDATA_H_ 
#define _LOCKDATA_H_ 

//�ٽ����ɸ��÷����� 2010��4��23�� 1.1��
/*
�������������߳��кܷ����ʹ��������������һ����
ʹ��ʱΪ�˷�ֹ�û����������ͣ����ڲ�ͬ���߳��з����䷽��
�����ǽ���ֵ������ɵ��������ʣ�����������ģ��������� 	union{ T data; } m_data;	//������ֻ��ʹ����������
CLockData<int> a;
CLockData<int> b = 10;
CLockData<int> c = b;
CLockData<int> d(c);
a = 20;
int e = a;
c = a;
TRACE(_T("%d %d %d %d %d\n"), (int)a, (int)b, (int)c, (int)d, (int)e);

*/ 

//CLockValue ֻ������ֵ����

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

	typedef _Auto<T>	AUTO;		//�Զ���
	typedef T			DATA_TYPE;	//��������

	//Ĭ�Ϲ��캯��
	CLockValue(){ ::InitializeCriticalSection(&m_sect); }
	//�������Ĺ��캯��
	CLockValue(const T & data):  m_data(data){ ::InitializeCriticalSection(&m_sect); }
	//�������캯��
	CLockValue( CLockValue<T> & other) { ::InitializeCriticalSection(&m_sect);
		AUTO lock(other); m_data = other.m_data; }
	//��������
	~CLockValue(){ ::DeleteCriticalSection(&m_sect); }

	//��ֵ����
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
	//ȡֵ����
	operator T ()
	{ 
		AUTO lock(*this); 
		return m_data; 
	}
	//������������
	//union{ T data; } m_data;	//������ֻ��ʹ����������
protected:
	//����
	BOOL Lock(){ ::EnterCriticalSection(&m_sect); return TRUE; }
	//���� �ο���CCriticalSectionr��ʵ�֣������������β� dwTimeout
	BOOL Lock(DWORD dwTimeout ){ return Lock(); };
	//����
	BOOL Unlock(){ ::LeaveCriticalSection(&m_sect); return TRUE; }
private:
	T m_data;
	CRITICAL_SECTION m_sect;
};

//�Զ��ٽ�����
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

//�¼���
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
