#pragma once

#include "stdafx.h"

class CPeParser
{
public:
	CPeParser();
	~CPeParser();

	bool parsePE(QString strFile);
	void showExportTableInfo();
	void showImportTableInfo();

private:
	void getNtHeaderInfo(PIMAGE_NT_HEADERS pNtHeader);
	void FILE_PE_32();
	void FILE_PE_64();
	DWORD rva2foa(DWORD dwRva);

private:
	bool m_bPEIs32;
	BYTE *m_pFileImageBase;
	PIMAGE_NT_HEADERS m_pNtHeader;
	PIMAGE_DATA_DIRECTORY m_pDataDir;
};

