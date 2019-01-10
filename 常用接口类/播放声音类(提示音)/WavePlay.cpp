// Sun.cpp: implementation of the WavePlay class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WavePlay.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CWavePlay::CWavePlay()
{
	the3Flage = false;
	the2Flage1 = false;
	the2Flage2 = false;
	mPlayState.isPlaied = false;
	mPlayState.isLoopFileEnd = false;
	mPlayState.nowisfileno = 0;
	once = false;
	bPlay = false;
	DataSize = 0;
	DataSize1 = 0;
	hData = 0;
	hData1 = 0;
	hWaveOut = NULL;
	lpData = NULL;
	lpData1 = NULL;
	OpenFlage = 0;
	bPause = false;
	bPlay2Pause = false;
	bPlay3Pause = false;
}

CWavePlay::~CWavePlay()
{
	if (1 == OpenFlage || 3 == OpenFlage)//无重放的文件
	{
		if (hWaveOut)
		{
			waveOutPause(hWaveOut);
//			waveOutReset(hWaveOut);
			waveOutClose(hWaveOut);
			waveOutUnprepareHeader(hWaveOut, &WaveHead, sizeof(WAVEHDR));
		}
		if (lpData)
		{
			LocalUnlock(hData);
			lpData = NULL;
		}
		if (hData)
		{
			GlobalFree(hData);
			hData = NULL;
		}
		OpenFlage = 0;
		DataSize = 0;
	}
	else if (2 == OpenFlage)//有重放的文件
	{
		if (hWaveOut)
		{
			waveOutReset(hWaveOut);
			waveOutClose(hWaveOut);
			waveOutUnprepareHeader(hWaveOut, &WaveHead, sizeof(WAVEHDR));
		}
		if (lpData)
		{
			LocalUnlock(hData);
			lpData = NULL;
		}
		if (hData)
		{
			GlobalFree(hData);
			hData = NULL;
		}
		DataSize = 0;

		if (lpData1)
		{
			LocalUnlock(hData1);
			lpData1 = NULL;
		}
		if (hData1)
		{
			GlobalFree(hData1);
			hData1 = NULL;
		}
	
		OpenFlage = 0;
		DataSize1 = 0;

	}
	once = false;
	bPlay = false;
	//Sleep(50);
	//m_plogText->WriteLogFileString( "WavePlay::~WavePlay()\r" );
}

int CWavePlay::Open(LPCTSTR name, bool Loop)
{
	if (OpenFlage)//如果已经打开过了就返回 OpenFlage=1 or OpenFlage=3
	{
		TRACE(_T("FILE IS OPEND!\n"));
		return -1;
	}

	HMMIO hMmio = NULL;
	MMCKINFO pinfo;
	MMCKINFO cinfo;

	if (hMmio)
		Close();

	//打开WAV文件，返回一个HMMIO句柄
	hMmio = mmioOpen((LPTSTR) name, NULL, MMIO_READ);
	if (!hMmio)
		return FALSE;

	if (Loop)
		OpenFlage = 3;//第三种情况 单个循环
	else
		OpenFlage = 1;//不循环情况

	//查找父块“wave”
	pinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	if (mmioDescend(hMmio, &pinfo, NULL, MMIO_FINDRIFF))
		goto FALSE_END;

	//查找子块"fmt"    parent"riff"
	cinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (mmioDescend(hMmio, &cinfo, NULL, MMIO_FINDCHUNK))
		goto FALSE_END;

	mmioRead(hMmio, (LPSTR) & pFormat, sizeof(PCMWAVEFORMAT));//cinfo.cksize
	if (pFormat.wf.wFormatTag != WAVE_FORMAT_PCM)
		goto FALSE_END;

	//跳入块"FMT"
	mmioAscend(hMmio, &cinfo, 0);

	//查找数据块
	cinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (mmioDescend(hMmio, &cinfo, &pinfo, MMIO_FINDCHUNK))
		goto FALSE_END;
	DataSize = cinfo.cksize;

	//读取数据
	hData = GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, DataSize);
	lpData = (UINT32 *) GlobalLock(hData);
	if (!hData || !lpData)
		goto FALSE_END;
	if (mmioRead(hMmio, (HPSTR) lpData, DataSize) != (LRESULT) DataSize)
		goto FALSE_END;

	//close and return
	mmioClose(hMmio, MMIO_FHOPEN);
	return TRUE;

	FALSE_END:
	if (hMmio)
		mmioClose(hMmio, MMIO_FHOPEN);
	if (lpData)
		LocalUnlock(hData);
	if (hData)
	{
		GlobalFree(hData);
		hData = NULL;
	}
	memset(this, 0, sizeof(CWavePlay));
	return FALSE;
}


