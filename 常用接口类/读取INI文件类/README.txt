读取值
ReadString
ReadBool
ReadInt
ReadByte
ReadFloat
(节名，项名，默认返回值)

写入值
WriteString
WriteBool
WriteInt
WriteFloat
(节名，项名，写入的值)

BOOL IsExistFile();		//判读文件是否存在 
BOOL HasWritePermission();	//判断文件是否可写

GetAllSections(CStringArray数组)		//获取所有节名
GetAllItems(节名，CStringArray数组)		//获取所有项名

ReadSection(节名，CStringList链表)		//获取所有项名

删除
DelSection_Key	
DelSection_AllKey

备注：不要使用无参构造
