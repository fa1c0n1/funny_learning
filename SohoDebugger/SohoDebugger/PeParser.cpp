#include "PeParser.h"
#include "Comm.h"

CPeParser::CPeParser()
	: m_pDataDir(nullptr), m_pNtHeader(nullptr),
	m_pImageBase(nullptr)
{
}


CPeParser::~CPeParser()
{
	if (m_pImageBase) {
		delete[] m_pImageBase;
		m_pImageBase = nullptr;
	}
}

bool CPeParser::parsePE(HANDLE hProcess, DWORD dwBaseAddr, DWORD dwSize)
{
/*	TCHAR szFilePath[MAX_PATH] = {};
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
		qout << qtr("�����ļ�ʧ��") << endl;
		return false;
	}*/

	m_pImageBase = new BYTE[dwSize]{};

	DWORD dwByteRead = 0;
	if (!ReadProcessMemory(hProcess, (LPCVOID)dwBaseAddr, (LPVOID)m_pImageBase, dwSize, &dwByteRead)) {
		DBGPRINT("��ȡ�����ڴ�ʧ��");
		return false;
	}

	IMAGE_DOS_HEADER *pDosHeader = (IMAGE_DOS_HEADER*)m_pImageBase;
	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
		qout << qtr("����PE�ļ�") << endl;
		return false;
	}

	m_pNtHeader = (IMAGE_NT_HEADERS*)(pDosHeader->e_lfanew + (ULONG64)pDosHeader);
	if (m_pNtHeader->Signature != IMAGE_NT_SIGNATURE) {
		qout << qtr("����PE�ļ�") << endl;
		return false;
	}

	getNtHeaderInfo(m_pNtHeader);
}

void CPeParser::getNtHeaderInfo(PIMAGE_NT_HEADERS pNtHeader)
{
	if (pNtHeader == NULL)
		return;

	IMAGE_FILE_HEADER *pFileHeader = &pNtHeader->FileHeader;

	//32λPE����64λPE
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
	IMAGE_DOS_HEADER *pDosHeader = (IMAGE_DOS_HEADER*)m_pImageBase;

	//Ntͷ
	IMAGE_NT_HEADERS32 *pNtHeader32 =
		(IMAGE_NT_HEADERS32*)(pDosHeader->e_lfanew + (ULONG)pDosHeader);
	//��չͷ
	IMAGE_OPTIONAL_HEADER32 *pOptionalHeader32 =
		(IMAGE_OPTIONAL_HEADER32*)(&pNtHeader32->OptionalHeader);
	//����Ŀ¼��
	m_pDataDir = pOptionalHeader32->DataDirectory;
}

void CPeParser::FILE_PE_64()
{
	IMAGE_DOS_HEADER* pDosHeader = (IMAGE_DOS_HEADER*)m_pImageBase;
	IMAGE_NT_HEADERS64 *pNtHeader64 =
		(IMAGE_NT_HEADERS64*)(pDosHeader->e_lfanew + (ULONG64)pDosHeader);
	IMAGE_OPTIONAL_HEADER64 *pOptionalHeader64 =
		(IMAGE_OPTIONAL_HEADER64*)&pNtHeader64->OptionalHeader;
	m_pDataDir = pOptionalHeader64->DataDirectory;
}

