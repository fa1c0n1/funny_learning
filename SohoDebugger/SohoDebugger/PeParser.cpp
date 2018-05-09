#include "PeParser.h"


CPeParser::CPeParser()
	: m_pFileImageBase(nullptr), m_pDataDir(nullptr),
	m_pNtHeader(nullptr)
{
}


CPeParser::~CPeParser()
{
	if (m_pFileImageBase) {
		delete[] m_pFileImageBase;
		m_pFileImageBase = nullptr;
	}
}

bool CPeParser::parsePE(QString strFile)
{
	TCHAR szFilePath[MAX_PATH] = {};
	strFile.toWCharArray(szFilePath);

	HANDLE hFile = CreateFile(szFilePath,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	DWORD dwFileSize = GetFileSize(hFile, NULL);
	m_pFileImageBase = new BYTE[dwFileSize]{};
	DWORD dwReadSize = 0;
	if (!ReadFile(hFile, m_pFileImageBase, dwFileSize, &dwReadSize, NULL)) {
		qout << qtr("加载文件失败") << endl;
		return false;
	}

	IMAGE_DOS_HEADER *pDosHeader = (IMAGE_DOS_HEADER*)m_pFileImageBase;
	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
		qout << qtr("不是PE文件") << endl;
		return false;
	}

	m_pNtHeader = (IMAGE_NT_HEADERS*)(pDosHeader->e_lfanew + (ULONG64)pDosHeader);
	if (m_pNtHeader->Signature != IMAGE_NT_SIGNATURE) {
		qout << qtr("不是PE文件") << endl;
		return false;
	}

	getNtHeaderInfo(m_pNtHeader);
	CloseHandle(hFile);
}

void CPeParser::getNtHeaderInfo(PIMAGE_NT_HEADERS pNtHeader)
{
	if (pNtHeader == NULL)
		return;

	IMAGE_FILE_HEADER *pFileHeader = &pNtHeader->FileHeader;

	//32位PE还是64位PE
	if (pNtHeader->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
		m_bPEIs32 = true;
		FILE_PE_32();
	}
	else if (pNtHeader->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC){
		m_bPEIs32 = false;
		FILE_PE_64();
	}
}

void CPeParser::FILE_PE_32()
{
	IMAGE_DOS_HEADER *pDosHeader = (IMAGE_DOS_HEADER*)m_pFileImageBase;

	//Nt头
	IMAGE_NT_HEADERS32 *pNtHeader32 =
		(IMAGE_NT_HEADERS32*)(pDosHeader->e_lfanew + (ULONG)pDosHeader);
	//扩展头
	IMAGE_OPTIONAL_HEADER32 *pOptionalHeader32 =
		(IMAGE_OPTIONAL_HEADER32*)(&pNtHeader32->OptionalHeader);
	//数据目录表
	m_pDataDir = pOptionalHeader32->DataDirectory;
}

void CPeParser::FILE_PE_64()
{
	IMAGE_DOS_HEADER* pDosHeader = (IMAGE_DOS_HEADER*)m_pFileImageBase;
	IMAGE_NT_HEADERS64 *pNtHeader64 =
		(IMAGE_NT_HEADERS64*)(pDosHeader->e_lfanew + (ULONG64)pDosHeader);
	IMAGE_OPTIONAL_HEADER64 *pOptionalHeader64 =
		(IMAGE_OPTIONAL_HEADER64*)&pNtHeader64->OptionalHeader;
	m_pDataDir = pOptionalHeader64->DataDirectory;
}

