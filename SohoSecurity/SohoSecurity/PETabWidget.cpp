#include "stdafx.h"
#include "PETabWidget.h"
#include "Md5.h"
#include "OffsetCounterDlg.h"
#include <strsafe.h>
#include <time.h>

#pragma comment(lib, "Version.lib")

PETabWidget::PETabWidget(QWidget *parent)
	: QWidget(parent), m_pFileImageBase(nullptr), m_bPEIs32(true)
{
	ui.setupUi(this);

	ui.textBrwFileInfo->setText("<br><h1><center>���ļ���ק������</center></h1>");
	ui.textBrwFileInfo->setFontFamily(tr("΢���ź�"));
	ui.textBrwPEHeader->setFontFamily(tr("΢���ź�"));
	ui.textBrwPEData->setFontFamily(tr("΢���ź�"));
	ui.psBtnOffsetCounter->setEnabled(false);
	setAcceptDrops(true);
}

PETabWidget::~PETabWidget()
{
	if (m_pFileImageBase) {
		delete[] m_pFileImageBase;
		m_pFileImageBase = nullptr;
	}
}

void PETabWidget::dragEnterEvent(QDragEnterEvent *event)
{
	event->acceptProposedAction();
}

void PETabWidget::dropEvent(QDropEvent *event)
{
	QList<QUrl> urls = event->mimeData()->urls();
	if (urls.isEmpty()) {
		return;
	}

	QString strFileName = urls.first().toLocalFile();
	if (strFileName.isEmpty()) {
		return;
	}

	ui.textBrwFileInfo->clear();
	ui.textBrwPEHeader->clear();
	ui.textBrwPEData->clear();

	getFileInfo(strFileName);
	getPEInfo(strFileName);
}

void PETabWidget::getPEInfo(QString strFilePath)
{
	TCHAR szFilePath[MAX_PATH] = {};
	strFilePath.toWCharArray(szFilePath);

	HANDLE hFile = CreateFile(szFilePath,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		ui.psBtnOffsetCounter->setEnabled(false);
		QMessageBox::critical(this, tr("����"), tr("�����ļ�ʧ��"));
		return;
	}

	DWORD dwFileSize = GetFileSize(hFile, NULL);
	m_pFileImageBase = new BYTE[dwFileSize]{};
	DWORD dwReadSize = 0;
	if (!ReadFile(hFile, m_pFileImageBase, dwFileSize, &dwReadSize, NULL)) {
		ui.psBtnOffsetCounter->setEnabled(false);
		QMessageBox::critical(this, tr("����"), tr("�����ļ�ʧ��"));
		return;
	}

	IMAGE_DOS_HEADER *pDosHeader = (IMAGE_DOS_HEADER*)m_pFileImageBase;
	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
		ui.psBtnOffsetCounter->setEnabled(false);
		QMessageBox::critical(this, tr("����"), tr("����PE�ļ�"));
		return;
	}

	m_pNtHeader = (IMAGE_NT_HEADERS*)(pDosHeader->e_lfanew + (ULONG64)pDosHeader);
	if (m_pNtHeader->Signature != IMAGE_NT_SIGNATURE) {
		ui.psBtnOffsetCounter->setEnabled(false);
		QMessageBox::critical(this, tr("����"), tr("����PE�ļ�"));
		return;
	}

	getNtHeaderInfo(m_pNtHeader);
	CloseHandle(hFile);
	ui.psBtnOffsetCounter->setEnabled(true);
}

void PETabWidget::getNtHeaderInfo(PIMAGE_NT_HEADERS pNtHeader)
{
	if (pNtHeader == NULL)
		return;

	ui.textBrwPEHeader->append(tr("===========PE�ļ�ͷ����=========="));
	IMAGE_FILE_HEADER *pFileHeader = &pNtHeader->FileHeader;

	//����ƽ̨
	if (pFileHeader->Machine == IMAGE_FILE_MACHINE_I386)
		ui.textBrwPEHeader->append(tr("����ƽ̨: Intel 386"));
	else if (pFileHeader->Machine == IMAGE_FILE_MACHINE_IA64)
		ui.textBrwPEHeader->append(tr("����ƽ̨: Intel 64"));
	else if (pFileHeader->Machine == IMAGE_FILE_MACHINE_AMD64)
		ui.textBrwPEHeader->append(tr("����ƽ̨: AMD64"));
	else
		ui.textBrwPEHeader->append(tr("����ƽ̨: ����"));

	//�ļ���������
	ui.textBrwPEHeader->append(tr("�ļ���������: %1").arg(pFileHeader->NumberOfSections));

	//�ļ�����ʱ��
	tm tmGTime = { 0 };
	_gmtime64_s(&tmGTime, (__time64_t*)&pFileHeader->TimeDateStamp);
	TCHAR tmpTimeBuf[64] = {};
	_wasctime_s(tmpTimeBuf, &tmGTime);
	ui.textBrwPEHeader->append(tr("�ļ�����ʱ��: ") + QString::asprintf("%d-%d-%d %d:%d:%d",
		tmGTime.tm_year + 1900, tmGTime.tm_mon + 1, tmGTime.tm_mday,
		tmGTime.tm_hour, tmGTime.tm_min, tmGTime.tm_sec));

	//��չͷ��С
	ui.textBrwPEHeader->append(QString::asprintf("��չͷ��С: %d �ֽ�", pFileHeader->SizeOfOptionalHeader));

	//32λPE����64λPE
	if (pNtHeader->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
		m_bPEIs32 = true;
		ui.textBrwPEHeader->append(tr("PE����: 32λ\n"));
		FILE_PE_32();
	}
	else if (pNtHeader->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC){
		m_bPEIs32 = false;
		ui.textBrwPEHeader->append(tr("PE����: 64λ\n"));
		FILE_PE_64();
	}
}

