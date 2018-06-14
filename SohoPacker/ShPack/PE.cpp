#include "stdafx.h"
#include "PE.h"


CPE::CPE() :m_dwFileSize(0), m_pFileBase(NULL),
m_dwFileAlign(0), m_dwMemAlign(0)
{
}


CPE::~CPE()
{
}

DWORD CPE::RVA2OffSet(DWORD dwRVA, PIMAGE_NT_HEADERS  pNt)
{
	DWORD dwOffset = 0;
	// 1. ��ȡ��һ�����νṹ��
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);
	// 2. ��ȡ��������
	DWORD dwSectionCount = pNt->FileHeader.NumberOfSections;
	// 3. ����������Ϣ��
	for (DWORD i = 0; i < dwSectionCount; i++)
	{
		// 4. ƥ��RVA���ڵ�����
		if (dwRVA >= pSection[i].VirtualAddress &&
			dwRVA < (pSection[i].VirtualAddress + pSection[i].Misc.VirtualSize)
			)
		{   // �����Ӧ���ļ�ƫ��
			dwOffset = dwRVA - pSection[i].VirtualAddress +
				pSection[i].PointerToRawData;
			return dwOffset;
		}
	}
	return dwOffset;
}


BOOL CPE::InitPE(CString strPath)
{
	if (m_objFile.m_hFile == INVALID_HANDLE_VALUE && m_pFileBase) {
		m_objFile.Close(); delete m_pFileBase; m_pFileBase = NULL;
	}

	m_objFile.Open(strPath, CFile::modeRead);
	m_dwFileSize = (DWORD)m_objFile.GetLength();
	m_pFileBase = new BYTE[m_dwFileSize];
	if (m_objFile.Read(m_pFileBase, (DWORD)m_dwFileSize))
	{
		// 1. ��ȡDOSͷ
		PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)m_pFileBase;
		// 2. ��ȡNTͷ
		m_pNT = (PIMAGE_NT_HEADERS)((ULONG)m_pFileBase + pDos->e_lfanew);
		// 3. ��ȡ�ļ����롢�ڴ�������Ϣ
		m_dwFileAlign = m_pNT->OptionalHeader.FileAlignment;
		m_dwMemAlign = m_pNT->OptionalHeader.SectionAlignment;
		m_dwImageBase = m_pNT->OptionalHeader.ImageBase;
		m_dwOEP = m_pNT->OptionalHeader.AddressOfEntryPoint;
		m_dwCodeBase = m_pNT->OptionalHeader.BaseOfCode;
		m_dwCodeSize = m_pNT->OptionalHeader.SizeOfCode;

		PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(m_pNT);
		while (pSectionHeader->VirtualAddress) {
			if (strcmp(".rdata", (char*)pSectionHeader->Name) == 0) {
				m_dwRDataRVA = pSectionHeader->VirtualAddress;
				m_dwRDataSize = pSectionHeader->Misc.VirtualSize;
				printf(".rdata Found!, m_dwRDataRVA=%X, m_dwRDataSize=%X\n", m_dwRDataRVA, m_dwRDataSize);
				break;
			}
			pSectionHeader++;
		}

		// 4. ��ȡ�ض�λ���RVA
		PIMAGE_DATA_DIRECTORY pRelocDir = (PIMAGE_DATA_DIRECTORY)&m_pNT->OptionalHeader.DataDirectory[5];
		m_dwRelocTableRVA = pRelocDir->VirtualAddress;
		printf("Ori PE RelocTableRVA = %X\n", m_dwRelocTableRVA);

		// 5. ��ȡ���һ�����κ�ĵ�ַ
		PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(m_pNT);
		m_pLastSection = &pSection[m_pNT->FileHeader.NumberOfSections];

		// 6. ��ȡ�¼����ε���ʼRVA,  �ڴ�4K����
		DWORD dwVirtualSize = m_pLastSection[-1].Misc.VirtualSize;
		if (dwVirtualSize % m_dwMemAlign)
			dwVirtualSize = (dwVirtualSize / m_dwMemAlign + 1) * m_dwMemAlign;
		else
			dwVirtualSize = (dwVirtualSize / m_dwMemAlign) * m_dwMemAlign;
		m_dwNewSectionRVA = m_pLastSection[-1].VirtualAddress + dwVirtualSize;

		return TRUE;
	}
	return FALSE;
}