void CPeParser::showExportTableInfo()
{
	if (m_pDataDir == nullptr) {
		qout << qtr("���ȵ��� parsePE ������PE�ļ����н���") << endl;
		return;
	}

	qout << qtr("��������Ϣ:") << endl;

	//��ȡ�������RVA
	DWORD dwExpTableRVA = m_pDataDir[0].VirtualAddress;

	if (m_pDataDir[0].Size == 0) {
		qout << qtr("û������") << endl;
		return;
	}

	//ULONG64 ul64ExpTableFOA = rva2foa(dwExpTableRVA);
	IMAGE_EXPORT_DIRECTORY *pExpTable = (IMAGE_EXPORT_DIRECTORY*)(dwExpTableRVA + (ULONG64)m_pImageBase);

	//ULONG64 ul64NameOffset = rva2foa(pExpTable->Name);
	char *pDllName = (char*)(pExpTable->Name + (ULONG64)m_pImageBase);
	qout << qtr(pDllName) << endl;

	//��������ַ���RVAת��FOA
	//DWORD dwAddrTableFoa = rva2foa(pExpTable->AddressOfFunctions);
	//�õ�������ַ��
	ULONG64 *pAddrTable = (ULONG64*)(pExpTable->AddressOfFunctions + (ULONG64)m_pImageBase);

	//�õ�������ű�ĵ�ַ
	//WORD *pOrdinalTable = (WORD*)((ULONG64)m_pImageBase + rva2foa(pExpTable->AddressOfNameOrdinals));
	WORD *pOrdinalTable = (WORD*)((ULONG64)m_pImageBase + pExpTable->AddressOfNameOrdinals);
	//�õ��������Ʊ�ĵ�ַ
	//DWORD *pNameTable = (DWORD*)((ULONG64)m_pImageBase + rva2foa(pExpTable->AddressOfNames));
	DWORD *pNameTable = (DWORD*)((ULONG64)m_pImageBase + pExpTable->AddressOfNames);

	bool bIndexExist = false;
	for (DWORD i = 0; i < pExpTable->NumberOfFunctions; i++) {
		//��ӡ��ַ
		qout << QString::asprintf("�����[%d]  ", i) << endl;

		//RVA
		qout << QString::asprintf("��ַ(RVA): %08X (h)  ", pAddrTable[i]) << endl;

		bIndexExist = false;
		DWORD dwNameIdx = 0;
		for (; dwNameIdx < pExpTable->NumberOfNames; dwNameIdx++) {
			//�жϵ�����ַ����±��Ƿ��������ű���
			if (i == pOrdinalTable[dwNameIdx]) {
				bIndexExist = true;
				break;
			}
		}

		if (bIndexExist) {
			//�õ��������Ʊ��RVA
			DWORD dwNameRVA = pNameTable[dwNameIdx];
			//char *pFuncName = (char*)((ULONG64)m_pImageBase + rva2foa(dwNameRVA));
			char *pFuncName = (char*)((ULONG64)m_pImageBase + dwNameRVA);
			qout << qtr("������: %1").arg(pFuncName) << endl;
		}
		else {
			//�жϵ�����ַ��ǰ��������Ԫ���Ƿ񱣴��ŵ�ַ	
			if (pAddrTable[i] != NULL) {
				//i �ǵ�����ַ���е������ţ���һ�������
				// ����������� = ����� + ��Ż���
				qout << QString::asprintf("���:[%d]", pExpTable->Base) << endl;
			}
		}

		qout << endl;
	}
}