//���32λPE�ļ����н���
void PETabWidget::FILE_PE_32()
{
	IMAGE_DOS_HEADER *pDosHeader = (IMAGE_DOS_HEADER*)m_pFileImageBase;

	//Ntͷ
	IMAGE_NT_HEADERS32 *pNtHeader32 =
		(IMAGE_NT_HEADERS32*)(pDosHeader->e_lfanew + (ULONG)pDosHeader);
	//��չͷ
	IMAGE_OPTIONAL_HEADER32 *pOptionalHeader32 =
		(IMAGE_OPTIONAL_HEADER32*)(&pNtHeader32->OptionalHeader);
	//����Ŀ¼��
	IMAGE_DATA_DIRECTORY *pDataDir = pOptionalHeader32->DataDirectory;

	getSectionsTableInfo(pNtHeader32, NULL);
	getOptionalInfo(pOptionalHeader32, NULL);
	getDataDirTableInfo(pOptionalHeader32, NULL);
	getExportTableInfo(pDataDir);
	getImportTableInfo(pDataDir);
	getResourceTableInfo(pDataDir);
	getRelocationeTableInfo(pDataDir);
	getDelayImportTableInfo(pDataDir);
	getTLSTableInfo(pDataDir);
}

//���64λPE�ļ����н���
void PETabWidget::FILE_PE_64()
{
	IMAGE_DOS_HEADER* pDosHeader = (IMAGE_DOS_HEADER*)m_pFileImageBase;
	IMAGE_NT_HEADERS64 *pNtHeader64 =
		(IMAGE_NT_HEADERS64*)(pDosHeader->e_lfanew + (ULONG64)pDosHeader);
	IMAGE_OPTIONAL_HEADER64 *pOptionalHeader64 =
		(IMAGE_OPTIONAL_HEADER64*)&pNtHeader64->OptionalHeader;
	IMAGE_DATA_DIRECTORY *pDataDir = pOptionalHeader64->DataDirectory;

	getSectionsTableInfo(NULL, pNtHeader64);
	getOptionalInfo(NULL, pOptionalHeader64);
	getDataDirTableInfo(NULL, pOptionalHeader64);
	getExportTableInfo(pDataDir);
	getImportTableInfo(pDataDir);
	getResourceTableInfo(pDataDir);
	getRelocationeTableInfo(pDataDir);
	getDelayImportTableInfo(pDataDir);
	getTLSTableInfo(pDataDir);
}

void PETabWidget::getSectionsTableInfo(PIMAGE_NT_HEADERS32 pNtHeader32, PIMAGE_NT_HEADERS64 pNtHeader64)
{
	m_strSectionTable.clear();
	PIMAGE_SECTION_HEADER pSectionHeader = NULL;
	QString strTmp;
	int nCnt = 0;

	if (m_bPEIs32) { //32λPE
		pSectionHeader = IMAGE_FIRST_SECTION(pNtHeader32);
		nCnt = pNtHeader32->FileHeader.NumberOfSections;
	}
	else {  //64λPE
		pSectionHeader = IMAGE_FIRST_SECTION(pNtHeader64);
		nCnt = pNtHeader64->FileHeader.NumberOfSections;
	}

	for (int i = 0; i < nCnt; i++) {
		char szTmpBuf[9] = { 0 };
		memcpy_s(szTmpBuf, _countof(szTmpBuf), pSectionHeader[i].Name, _countof(pSectionHeader[i].Name));
		strTmp = tr("������: %1\n").arg(szTmpBuf);
		m_strSectionTable += strTmp;

		strTmp = QString::asprintf("�������ݵ��ļ�ƫ��: %X (h)\n", pSectionHeader[i].PointerToRawData);
		m_strSectionTable += strTmp;

		strTmp = QString::asprintf("�������ݵ�RVA: %X (h)\n", pSectionHeader[i].VirtualAddress);
		m_strSectionTable += strTmp;

		strTmp = QString::asprintf("�������ݵĴ�С: %X (h)\n\n\n", pSectionHeader[i].SizeOfRawData);
		m_strSectionTable += strTmp;
	}
}