int CWavePlay::Play(bool bStartplay)
{
	if (1 == OpenFlage)//单个文件不重复播放
	{
		Play1(bStartplay);		
		return 1;
	}
	else if (2 == OpenFlage)//后一个文件重复播放
	{
		Play2(bStartplay);
		return 2;
	}
	else if (3 == OpenFlage)//单个文件重复播放
	{
		Play3(bStartplay);
		return 3;
	}
	else
	{	
		//TRACE(_T("没有打开文件, OpenFlag = %d\n"), OpenFlage );
		return -1;
	}
}




int CWavePlay::Stop()
{
	return !waveOutReset(hWaveOut);
}

int CWavePlay::Close(int no)
{
	if (1 == no)//重复中不重复的文件
	{
		if (lpData1 != NULL)
		{
			if (hWaveOut)
			{
				waveOutReset(hWaveOut);
				waveOutClose(hWaveOut);
				waveOutUnprepareHeader(hWaveOut, &WaveHead, sizeof(WAVEHDR));
			}
		}
		if (lpData1)
		{
			LocalUnlock(hData1);
			lpData1 = NULL;
		}
		if (hData1)
		{
			GlobalFree(hData1);
			hData1 = NULL;
		}
		return 1;
	}
	if (2 == no)//要重复的第二个文件
	{
		if (hWaveOut)
		{
			waveOutReset(hWaveOut);
			waveOutClose(hWaveOut);
			waveOutUnprepareHeader(hWaveOut, &WaveHead, sizeof(WAVEHDR));
		}		
		if (lpData1)
		{
			LocalUnlock(hData1);
			lpData1 = NULL;
		}		
		if (hData1)
		{
			GlobalFree(hData1);
			hData1 = NULL;
		}
		if (lpData)
		{
			LocalUnlock(hData);
			lpData = NULL;
		}		
		if (hData)
		{
			GlobalFree(hData);
			hData = NULL;
		}
		return 2;
	}
	//if(3 == no || 1 == no)
	{
		if (hWaveOut)
		{
			waveOutReset(hWaveOut);
			waveOutClose(hWaveOut);
			waveOutUnprepareHeader(hWaveOut, &WaveHead, sizeof(WAVEHDR));
		}		
		if (lpData)
		{
			LocalUnlock(hData);
			lpData = NULL;
		}		
		if (hData)
		{
			GlobalFree(hData);
			hData = NULL;
		}
		if (lpData1)
		{
			LocalUnlock(hData1);
			lpData1 = NULL;
		}		
		if (hData1)
		{
			GlobalFree(hData1);
			hData1 = NULL;
		}
		return 3;
	}

	return 0;
}



