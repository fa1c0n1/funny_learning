		.386
		.model flat,stdcall
		option casemap:none
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 项目：电话号码本
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
 
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 头文件
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
include windows.inc
include msvcrt.inc
includelib msvcrt.lib
include user32.inc
includelib user32.lib
include kernel32.inc
includelib kernel32.lib
include PhoneNumberBook.inc

;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; 代码区
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.code

;>>>>>>>>>>>>>>>>>>>>>>>>>>>添加联系人>>>>>>>>>>>>>>>>>>>>>>>>
AddUser proc            ;无参数
	push eax
	push ebx
	push ecx

	lea eax,g_szAddstr
	push eax
	call crt_printf
	add esp,4

	;根据 ecx 的值找到下一个结构体名字数组的地址
	lea esi,[g_stContacts]              ;保存数据的结构体数组
	mov ecx,g_nCount                    ;获取当前以插入的用户个数
	mov eax,sizeof(CONTACTSTRUCT)       ;计算结构体的大小
	imul eax,ecx
	add esi,eax                         ;移动结构体数组的指针

	;调 crt_scanf 函数接收输入的数据
	lea eax,[esi+CONTACTSTRUCT.szPhNumber]
	lea ebx,[esi+CONTACTSTRUCT.szName]
	push eax
	push ebx 
	push offset g_szScanfFmt
	call crt_scanf
	add esp,12
	inc g_nCount

	pop ecx
	pop ebx
	pop eax
	ret
AddUser endp

;>>>>>>>>>>>>>>>>>>>>>>>>>>>查询联系人信息>>>>>>>>>>>>>>>>>>>>>>
FindData proc            ;无参数
	push eax
	push ebx
	push ecx
	push edi
	push esi

	lea eax,[g_szFindStr]
	push eax
	call crt_printf
	add esp,4

	;1.输入数据
	;先清空缓冲区
	mov ebx,sizeof(CONTACTSTRUCT)
	push ebx                           ;crt_memset  参数3
	push 0                             ;crt_memset  参数2
	lea edi,[g_strTmpContact.szName]
	push edi                           ;crt_memset  参数1
	call crt_memset
	add esp,12

	push edi
	push offset g_szScanfNameFmt
	call crt_scanf
	add esp,8

	;2.开始查询
	mov ecx,0
LOOP_MARK:
	cmp ecx,g_nCount
	jnz FIND_MARK
	push eax                       
	push ecx                       ;保存 eax,ecx，crt_printf会改变这两个寄存器
	push offset g_szNotFound
	call crt_printf
	add esp,4
	pop ecx                        ;恢复 eax,ecx
	pop eax                    
	jmp FINISH

FIND_MARK:
	;2.1根据 ecx 的值找到下一个结构体的地址
	lea esi,[g_stContacts]
	lea edi,[g_strTmpContact.szName]
	mov eax,sizeof(CONTACTSTRUCT)
	imul eax,ecx
	add esi,eax

	;2.2比较字符串
	mov eax,ecx              ;保存外层循环的次数
	mov ecx,8                ;初始化串操作的循环次数(4字节比较)                ;
	repe cmpsd
	je OUTPUT_MARK
	mov ecx,eax              ;如果没有找到则继续外层循环
	inc ecx
	jmp LOOP_MARK

OUTPUT_MARK:
	;输出信息
	mov ecx,eax
	lea esi,[g_stContacts]
	mov ebx,sizeof(CONTACTSTRUCT)
	imul ebx,ecx
	add esi,ebx
	lea eax,[esi+CONTACTSTRUCT.szPhNumber]
	push eax
	push offset g_szPrnPhNumberFmt
	call crt_printf
	add esp,8

FINISH:
	pop esi
	pop edi
	pop ecx
	pop ebx
	pop eax
	ret
FindData endp

