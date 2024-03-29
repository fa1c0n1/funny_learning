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
	// 1. 获取第一个区段结构体
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);
	// 2. 获取区段数量
	DWORD dwSectionCount = pNt->FileHeader.NumberOfSections;
	// 3. 遍历区段信息表
	for (DWORD i = 0; i < dwSectionCount; i++)
	{
		// 4. 匹配RVA所在的区段
		if (dwRVA >= pSection[i].VirtualAddress &&
			dwRVA < (pSection[i].VirtualAddress + pSection[i].Misc.VirtualSize)
			)
		{   // 计算对应的文件偏移
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
		// 1. 获取DOS头
		PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)m_pFileBase;
		// 2. 获取NT头
		m_pNT = (PIMAGE_NT_HEADERS)((ULONG)m_pFileBase + pDos->e_lfanew);
		// 3. 获取文件对齐、内存对齐等信息
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

		// 4. 获取重定位表的RVA
		PIMAGE_DATA_DIRECTORY pRelocDir = (PIMAGE_DATA_DIRECTORY)&m_pNT->OptionalHeader.DataDirectory[5];
		m_dwRelocTableRVA = pRelocDir->VirtualAddress;
		printf("Ori PE RelocTableRVA = %X\n", m_dwRelocTableRVA);

		// 5. 获取最后一个区段后的地址
		PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(m_pNT);
		m_pLastSection = &pSection[m_pNT->FileHeader.NumberOfSections];

		// 6. 获取新加区段的起始RVA,  内存4K对齐
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
	// 1 修改文件头中的区段数量
	m_pNT->FileHeader.NumberOfSections++;

	// 2 增加区段表项
	memset(m_pLastSection, 0, sizeof(IMAGE_SECTION_HEADER));
	strcpy_s((char*)m_pLastSection->Name, IMAGE_SIZEOF_SHORT_NAME, pszSectionName);

	DWORD dwVirtualSize = 0; // 区段虚拟大小
	DWORD dwSizeOfRawData = 0; // 区段文件大小
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
	dwTmpFileSize += dwSize;
	m_pLastSection->SizeOfRawData = dwSizeOfRawData;
	m_pLastSection->Misc.VirtualSize = dwSize;
	m_pLastSection->Characteristics = 0xE0000040;

	// 3 增加文件大小,创建文件，添加代码
	m_pNT->OptionalHeader.SizeOfImage = dwSizeOfImage + dwVirtualSize;
	if (bAddStubOEP)
		m_pNT->OptionalHeader.AddressOfEntryPoint = m_dwNewOEP + m_pLastSection->VirtualAddress;

	m_pLastSection++;

	return m_pLastSection[-1].VirtualAddress;
}

// 在内存中重定位Stub
void CPE::FixReloc(PBYTE lpImage, PBYTE lpCode, DWORD dwCodeRVA)
{
	// 1. 获取DOS头
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)lpImage;
	// 2. 获取NT头
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)((ULONG)lpImage + pDos->e_lfanew);
	// 3. 获取数据目录表
	PIMAGE_DATA_DIRECTORY pRelocDir = pNt->OptionalHeader.DataDirectory;
	pRelocDir = &(pRelocDir[IMAGE_DIRECTORY_ENTRY_BASERELOC]);
	// 4. 获取重定位目录
	PIMAGE_BASE_RELOCATION pReloc = (PIMAGE_BASE_RELOCATION)((ULONG)lpImage + pRelocDir->VirtualAddress);
	typedef struct {
		WORD Offset : 12;          // (1) 大小为12Bit的重定位偏移
		WORD Type : 4;             // (2) 大小为4Bit的重定位信息类型值
	}TypeOffset, *PTypeOffset;	   

	// 循环获取每一个MAGE_BASE_RELOCATION结构的重定位信息
	while (pReloc->VirtualAddress)
	{
		PTypeOffset pTypeOffset = (PTypeOffset)(pReloc + 1);
		ULONG dwSize = sizeof(IMAGE_BASE_RELOCATION);
		ULONG dwCount = (pReloc->SizeOfBlock - dwSize) / 2;
		for (ULONG i = 0; i < dwCount; i++)
		{
			if (*(WORD*)&pTypeOffset[i] == 0)
				continue;

			ULONG dwRVA = pReloc->VirtualAddress + pTypeOffset[i].Offset;
			PULONG pRelocAddr = (PULONG)((ULONG)lpImage + dwRVA);
			// 修复重定位信息   公式：需要修复的地址-原映像基址-原区段基址+现区段基址+现映像基址
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
	// 1. 获取DOS头
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)m_pFileBase;
	// 2. 获取NT头
	PIMAGE_NT_HEADERS pNt = (PIMAGE_NT_HEADERS)((ULONG)m_pFileBase + pDos->e_lfanew);
	// 3. 获取数据目录表
	PIMAGE_DATA_DIRECTORY pDir = pNt->OptionalHeader.DataDirectory;
	// 4. 清空绑定输入表项
	ZeroMemory(&(pDir[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT]), sizeof(IMAGE_DATA_DIRECTORY));
}

void CPE::ClearRandBase()
{
	m_pNT->OptionalHeader.DllCharacteristics = 0;
}