void CPeParser::showImportTableInfo()
{
	if (m_pDataDir == nullptr) {
		qout << qtr("���ȵ��� parsePE ������PE�ļ����н���") << endl;
		return;
	}

	qout << qtr("�������Ϣ:") << endl;

	//��õ�����RVA
	DWORD dwImpTableRVA = m_pDataDir[1].VirtualAddress;
	if (m_pDataDir[1].Size == 0) {
		qout << qtr("û������") << endl;
		return;
	}

	//��������RVAת��FOA
	//DWORD dwImpTableFOA = rva2foa(dwImpTableRVA);

	//��ȡ�����
	IMAGE_IMPORT_DESCRIPTOR *pImpTable =
		(IMAGE_IMPORT_DESCRIPTOR*)((ULONG64)m_pImageBase + dwImpTableRVA);

	//���������
	if (m_bPEIs32) { //---------32λPE

		//�ж��Ƿ�����������һ���ṹ��
		while (pImpTable->Name != 0) {
			//����������� Dll ��ģ����
			//DWORD dwNameFOA = rva2foa(pImpTable->Name);
			char *pDllName = (char*)(pImpTable->Name + (ULONG64)m_pImageBase);
			qout << qtr(pDllName) << endl;

			//������ǰdll�ĵ��뺯������
			//pImpTable->OriginalFirstThunk;  //�������Ʊ�
			//pImpTable->FirstThunk;          //�����ַ��
			//����˵�����������ļ��б������������ȫ��ͬ��

			//DWORD dwIATfoa = rva2foa(pImpTable->OriginalFirstThunk);
			IMAGE_THUNK_DATA32 *pIAT = (IMAGE_THUNK_DATA32*)((ULONG64)m_pImageBase + pImpTable->OriginalFirstThunk);

			//����IAT
			//  IAT ��һ�� IMAGE_THUNK_DATA �Ľṹ������
			while (pIAT->u1.AddressOfData != 0) {
				//�ж����λ�Ƿ���1
				if (IMAGE_SNAP_BY_ORDINAL32(pIAT->u1.Ordinal)) { //����������ŵ����
					// ��Ŷ���WORD���ͣ�����ֻȡ���ĵ�16λ
					qout << QString::asprintf("�������:[%d]", pIAT->u1.Ordinal & 0xFFFF) << endl;
				}
				else { //�����������Ƶ����
					//�ֶα�����һ��ָ��IMAGE_IMPORT_BY_NAME�ṹ���RVA
					//DWORD dwFuncNameFOA = rva2foa(pIAT->u1.AddressOfData);
					PIMAGE_IMPORT_BY_NAME pImpName =
						(PIMAGE_IMPORT_BY_NAME)((ULONG64)m_pImageBase + pIAT->u1.AddressOfData);

					qout << qtr("��������:[%1]").arg(pImpName->Name) << endl;
				}

				pIAT++;
			}

			pImpTable++;
			qout << endl;
		}
	}
	else {  //------------64λPE
		//�ж��Ƿ�����������һ���ṹ��
		while (pImpTable->Name != 0) {
			//����������� Dll ��ģ����
			//DWORD dwNameFOA = rva2foa(pImpTable->Name);
			char *pDllName = (char*)(pImpTable->Name + (ULONG64)m_pImageBase);
			qout << qtr(pDllName) << endl;

			//������ǰdll�ĵ��뺯������
			//pImpTable->OriginalFirstThunk;  //�������Ʊ�
			//pImpTable->FirstThunk;          //�����ַ��

			//DWORD dwIATfoa = rva2foa(pImpTable->OriginalFirstThunk);
			IMAGE_THUNK_DATA64 *pIAT = (IMAGE_THUNK_DATA64*)((ULONG64)m_pImageBase + pImpTable->OriginalFirstThunk);

			//����IAT
			//  IAT ��һ�� IMAGE_THUNK_DATA �Ľṹ������
			while (pIAT->u1.AddressOfData != 0) {
				//�ж����λ�Ƿ���1
				if (IMAGE_SNAP_BY_ORDINAL64(pIAT->u1.Ordinal)) { //����������ŵ����
					// ��Ŷ���WORD���ͣ�����ֻȡ���ĵ�16λ
					qout << QString::asprintf("�������:[%d]", pIAT->u1.Ordinal & 0xFFFF) << endl;
				}
				else { //�����������Ƶ����
					//�ֶα�����һ��ָ��IMAGE_IMPORT_BY_NAME�ṹ���RVA
					//DWORD dwFuncNameFOA = rva2foa(pIAT->u1.AddressOfData);
					PIMAGE_IMPORT_BY_NAME pImpName =
						(PIMAGE_IMPORT_BY_NAME)((ULONG64)m_pImageBase + pIAT->u1.AddressOfData);

					qout << qtr("��������:[%1]").arg(pImpName->Name) << endl;
				}

				pIAT++;
			}

			pImpTable++;
			qout << endl;
		}
	}
}

#if 0
DWORD CPeParser::rva2foa(DWORD dwRva)
{
	//��ȡ���θ���
	DWORD dwSectionNum = m_pNtHeader->FileHeader.NumberOfSections;
	//�õ���һ������
	IMAGE_SECTION_HEADER *pSectionHeader = IMAGE_FIRST_SECTION(m_pNtHeader);

	for (DWORD i = 0; i < dwSectionNum; i++) {
		//�ж�RVA�Ƿ��ڵ�ǰ��������
		DWORD dwSectionEndRva =
			pSectionHeader[i].VirtualAddress + pSectionHeader[i].SizeOfRawData;

		if (dwRva >= pSectionHeader[i].VirtualAddress && dwRva <= dwSectionEndRva) {
			//�ڵ�ǰ���η�Χ�ڣ������FOA
			DWORD dwFoa = dwRva - pSectionHeader[i].VirtualAddress + pSectionHeader[i].PointerToRawData;
			return dwFoa;
		}
	}

	return -1;
}
#endif
