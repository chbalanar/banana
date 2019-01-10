// DelayMillisecond.h: interface for the DelayMillisecond class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DELAYMILLISECOND_H__7ED64C9A_6B94_45CA_812F_EAA09C11417C__INCLUDED_)
#define AFX_DELAYMILLISECOND_H__7ED64C9A_6B94_45CA_812F_EAA09C11417C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DelayMillisecond  
{
public:
	DelayMillisecond();
	virtual ~DelayMillisecond();

private:
	DWORD StartTime;   // 记录开始时间
    DWORD NowTime;     // 是临时变量，判断停留时间时用
	bool b_Start;        // 开始计时标志，已开始计时为true，否则为false.
	DWORD m_dwTime;		//设定时间
public:
    void Init(void);       // 初始化计时器
    void Start(void);      // 开始计时
    DWORD GetTimer(void);  // 在同一个位置上停留的时间
	bool IsStart(void);    // 判断是否已经开始计时，如是，则返回true,否则false.
    void End(void);        // 结束计时
	void SetTimer( int iTime ) {m_dwTime = iTime;} //毫秒1S =1000;
	bool GetTimerEndFlag();  //获取设定的时间已过,已过为true
	DWORD GetEndTimer(void);  // 获取剩下的时间

};

#endif // !defined(AFX_DELAYMILLISECOND_H__7ED64C9A_6B94_45CA_812F_EAA09C11417C__INCLUDED_)