int CWavePlay::Open(LPCTSTR name1, LPCTSTR name2, bool Loop)
{
	if (OpenFlage == 2)//如果已经打开过了就返回
	{
		TRACE(_T("FILE IS OPEND!\n"));
		return 2;
	}
	
	HMMIO hMmio = NULL;
	MMCKINFO pinfo;
	MMCKINFO cinfo;

	if (hMmio)
		Close();

	//打开WAV文件1，返回一个HMMIO句柄
	hMmio = mmioOpen((LPTSTR) name1, NULL, MMIO_READ);
	if (!hMmio)
		return FALSE;
	OpenFlage = 1;

	//查找父块“wave”
	pinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	if (mmioDescend(hMmio, &pinfo, NULL, MMIO_FINDRIFF))
		goto FALSE_END1;

	//查找子块"fmt"    parent"riff"
	cinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (mmioDescend(hMmio, &cinfo, NULL, MMIO_FINDCHUNK))
		goto FALSE_END1;

	mmioRead(hMmio, (LPSTR) & pFormat, sizeof(PCMWAVEFORMAT));//cinfo.cksize
	if (pFormat.wf.wFormatTag != WAVE_FORMAT_PCM)
		goto FALSE_END1;

	//跳入块"FMT"
	mmioAscend(hMmio, &cinfo, 0);

	//查找数据块
	cinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (mmioDescend(hMmio, &cinfo, &pinfo, MMIO_FINDCHUNK))
		goto FALSE_END1; 
	DataSize = cinfo.cksize;

	//读取数据
	hData = GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, DataSize);
	lpData = (UINT32 *) GlobalLock(hData);
	if (!hData || !lpData)
		goto FALSE_END1;
	if (mmioRead(hMmio, (HPSTR) lpData, DataSize) != (LRESULT) DataSize)
		goto FALSE_END1;

	//close and return
	mmioClose(hMmio, MMIO_FHOPEN);

	//+++++++++++++++++++++++++++
	//打开下一个WAV文件，返回一个HMMIO句柄
	hMmio = mmioOpen((LPTSTR) name2, NULL, MMIO_READ);
	if (!hMmio)
		return FALSE;
	OpenFlage = 2;

	//查找父块“wave”
	pinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	if (mmioDescend(hMmio, &pinfo, NULL, MMIO_FINDRIFF))
		goto FALSE_END2;

	//查找子块"fmt"    parent"riff"
	cinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (mmioDescend(hMmio, &cinfo, NULL, MMIO_FINDCHUNK))
		goto FALSE_END2;

	mmioRead(hMmio, (LPSTR) & pFormat1, sizeof(PCMWAVEFORMAT));//cinfo.cksize
	if (pFormat1.wf.wFormatTag != WAVE_FORMAT_PCM)
		goto FALSE_END2;

	//跳入块"FMT"
	mmioAscend(hMmio, &cinfo, 0);

	//查找数据块
	cinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if (mmioDescend(hMmio, &cinfo, &pinfo, MMIO_FINDCHUNK))
		goto FALSE_END2;
	DataSize1 = cinfo.cksize;

	//读取数据
	hData1 = GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, DataSize1);
	lpData1 = (UINT32 *) GlobalLock(hData1);
	if (!hData1 || !lpData1)
		goto FALSE_END2;
	if (mmioRead(hMmio, (HPSTR) lpData1, DataSize1) != (LRESULT) DataSize1)
		goto FALSE_END2;

	//close and return
	mmioClose(hMmio, MMIO_FHOPEN);
	return TRUE;

	FALSE_END1:
	if (hMmio)
		mmioClose(hMmio, MMIO_FHOPEN);
	if (lpData1)
		LocalUnlock(hData1);
	if (hData1)
	{
		GlobalFree(hData1);
		hData1 = NULL;
	}
	memset(this, 0, sizeof(CWavePlay));
	return FALSE;

	FALSE_END2:
	if (hMmio)
		mmioClose(hMmio, MMIO_FHOPEN);
	if (lpData1)
		LocalUnlock(hData1);
	if (hData1)
	{
		GlobalFree(hData1);
		hData1 = NULL;
	}
	memset(this, 0, sizeof(CWavePlay));
	return FALSE;

	return 0;
}
int CWavePlay::Play1(bool bStartplay1)//不重复文件播放
{
	if (!bStartplay1)
	{
		bPlay = false;
		mPlayState.isPlaied = false;
		if(hWaveOut != NULL)
		{
			waveOutPause(hWaveOut);
			waveOutReset(hWaveOut);
		}
		return 0;
		//return Close();
	}

	if (bPlay)
		return 0;
	if (!once)//数据只载入一次
	{
		once = true;
		if (waveOutOpen(NULL, WAVE_MAPPER, (WAVEFORMATEX *) &pFormat, NULL,
				NULL, WAVE_FORMAT_QUERY))
			return Close();
		if (waveOutOpen(&hWaveOut, WAVE_MAPPER, (WAVEFORMATEX *) &pFormat, 0,
				0, CALLBACK_NULL))
			return Close();	

		WaveHead.lpData = (LPSTR) lpData;
		WaveHead.dwBufferLength = DataSize;
		WaveHead.dwFlags = 0L;
		WaveHead.dwLoops = 0L;	

		if (waveOutPrepareHeader(hWaveOut, &WaveHead, sizeof(WAVEHDR)))
			return Close();
	}

	if (waveOutWrite(hWaveOut, &WaveHead, sizeof(WAVEHDR)))
		return Close();

	bPlay = true;//表示已经正在播放了	
	mPlayState.isPlaied = true;//外部标志，表示已经播放过了。

	return 0;
}