ULONG CPE::AddSection(LPBYTE pBuffer, DWORD dwSize, PCHAR pszSectionName, bool bAddStubOEP)
{
	// 1 �޸��ļ�ͷ�е���������
	m_pNT->FileHeader.NumberOfSections++;

	// 2 �������α���
	memset(m_pLastSection, 0, sizeof(IMAGE_SECTION_HEADER));
	strcpy_s((char*)m_pLastSection->Name, IMAGE_SIZEOF_SHORT_NAME, pszSectionName);

	DWORD dwVirtualSize = 0; // ���������С
	DWORD dwSizeOfRawData = 0; // �����ļ���С
	DWORD dwSizeOfImage = m_pNT->OptionalHeader.SizeOfImage;
	{
		if (dwSizeOfImage % m_dwMemAlign)
			dwSizeOfImage = (dwSizeOfImage / m_dwMemAlign + 1) * m_dwMemAlign;
		else
			dwSizeOfImage = (dwSizeOfImage / m_dwMemAlign) * m_dwMemAlign;

		if (dwSize % m_dwMemAlign)
			dwVirtualSize = (dwSize / m_dwMemAlign + 1) * m_dwMemAlign;
		else
			dwVirtualSize = (dwSize / m_dwMemAlign) * m_dwMemAlign;

		if (dwSize % m_dwFileAlign)
			dwSizeOfRawData = (dwSize / m_dwFileAlign + 1) * m_dwFileAlign;
		else
			dwSizeOfRawData = (dwSize / m_dwFileAlign) * m_dwFileAlign;
	}

	static DWORD dwTmpFileSize = m_dwFileSize;

	m_pLastSection->VirtualAddress = dwSizeOfImage;
	m_pLastSection->PointerToRawData = dwTmpFileSize;
	printf("----------- %X ------------\n", dwTmpFileSize);
	dwTmpFileSize += dwSize;
	m_pLastSection->SizeOfRawData = dwSizeOfRawData;
	m_pLastSection->Misc.VirtualSize = dwSize;
	m_pLastSection->Characteristics = 0xE0000040;

	// 3 �����ļ���С,�����ļ�����Ӵ���
	m_pNT->OptionalHeader.SizeOfImage = dwSizeOfImage + dwVirtualSize;
	if (bAddStubOEP)
		m_pNT->OptionalHeader.AddressOfEntryPoint = m_dwNewOEP + m_pLastSection->VirtualAddress;

	//// 3.1 ��������ļ�·��
	//CString strPath = m_objFile.GetFilePath();
	//TCHAR szOutPath[MAX_PATH] = { 0 };
	//LPWSTR strSuffix = PathFindExtension(strPath);                     // ��ȡ�ļ��ĺ�׺��
	//wcsncpy_s(szOutPath, MAX_PATH, strPath, wcslen(strPath)); // ����Ŀ���ļ�·����szOutPath
	//PathRemoveExtension(szOutPath);                                         // ��szOutPath�б���·���ĺ�׺��ȥ��
	//wcscat_s(szOutPath, MAX_PATH, L"_SohoPack");                       // ��·����󸽼�"_SohoPack"
	//wcscat_s(szOutPath, MAX_PATH, strSuffix);                           // ��·����󸽼Ӹոձ���ĺ�׺��

	//// 3.2 �����ļ�
	//CFile objFile(szOutPath, CFile::modeCreate | CFile::modeReadWrite);
	//objFile.Write(m_pFileBase, m_dwFileSize);
	//objFile.SeekToEnd();
	//objFile.Write(pBuffer, dwSize);

	m_pLastSection++;

	return m_pLastSection[-1].VirtualAddress;
}

void CPE::save2File()
{
#if 0
	// 3.1 ��������ļ�·��
	CString strPath = m_objFile.GetFilePath();
	TCHAR szOutPath[MAX_PATH] = { 0 };
	LPWSTR strSuffix = PathFindExtension(strPath);                     // ��ȡ�ļ��ĺ�׺��
	wcsncpy_s(szOutPath, MAX_PATH, strPath, wcslen(strPath)); // ����Ŀ���ļ�·����szOutPath
	PathRemoveExtension(szOutPath);                                         // ��szOutPath�б���·���ĺ�׺��ȥ��
	wcscat_s(szOutPath, MAX_PATH, L"_SohoPack");                       // ��·����󸽼�"_SohoPack"
	wcscat_s(szOutPath, MAX_PATH, strSuffix);                           // ��·����󸽼Ӹոձ���ĺ�׺��

	// 3.2 �����ļ�
	CFile objFile(szOutPath, CFile::modeCreate | CFile::modeReadWrite);
	objFile.Write(m_pFileBase, m_dwFileSize);
	objFile.SeekToEnd();
	objFile.Write(pBuffer, dwSize);
#endif
}