void CPeParser::showExportTableInfo()
{
	if (m_pDataDir == nullptr) {
		qout << qtr("请先调用 parsePE 函数对PE文件进行解析") << endl;
		return;
	}

	qout << qtr("导出表信息:") << endl;

	//获取导出表的RVA
	DWORD dwExpTableRVA = m_pDataDir[0].VirtualAddress;

	if (m_pDataDir[0].Size == 0) {
		qout << qtr("没有数据") << endl;
		return;
	}

	ULONG64 ul64ExpTableFOA = rva2foa(dwExpTableRVA);
	IMAGE_EXPORT_DIRECTORY *pExpTable = (IMAGE_EXPORT_DIRECTORY*)(ul64ExpTableFOA + (ULONG64)m_pFileImageBase);

	ULONG64 ul64NameOffset = rva2foa(pExpTable->Name);
	char *pDllName = (char*)(ul64NameOffset + (ULONG64)m_pFileImageBase);
	qout << qtr(pDllName) << endl;

	//将导出地址表的RVA转成FOA
	DWORD dwAddrTableFoa = rva2foa(pExpTable->AddressOfFunctions);
	//得到导出地址表
	ULONG64 *pAddrTable = (ULONG64*)(dwAddrTableFoa + (ULONG64)m_pFileImageBase);

	//得到导出序号表的地址
	WORD *pOrdinalTable = (WORD*)((ULONG64)m_pFileImageBase + rva2foa(pExpTable->AddressOfNameOrdinals));
	//得到导出名称表的地址
	DWORD *pNameTable = (DWORD*)((ULONG64)m_pFileImageBase + rva2foa(pExpTable->AddressOfNames));

	bool bIndexExist = false;
	for (DWORD i = 0; i < pExpTable->NumberOfFunctions; i++) {
		//打印地址
		qout << QString::asprintf("虚序号[%d]  ", i) << endl;

		//RVA
		qout << QString::asprintf("地址(RVA): %08X (h)  ", pAddrTable[i]) << endl;

		bIndexExist = false;
		DWORD dwNameIdx = 0;
		for (; dwNameIdx < pExpTable->NumberOfNames; dwNameIdx++) {
			//判断导出地址表的下标是否存在于序号表中
			if (i == pOrdinalTable[dwNameIdx]) {
				bIndexExist = true;
				break;
			}
		}

		if (bIndexExist) {
			//得到导出名称表的RVA
			DWORD dwNameRVA = pNameTable[dwNameIdx];
			//将导出名称表RVA转换成FOA
			char *pFuncName = (char*)((ULONG64)m_pFileImageBase + rva2foa(dwNameRVA));
			qout << qtr("函数名: %1").arg(pFuncName) << endl;
		}
		else {
			//判断导出地址表当前索引到的元素是否保存着地址	
			if (pAddrTable[i] != NULL) {
				//i 是导出地址表中的索引号，即一个虚序号
				// 而真正的序号 = 虚序号 + 序号基数
				qout << QString::asprintf("序号:[%d]", pExpTable->Base) << endl;
			}
		}

		qout << endl;
	}
}

