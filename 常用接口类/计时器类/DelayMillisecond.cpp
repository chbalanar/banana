// DelayMillisecond.cpp: implementation of the DelayMillisecond class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DelayMillisecond.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DelayMillisecond::DelayMillisecond()
{
    NowTime=0;
	StartTime=0;
	b_Start=false;
}

DelayMillisecond::~DelayMillisecond()
{

}

void DelayMillisecond::Init()       // ��ʼ����ʱ��
{
    NowTime=0;
	StartTime=0;
	b_Start=false;
}

void DelayMillisecond::Start()      // ��ʼ��ʱ
{
    StartTime=::GetTickCount();		//��ȡϵͳ��ʼʱ��
    b_Start=true;
}

bool DelayMillisecond::GetTimerEndFlag()
{
    NowTime=GetTickCount();			//��¼ͣ��ʱ��
	if( (NowTime-StartTime )> m_dwTime )		//�ж�ͣ��ʱ���Ƿ�����趨ʱ��
	{
		return true;
	}
	return false;
}


DWORD DelayMillisecond::GetEndTimer()   // ��ȡʣ�µ�ʱ��
{
    DWORD Temp_Millisecond=0;
    NowTime=GetTickCount();
    Temp_Millisecond = m_dwTime - (NowTime-StartTime);		//ʣ��ʱ��Ϊ�趨ʱ���ȥͣ��ʱ��
	return Temp_Millisecond;
}

DWORD DelayMillisecond::GetTimer()  // ��ͬһ��λ����ͣ����ʱ��
{
    DWORD Temp_Millisecond=0;
    NowTime=GetTickCount();
    Temp_Millisecond=NowTime-StartTime;
	return Temp_Millisecond;
}


bool DelayMillisecond::IsStart()    // �ж��Ƿ��Ѿ���ʼ��ʱ�����ǣ��򷵻�true,����false.
{
    return b_Start;   
}

void DelayMillisecond::End(void)        // ������ʱ
{
    b_Start=false;
    NowTime=StartTime=0;
}