DWORD CPE::GetSectionData(PBYTE lpImage, DWORD dwSectionIndex, PBYTE& lpBuffer, DWORD& dwCodeBaseRVA)
{
	// 1. 获取DOS头
	PIMAGE_DOS_HEADER pDos = (PIMAGE_DOS_HEADER)lpImage;
	// 2. 获取NT头
	PIMAGE_NT_HEADERS  pNt = (PIMAGE_NT_HEADERS)((ULONG)lpImage + pDos->e_lfanew);
	// 3. 获取第一个区段结构体
	PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNt);
	DWORD dwSize = pSection[0].SizeOfRawData;

	ULONG dwCodeAddr = (ULONG)lpImage + pSection[0].VirtualAddress;
	lpBuffer = (PBYTE)dwCodeAddr;
	dwCodeBaseRVA = pSection[0].VirtualAddress;

	if (dwSize == 0) {
		//兼容Debug版本，第一个区段没有大小
		dwSize = pSection[1].SizeOfRawData;
		dwCodeAddr = (ULONG)lpImage + pSection[1].VirtualAddress;
		lpBuffer = (PBYTE)dwCodeAddr;
		dwCodeBaseRVA = pSection[1].VirtualAddress;
	} 

	return dwSize;
}

//注意: 
//    参数1:这里Stub.dll的基址是内存中的模块基址
void CPE::ChangeReloc(PBYTE lpStubMod, PBYTE &pNewRelocSection, DWORD &dwNewRelocTableSize)
{
	PIMAGE_DOS_HEADER pHostDos = (PIMAGE_DOS_HEADER)m_pFileBase;
	PIMAGE_NT_HEADERS32 pHostNtHeader = (PIMAGE_NT_HEADERS32)((ULONG)m_pFileBase + pHostDos->e_lfanew);

	PIMAGE_DOS_HEADER pStubDos = (PIMAGE_DOS_HEADER)lpStubMod;
	PIMAGE_NT_HEADERS32 pStubNtHeader = (PIMAGE_NT_HEADERS32)((ULONG)lpStubMod + pStubDos->e_lfanew);
	PIMAGE_OPTIONAL_HEADER32 pStubOptionHeader = (PIMAGE_OPTIONAL_HEADER32)&pStubNtHeader->OptionalHeader;
	PIMAGE_DATA_DIRECTORY pStubRelocDir = (PIMAGE_DATA_DIRECTORY)&pStubNtHeader->OptionalHeader.DataDirectory[5];

	//----------------------//先把ShStub.dll里的重定位块的RVA改为 .Soho 节区的RVA---------------------------------
	PIMAGE_BASE_RELOCATION pRelocBlock = (PIMAGE_BASE_RELOCATION)((ULONG)lpStubMod + pStubRelocDir->VirtualAddress);
	typedef struct {
		WORD Offset : 12;          // (1) 大小为12Bit的重定位偏移
		WORD Type : 4;             // (2) 大小为4Bit的重定位信息类型值
	}TypeOffset, *PTypeOffset;

	// 循环获取每一个MAGE_BASE_RELOCATION结构的重定位信息
	DWORD dwNewRelocBlockRVA = m_pLastSection[-1].VirtualAddress;
	DWORD dwOldRelocBlockRVA = 0;
	while (pRelocBlock->VirtualAddress)
	{
		dwOldRelocBlockRVA = pRelocBlock->VirtualAddress - dwOldRelocBlockRVA;   //
		pRelocBlock->VirtualAddress = dwNewRelocBlockRVA;
		printf("************* %X \n", pRelocBlock->VirtualAddress);
		pRelocBlock = (PIMAGE_BASE_RELOCATION)((ULONG)pRelocBlock + pRelocBlock->SizeOfBlock);

		if (pRelocBlock->VirtualAddress) {
			dwNewRelocBlockRVA += (pRelocBlock->VirtualAddress - dwOldRelocBlockRVA);
		}
	}
	//----------------------------------------------------------------

	//将壳Stub.dll中的重定位表的内容，拷贝到新的区段中
	dwNewRelocTableSize = pStubRelocDir->Size;
	printf("before: dwNewRelocTableSize=%X\n", dwNewRelocTableSize);

	DWORD dwNewRelocTableAlignSize = dwNewRelocTableSize;

	if (dwNewRelocTableSize % m_dwFileAlign)
		dwNewRelocTableAlignSize = (dwNewRelocTableSize / m_dwFileAlign + 1) * m_dwFileAlign;
	else
		dwNewRelocTableAlignSize = (dwNewRelocTableSize / m_dwFileAlign) * m_dwFileAlign;

	pNewRelocSection = new BYTE[dwNewRelocTableAlignSize]{};
	PBYTE pStubReloc = (PBYTE)((ULONG)lpStubMod + pStubRelocDir->VirtualAddress);

	
	memcpy_s(pNewRelocSection, dwNewRelocTableAlignSize, pStubReloc, pStubRelocDir->Size);

	DWORD dwNewRelocSectionRVA = this->AddSection(pNewRelocSection, dwNewRelocTableSize, ".nrelc", false);
	pHostNtHeader->OptionalHeader.DataDirectory[5].VirtualAddress = dwNewRelocSectionRVA;
	pHostNtHeader->OptionalHeader.DataDirectory[5].Size = dwNewRelocTableSize;
	dwNewRelocTableSize = dwNewRelocTableAlignSize;
}

DWORD CPE::Encrypt(BYTE &bKey)
{
	DWORD dwVirtualAddr = m_dwCodeBase;
	DWORD dwOffset = RVA2OffSet(m_dwCodeBase, m_pNT);
	if (!dwOffset)
	{// 兼容 debug版本，第一个区段没有大小
		PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(m_pNT);
		dwOffset = RVA2OffSet(pSection[1].VirtualAddress, m_pNT);
		dwVirtualAddr = pSection[1].VirtualAddress;
	}
	PBYTE pBase = (PBYTE)((ULONG)m_pFileBase + dwOffset);

	printf("m_dwCodeSize=%X, bKey=0x%02X\n", m_dwCodeSize, bKey);
	for (DWORD i = 0; i < m_dwCodeSize; i++)
	{
		pBase[i] = pBase[i] + bKey;
		bKey = pBase[i] ^ bKey;
	}

	return dwVirtualAddr;
}