void CPeParser::showImportTableInfo()
{
	if (m_pDataDir == nullptr) {
		qout << qtr("请先调用 parsePE 函数对PE文件进行解析") << endl;
		return;
	}

	qout << qtr("导入表信息:") << endl;

	//获得导入表的RVA
	DWORD dwImpTableRVA = m_pDataDir[1].VirtualAddress;
	if (m_pDataDir[1].Size == 0) {
		qout << qtr("没有数据") << endl;
		return;
	}

	//将导入表的RVA转成FOA
	DWORD dwImpTableFOA = rva2foa(dwImpTableRVA);

	//获取导入表
	IMAGE_IMPORT_DESCRIPTOR *pImpTable =
		(IMAGE_IMPORT_DESCRIPTOR*)((ULONG64)m_pFileImageBase + dwImpTableFOA);

	//遍历导入表
	if (m_bPEIs32) { //---------32位PE

		//判断是否遍历到了最后一个结构体
		while (pImpTable->Name != 0) {
			//解析出导入的 Dll 的模块名
			DWORD dwNameFOA = rva2foa(pImpTable->Name);
			char *pDllName = (char*)(dwNameFOA + (ULONG64)m_pFileImageBase);
			qout << qtr(pDllName) << endl;

			//解析当前dll的导入函数名称
			//pImpTable->OriginalFirstThunk;  //导入名称表
			//pImpTable->FirstThunk;          //导入地址表
			//上面说的两个表，在文件中保存的内容是完全相同的

			DWORD dwIATfoa = rva2foa(pImpTable->OriginalFirstThunk);
			IMAGE_THUNK_DATA32 *pIAT = (IMAGE_THUNK_DATA32*)((ULONG64)m_pFileImageBase + dwIATfoa);

			//遍历IAT
			//  IAT 是一个 IMAGE_THUNK_DATA 的结构体数组
			while (pIAT->u1.AddressOfData != 0) {
				//判断最高位是否是1
				if (IMAGE_SNAP_BY_ORDINAL32(pIAT->u1.Ordinal)) { //函数是以序号导入的
					// 序号都是WORD类型，所以只取它的低16位
					qout << QString::asprintf("导入序号:[%d]", pIAT->u1.Ordinal & 0xFFFF) << endl;
				}
				else { //函数是以名称导入的
					//字段保存着一个指向IMAGE_IMPORT_BY_NAME结构体的RVA
					DWORD dwFuncNameFOA = rva2foa(pIAT->u1.AddressOfData);
					PIMAGE_IMPORT_BY_NAME pImpName =
						(PIMAGE_IMPORT_BY_NAME)((ULONG64)m_pFileImageBase + dwFuncNameFOA);

					qout << qtr("函数名称:[%1]").arg(pImpName->Name) << endl;
				}

				pIAT++;
			}

			pImpTable++;
			qout << endl;
		}
	}
	else {  //------------64位PE
		//判断是否遍历到了最后一个结构体
		while (pImpTable->Name != 0) {
			//解析出导入的 Dll 的模块名
			DWORD dwNameFOA = rva2foa(pImpTable->Name);
			char *pDllName = (char*)(dwNameFOA + (ULONG64)m_pFileImageBase);
			qout << qtr(pDllName) << endl;

			//解析当前dll的导入函数名称
			//pImpTable->OriginalFirstThunk;  //导入名称表
			//pImpTable->FirstThunk;          //导入地址表

			DWORD dwIATfoa = rva2foa(pImpTable->OriginalFirstThunk);
			IMAGE_THUNK_DATA64 *pIAT = (IMAGE_THUNK_DATA64*)((ULONG64)m_pFileImageBase + dwIATfoa);

			//遍历IAT
			//  IAT 是一个 IMAGE_THUNK_DATA 的结构体数组
			while (pIAT->u1.AddressOfData != 0) {
				//判断最高位是否是1
				if (IMAGE_SNAP_BY_ORDINAL64(pIAT->u1.Ordinal)) { //函数是以序号导入的
					// 序号都是WORD类型，所以只取它的低16位
					qout << QString::asprintf("导入序号:[%d]", pIAT->u1.Ordinal & 0xFFFF) << endl;
				}
				else { //函数是以名称导入的
					//字段保存着一个指向IMAGE_IMPORT_BY_NAME结构体的RVA
					DWORD dwFuncNameFOA = rva2foa(pIAT->u1.AddressOfData);
					PIMAGE_IMPORT_BY_NAME pImpName =
						(PIMAGE_IMPORT_BY_NAME)((ULONG64)m_pFileImageBase + dwFuncNameFOA);

					qout << qtr("函数名称:[%1]").arg(pImpName->Name) << endl;
				}

				pIAT++;
			}

			pImpTable++;
			qout << endl;
		}
	}
}

DWORD CPeParser::rva2foa(DWORD dwRva)
{
	//获取区段个数
	DWORD dwSectionNum = m_pNtHeader->FileHeader.NumberOfSections;
	//得到第一个区段
	IMAGE_SECTION_HEADER *pSectionHeader = IMAGE_FIRST_SECTION(m_pNtHeader);

	for (DWORD i = 0; i < dwSectionNum; i++) {
		//判断RVA是否在当前的区段中
		DWORD dwSectionEndRva =
			pSectionHeader[i].VirtualAddress + pSectionHeader[i].SizeOfRawData;

		if (dwRva >= pSectionHeader[i].VirtualAddress && dwRva <= dwSectionEndRva) {
			//在当前区段范围内，则计算FOA
			DWORD dwFoa = dwRva - pSectionHeader[i].VirtualAddress + pSectionHeader[i].PointerToRawData;
			return dwFoa;
		}
	}

	return -1;
}
