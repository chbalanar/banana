// Sun.h: interface for the WavePlay class.
//
//////////////////////////////////////////////////////////////////////
/*
打开声音文件的三种开式
1、只播放一次的文件：
zd.Open("names.ex",false)  或者 zd.Open("names.ex")
2、前一个文件播放完了，后一个文件循环播放
pxj.Open("names1.ex","names2.ex",true)  或者 pxj.Open("names1.ex","names2.ex")
3、一个文件循环播放
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
#include "mmsystem.h"//音频设备和声音文件操作函数的声明包含在mmsystem.h头文件中
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
	struct PlayState//播放的相关标志
	{
		bool isPlaied;//是否已经播放过一次
		BYTE nowisfileno;//循环播放中，现在是播放的第几个文件(从0开始)
		bool isLoopFileEnd;//循环两次的文件，第一次已经完了.
	}mPlayState, *pPlayState;	
	
	int Play(bool bStartplay = true);//声音输出
	int Open(LPCTSTR name1, LPCTSTR name2, bool Loop = true);
	int Open(LPCTSTR name, bool Loop = false);//打开声音文件和载入内存	默认为播放单文件
	
private:	
	int Play3(bool bStartplay3 = true);//正常退出0  
	int Play2(bool bStartplay2 = true);
	int Play1(bool bStartplay1 = true);
	void Pause(bool bPause = true);
	int Close(int no = 0);//关闭声音文件和一些善后工作
	int Stop();//停止所有声音	
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
	UINT32* lpData;//有一个WAVEDATA的数据结构，所以为不重复
	UINT32* lpData1;
	HGLOBAL hData;//Handle to a global memory block.
	HGLOBAL hData1;
	DWORD DataSize;//A 32-bit unsigned integer一般用来表示地址或参数
	DWORD DataSize1;
	
	int OpenFlage;//控制对象的播放类型标志
	BOOL bPlay;//是否播放过
	bool once;//让声音文件只装载一次标志
	bool bPause;//是否暂定
	bool the3Flage;
	bool the2Flage1;
	bool the2Flage2;
	
	bool bPlay2Pause;
	bool bPlay3Pause;
};

#endif // !defined(AFX_SUN_H__BB7077B6_B748_43B2_A00C_8397CE4CD0C2__INCLUDED_)
