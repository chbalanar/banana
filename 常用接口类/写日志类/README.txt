virtual close()					关闭log文件
BOOL Initialize(log文件名)			打开/创建log文件,并写入开始时间
virtual WriteLogFileString(要写入的字符串)	写入信息
m_bOpenFlag					开关标记

备注：写入结束时间在类的析构中,不要使用无参构造。
