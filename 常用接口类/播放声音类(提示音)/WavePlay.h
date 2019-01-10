// Sun.h: interface for the WavePlay class.
//
//////////////////////////////////////////////////////////////////////
/*
�������ļ������ֿ�ʽ
1��ֻ����һ�ε��ļ���
zd.Open("names.ex",false)  ���� zd.Open("names.ex")
2��ǰһ���ļ��������ˣ���һ���ļ�ѭ������
pxj.Open("names1.ex","names2.ex",true)  ���� pxj.Open("names1.ex","names2.ex")
3��һ���ļ�ѭ������
speed.Open("names.ex",true)
*/


#if !defined(AFX_SUN_H__BB7077B6_B748_43B2_A00C_8397CE4CD0C2__INCLUDED_)
#define AFX_SUN_H__BB7077B6_B748_43B2_A00C_8397CE4CD0C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "stdafx.h"//add stdafx.h
#pragma comment (lib,"winmm.lib")//input winmm.lib
#include "afxmt.h"//add CEvent class
#include "mmsystem.h"//��Ƶ�豸�������ļ���������������������mmsystem.hͷ�ļ���
/////////////////////
class  CWavePlay;
struct stSndCtrl
{
	CWavePlay* wavFilename;
	CString fileState;
};
 

class _XK_DLL_EXPORT_ CWavePlay
{
	friend class SoundCtrl;
	
public:
	CWavePlay();
	virtual ~CWavePlay(); 
	struct PlayState//���ŵ���ر�־
	{
		bool isPlaied;//�Ƿ��Ѿ����Ź�һ��
		BYTE nowisfileno;//ѭ�������У������ǲ��ŵĵڼ����ļ�(��0��ʼ)
		bool isLoopFileEnd;//ѭ�����ε��ļ�����һ���Ѿ�����.
	}mPlayState, *pPlayState;	
	
	int Play(bool bStartplay = true);//�������
	int Open(LPCTSTR name1, LPCTSTR name2, bool Loop = true);
	int Open(LPCTSTR name, bool Loop = false);//�������ļ��������ڴ�	Ĭ��Ϊ���ŵ��ļ�
	
private:	
	int Play3(bool bStartplay3 = true);//�����˳�0  
	int Play2(bool bStartplay2 = true);
	int Play1(bool bStartplay1 = true);
	void Pause(bool bPause = true);
	int Close(int no = 0);//�ر������ļ���һЩ�ƺ���
	int Stop();//ֹͣ��������	
	//static
	static UINT CALLBACK SoundFun2_1(HWAVEOUT hWo1, UINT uMsg,
		DWORD dwInstance, DWORD dwwParam, DWORD dwlParam);
	static UINT CALLBACK SoundFun2_2(HWAVEOUT hWo2, UINT uMsg,
		DWORD dwInstance, DWORD dwwParam, DWORD dwlParam);
	static UINT CALLBACK SoundFun3(HWAVEOUT hWo3, UINT uMsg, DWORD dwInstance,
		DWORD dwwParam, DWORD dwlParam);
		
private:	
	HWAVEOUT hWaveOut;//Handle of the waveform-audio output device
	WAVEHDR WaveHead;//identify a waveform-audio buffer.	
	PCMWAVEFORMAT pFormat;//structure describes the data format for PCM waveform-audio data
	PCMWAVEFORMAT pFormat1;
	UINT32* lpData;//��һ��WAVEDATA�����ݽṹ������Ϊ���ظ�
	UINT32* lpData1;
	HGLOBAL hData;//Handle to a global memory block.
	HGLOBAL hData1;
	DWORD DataSize;//A 32-bit unsigned integerһ��������ʾ��ַ�����
	DWORD DataSize1;
	
	int OpenFlage;//���ƶ���Ĳ������ͱ�־
	BOOL bPlay;//�Ƿ񲥷Ź�
	bool once;//�������ļ�ֻװ��һ�α�־
	bool bPause;//�Ƿ��ݶ�
	bool the3Flage;
	bool the2Flage1;
	bool the2Flage2;
	
	bool bPlay2Pause;
	bool bPlay3Pause;
};

#endif // !defined(AFX_SUN_H__BB7077B6_B748_43B2_A00C_8397CE4CD0C2__INCLUDED_)