void PETabWidget::getOptionalInfo(PIMAGE_OPTIONAL_HEADER32 pOptionalHeader32, 
	PIMAGE_OPTIONAL_HEADER64 pOptionalHeader64)
{
	ui.textBrwPEHeader->append(tr("===========PE��չͷ����=========="));

	QString strTmp;
	if (m_bPEIs32) {  //32λPE
		strTmp = QString::asprintf("����ִ�����RVA: %X (h)", pOptionalHeader32->AddressOfEntryPoint);
		ui.textBrwPEHeader->append(strTmp);

		strTmp = QString::asprintf("�����ַRVA: %X (h)", pOptionalHeader32->ImageBase);
		ui.textBrwPEHeader->append(strTmp);

		strTmp = QString::asprintf("�����ַRVA: %X (h)", pOptionalHeader32->BaseOfCode);
		ui.textBrwPEHeader->append(strTmp);

		strTmp = QString::asprintf("���ݻ�ַRVA: %X (h)", pOptionalHeader32->BaseOfData);
		ui.textBrwPEHeader->append(strTmp);

		strTmp = QString::asprintf("�ڴ�������Ķ����С: %X (h)", pOptionalHeader32->SectionAlignment);
		ui.textBrwPEHeader->append(strTmp);

		strTmp = QString::asprintf("�ļ�������Ķ����С: %X (h)", pOptionalHeader32->FileAlignment);
		ui.textBrwPEHeader->append(strTmp);

		strTmp = QString::asprintf("PE���ص��ڴ��Ĵ�С: %X (h)", pOptionalHeader32->SizeOfImage);
		ui.textBrwPEHeader->append(strTmp);

		strTmp = QString::asprintf("����ͷ+���α�Ĵ�С: %X (h)\n", pOptionalHeader32->SizeOfHeaders);
		ui.textBrwPEHeader->append(strTmp);
	}
	else {  //64λPE
		strTmp = QString::asprintf("����ִ�����RVA: %X (h)", pOptionalHeader64->AddressOfEntryPoint);
		ui.textBrwPEHeader->append(strTmp);

		strTmp = QString::asprintf("�����ַRVA: %X (h)", pOptionalHeader64->ImageBase);
		ui.textBrwPEHeader->append(strTmp);

		strTmp = QString::asprintf("�����ַRVA: %X (h)", pOptionalHeader64->BaseOfCode);
		ui.textBrwPEHeader->append(strTmp);

		strTmp = QString::asprintf("�ڴ�������Ķ����С: %X (h)", pOptionalHeader64->SectionAlignment);
		ui.textBrwPEHeader->append(strTmp);

		strTmp = QString::asprintf("�ļ�������Ķ����С: %X (h)", pOptionalHeader64->FileAlignment);
		ui.textBrwPEHeader->append(strTmp);

		strTmp = QString::asprintf("PE���ص��ڴ��Ĵ�С: %X (h)", pOptionalHeader64->SizeOfImage);
		ui.textBrwPEHeader->append(strTmp);

		strTmp = QString::asprintf("����ͷ+���α�Ĵ�С: %X (h)\n", pOptionalHeader64->SizeOfHeaders);
		ui.textBrwPEHeader->append(strTmp);
	}
}

void PETabWidget::getDataDirTableInfo(PIMAGE_OPTIONAL_HEADER32 pOptionalHeader32, 
	PIMAGE_OPTIONAL_HEADER64 pOptionalHeader64)
{
	m_strDataDirTable.clear();

	if (m_bPEIs32) { //32λPE
		for (DWORD i = 0; i < pOptionalHeader32->NumberOfRvaAndSizes; i++) {
			m_strDataDirTable += 
				QString::asprintf("��%d��RVA: %X (h)   ---- ��Ĵ�С: %X (h)\n\n",
				i, pOptionalHeader32->DataDirectory[i].VirtualAddress, pOptionalHeader32->DataDirectory[i].Size);
		}
	}
	else { //64λPE
		for (DWORD i = 0; i < pOptionalHeader64->NumberOfRvaAndSizes; i++) {
			m_strDataDirTable += 
				QString::asprintf("��%d��RVA: %X (h)   ---- ��Ĵ�С: %X (h)\n\n",
				i, pOptionalHeader64->DataDirectory[i].VirtualAddress, pOptionalHeader64->DataDirectory[i].Size);
		}
	}
}