// ���ڴ����ض�λStub
void CPE::FixReloc(PBYTE lpImage, PBYTE lpCode, DWORD dwCodeRVA)
{
	// 1. ��ȡDOSͷ
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)lpImage;
	// 2. ��ȡNTͷ
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)((ULONG)lpImage + pDos->e_lfanew);
	// 3. ��ȡ����Ŀ¼��
	PIMAGE_DATA_DIRECTORY pRelocDir = pNt->OptionalHeader.DataDirectory;
	pRelocDir = &(pRelocDir[IMAGE_DIRECTORY_ENTRY_BASERELOC]);
	// 4. ��ȡ�ض�λĿ¼
	PIMAGE_BASE_RELOCATION pReloc = (PIMAGE_BASE_RELOCATION)((ULONG)lpImage + pRelocDir->VirtualAddress);
	typedef struct {
		WORD Offset : 12;          // (1) ��СΪ12Bit���ض�λƫ��
		WORD Type : 4;             // (2) ��СΪ4Bit���ض�λ��Ϣ����ֵ
	}TypeOffset, *PTypeOffset;	   

	// ѭ����ȡÿһ��MAGE_BASE_RELOCATION�ṹ���ض�λ��Ϣ
	while (pReloc->VirtualAddress)
	{
		PTypeOffset pTypeOffset = (PTypeOffset)(pReloc + 1);
		ULONG dwSize = sizeof(IMAGE_BASE_RELOCATION);
		ULONG dwCount = (pReloc->SizeOfBlock - dwSize) / 2;
		for (ULONG i = 0; i < dwCount; i++)
		{
			if (*(PULONG)(&pTypeOffset[i]) == NULL)
				break;
			ULONG dwRVA = pReloc->VirtualAddress + pTypeOffset[i].Offset;
			PULONG pRelocAddr = (PULONG)((ULONG)lpImage + dwRVA);
			// �޸��ض�λ��Ϣ   ��ʽ����Ҫ�޸��ĵ�ַ-ԭӳ���ַ-ԭ���λ�ַ+�����λ�ַ+��ӳ���ַ
			ULONG dwRelocCode = *pRelocAddr - pNt->OptionalHeader.ImageBase - pNt->OptionalHeader.BaseOfCode +
				dwCodeRVA + m_dwImageBase;
			*pRelocAddr = dwRelocCode;
		}
		pReloc = (PIMAGE_BASE_RELOCATION)((ULONG)pReloc + pReloc->SizeOfBlock);
	}
}

void CPE::SetNewOEP(DWORD dwOEP)
{
	m_dwNewOEP = dwOEP;
}

void CPE::ClearBundleImport()
{
	// 1. ��ȡDOSͷ
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)m_pFileBase;
	// 2. ��ȡNTͷ
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)((ULONG)m_pFileBase + pDos->e_lfanew);
	// 3. ��ȡ����Ŀ¼��
	PIMAGE_DATA_DIRECTORY pDir = pNt->OptionalHeader.DataDirectory;
	// 4. ��հ��������
	ZeroMemory(&(pDir[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT]), sizeof(IMAGE_DATA_DIRECTORY));
}

void CPE::ClearRandBase()
{
	m_pNT->OptionalHeader.DllCharacteristics = 0;
}

DWORD CPE::GetSectionData(PBYTE lpImage, DWORD dwSectionIndex, PBYTE& lpBuffer, DWORD& dwCodeBaseRVA)
{
	// 1. ��ȡDOSͷ
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)lpImage;
	// 2. ��ȡNTͷ
	PIMAGE_NT_HEADERS  pNt = (PIMAGE_NT_HEADERS)((ULONG)lpImage + pDos->e_lfanew);
	// 3. ��ȡ��һ�����νṹ��
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);
	DWORD dwSize = pSection[0].SizeOfRawData;

	ULONG dwCodeAddr = (ULONG)lpImage + pSection[0].VirtualAddress;
	lpBuffer = (PBYTE)dwCodeAddr;
	dwCodeBaseRVA = pSection[0].VirtualAddress;

	if (dwSize == 0) {
		//����Debug�汾����һ������û�д�С
		dwSize = pSection[1].SizeOfRawData;
		dwCodeAddr = (ULONG)lpImage + pSection[1].VirtualAddress;
		lpBuffer = (PBYTE)dwCodeAddr;
		dwCodeBaseRVA = pSection[1].VirtualAddress;
	} 

	return dwSize;
}