;>>>>>>>>>>>>>>>>>>>>>>>>>>>修改联系人信息>>>>>>>>>>>>>>>>>>>>
UpdateData proc          ;无参数
	push eax
	push ebx
	push ecx
	push esi
	push edi

	lea eax,[g_szUpdateStr1]
	push eax
	call crt_printf
	add esp,4

	;1.输入数据
	;先清空缓冲区
	mov ebx,sizeof(CONTACTSTRUCT)
	push ebx                           ;crt_memset  参数3
	push 0                             ;crt_memset  参数2
	lea edi,[g_strTmpContact.szName]
	push edi                           ;crt_memset  参数1
	call crt_memset
	add esp,12

	push edi
	push offset g_szScanfNameFmt
	call crt_scanf
	add esp,8

	;2.开始查询
	mov ecx,0
LOOP_MARK:
	cmp ecx,g_nCount
	jnz FIND_MARK
	push eax                       
	push ecx                       ;保存 eax,ecx，crt_printf会改变这两个寄存器
	push offset g_szNotFound
	call crt_printf
	add esp,4
	pop ecx                        ;恢复 eax,ecx
	pop eax                    
	jmp FINISH

FIND_MARK:
	;2.1根据 ecx 的值找到下一个结构体的地址
	lea esi,[g_stContacts]
	lea edi,[g_strTmpContact.szName]
	mov eax,sizeof(CONTACTSTRUCT)
	imul eax,ecx
	add esi,eax

	;2.2比较字符串
	mov eax,ecx              ;保存外层循环的次数
	mov ecx,8                ;初始化串操作的循环次数(4字节比较)                ;
	repe cmpsd
	je UPDATE_MARK
	mov ecx,eax              ;如果没有找到则继续外层循环
	inc ecx
	jmp LOOP_MARK

UPDATE_MARK:
	mov ebx,eax              ;先保存查询到的联系人在结构体中的索引,
	                         ;  因为 crt_printf 会改变eax的值
	push offset g_szUpdateStr2
	call crt_printf
	add esp,4
	mov eax,ebx

	;修改信息
	mov ecx,eax
	lea esi,[g_stContacts]
	mov ebx,sizeof(CONTACTSTRUCT)
	imul ebx,ecx
	add esi,ebx
	lea eax,[esi+CONTACTSTRUCT.szPhNumber]
	push eax
	push offset g_szScanfNewNumFmt
	call crt_scanf
	add esp,8

FINISH:
	pop edi
	pop esi
	pop ecx
	pop ebx
	pop eax
	ret
UpdateData endp

;>>>>>>>>>>>>>>>>>>>>>>>>>>>删除联系人>>>>>>>>>>>>>>>>>>>>>>>>
RemoveUser proc
	push eax
	push ebx
	push ecx
	push esi
	push edi

	push offset g_szRmStr
	call crt_printf
	add esp,4

	;1.输入数据
	;先清空缓冲区
	mov ebx,sizeof(CONTACTSTRUCT)
	push ebx                           ;crt_memset  参数3
	push 0                             ;crt_memset  参数2
	lea edi,[g_strTmpContact.szName]
	push edi                           ;crt_memset  参数1
	call crt_memset
	add esp,12

	push edi
	push offset g_szScanfNameFmt
	call crt_scanf
	add esp,8

	;2.开始查询
	mov ecx,0
LOOP_MARK:
	cmp ecx,g_nCount
	jnz FIND_MARK
	push eax                       
	push ecx                       ;保存 eax,ecx，crt_printf会改变这两个寄存器
	push offset g_szNotFound
	call crt_printf
	add esp,4
	pop ecx                        ;恢复 eax,ecx
	pop eax                    
	jmp FINISH

FIND_MARK:
	;2.1根据 ecx 的值找到下一个结构体的地址
	lea esi,[g_stContacts]
	lea edi,[g_strTmpContact.szName]
	mov eax,sizeof(CONTACTSTRUCT)
	imul eax,ecx
	add esi,eax

	;2.2比较字符串
	mov eax,ecx              ;保存外层循环的次数
	mov ecx,8                ;初始化串操作的循环次数(4字节比较)                ;
	repe cmpsd
	je REMOVE_MARK
	mov ecx,eax              ;如果没有找到则继续外层循环
	inc ecx
	jmp LOOP_MARK