int CWavePlay::Play2(bool bStartplay2)//前一个文件不重复,后一个文件重复
{
	MMRESULT mmr;
	//static bool bPlay2Pause = false;	
	if (!bStartplay2)
	{
		if (!once)//第一次进入(false)
		{
			return 0;	
		}
		else
		{	
			if (bPlay2Pause)
				return 0;
			if (hWaveOut != NULL)
			{			
				waveOutPause(hWaveOut);			
			}
			bPlay2Pause = true;			
		}
		the2Flage1 = false;
		the2Flage2 = false;
		bPlay = false;		
		mPlayState.isPlaied = false;
		//return Close();
		return 0;
	}
	else
	{
		if (bPlay2Pause)
		{
			if (hWaveOut != NULL)
			{
				mmr = waveOutReset(hWaveOut);
				if (mmr == MMSYSERR_NOERROR)
					TRACE(_T("reset OK1!\n"));
				else
					TRACE(_T("error1!\n"));
				mmr = MMSYSERR_INVALHANDLE;
			}
			once = false;
			bPlay2Pause = false;
		}
	}

	if (!once)//播放前一个不重复播放的文件
	{
		if (hWaveOut != NULL)
		{
			mmr = waveOutReset(hWaveOut);
			if (mmr == MMSYSERR_NOERROR)
				TRACE(_T("reset OK2!\n"));
			else
				TRACE(_T("error2!\n"));
			mmr = MMSYSERR_INVALHANDLE;
		}
		once = true;		
		//检测系统播放功能
		if (waveOutOpen(NULL, WAVE_MAPPER, (WAVEFORMATEX *) &pFormat, NULL,
			NULL, WAVE_FORMAT_QUERY))
			return Close();
		if (waveOutOpen(&hWaveOut, WAVE_MAPPER, (WAVEFORMATEX *) &pFormat,
			(DWORD) SoundFun2_1, (DWORD)this, CALLBACK_FUNCTION))
			return Close();	
		WaveHead.lpData = (LPSTR) lpData;
		WaveHead.dwBufferLength = DataSize;
		WaveHead.dwFlags = 0L;
		WaveHead.dwLoops = 0L;	
		//往WAV设备中添加数据
		if (waveOutPrepareHeader(hWaveOut, &WaveHead, sizeof(WAVEHDR)))
			return Close();
	}
	if (!the2Flage1)//第一个文件还没有播放
	{
		if (waveOutWrite(hWaveOut, &WaveHead, sizeof(WAVEHDR)))
			return Close();
		bPlay = true;
		the2Flage1 = true;
	}

	if (the2Flage2)//播放后一个文件
	{		
		WaveHead.lpData = (LPSTR) lpData1;
		WaveHead.dwBufferLength = DataSize1;
		WaveHead.dwFlags = 0L;
		WaveHead.dwLoops = 0L;	
		//往WAV设备中添加数据
		if (waveOutPrepareHeader(hWaveOut, &WaveHead, sizeof(WAVEHDR)))
			return Close();
		waveOutWrite(hWaveOut, &WaveHead, sizeof(WAVEHDR));
		the2Flage2 = false;
	}
	return 0;
}