void PETabWidget::getExportTableInfo(PIMAGE_DATA_DIRECTORY pDataDir)
{
	m_strExpTable.clear();

	//��ȡ�������RVA
	DWORD dwExpTableRVA = pDataDir[0].VirtualAddress;
	
	if (pDataDir[0].Size == 0) {
		m_strExpTable = tr("û������");
		return;
	}

	ULONG64 ul64ExpTableFOA = rva2foa(dwExpTableRVA);
	IMAGE_EXPORT_DIRECTORY *pExpTable = (IMAGE_EXPORT_DIRECTORY*)(ul64ExpTableFOA + (ULONG64)m_pFileImageBase);

	ULONG64 ul64NameOffset = rva2foa(pExpTable->Name);
	char *pDllName = (char*)(ul64NameOffset + (ULONG64)m_pFileImageBase);
	m_strExpTable += pDllName;
	m_strExpTable += tr("\n\n");

	//��������ַ���RVAת��FOA
	DWORD dwAddrTableFoa = rva2foa(pExpTable->AddressOfFunctions);
	//�õ�������ַ��
	ULONG64 *pAddrTable = (ULONG64*)(dwAddrTableFoa + (ULONG64)m_pFileImageBase);

	//�õ�������ű�ĵ�ַ
	WORD *pOrdinalTable = (WORD*)((ULONG64)m_pFileImageBase + rva2foa(pExpTable->AddressOfNameOrdinals));
	//�õ��������Ʊ�ĵ�ַ
	DWORD *pNameTable = (DWORD*)((ULONG64)m_pFileImageBase + rva2foa(pExpTable->AddressOfNames));

	bool bIndexExist = false;
	for (DWORD i = 0; i < pExpTable->NumberOfFunctions; i++) {
		//��ӡ��ַ
		m_strExpTable += QString::asprintf("�����[%d]  ", i);

		//RVA
		m_strExpTable += QString::asprintf("��ַ(RVA): %08X (h)    ", pAddrTable[i]);

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
			//���������Ʊ�RVAת����FOA
			char *pFuncName = (char*)((ULONG64)m_pFileImageBase + rva2foa(dwNameRVA));
			m_strExpTable += tr("������: %1").arg(pFuncName);
		}
		else {
			//�жϵ�����ַ��ǰ��������Ԫ���Ƿ񱣴��ŵ�ַ	
			if (pAddrTable[i] != NULL) {
				//i �ǵ�����ַ���е������ţ���һ�������
				// ����������� = ����� + ��Ż���
				m_strExpTable += QString::asprintf("���:[%d]", pExpTable->Base);
			}
		}

		m_strExpTable += tr("\n\n");
	}
}

void PETabWidget::getImportTableInfo(PIMAGE_DATA_DIRECTORY pDataDir)
{
	m_strImpTable.clear();

	//��õ�����RVA
	DWORD dwImpTableRVA = pDataDir[1].VirtualAddress;
	if (pDataDir[1].Size == 0) {
		m_strImpTable = tr("û������");
		return;
	}

	//��������RVAת��FOA
	DWORD dwImpTableFOA = rva2foa(dwImpTableRVA);

	//��ȡ�����
	IMAGE_IMPORT_DESCRIPTOR *pImpTable =
		(IMAGE_IMPORT_DESCRIPTOR*)((ULONG64)m_pFileImageBase + dwImpTableFOA);

	//���������
	if (m_bPEIs32) { //---------32λPE

		//�ж��Ƿ�����������һ���ṹ��
		while (pImpTable->Name != 0) {
			//����������� Dll ��ģ����
			DWORD dwNameFOA = rva2foa(pImpTable->Name);
			char *pDllName = (char*)(dwNameFOA + (ULONG64)m_pFileImageBase);
			m_strImpTable += tr(pDllName);
			m_strImpTable += tr("\n\n");

			//������ǰdll�ĵ��뺯������
			//pImpTable->OriginalFirstThunk;  //�������Ʊ�
			//pImpTable->FirstThunk;          //�����ַ��
			//����˵�����������ļ��б������������ȫ��ͬ��

			DWORD dwIATfoa = rva2foa(pImpTable->OriginalFirstThunk);
			IMAGE_THUNK_DATA32 *pIAT = (IMAGE_THUNK_DATA32*)((ULONG64)m_pFileImageBase + dwIATfoa);

			//����IAT
			//  IAT ��һ�� IMAGE_THUNK_DATA �Ľṹ������
			while (pIAT->u1.AddressOfData != 0) {
				//�ж����λ�Ƿ���1
				if (IMAGE_SNAP_BY_ORDINAL32(pIAT->u1.Ordinal)) { //����������ŵ����
					// ��Ŷ���WORD���ͣ�����ֻȡ���ĵ�16λ
					m_strImpTable += QString::asprintf("�������:[%d]", pIAT->u1.Ordinal & 0xFFFF);
				}
				else { //�����������Ƶ����
					//�ֶα�����һ��ָ��IMAGE_IMPORT_BY_NAME�ṹ���RVA
					DWORD dwFuncNameFOA = rva2foa(pIAT->u1.AddressOfData);
					PIMAGE_IMPORT_BY_NAME pImpName = 
						(PIMAGE_IMPORT_BY_NAME)((ULONG64)m_pFileImageBase + dwFuncNameFOA);

					m_strImpTable += tr("��������:[%1]").arg(pImpName->Name);
				}

				pIAT++;
				m_strImpTable += tr("\n");
			}

			pImpTable++;
			m_strImpTable += tr("\n\n");
		}
	}
	else {  //------------64λPE
		//�ж��Ƿ�����������һ���ṹ��
		while (pImpTable->Name != 0) {
			//����������� Dll ��ģ����
			DWORD dwNameFOA = rva2foa(pImpTable->Name);
			char *pDllName = (char*)(dwNameFOA + (ULONG64)m_pFileImageBase);
			m_strImpTable += tr(pDllName);
			m_strImpTable += tr("\n\n");

			//������ǰdll�ĵ��뺯������
			//pImpTable->OriginalFirstThunk;  //�������Ʊ�
			//pImpTable->FirstThunk;          //�����ַ��
			//����˵�����������ļ��б������������ȫ��ͬ��

			DWORD dwIATfoa = rva2foa(pImpTable->OriginalFirstThunk);
			IMAGE_THUNK_DATA64 *pIAT = (IMAGE_THUNK_DATA64*)((ULONG64)m_pFileImageBase + dwIATfoa);

			//����IAT
			//  IAT ��һ�� IMAGE_THUNK_DATA �Ľṹ������
			while (pIAT->u1.AddressOfData != 0) {
				//�ж����λ�Ƿ���1
				if (IMAGE_SNAP_BY_ORDINAL64(pIAT->u1.Ordinal)) { //����������ŵ����
					// ��Ŷ���WORD���ͣ�����ֻȡ���ĵ�16λ
					m_strImpTable += QString::asprintf("�������:[%d]", pIAT->u1.Ordinal & 0xFFFF);
				}
				else { //�����������Ƶ����
					//�ֶα�����һ��ָ��IMAGE_IMPORT_BY_NAME�ṹ���RVA
					DWORD dwFuncNameFOA = rva2foa(pIAT->u1.AddressOfData);
					PIMAGE_IMPORT_BY_NAME pImpName =
						(PIMAGE_IMPORT_BY_NAME)((ULONG64)m_pFileImageBase + dwFuncNameFOA);

					m_strImpTable += tr("��������:[%1]").arg(pImpName->Name);
				}

				pIAT++;
				m_strImpTable += tr("\n");
			}

			pImpTable++;
			m_strImpTable += tr("\n\n");
		}
	}
}

