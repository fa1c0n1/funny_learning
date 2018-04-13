		.data

;定义结构体
CONTACTSTRUCT struct
	szName BYTE 33 dup(0)       ;名字
	szPhNumber BYTE 12 dup(0)   ;电话号码
CONTACTSTRUCT ends
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
PCONTACKSTRUCT TYPEDEF PTR CONTACTSTRUCT      ;取别名(指针类型)
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

;声明全局变量
g_stContacts       CONTACTSTRUCT 100 dup (<'0'>)     ;定义结构体数组
g_nCount           DWORD 0                           ;元素个数
g_nCountMax        DWORD 100                         ;最大存放个数
g_strTmpContact    CONTACTSTRUCT  <'0','0'>         ;接收输入信息
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;定义格式控制符,用于输入输出时
g_szScanfFmt           BYTE '%s %s',0
g_szPrnFmt             BYTE '┃%-10s┃%-20s┃',0ah,0dh,0
g_szScanfNameFmt       BYTE '%s',0
g_szScanfNewNumFmt     BYTE '%s',0
g_szPrnPhNumberFmt     BYTE '该联系人的号码：%s',0ah,0dh,0
g_szScanfChoiceFmt     BYTE '%d',0
g_szCountFmt           BYTE '一共有 %d 个联系人.',0ah,0dh,0
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;定义提示
g_szAddstr             BYTE '请输入要添加的联系人信息(格式:姓名 号码): ',0
g_szFindStr            BYTE '请输入要查询的联系人姓名: ',0
g_szUpdateStr1         BYTE '请输入要修改的联系人姓名: ',0
g_szUpdateStr2         BYTE '请输入新的号码：',0 
g_szRmStr              BYTE '请输入要删除的联系人的姓名：',0
g_szRmOK               BYTE '删除成功',0ah,0dh,0
g_szSaveOK             BYTE '保存成功',0ah,0dh,0
g_szNotFound           BYTE '未找到该联系人的信息!',0ah,0dh,0
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;菜单
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
g_szLine               BYTE ' ━━━━━━━━━',0ah,0dh,0
g_szMenu               BYTE '┃1.添加联系人    ┃',0ah,0dh,
                            '┃2.查询联系人    ┃',0ah,0dh,
                            '┃3.更新联系人    ┃',0ah,0dh,
                            '┃4.删除联系人    ┃',0ah,0dh,
                            '┃5.打印所有联系人┃',0ah,0dh,
                            '┃6.保存电话本    ┃',0ah,0dh,
                            '┃7.退出程序      ┃',0ah,0dh,0

g_szChoice             BYTE 0ah,0dh,'请选择(1-6): ',0
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
g_szCmdCleanScreen     BYTE 'cls',0                         ;清屏命令
g_szPhNumberFile       BYTE 'PhoneNumberBook.txt',0         ;电话本文件名
g_szFileOpenMode       BYTE 'wb+',0                         ;保存文件时使用的打开模式
g_szFileReadMode       BYTE 'rb',0                          ;读取文件时使用的打开模式
