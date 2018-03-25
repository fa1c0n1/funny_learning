#pragma once

//自定义消息
#define WM_GETINFO_REFRESH			(WM_USER+1)
#define WM_GETINFO_FINISH			(WM_USER+2)
#define WM_DELFILE_REFRESH			(WM_USER+3)
#define WM_DELFILE_FINISH			(WM_USER+4)
#define WM_USER_UPDATE_CPUUSAGE     (WM_USER+5)
#define WM_USER_UPDATE_MEMUSAGE     (WM_USER+6) 
#define WM_USER_TRAVERSEHEAP_FINISH (WM_USER+7)

#define WCHAR_TO_CHAR(lpW_Char, lpChar, nCharSize) \
    WideCharToMultiByte(CP_ACP, NULL, lpW_Char, -1, lpChar, nCharSize, NULL, FALSE);

#define CHAR_TO_WCHAR(lpChar, lpW_Char, nWCharSize) \
    MultiByteToWideChar(CP_ACP, NULL, lpChar, -1, lpW_Char, nWCharSize);