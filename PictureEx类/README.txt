1.BOOL Load(...) - װ��һ��GIF��Ϊ��ʾ׼��һ������

2.BOOL Draw() - ��ʾGIF��GIF����

3.void Stop() - ֹͣ����

4.void UnLoad() - ֹͣ�������ͷ�������Դ

5.void SetBkColor(COLORREF) - �趨������ɫ(͸������)

6.COLORREF GetBkColor() - ȡ�ñ�����ɫ

7.BOOL IsGIF() - �ж��Ƿ�GIF

8.BOOL IsAnimatedGIF() - �ж��Ƿ�GIF����

9.BOOL IsPlaying() - �ж϶����Ƿ��ڲ�����

10.SIZE GetSize() - ��ȡ��ǰͼ�ĳߴ�

11.int GetFrameCount() - ��ȡ��ǰ������֡��

12.BOOL GetPaintRect(RECT *lpRect) - ��ȡ��ǰ��ͼ����

13.BOOL SetPaintRect(const RECT *lpRect) - �趨��ͼ����

14.CPictureEx[Wnd]::Load�������汾�����غ�����

15.BOOL Load(LPCTSTR szFileName);//�˰汾���غ����Ĳ�����ͼ��·��������ֵ�ǳɹ��ѷ�Ĳ���ֵ.


16.BOOL Load(HGLOBAL hGlobal, DWORD dwSize);//�˰汾�Ĳ�����GlobalAllocʹ��GMEM_MOVABLE��Ƿ����ȫ���ڴ��ľ�������С���ú��������ͷ��ڴ棬���Լǵ���GlobalFree�ͷ��ڴ档



17.BOOL Load(LPCTSTR szResourceName,LPCTSTR szResourceType);//�˰汾����Դ�л�ȡͼƬ����ͼƬ���͡����磺
? ?m_Picture.Load(MAKEINTRESOURCE(IDR_MYPIC),_T("GIFTYPE"));