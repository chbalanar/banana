一般使用方法为：
在.h中声明CWavePlay* 指针，在需要的地方new CWavePlay();

注：此方法为了让CWavePlay对象不随消息释放，由自己指定什么时候释放(一般为程序结束或对话框消失时释放)


open(str1,loop=false);	//loop控制循环播放，默认为不循环播放,str为要打开的文件
open(str1,str2,loop=true);	//未理解使用

play(bStartplay);  为声音输出函数
virtual ~CWavePlay();	//未理解使用