UINT CALLBACK CWavePlay::SoundFun2_1(HWAVEOUT hWo1, UINT uMsg, DWORD dwInstance,
	DWORD dwwParam, DWORD dwlParam)
{
	CWavePlay *p = (CWavePlay *)dwInstance;
	LPWAVEHDR lpWaveHdr = (LPWAVEHDR) dwwParam;
	if (WOM_DONE == uMsg && p->the2Flage1)
	{	
		p->the2Flage2 = true;		
	}
	return 0;
}

UINT CALLBACK CWavePlay::SoundFun2_2(HWAVEOUT hWo2, UINT uMsg, DWORD dwInstance,
	DWORD dwwParam, DWORD dwlParam)
{	
	LPWAVEHDR lpWaveHdr = (LPWAVEHDR) dwwParam;
	if (WOM_DONE == uMsg )
	{
		waveOutWrite(hWo2, lpWaveHdr, sizeof(WAVEHDR));
	}	
	return 0;
}


int CWavePlay::Play3(bool bStartplay3)//单个文件重复
{
	MMRESULT mmr;
	//static bool bnowPause = false;
	if (!bStartplay3)//让它停止(如已经播放设置标志bnowPause)
	{
		bPlay = false;
		the3Flage = false;	
		if (once)
		{
			waveOutPause(hWaveOut);
			bPlay3Pause = true;
			//bnowPause = true;
		}
		
		//return Close();
		return 0;
	}	
	else
	{
		if (bPlay3Pause)
		//if (bnowPause)
		{
			if (hWaveOut != NULL)
			{
				mmr = waveOutReset(hWaveOut);
				if (mmr == MMSYSERR_NOERROR)
					TRACE(_T("reset OK play3!\n"));
				else
					TRACE(_T("error play3!\n"));
				mmr = MMSYSERR_INVALHANDLE;
			}
			bPlay3Pause = false;
			//bnowPause = false;
			once = false;
		}
	}
	if (bPlay)
		return 0;
	if (!once)// bStartplay3)
	{
		once = true;
		//检测系统播放功能
		if (waveOutOpen(NULL, WAVE_MAPPER, (WAVEFORMATEX *) &pFormat, NULL,
				NULL, WAVE_FORMAT_QUERY))
			return Close();
		if (waveOutOpen(&hWaveOut, WAVE_MAPPER, (WAVEFORMATEX *) &pFormat,
				(DWORD) SoundFun3, (DWORD)this, CALLBACK_FUNCTION))
			return Close();	
		WaveHead.lpData = (LPSTR) lpData;
		WaveHead.dwBufferLength = DataSize;
		WaveHead.dwFlags = 0L;
		WaveHead.dwLoops = 0L;	
		if (waveOutPrepareHeader(hWaveOut, &WaveHead, sizeof(WAVEHDR)))
			return Close();
	}
	
	if (!the3Flage)
	{	
		if (waveOutWrite(hWaveOut, &WaveHead, sizeof(WAVEHDR)))
			return Close();
		bPlay = true;//表示已经播放和循环了		
		the3Flage = true;
	}
	return 0;
}

UINT CALLBACK CWavePlay::SoundFun3(HWAVEOUT hWo1, UINT uMsg, DWORD dwInstance,
	DWORD dwwParam, DWORD dwlParam)
{
	CWavePlay *p = (CWavePlay *)dwInstance;
	LPWAVEHDR lpWaveHdr = (LPWAVEHDR) dwwParam;	
	if (WOM_DONE == uMsg && p->the3Flage)
	{
		waveOutWrite(hWo1, lpWaveHdr, sizeof(WAVEHDR));
	}	
	return 0;
}

void CWavePlay::Pause(bool bPause)
{
	if (bPause)
	{
		if (hWaveOut != NULL)
		{
			waveOutPause(hWaveOut);
		}
		else
		{
			TRACE(_T("没有工作的线程"), MB_OK);
		}
		bPause = true;
	}
	else
	{
		if (bPause)
		{
			waveOutRestart(hWaveOut);
			bPause = false;
		}
	}
}
