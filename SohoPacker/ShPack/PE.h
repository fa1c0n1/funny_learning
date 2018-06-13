#pragma once
class CPE
{
public:
	CPE();
	~CPE();
	DWORD RVA2OffSet(DWORD dwRVA, PIMAGE_NT_HEADERS  pNt);
	BOOL InitPE(CString strPath);
	ULONG AddSection(LPBYTE pBuffer, DWORD dwSectionSize, PCHAR pszSectionName, bool bAddSubOEP);
	void SetNewOEP(DWORD dwOEP);
	void ClearRandBase();
	void ClearBundleImport();
	DWORD XorCode(BYTE byXOR);

	// Stub信息处理
	void FixReloc(PBYTE lpImage, PBYTE lpCode, DWORD dwCodeRVA);// 在内存中重定位Stub
	DWORD GetSectionData(PBYTE lpImage, DWORD dwSectionIndex, PBYTE& lpBuffer, DWORD& dwCodeBaseRVA);
	void ChangeReloc(PBYTE lpStubMod, PBYTE &pNewRelocSection, DWORD &dwNewRelocTableSize);
	void save2File();

public:
	PBYTE  m_pFileBase;
	DWORD  m_dwFileSize;
	PIMAGE_NT_HEADERS  m_pNT;
	PIMAGE_SECTION_HEADER m_pLastSection;//新添加区段地址,即最后一个区段地址
	DWORD   m_dwFileAlign;
	DWORD   m_dwMemAlign;
	ULONG   m_dwImageBase;
	DWORD   m_dwOEP;
	DWORD   m_dwCodeBase;
	DWORD   m_dwCodeSize;
	DWORD   m_dwNewOEP;
	DWORD   m_dwNewSectionRVA;
	CFile   m_objFile;
};

