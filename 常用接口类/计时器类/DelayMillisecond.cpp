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

void DelayMillisecond::Init()       // 初始化计时器
{
    NowTime=0;
	StartTime=0;
	b_Start=false;
}

void DelayMillisecond::Start()      // 开始计时
{
    StartTime=::GetTickCount();		//获取系统开始时间
    b_Start=true;
}

bool DelayMillisecond::GetTimerEndFlag()
{
    NowTime=GetTickCount();			//记录停留时间
	if( (NowTime-StartTime )> m_dwTime )		//判断停留时间是否大于设定时间
	{
		return true;
	}
	return false;
}


DWORD DelayMillisecond::GetEndTimer()   // 获取剩下的时间
{
    DWORD Temp_Millisecond=0;
    NowTime=GetTickCount();
    Temp_Millisecond = m_dwTime - (NowTime-StartTime);		//剩下时间为设定时间减去停留时间
	return Temp_Millisecond;
}

DWORD DelayMillisecond::GetTimer()  // 在同一个位置上停留的时间
{
    DWORD Temp_Millisecond=0;
    NowTime=GetTickCount();
    Temp_Millisecond=NowTime-StartTime;
	return Temp_Millisecond;
}


bool DelayMillisecond::IsStart()    // 判断是否已经开始计时，如是，则返回true,否则false.
{
    return b_Start;   
}

void DelayMillisecond::End(void)        // 结束计时
{
    b_Start=false;
    NowTime=StartTime=0;
}
