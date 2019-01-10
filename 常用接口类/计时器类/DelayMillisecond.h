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
	DWORD StartTime;   // ��¼��ʼʱ��
    DWORD NowTime;     // ����ʱ�������ж�ͣ��ʱ��ʱ��
	bool b_Start;        // ��ʼ��ʱ��־���ѿ�ʼ��ʱΪtrue������Ϊfalse.
	DWORD m_dwTime;		//�趨ʱ��
public:
    void Init(void);       // ��ʼ����ʱ��
    void Start(void);      // ��ʼ��ʱ
    DWORD GetTimer(void);  // ��ͬһ��λ����ͣ����ʱ��
	bool IsStart(void);    // �ж��Ƿ��Ѿ���ʼ��ʱ�����ǣ��򷵻�true,����false.
    void End(void);        // ������ʱ
	void SetTimer( int iTime ) {m_dwTime = iTime;} //����1S =1000;
	bool GetTimerEndFlag();  //��ȡ�趨��ʱ���ѹ�,�ѹ�Ϊtrue
	DWORD GetEndTimer(void);  // ��ȡʣ�µ�ʱ��

};

#endif // !defined(AFX_DELAYMILLISECOND_H__7ED64C9A_6B94_45CA_812F_EAA09C11417C__INCLUDED_)
