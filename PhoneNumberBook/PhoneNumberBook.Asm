		.386
		.model flat,stdcall
		option casemap:none
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; ��Ŀ���绰���뱾
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
 
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
; ͷ�ļ�
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
; ������
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		.code

;>>>>>>>>>>>>>>>>>>>>>>>>>>>������ϵ��>>>>>>>>>>>>>>>>>>>>>>>>
AddUser proc            ;�޲���
	push eax
	push ebx
	push ecx

	lea eax,g_szAddstr
	push eax
	call crt_printf
	add esp,4

	;���� ecx ��ֵ�ҵ���һ���ṹ����������ĵ�ַ
	lea esi,[g_stContacts]              ;�������ݵĽṹ������
	mov ecx,g_nCount                    ;��ȡ��ǰ�Բ�����û�����
	mov eax,sizeof(CONTACTSTRUCT)       ;����ṹ��Ĵ�С
	imul eax,ecx
	add esi,eax                         ;�ƶ��ṹ�������ָ��

	;�� crt_scanf �����������������
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

;>>>>>>>>>>>>>>>>>>>>>>>>>>>��ѯ��ϵ����Ϣ>>>>>>>>>>>>>>>>>>>>>>
FindData proc            ;�޲���
	push eax
	push ebx
	push ecx
	push edi
	push esi

	lea eax,[g_szFindStr]
	push eax
	call crt_printf
	add esp,4

	;1.��������
	;����ջ�����
	mov ebx,sizeof(CONTACTSTRUCT)
	push ebx                           ;crt_memset  ����3
	push 0                             ;crt_memset  ����2
	lea edi,[g_strTmpContact.szName]
	push edi                           ;crt_memset  ����1
	call crt_memset
	add esp,12

	push edi
	push offset g_szScanfNameFmt
	call crt_scanf
	add esp,8

	;2.��ʼ��ѯ
	mov ecx,0
LOOP_MARK:
	cmp ecx,g_nCount
	jnz FIND_MARK
	push eax                       
	push ecx                       ;���� eax,ecx��crt_printf��ı��������Ĵ���
	push offset g_szNotFound
	call crt_printf
	add esp,4
	pop ecx                        ;�ָ� eax,ecx
	pop eax                    
	jmp FINISH

FIND_MARK:
	;2.1���� ecx ��ֵ�ҵ���һ���ṹ��ĵ�ַ
	lea esi,[g_stContacts]
	lea edi,[g_strTmpContact.szName]
	mov eax,sizeof(CONTACTSTRUCT)
	imul eax,ecx
	add esi,eax

	;2.2�Ƚ��ַ���
	mov eax,ecx              ;�������ѭ���Ĵ���
	mov ecx,8                ;��ʼ����������ѭ������(4�ֽڱȽ�)                ;
	repe cmpsd
	je OUTPUT_MARK
	mov ecx,eax              ;���û���ҵ���������ѭ��
	inc ecx
	jmp LOOP_MARK

OUTPUT_MARK:
	;�����Ϣ
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

;>>>>>>>>>>>>>>>>>>>>>>>>>>>�޸���ϵ����Ϣ>>>>>>>>>>>>>>>>>>>>
UpdateData proc          ;�޲���
	push eax
	push ebx
	push ecx
	push esi
	push edi

	lea eax,[g_szUpdateStr1]
	push eax
	call crt_printf
	add esp,4

	;1.��������
	;����ջ�����
	mov ebx,sizeof(CONTACTSTRUCT)
	push ebx                           ;crt_memset  ����3
	push 0                             ;crt_memset  ����2
	lea edi,[g_strTmpContact.szName]
	push edi                           ;crt_memset  ����1
	call crt_memset
	add esp,12

	push edi
	push offset g_szScanfNameFmt
	call crt_scanf
	add esp,8

	;2.��ʼ��ѯ
	mov ecx,0
LOOP_MARK:
	cmp ecx,g_nCount
	jnz FIND_MARK
	push eax                       
	push ecx                       ;���� eax,ecx��crt_printf��ı��������Ĵ���
	push offset g_szNotFound
	call crt_printf
	add esp,4
	pop ecx                        ;�ָ� eax,ecx
	pop eax                    
	jmp FINISH

