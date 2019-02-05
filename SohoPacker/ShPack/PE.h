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
	DWORD Encrypt(BYTE &bKey);

	// Stub��Ϣ����
	void FixReloc(PBYTE lpImage, PBYTE lpCode, DWORD dwCodeRVA);// ���ڴ����ض�λStub
	DWORD GetSectionData(PBYTE lpImage, DWORD dwSectionIndex, PBYTE& lpBuffer, DWORD& dwCodeBaseRVA);
	void ChangeReloc(PBYTE lpStubMod, PBYTE &pNewRelocSection, DWORD &dwNewRelocTableSize);

public:
	PBYTE  m_pFileBase;
	DWORD  m_dwFileSize;
	PIMAGE_NT_HEADERS  m_pNT;
	PIMAGE_SECTION_HEADER m_pLastSection;//���������ε�ַ,�����һ�����ε�ַ
	DWORD   m_dwFileAlign;
	DWORD   m_dwMemAlign;
	ULONG   m_dwImageBase;
	DWORD   m_dwOEP;
	DWORD   m_dwCodeBase;
	DWORD   m_dwCodeSize;
	DWORD   m_dwNewOEP;
	DWORD   m_dwNewSectionRVA;
	DWORD   m_dwRelocTableRVA;
	DWORD   m_dwRDataRVA;
	DWORD   m_dwRDataSize;
	CFile   m_objFile;
};
