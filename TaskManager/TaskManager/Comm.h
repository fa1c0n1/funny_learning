#pragma once

#define WCHAR_TO_CHAR(lpW_Char, lpChar, nCharSize) \
    WideCharToMultiByte(CP_ACP, NULL, lpW_Char, -1, lpChar, nCharSize, NULL, FALSE);

#define CHAR_TO_WCHAR(lpChar, lpW_Char, nWCharSize) \
    MultiByteToWideChar(CP_ACP, NULL, lpChar, -1, lpW_Char, nWCharSize);