FIND_MARK:
	;2.1���� ecx ��ֵ�ҵ���һ���ṹ��ĵ�ַ
	lea esi,[g_stContacts]
	lea edi,[g_strTmpContact.szName]
	mov eax,sizeof(CONTACTSTRUCT)
	imul eax,ecx
	add esi,eax

	;2.2�Ƚ��ַ���
	mov eax,ecx              ;�������ѭ���Ĵ���
	mov ecx,8                ;��ʼ����������ѭ������(4�ֽڱȽ�)                ;
	repe cmpsd
	je UPDATE_MARK
	mov ecx,eax              ;���û���ҵ���������ѭ��
	inc ecx
	jmp LOOP_MARK

UPDATE_MARK:
	mov ebx,eax              ;�ȱ����ѯ������ϵ���ڽṹ���е�����,
	                         ;  ��Ϊ crt_printf ��ı�eax��ֵ
	push offset g_szUpdateStr2
	call crt_printf
	add esp,4
	mov eax,ebx

	;�޸���Ϣ
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

;>>>>>>>>>>>>>>>>>>>>>>>>>>>ɾ����ϵ��>>>>>>>>>>>>>>>>>>>>>>>>
RemoveUser proc
	push eax
	push ebx
	push ecx
	push esi
	push edi

	push offset g_szRmStr
	call crt_printf
	add esp,4

	;1.��������
	;����ջ�����
	mov ebx,sizeof(CONTACTSTRUCT)
	push ebx                           ;crt_memset  ����3
	push 0                             ;crt_memset  ����2
	lea edi,[g_strTmpContact.szName]
	push edi                           ;crt_memset  ����1
	call crt_memset
	add esp,12

	push edi
	push offset g_szScanfNameFmt
	call crt_scanf
	add esp,8

	;2.��ʼ��ѯ
	mov ecx,0
LOOP_MARK:
	cmp ecx,g_nCount
	jnz FIND_MARK
	push eax                       
	push ecx                       ;���� eax,ecx��crt_printf��ı��������Ĵ���
	push offset g_szNotFound
	call crt_printf
	add esp,4
	pop ecx                        ;�ָ� eax,ecx
	pop eax                    
	jmp FINISH

FIND_MARK:
	;2.1���� ecx ��ֵ�ҵ���һ���ṹ��ĵ�ַ
	lea esi,[g_stContacts]
	lea edi,[g_strTmpContact.szName]
	mov eax,sizeof(CONTACTSTRUCT)
	imul eax,ecx
	add esi,eax

	;2.2�Ƚ��ַ���
	mov eax,ecx              ;�������ѭ���Ĵ���
	mov ecx,8                ;��ʼ����������ѭ������(4�ֽڱȽ�)                ;
	repe cmpsd
	je REMOVE_MARK
	mov ecx,eax              ;���û���ҵ���������ѭ��
	inc ecx
	jmp LOOP_MARK

REMOVE_MARK:
	;ɾ��
	mov ecx,eax
	lea edi,[g_stContacts]
	mov ebx,sizeof(CONTACTSTRUCT)
	imul ebx,ecx
	add edi,ebx
	mov esi,edi              
	mov ebx,sizeof(CONTACTSTRUCT)
	add esi,ebx              ;esi ָ��Ҫɾ���Ľṹ���ڽṹ�������еĵ���һ���ṹ��

	add ecx,1
	mov eax,g_nCount
	sub eax,ecx
	mov ebx,sizeof(CONTACTSTRUCT)
	imul ebx,eax
	mov ecx,ebx
	rep movsb

	;�����ɾ�����һ���ṹ�����Ϣ
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

;>>>>>>>>>>>>>>>>>>>>>��ӡ������ϵ����Ϣ>>>>>>>>>>>>>>>>>>
ShowAllInfo proc
	push eax
	push ebx
	push ecx	
	push esi

	;�������ϵ����
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
	push ecx       ;crt_printf ��ı� ecx��
	               ;  Ȼ��ecx����Ϊѭ���ļ������������ȱ���һ��
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