void PETabWidget::getResourceTableInfo(PIMAGE_DATA_DIRECTORY pDataDir)
{
	m_strResTable.clear();

	if (pDataDir[2].Size == 0) {
		m_strResTable += tr("û������");
		return;
	}

	//��ȡ��Դ���FOA
	DWORD dwResTableFOA = rva2foa(pDataDir[2].VirtualAddress);

	//ָ���һ��Ŀ¼
	IMAGE_RESOURCE_DIRECTORY *pRoot = 
		(IMAGE_RESOURCE_DIRECTORY*)((ULONG64)m_pFileImageBase + dwResTableFOA);

	IMAGE_RESOURCE_DIRECTORY *pDir2 = NULL;  //ָ��ڶ���Ŀ¼
	IMAGE_RESOURCE_DIRECTORY *pDir3 = NULL;  //ָ�������Ŀ¼

	IMAGE_RESOURCE_DIRECTORY_ENTRY *pEntry1 = NULL;
	IMAGE_RESOURCE_DIRECTORY_ENTRY *pEntry2 = NULL;
	IMAGE_RESOURCE_DIRECTORY_ENTRY *pEntry3 = NULL;

	IMAGE_RESOURCE_DATA_ENTRY *pDataEntry = NULL;
	IMAGE_RESOURCE_DIR_STRING_U *pIdStr = NULL;

	pEntry1 = (IMAGE_RESOURCE_DIRECTORY_ENTRY *)(pRoot + 1);
	for (int i = 0; i < pRoot->NumberOfIdEntries + pRoot->NumberOfNamedEntries; i++) {
		//��ȡ��һ��Ŀ¼��ڵ�ID(ID������Դ������)
		if (pEntry1->NameIsString == 1) {
			//NameOffset����ֶε�ֵ��һ��ƫ��
			//  ���ƫ��������Դ���Ŀ¼�ĵ�ַ��Ϊ��ַ
			pIdStr = (IMAGE_RESOURCE_DIR_STRING_U *)(pEntry1->NameOffset + (ULONG64)pRoot);
			TCHAR *pNameBuf = new TCHAR[pIdStr->Length + 1]{};
			StringCchCopy(pNameBuf, pIdStr->Length + 1, pIdStr->NameString);
			m_strResTable += tr("��Դ����: %1\n\n").arg(QString::fromWCharArray(pNameBuf));
			delete[] pNameBuf; pNameBuf = nullptr;
		}
		else {
			QString strTypeArr[] = {
				tr(""),          //0
				tr("���ָ��"),   //1
				tr("λͼ"),       //2
				tr("ͼ��"),	      //3
				tr("�˵�"),       //4
				tr("�Ի���"),     //5
				tr("�ַ����б�"),  //6
				tr("����Ŀ¼"),    //7
				tr("����"),       //8
				tr("��ݼ�"),      //9
				tr("�Ǹ�ʽ����Դ"), //A
				tr("��Ϣ�б�"),     //B
				tr("���ָ������"),  //C
				tr(""),            //D
				tr("ͼ����"),       //E
				tr(""),            //F
				tr("�汾��Ϣ"),     //10
			};

			if (pEntry1->Id > 16) {
				m_strResTable += QString::asprintf("��Դ����: %d\n\n", pEntry1->Id);
			}
			else {
				m_strResTable += tr("��Դ����: %1").arg(strTypeArr[pEntry1->Id]);
			}
		}

		if (pEntry1->DataIsDirectory == 1) {
			//�õ��ڶ���Ŀ¼�ĵ�ַ
			pDir2 =
				(IMAGE_RESOURCE_DIRECTORY*)(pEntry1->OffsetToDirectory + (ULONG64)pRoot);

			//�����ڶ�����ԴĿ¼������Ŀ¼���	
			pEntry2 = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pDir2 + 1);
			for (int j = 0; j < pDir2->NumberOfIdEntries + pDir2->NumberOfNamedEntries; j++) {
				//�õ���Դ��ID
				if (pEntry2->NameIsString == 1) {
					pIdStr = (IMAGE_RESOURCE_DIR_STRING_U *)(pEntry2->NameOffset + (ULONG64)pRoot);
					TCHAR *pNameBuf = new TCHAR[pIdStr->Length + 1]{};
					StringCchCopy(pNameBuf, pIdStr->Length + 1, pIdStr->NameString);
					m_strResTable += tr(" +��ԴID: %1\n").arg(QString::fromWCharArray(pNameBuf));
					delete[] pNameBuf; pNameBuf = nullptr;
				}
				else {
					m_strResTable += QString::asprintf(" +��ԴID: %d\n", (DWORD)pEntry2->Id);
				}

				if (pEntry2->DataIsDirectory == 1) {
					//�õ�������Ŀ¼�ĵ�ַ
					pDir3 =
						(IMAGE_RESOURCE_DIRECTORY*)(pEntry2->OffsetToDirectory + (ULONG64)pRoot);

					pEntry3 =
						(IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pDir3 + 1);

					m_strResTable += QString::asprintf("   +id: %d\n", pEntry3->Id);

					//�õ��������
					pDataEntry =
						(IMAGE_RESOURCE_DATA_ENTRY*)(pEntry3->OffsetToData + (ULONG64)pRoot);

					m_strResTable += QString::asprintf("   +��Դƫ��(RVA): %X (h)\n", pDataEntry->OffsetToData);
					m_strResTable += QString::asprintf("   +��Դ��С: %X (h)\n\n", pDataEntry->Size);
				}
			}
		}

		m_strResTable += tr("\n\n");
	}
}