REMOVE_MARK:
	;删除
	mov ecx,eax
	lea edi,[g_stContacts]
	mov ebx,sizeof(CONTACTSTRUCT)
	imul ebx,ecx
	add edi,ebx
	mov esi,edi              
	mov ebx,sizeof(CONTACTSTRUCT)
	add esi,ebx              ;esi 指向要删除的结构体在结构体数组中的的下一个结构体

	add ecx,1
	mov eax,g_nCount
	sub eax,ecx
	mov ebx,sizeof(CONTACTSTRUCT)
	imul ebx,eax
	mov ecx,ebx
	rep movsb

	;最后再删除最后一个结构体的信息
	mov ebx,sizeof(CONTACTSTRUCT)
	push ebx
	push 0
	push edi
	call crt_memset
	add esp,12
	dec g_nCount

	push offset g_szRmOK
	call crt_printf
	add esp,4

FINISH:
	pop edi
	pop esi
	pop ecx
	pop ebx
	pop eax
	ret
RemoveUser endp

;>>>>>>>>>>>>>>>>>>>>>打印所有联系人信息>>>>>>>>>>>>>>>>>>
ShowAllInfo proc
	push eax
	push ebx
	push ecx	
	push esi

	;先输出联系人数
	push g_nCount
	push offset g_szCountFmt
	call crt_printf
	add esp,8

	mov ecx,0

TRAVERSE:
	cmp ecx,g_nCount 
	je FINISH

	lea esi,[g_stContacts]
	mov ebx,sizeof(CONTACTSTRUCT)
	imul ebx,ecx
	add esi,ebx
	lea eax,[esi+CONTACTSTRUCT.szPhNumber]
	lea ebx,[esi+CONTACTSTRUCT.szName]
	push ecx       ;crt_printf 会改变 ecx，
	               ;  然而ecx是作为循环的计数器，所以先保存一下
	push eax
	push ebx
	push offset g_szPrnFmt
	call crt_printf
	add esp,12
	pop ecx
	inc ecx
	jmp TRAVERSE

FINISH:
	pop esi
	pop ecx
	pop ebx
	pop eax
	ret
ShowAllInfo endp

;>>>>>>>>>>>>>>>>>>>>>>>>>>>读取电话簿文件>>>>>>>>>>>>>>>>>>>>>>>>>>
LoadFromFile proc                 ;无参数
	push eax
	push ebx

	push offset g_szFileReadMode
	push offset g_szPhNumberFile
	call crt_fopen
	add esp,8

	;先读取联系人数
	push eax                      ;备份文件指针
	push eax                      ;crt_fread 参数4
	mov ebx,1
	push ebx                      ;crt_fread 参数3
	mov ebx,sizeof(DWORD)
	push ebx                      ;crt_fread 参数2
	lea ebx,[g_nCount]            
	push ebx                      ;crt_fread 参数1
	call crt_fread
	add esp,16
	pop eax                       ;恢复文件指针到 eax

	;循环读取联系人信息
	mov esi,0                       ;数组索引初始化为0
LOOP_LOAD_CONTACT:
	cmp esi,g_nCount
	je FINISH

	push eax                        ;备份文件指针
	push esi                        ;备份数组索引
	push eax                        ;crt_fread 参数4           
	mov ebx,1
	push ebx                        ;crt_fread 参数3
	mov ebx,sizeof(CONTACTSTRUCT)   
	push ebx                        ;crt_fread 参数2
	lea edi,[g_stContacts]
	imul ebx,esi
	add edi,ebx
	push edi                        ;crt_fread 参数1

	call crt_fread
	add esp,16
	pop esi                         ;恢复数组索引到 esi
	inc esi
	pop eax                         ;恢复文件指针到 eax
	jmp LOOP_LOAD_CONTACT

FINISH:
	push eax                      ;crt_fclose 参数1
	call crt_fclose
	add esp,4


	pop ebx
	pop eax
	ret
LoadFromFile endp