;>>>>>>>>>>>>>>>>>>>>>>>>>>>��ȡ�绰���ļ�>>>>>>>>>>>>>>>>>>>>>>>>>>
LoadFromFile proc                 ;�޲���
	push eax
	push ebx

	push offset g_szFileReadMode
	push offset g_szPhNumberFile
	call crt_fopen
	add esp,8

	;�ȶ�ȡ��ϵ����
	push eax                      ;�����ļ�ָ��
	push eax                      ;crt_fread ����4
	mov ebx,1
	push ebx                      ;crt_fread ����3
	mov ebx,sizeof(DWORD)
	push ebx                      ;crt_fread ����2
	lea ebx,[g_nCount]            
	push ebx                      ;crt_fread ����1
	call crt_fread
	add esp,16
	pop eax                       ;�ָ��ļ�ָ�뵽 eax

	;ѭ����ȡ��ϵ����Ϣ
	mov esi,0                       ;����������ʼ��Ϊ0
LOOP_LOAD_CONTACT:
	cmp esi,g_nCount
	je FINISH

	push eax                        ;�����ļ�ָ��
	push esi                        ;������������
	push eax                        ;crt_fread ����4           
	mov ebx,1
	push ebx                        ;crt_fread ����3
	mov ebx,sizeof(CONTACTSTRUCT)   
	push ebx                        ;crt_fread ����2
	lea edi,[g_stContacts]
	imul ebx,esi
	add edi,ebx
	push edi                        ;crt_fread ����1

	call crt_fread
	add esp,16
	pop esi                         ;�ָ����������� esi
	inc esi
	pop eax                         ;�ָ��ļ�ָ�뵽 eax
	jmp LOOP_LOAD_CONTACT

FINISH:
	push eax                      ;crt_fclose ����1
	call crt_fclose
	add esp,4


	pop ebx
	pop eax
	ret
LoadFromFile endp

;>>>>>>>>>>>>>>>>>>>>>>>>>>>����绰�����ļ�>>>>>>>>>>>>>>>>>>>>>>>>>>
SaveToFile proc                   ;�޲���
	push eax
	push ebx
	push esi
	push edi

	push offset g_szFileOpenMode
	push offset g_szPhNumberFile
	call crt_fopen
	add esp,8

	;��д����ϵ����
	push eax                        ;�����ļ�ָ��
	push eax                        ;crt_fwrite ����4
	mov ebx,1
	push ebx                        ;crt_fwrite ����3
	mov ebx,sizeof(DWORD)           
	push ebx                        ;crt_fwrite ����2
	lea ebx,[g_nCount]              
	push ebx                        ;crt_fwrite ����1
	call crt_fwrite
	add esp,16
	pop eax                         ;�ָ��ļ�ָ�뵽 eax

	;ѭ��д����ϵ����Ϣ
	mov esi,0                       ;����������ʼ��Ϊ0
LOOP_SAVE_CONTACT:
	cmp esi,g_nCount
	je FINISH

	push eax                        ;�����ļ�ָ��
	push esi                        ;������������
	push eax                        ;crt_fwrite ����4           
	mov ebx,1
	push ebx                        ;crt_fwrite ����3
	mov ebx,sizeof(CONTACTSTRUCT)   
	push ebx                        ;crt_fwrite ����2
	lea edi,[g_stContacts]
	imul ebx,esi
	add edi,ebx
	push edi                        ;crt_fwrite ����1

	call crt_fwrite
	add esp,16
	pop esi                         ;�ָ����������� esi
	inc esi
	pop eax                         ;�ָ��ļ�ָ�뵽 eax
	jmp LOOP_SAVE_CONTACT

FINISH:
	push eax                        ;crt_fclose ����1
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
;>>>>>>>>>>>>>>>>>>>>>>>>>>>����>>>>>>>>>>>>>>>>>>>>>>>>>>
CleanScreen proc                   ;�޲���
	push offset g_szCmdCleanScreen
	call crt_system
	add esp,4
	ret
CleanScreen endp

;>>>>>>>>>>>>>>>>>>>>>>>>>>>���˵�>>>>>>>>>>>>>>>>>>>>>>>>
ShowMenu proc                       ;�޲���
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


;>>>>>>>>>>>>>>>>>>>>>>>> ������� >>>>>>>>>>>>>>>>>>>>>>>>
start:
	call LoadFromFile
	call ShowMenu
	ret
end start
