		.data

;����ṹ��
CONTACTSTRUCT struct
	szName BYTE 33 dup(0)       ;����
	szPhNumber BYTE 12 dup(0)   ;�绰����
CONTACTSTRUCT ends
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
PCONTACKSTRUCT TYPEDEF PTR CONTACTSTRUCT      ;ȡ����(ָ������)
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

;����ȫ�ֱ���
g_stContacts       CONTACTSTRUCT 100 dup (<'0'>)     ;����ṹ������
g_nCount           DWORD 0                           ;Ԫ�ظ���
g_nCountMax        DWORD 100                         ;����Ÿ���
g_strTmpContact    CONTACTSTRUCT  <'0','0'>         ;����������Ϣ
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;�����ʽ���Ʒ�,�����������ʱ
g_szScanfFmt           BYTE '%s %s',0
g_szPrnFmt             BYTE '��%-10s��%-20s��',0ah,0dh,0
g_szScanfNameFmt       BYTE '%s',0
g_szScanfNewNumFmt     BYTE '%s',0
g_szPrnPhNumberFmt     BYTE '����ϵ�˵ĺ��룺%s',0ah,0dh,0
g_szScanfChoiceFmt     BYTE '%d',0
g_szCountFmt           BYTE 'һ���� %d ����ϵ��.',0ah,0dh,0
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;������ʾ
g_szAddstr             BYTE '������Ҫ���ӵ���ϵ����Ϣ(��ʽ:���� ����): ',0
g_szFindStr            BYTE '������Ҫ��ѯ����ϵ������: ',0
g_szUpdateStr1         BYTE '������Ҫ�޸ĵ���ϵ������: ',0
g_szUpdateStr2         BYTE '�������µĺ��룺',0 
g_szRmStr              BYTE '������Ҫɾ������ϵ�˵�������',0
g_szRmOK               BYTE 'ɾ���ɹ�',0ah,0dh,0
g_szSaveOK             BYTE '����ɹ�',0ah,0dh,0
g_szNotFound           BYTE 'δ�ҵ�����ϵ�˵���Ϣ!',0ah,0dh,0
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
;�˵�
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
g_szLine               BYTE ' ������������������',0ah,0dh,0
g_szMenu               BYTE '��1.������ϵ��    ��',0ah,0dh,
                            '��2.��ѯ��ϵ��    ��',0ah,0dh,
                            '��3.������ϵ��    ��',0ah,0dh,
                            '��4.ɾ����ϵ��    ��',0ah,0dh,
                            '��5.��ӡ������ϵ�˩�',0ah,0dh,
                            '��6.����绰��    ��',0ah,0dh,
                            '��7.�˳�����      ��',0ah,0dh,0

g_szChoice             BYTE 0ah,0dh,'��ѡ��(1-6): ',0
;>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
g_szCmdCleanScreen     BYTE 'cls',0                         ;��������
g_szPhNumberFile       BYTE 'PhoneNumberBook.txt',0         ;�绰���ļ���
g_szFileOpenMode       BYTE 'wb+',0                         ;�����ļ�ʱʹ�õĴ�ģʽ
g_szFileReadMode       BYTE 'rb',0                          ;��ȡ�ļ�ʱʹ�õĴ�ģʽ