;>>>>>>>>>>>>>>>>>>>>>>>>>>>保存电话簿到文件>>>>>>>>>>>>>>>>>>>>>>>>>>
SaveToFile proc                   ;无参数
	push eax
	push ebx
	push esi
	push edi

	push offset g_szFileOpenMode
	push offset g_szPhNumberFile
	call crt_fopen
	add esp,8

	;先写入联系人数
	push eax                        ;备份文件指针
	push eax                        ;crt_fwrite 参数4
	mov ebx,1
	push ebx                        ;crt_fwrite 参数3
	mov ebx,sizeof(DWORD)           
	push ebx                        ;crt_fwrite 参数2
	lea ebx,[g_nCount]              
	push ebx                        ;crt_fwrite 参数1
	call crt_fwrite
	add esp,16
	pop eax                         ;恢复文件指针到 eax

	;循环写入联系人信息
	mov esi,0                       ;数组索引初始化为0
LOOP_SAVE_CONTACT:
	cmp esi,g_nCount
	je FINISH

	push eax                        ;备份文件指针
	push esi                        ;备份数组索引
	push eax                        ;crt_fwrite 参数4           
	mov ebx,1
	push ebx                        ;crt_fwrite 参数3
	mov ebx,sizeof(CONTACTSTRUCT)   
	push ebx                        ;crt_fwrite 参数2
	lea edi,[g_stContacts]
	imul ebx,esi
	add edi,ebx
	push edi                        ;crt_fwrite 参数1

	call crt_fwrite
	add esp,16
	pop esi                         ;恢复数组索引到 esi
	inc esi
	pop eax                         ;恢复文件指针到 eax
	jmp LOOP_SAVE_CONTACT

FINISH:
	push eax                        ;crt_fclose 参数1
	call crt_fclose
	add esp,4

	push offset g_szSaveOK
	call crt_printf
	add esp,4

	pop edi
	pop esi
	pop ebx
	pop eax
	ret
SaveToFile endp
;>>>>>>>>>>>>>>>>>>>>>>>>>>>清屏>>>>>>>>>>>>>>>>>>>>>>>>>>
CleanScreen proc                   ;无参数
	push offset g_szCmdCleanScreen
	call crt_system
	add esp,4
	ret
CleanScreen endp

;>>>>>>>>>>>>>>>>>>>>>>>>>>>主菜单>>>>>>>>>>>>>>>>>>>>>>>>
ShowMenu proc                       ;无参数
	local nChoice:DWORD

BEGIN:
	push offset g_szLine
	call crt_printf
	add esp,4

	push offset g_szMenu
	call crt_printf
	add esp,4

	push offset g_szLine
	call crt_printf
	add esp,4

	push offset g_szChoice
	call crt_printf
	add esp,4

	lea eax,[nChoice]
	push eax
	push offset g_szScanfChoiceFmt
	call crt_scanf
	add esp,8

	call CleanScreen
	cmp nChoice,1
	je ADD_USER
	cmp nChoice,2
	je FIND_DATA
	cmp nChoice,3
	je UPDATE_DATA
	cmp nChoice,4
	je REMOVE_DATA
	cmp nChoice,5
	je SHOW_ALL_INFO
	cmp nChoice,6
	je SAVE_TO_FILE
	cmp nChoice,7
	je ENDING

ADD_USER:
	call AddUser
	jmp BEGIN

FIND_DATA:
	call FindData
	jmp BEGIN

UPDATE_DATA:
	call UpdateData
	jmp BEGIN

REMOVE_DATA:
	call RemoveUser
	jmp BEGIN

SHOW_ALL_INFO:
	call ShowAllInfo 
	jmp BEGIN

SAVE_TO_FILE:
	call SaveToFile
	jmp BEGIN

ENDING:
	ret
ShowMenu endp


;>>>>>>>>>>>>>>>>>>>>>>>> 程序入口 >>>>>>>>>>>>>>>>>>>>>>>>
start:
	call LoadFromFile
	call ShowMenu
	ret
end start