void PETabWidget::getRelocationeTableInfo(PIMAGE_DATA_DIRECTORY pDataDir)
{
	m_strRelocTable.clear();

	if (pDataDir[5].Size == 0) {
		m_strRelocTable = tr("û������");
		return;
	}

	//�õ���һ���ض�λ��������FOA
	DWORD dwRelocBlockFOA = rva2foa(pDataDir[5].VirtualAddress);
	
	//�õ���һ���ض�λ��ĵ�ַ
	IMAGE_BASE_RELOCATION *pRelocBlock =
		(IMAGE_BASE_RELOCATION*)((ULONG64)m_pFileImageBase + dwRelocBlockFOA);

	while (true) {
		//�ж��Ƿ����һ���ض�λ��
		if (pRelocBlock->SizeOfBlock == 0 && pRelocBlock->VirtualAddress == 0)
			break;

		m_strRelocTable +=
			QString::asprintf("�鿪ʼ��RVA: %X (h), �ֽ���: %X (h)\n\n",
			pRelocBlock->VirtualAddress, pRelocBlock->SizeOfBlock);

		//�����ض�λ���ͺ�ƫ�Ƶ����ݿ�
		TypeOffset *pTypeOffset =
			(TypeOffset*)((ULONG64)pRelocBlock + sizeof(IMAGE_BASE_RELOCATION));

		DWORD dwCount =
			(pRelocBlock->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(TypeOffset);

		for (DWORD i = 0; i < dwCount; i++) {
			m_strRelocTable += QString::asprintf("ƫ��: %X (h),  ", pTypeOffset->Offset);

			//�õ���Ҫ�ض�λ������
			DWORD dwRVA = pRelocBlock->VirtualAddress + pTypeOffset[i].Offset;
			DWORD dwFOA = rva2foa(dwRVA);
			DWORD *pRelocData = (DWORD*)(dwFOA + (ULONG64)m_pFileImageBase);

			m_strRelocTable += QString::asprintf("+Ҫ�޸ĵĵ�ַ: %08X (h)\n", *pRelocData);
		}
		m_strRelocTable += tr("\n\n");

		//��һ���ض�λ��
		pRelocBlock =
			(IMAGE_BASE_RELOCATION*)((ULONG64)pRelocBlock + pRelocBlock->SizeOfBlock);
	}
}

//�ӳټ��ص����
void PETabWidget::getDelayImportTableInfo(PIMAGE_DATA_DIRECTORY pDataDir)
{
	m_strDelayLoadTable.clear();

	if (pDataDir[13].Size == 0) {
		m_strDelayLoadTable = tr("û������");
		return;
	}

	DWORD dwDelayTableFOA = rva2foa(pDataDir[13].VirtualAddress);
	IMAGE_DELAYLOAD_DESCRIPTOR *pDelayTable =
		(IMAGE_DELAYLOAD_DESCRIPTOR*)(dwDelayTableFOA + (ULONG64)m_pFileImageBase);

	if (pDelayTable->DllNameRVA == 0x00000300) {
		m_strDelayLoadTable = tr("û������");
		return;
	}

	while (pDelayTable->DllNameRVA) {
		//DllName RVA
		m_strDelayLoadTable += QString::asprintf("DllName RVA: %X (h),  ", pDelayTable->DllNameRVA);

		//DllName
		DWORD dwDllNameFOA = rva2foa(pDelayTable->DllNameRVA);
		char *pDllName = (char*)(dwDllNameFOA + (ULONG64)m_pFileImageBase);
		m_strDelayLoadTable += tr("DllName: %1\n").arg(pDllName);

		//�����ַ��RVA
		m_strDelayLoadTable += QString::asprintf("IAT(R)RVA: %X (h),  ", pDelayTable->ImportAddressTableRVA);

		//�������Ʊ�RVA
		m_strDelayLoadTable += QString::asprintf("INT(R)RVA: %X (h),  \n", pDelayTable->ImportNameTableRVA);

		m_strDelayLoadTable += tr("\n");

		DWORD dwFuncNameFOA = rva2foa(pDelayTable->ImportNameTableRVA);

		if (m_bPEIs32) {  //--------32λPE
			IMAGE_THUNK_DATA32 *pIAT = (IMAGE_THUNK_DATA32*)((ULONG64)m_pFileImageBase + dwFuncNameFOA);

			//����IAT: ��һ��IMAGE_THUNK_DATA�ṹ������
			while (pIAT->u1.AddressOfData != 0) {
				//�ж����λ�Ƿ�Ϊ1
				if (IMAGE_SNAP_BY_ORDINAL32(pIAT->u1.Ordinal)) {
					//����������ŵ����
					// ��Ŷ���WORD���ͣ���ֻȡ���ĵ�16λ
					m_strDelayLoadTable += QString::asprintf("�������: %d\n", pIAT->u1.Ordinal & 0xFFFF);
				}
				else {
					//�����������Ƶ����
					// �ֶα�����һ��ָ�� IMAGE_IMPORT_BY_NAME �ṹ��RVA
					DWORD dwFuncNameFOA = rva2foa(pIAT->u1.AddressOfData);
					IMAGE_IMPORT_BY_NAME *pImportName =
						(IMAGE_IMPORT_BY_NAME*)((ULONG64)m_pFileImageBase + dwFuncNameFOA);

					m_strDelayLoadTable += tr("��������: %1").arg(pImportName->Name);
				}

				//��һ���ṹ��
				pIAT++;
				m_strDelayLoadTable += tr("\n");
			}
		}
		else {  //----------64λPE
			IMAGE_THUNK_DATA64 *pIAT = (IMAGE_THUNK_DATA64*)((ULONG64)m_pFileImageBase + dwFuncNameFOA);

			//����IAT: ��һ��IMAGE_THUNK_DATA�ṹ������
			while (pIAT->u1.AddressOfData != 0) {
				//�ж����λ�Ƿ�Ϊ1
				if (IMAGE_SNAP_BY_ORDINAL64(pIAT->u1.Ordinal)) {
					//����������ŵ����
					// ��Ŷ���WORD���ͣ���ֻȡ���ĵ�16λ
					m_strDelayLoadTable += QString::asprintf("�������: %d\n", pIAT->u1.Ordinal & 0xFFFF);
				}
				else {
					//�����������Ƶ����
					// �ֶα�����һ��ָ�� IMAGE_IMPORT_BY_NAME �ṹ��RVA
					DWORD dwFuncNameFOA = rva2foa(pIAT->u1.AddressOfData);
					IMAGE_IMPORT_BY_NAME *pImportName =
						(IMAGE_IMPORT_BY_NAME*)((ULONG64)m_pFileImageBase + dwFuncNameFOA);

					m_strDelayLoadTable += tr("��������: %1").arg(pImportName->Name);
				}

				//��һ���ṹ��
				pIAT++;
				m_strDelayLoadTable += tr("\n");
			}
		}

		m_strDelayLoadTable += tr("--------------------\n\n");
		pDelayTable++;
	}
}

void PETabWidget::getTLSTableInfo(PIMAGE_DATA_DIRECTORY pDataDir)
{
	m_strTLSTable.clear();

	if (pDataDir[9].Size == 0) {
		m_strTLSTable = "û������";
		return;
	}

	DWORD dwTLSFOA = rva2foa(pDataDir[9].VirtualAddress);

	PIMAGE_TLS_DIRECTORY32 pTLSTable = PIMAGE_TLS_DIRECTORY32(dwTLSFOA + (ULONG64)m_pFileImageBase);

	m_strTLSTable += QString::asprintf("StartAddressOfRawData: %X (h)\n\n", pTLSTable->StartAddressOfRawData);
	m_strTLSTable += QString::asprintf("EndAddressOfRawData: %X (h)\n\n", pTLSTable->EndAddressOfRawData);
	m_strTLSTable += QString::asprintf("AddressOfIndex: %X (h)\n\n", pTLSTable->AddressOfIndex);
	m_strTLSTable += QString::asprintf("AddressOfCallBacks: %X (h)\n\n", pTLSTable->AddressOfCallBacks);
	m_strTLSTable += QString::asprintf("SizeOfZeroFill: %X (h)\n\n", pTLSTable->SizeOfZeroFill);
	m_strTLSTable += QString::asprintf("Characteristics: %X (h)\n\n", pTLSTable->Characteristics);
}

DWORD PETabWidget::rva2foa(DWORD dwRva)
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

void PETabWidget::onPsBtnSectionTableClicked()
{
	ui.groupBoxPEData->setTitle(tr("���α�"));
	ui.textBrwPEData->setText(m_strSectionTable);
}

void PETabWidget::onPsBtnResTableClicked()
{
	ui.groupBoxPEData->setTitle(tr("��Դ��"));
	ui.textBrwPEData->setText(m_strResTable);
}

void PETabWidget::onPsBtnRelocTableClicked()
{
	ui.groupBoxPEData->setTitle(tr("�ض�λ��"));
	ui.textBrwPEData->setText(m_strRelocTable);
}

void PETabWidget::onPsBtnOffsetCounterClicked()
{
	OffsetCounterDlg offsetCounterDlg(m_pNtHeader, this);
	offsetCounterDlg.exec();
}

void PETabWidget::onPsBtnImpTableClicked()
{
	ui.groupBoxPEData->setTitle(tr("�����"));
	ui.textBrwPEData->setText(m_strImpTable);
}

void PETabWidget::onPsBtnExpTableClicked()
{
	ui.groupBoxPEData->setTitle(tr("������"));
	ui.textBrwPEData->setText(m_strExpTable);
}

void PETabWidget::onPsBtnDelayTableClicked()
{
	ui.groupBoxPEData->setTitle(tr("��ʱ���ر�"));
	ui.textBrwPEData->setText(m_strDelayLoadTable);
}

void PETabWidget::onPsBtnDataDirTableClicked()
{
	ui.groupBoxPEData->setTitle(tr("����Ŀ¼��"));
	ui.textBrwPEData->setText(m_strDataDirTable);
}

void PETabWidget::onPsBtnTLSTableClicked()
{
	ui.groupBoxPEData->setTitle(tr("TLS��"));
	ui.textBrwPEData->setText(m_strTLSTable);
}

void PETabWidget::getFileInfo(QString strFilePath)
{
	//�ļ�·��
	ui.textBrwFileInfo->append(tr("�ļ�·��Ϊ: %1").arg(strFilePath));

	//�ļ�MD5
	QString strFileMD5 = md5FileValue((char *)strFilePath.toStdString().c_str());
	ui.textBrwFileInfo->append(tr("�ļ�MD5: %1").arg(strFileMD5));

	//�ļ���Ϣ
	QFileInfo fileInfo(strFilePath);
	if (fileInfo.exists()) {
		ui.textBrwFileInfo->append(tr("����ʱ��: %1").arg(fileInfo.created().toString("yyyy-MM-dd hh:mm:ss")));
		ui.textBrwFileInfo->append(tr("�޸�ʱ��: %1").arg(fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss")));
		ui.textBrwFileInfo->append(tr("����ʱ��: %1").arg(fileInfo.lastRead().toString("yyyy-MM-dd hh:mm:ss")));
		ui.textBrwFileInfo->append(tr("�ļ���С: %1 �ֽ�").arg(fileInfo.size()));
	}

	//�ļ�����
	QString strFileDesc = getFileDescription(strFilePath);
	if (strFileDesc.isEmpty())
		ui.textBrwFileInfo->append(tr("������Ϣ: ��"));
	else
		ui.textBrwFileInfo->append(tr("������Ϣ: %1").arg(strFileDesc));
}

QString PETabWidget::getFileDescription(QString strFilePath)
{
	TCHAR szFilePath[MAX_PATH] = {};
	strFilePath.toWCharArray(szFilePath);

	DWORD dwInfoSize = GetFileVersionInfoSize(szFilePath, NULL);
	if (dwInfoSize > 0) {
		void *pvInfo = calloc(1, dwInfoSize);
		if (GetFileVersionInfo(szFilePath, 0, dwInfoSize, pvInfo)) {
			struct LANGANDCODEPAGE {
				WORD wLanguage;
				WORD wCodePage;
			} *lpTranslate;

			// Read the list of languages and code pages.
			UINT uTranslate = 0;
			if (VerQueryValue(pvInfo, L"\\VarFileInfo\\Translation", (LPVOID*)&lpTranslate, &uTranslate)) {
				if ((uTranslate / sizeof(struct LANGANDCODEPAGE)) > 0) {
					TCHAR *lpBuffer = NULL;
					UINT uSizeBuf = 0;
					TCHAR szSubBlock[64];
					wsprintf(szSubBlock, L"\\StringFileInfo\\%04x%04x\\FileDescription",
						lpTranslate[0].wLanguage, lpTranslate[0].wCodePage);
					if (VerQueryValue(pvInfo, szSubBlock, (LPVOID*)&lpBuffer, &uSizeBuf)) {
						QString strFileDesc = QString::fromWCharArray(lpBuffer);
						free(pvInfo); pvInfo = NULL;
						return strFileDesc;
					}
				}
			}
		}

		if (pvInfo) {
			free(pvInfo);
			pvInfo = NULL;
		}
	}

	return tr("");
}
