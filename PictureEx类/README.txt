1.BOOL Load(...) - 装载一个GIF并为显示准备一个对象

2.BOOL Draw() - 显示GIF或GIF动画

3.void Stop() - 停止动画

4.void UnLoad() - 停止动画并释放所有资源

5.void SetBkColor(COLORREF) - 设定背景颜色(透明区域)

6.COLORREF GetBkColor() - 取得背景颜色

7.BOOL IsGIF() - 判断是否GIF

8.BOOL IsAnimatedGIF() - 判断是否GIF动画

9.BOOL IsPlaying() - 判断动画是否在播放中

10.SIZE GetSize() - 获取当前图的尺寸

11.int GetFrameCount() - 获取当前动画的帧数

12.BOOL GetPaintRect(RECT *lpRect) - 获取当前绘图区域

13.BOOL SetPaintRect(const RECT *lpRect) - 设定绘图区域

14.CPictureEx[Wnd]::Load有三个版本的重载函数：

15.BOOL Load(LPCTSTR szFileName);//此版本重载函数的参数是图的路径，返回值是成功已否的布尔值.


16.BOOL Load(HGLOBAL hGlobal, DWORD dwSize);//此版本的参数是GlobalAlloc使用GMEM_MOVABLE标记分配的全局内存块的句柄及其大小。该函数不会释放内存，所以记得用GlobalFree释放内存。



17.BOOL Load(LPCTSTR szResourceName,LPCTSTR szResourceType);//此版本从资源中获取图片名和图片类型。例如：
? ?m_Picture.Load(MAKEINTRESOURCE(IDR_MYPIC),_T("GIFTYPE"));