//ע��: 
//    ����1:����Stub.dll�Ļ�ַ���ڴ��е�ģ���ַ
void CPE::ChangeReloc(PBYTE lpStubMod, PBYTE &pNewRelocSection, DWORD &dwNewRelocTableSize)
{
	PIMAGE_DOS_HEADER pHostDos = (PIMAGE_DOS_HEADER)m_pFileBase;
	PIMAGE_NT_HEADERS32 pHostNtHeader = (PIMAGE_NT_HEADERS32)((ULONG)m_pFileBase + pHostDos->e_lfanew);

	PIMAGE_DOS_HEADER pStubDos = (PIMAGE_DOS_HEADER)lpStubMod;
	PIMAGE_NT_HEADERS32 pStubNtHeader = (PIMAGE_NT_HEADERS32)((ULONG)lpStubMod + pStubDos->e_lfanew);
	PIMAGE_OPTIONAL_HEADER32 pStubOptionHeader = (PIMAGE_OPTIONAL_HEADER32)&pStubNtHeader->OptionalHeader;
	PIMAGE_DATA_DIRECTORY pStubRelocDir = (PIMAGE_DATA_DIRECTORY)&pStubNtHeader->OptionalHeader.DataDirectory[5];

	//----------------------//�Ȱ�ShStub.dll����ض�λ���RVA��Ϊ .Soho ������RVA------------------------------------------
	PIMAGE_BASE_RELOCATION pRelocBlock = (PIMAGE_BASE_RELOCATION)((ULONG)lpStubMod + pStubRelocDir->VirtualAddress);
	typedef struct {
		WORD Offset : 12;          // (1) ��СΪ12Bit���ض�λƫ��
		WORD Type : 4;             // (2) ��СΪ4Bit���ض�λ��Ϣ����ֵ
	}TypeOffset, *PTypeOffset;

	// ѭ����ȡÿһ��MAGE_BASE_RELOCATION�ṹ���ض�λ��Ϣ
	while (pRelocBlock->VirtualAddress)
	{
		pRelocBlock->VirtualAddress = m_pLastSection[-1].VirtualAddress;
		printf("************* %X \n", pRelocBlock->VirtualAddress);
		pRelocBlock = (PIMAGE_BASE_RELOCATION)((ULONG)pRelocBlock + pRelocBlock->SizeOfBlock);
	}
	//----------------------------------------------------------------

	//����Stub.dll�е��ض�λ������ݣ��������µ�������
	dwNewRelocTableSize = pStubRelocDir->Size;
	printf("before: dwNewRelocTableSize=%X\n", dwNewRelocTableSize);

	DWORD dwNewRelocTableAlignSize = dwNewRelocTableSize;

	if (dwNewRelocTableSize % m_dwFileAlign)
		dwNewRelocTableAlignSize = (dwNewRelocTableSize / m_dwFileAlign + 1) * m_dwFileAlign;
	else
		dwNewRelocTableAlignSize = (dwNewRelocTableSize / m_dwFileAlign) * m_dwFileAlign;
	printf("after: dwNewRelocTableAlignSize=%X\n", dwNewRelocTableAlignSize);

	pNewRelocSection = new BYTE[dwNewRelocTableAlignSize]{};
	printf("%p\n", pNewRelocSection);
	PBYTE pStubReloc = (PBYTE)((ULONG)lpStubMod + pStubRelocDir->VirtualAddress);

	
	memcpy_s(pNewRelocSection, dwNewRelocTableAlignSize, pStubReloc, pStubRelocDir->Size);

	DWORD dwNewRelocSectionRVA = this->AddSection(pNewRelocSection, dwNewRelocTableSize, ".nrelc", false);
	printf("555555555555555555, dwNewRelocSectionRVA=%X\n", dwNewRelocSectionRVA);
	pHostNtHeader->OptionalHeader.DataDirectory[5].VirtualAddress = dwNewRelocSectionRVA;
	pHostNtHeader->OptionalHeader.DataDirectory[5].Size = dwNewRelocTableSize;
	dwNewRelocTableSize = dwNewRelocTableAlignSize;
}

DWORD CPE::XorCode(BYTE byXOR)
{
	DWORD dwVirtualAddr = m_dwCodeBase;
	DWORD dwOffset = RVA2OffSet(m_dwCodeBase, m_pNT);
	if (!dwOffset)
	{// ���� debug�汾����һ������û�д�С
		PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(m_pNT);
		dwOffset = RVA2OffSet(pSection[1].VirtualAddress, m_pNT);
		dwVirtualAddr = pSection[1].VirtualAddress;
	}
	PBYTE pBase = (PBYTE)((ULONG)m_pFileBase + dwOffset);

	for (DWORD i = 0; i < m_dwCodeSize; i++)
	{
		pBase[i] ^= byXOR;
	}

	return dwVirtualAddr;
}