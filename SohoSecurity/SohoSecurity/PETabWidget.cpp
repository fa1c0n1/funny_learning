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

	ui.textBrwFileInfo->setText("<br><h1><center>将文件拖拽到窗口</center></h1>");
	ui.textBrwFileInfo->setFontFamily(tr("微软雅黑"));
	ui.textBrwPEHeader->setFontFamily(tr("微软雅黑"));
	ui.textBrwPEData->setFontFamily(tr("微软雅黑"));
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
		QMessageBox::critical(this, tr("错误"), tr("加载文件失败"));
		return;
	}

	DWORD dwFileSize = GetFileSize(hFile, NULL);
	m_pFileImageBase = new BYTE[dwFileSize]{};
	DWORD dwReadSize = 0;
	if (!ReadFile(hFile, m_pFileImageBase, dwFileSize, &dwReadSize, NULL)) {
		ui.psBtnOffsetCounter->setEnabled(false);
		QMessageBox::critical(this, tr("错误"), tr("加载文件失败"));
		return;
	}

	IMAGE_DOS_HEADER *pDosHeader = (IMAGE_DOS_HEADER*)m_pFileImageBase;
	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
		ui.psBtnOffsetCounter->setEnabled(false);
		QMessageBox::critical(this, tr("错误"), tr("不是PE文件"));
		return;
	}

	m_pNtHeader = (IMAGE_NT_HEADERS*)(pDosHeader->e_lfanew + (ULONG64)pDosHeader);
	if (m_pNtHeader->Signature != IMAGE_NT_SIGNATURE) {
		ui.psBtnOffsetCounter->setEnabled(false);
		QMessageBox::critical(this, tr("错误"), tr("不是PE文件"));
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

	ui.textBrwPEHeader->append(tr("===========PE文件头解析=========="));
	IMAGE_FILE_HEADER *pFileHeader = &pNtHeader->FileHeader;

	//运行平台
	if (pFileHeader->Machine == IMAGE_FILE_MACHINE_I386)
		ui.textBrwPEHeader->append(tr("运行平台: Intel 386"));
	else if (pFileHeader->Machine == IMAGE_FILE_MACHINE_IA64)
		ui.textBrwPEHeader->append(tr("运行平台: Intel 64"));
	else if (pFileHeader->Machine == IMAGE_FILE_MACHINE_AMD64)
		ui.textBrwPEHeader->append(tr("运行平台: AMD64"));
	else
		ui.textBrwPEHeader->append(tr("运行平台: 其它"));

	//文件的区段数
	ui.textBrwPEHeader->append(tr("文件的区段数: %1").arg(pFileHeader->NumberOfSections));

	//文件生成时间
	tm tmGTime = { 0 };
	_gmtime64_s(&tmGTime, (__time64_t*)&pFileHeader->TimeDateStamp);
	TCHAR tmpTimeBuf[64] = {};
	_wasctime_s(tmpTimeBuf, &tmGTime);
	ui.textBrwPEHeader->append(tr("文件生成时间: ") + QString::asprintf("%d-%d-%d %d:%d:%d",
		tmGTime.tm_year + 1900, tmGTime.tm_mon + 1, tmGTime.tm_mday,
		tmGTime.tm_hour, tmGTime.tm_min, tmGTime.tm_sec));

	//扩展头大小
	ui.textBrwPEHeader->append(QString::asprintf("扩展头大小: %d 字节", pFileHeader->SizeOfOptionalHeader));

	//32位PE还是64位PE
	if (pNtHeader->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
		m_bPEIs32 = true;
		ui.textBrwPEHeader->append(tr("PE类型: 32位\n"));
		FILE_PE_32();
	}
	else if (pNtHeader->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC){
		m_bPEIs32 = false;
		ui.textBrwPEHeader->append(tr("PE类型: 64位\n"));
		FILE_PE_64();
	}
}

//针对32位PE文件进行解析
void PETabWidget::FILE_PE_32()
{
	IMAGE_DOS_HEADER *pDosHeader = (IMAGE_DOS_HEADER*)m_pFileImageBase;

	//Nt头
	IMAGE_NT_HEADERS32 *pNtHeader32 =
		(IMAGE_NT_HEADERS32*)(pDosHeader->e_lfanew + (ULONG)pDosHeader);
	//扩展头
	IMAGE_OPTIONAL_HEADER32 *pOptionalHeader32 =
		(IMAGE_OPTIONAL_HEADER32*)(&pNtHeader32->OptionalHeader);
	//数据目录表
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

//针对64位PE文件进行解析
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

	if (m_bPEIs32) { //32位PE
		pSectionHeader = IMAGE_FIRST_SECTION(pNtHeader32);
		nCnt = pNtHeader32->FileHeader.NumberOfSections;
	}
	else {  //64位PE
		pSectionHeader = IMAGE_FIRST_SECTION(pNtHeader64);
		nCnt = pNtHeader64->FileHeader.NumberOfSections;
	}

	for (int i = 0; i < nCnt; i++) {
		char szTmpBuf[9] = { 0 };
		memcpy_s(szTmpBuf, _countof(szTmpBuf), pSectionHeader[i].Name, _countof(pSectionHeader[i].Name));
		strTmp = tr("区段名: %1\n").arg(szTmpBuf);
		m_strSectionTable += strTmp;

		strTmp = QString::asprintf("区段数据的文件偏移: %X (h)\n", pSectionHeader[i].PointerToRawData);
		m_strSectionTable += strTmp;

		strTmp = QString::asprintf("区段数据的RVA: %X (h)\n", pSectionHeader[i].VirtualAddress);
		m_strSectionTable += strTmp;

		strTmp = QString::asprintf("区段数据的大小: %X (h)\n\n\n", pSectionHeader[i].SizeOfRawData);
		m_strSectionTable += strTmp;
	}
}

void PETabWidget::getOptionalInfo(PIMAGE_OPTIONAL_HEADER32 pOptionalHeader32, 
	PIMAGE_OPTIONAL_HEADER64 pOptionalHeader64)
{
	ui.textBrwPEHeader->append(tr("===========PE扩展头解析=========="));

	QString strTmp;
	if (m_bPEIs32) {  //32位PE
		strTmp = QString::asprintf("程序执行入口RVA: %X (h)", pOptionalHeader32->AddressOfEntryPoint);
		ui.textBrwPEHeader->append(strTmp);

		strTmp = QString::asprintf("程序基址RVA: %X (h)", pOptionalHeader32->ImageBase);
		ui.textBrwPEHeader->append(strTmp);

		strTmp = QString::asprintf("代码基址RVA: %X (h)", pOptionalHeader32->BaseOfCode);
		ui.textBrwPEHeader->append(strTmp);

		strTmp = QString::asprintf("数据基址RVA: %X (h)", pOptionalHeader32->BaseOfData);
		ui.textBrwPEHeader->append(strTmp);

		strTmp = QString::asprintf("内存中区块的对齐大小: %X (h)", pOptionalHeader32->SectionAlignment);
		ui.textBrwPEHeader->append(strTmp);

		strTmp = QString::asprintf("文件中区块的对齐大小: %X (h)", pOptionalHeader32->FileAlignment);
		ui.textBrwPEHeader->append(strTmp);

		strTmp = QString::asprintf("PE加载到内存后的大小: %X (h)", pOptionalHeader32->SizeOfImage);
		ui.textBrwPEHeader->append(strTmp);

		strTmp = QString::asprintf("所有头+区段表的大小: %X (h)\n", pOptionalHeader32->SizeOfHeaders);
		ui.textBrwPEHeader->append(strTmp);
	}
	else {  //64位PE
		strTmp = QString::asprintf("程序执行入口RVA: %X (h)", pOptionalHeader64->AddressOfEntryPoint);
		ui.textBrwPEHeader->append(strTmp);

		strTmp = QString::asprintf("程序基址RVA: %X (h)", pOptionalHeader64->ImageBase);
		ui.textBrwPEHeader->append(strTmp);

		strTmp = QString::asprintf("代码基址RVA: %X (h)", pOptionalHeader64->BaseOfCode);
		ui.textBrwPEHeader->append(strTmp);

		strTmp = QString::asprintf("内存中区块的对齐大小: %X (h)", pOptionalHeader64->SectionAlignment);
		ui.textBrwPEHeader->append(strTmp);

		strTmp = QString::asprintf("文件中区块的对齐大小: %X (h)", pOptionalHeader64->FileAlignment);
		ui.textBrwPEHeader->append(strTmp);

		strTmp = QString::asprintf("PE加载到内存后的大小: %X (h)", pOptionalHeader64->SizeOfImage);
		ui.textBrwPEHeader->append(strTmp);

		strTmp = QString::asprintf("所有头+区段表的大小: %X (h)\n", pOptionalHeader64->SizeOfHeaders);
		ui.textBrwPEHeader->append(strTmp);
	}
}

void PETabWidget::getDataDirTableInfo(PIMAGE_OPTIONAL_HEADER32 pOptionalHeader32, 
	PIMAGE_OPTIONAL_HEADER64 pOptionalHeader64)
{
	m_strDataDirTable.clear();

	if (m_bPEIs32) { //32位PE
		for (DWORD i = 0; i < pOptionalHeader32->NumberOfRvaAndSizes; i++) {
			m_strDataDirTable += 
				QString::asprintf("表%d的RVA: %X (h)   ---- 表的大小: %X (h)\n\n",
				i, pOptionalHeader32->DataDirectory[i].VirtualAddress, pOptionalHeader32->DataDirectory[i].Size);
		}
	}
	else { //64位PE
		for (DWORD i = 0; i < pOptionalHeader64->NumberOfRvaAndSizes; i++) {
			m_strDataDirTable += 
				QString::asprintf("表%d的RVA: %X (h)   ---- 表的大小: %X (h)\n\n",
				i, pOptionalHeader64->DataDirectory[i].VirtualAddress, pOptionalHeader64->DataDirectory[i].Size);
		}
	}
}

void PETabWidget::getExportTableInfo(PIMAGE_DATA_DIRECTORY pDataDir)
{
	m_strExpTable.clear();

	//获取导出表的RVA
	DWORD dwExpTableRVA = pDataDir[0].VirtualAddress;
	
	if (pDataDir[0].Size == 0) {
		m_strExpTable = tr("没有数据");
		return;
	}

	ULONG64 ul64ExpTableFOA = rva2foa(dwExpTableRVA);
	IMAGE_EXPORT_DIRECTORY *pExpTable = (IMAGE_EXPORT_DIRECTORY*)(ul64ExpTableFOA + (ULONG64)m_pFileImageBase);

	ULONG64 ul64NameOffset = rva2foa(pExpTable->Name);
	char *pDllName = (char*)(ul64NameOffset + (ULONG64)m_pFileImageBase);
	m_strExpTable += pDllName;
	m_strExpTable += tr("\n\n");

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
		m_strExpTable += QString::asprintf("虚序号[%d]  ", i);

		//RVA
		m_strExpTable += QString::asprintf("地址(RVA): %08X (h)    ", pAddrTable[i]);

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
			m_strExpTable += tr("函数名: %1").arg(pFuncName);
		}
		else {
			//判断导出地址表当前索引到的元素是否保存着地址	
			if (pAddrTable[i] != NULL) {
				//i 是导出地址表中的索引号，即一个虚序号
				// 而真正的序号 = 虚序号 + 序号基数
				m_strExpTable += QString::asprintf("序号:[%d]", pExpTable->Base);
			}
		}

		m_strExpTable += tr("\n\n");
	}
}

void PETabWidget::getImportTableInfo(PIMAGE_DATA_DIRECTORY pDataDir)
{
	m_strImpTable.clear();

	//获得导入表的RVA
	DWORD dwImpTableRVA = pDataDir[1].VirtualAddress;
	if (pDataDir[1].Size == 0) {
		m_strImpTable = tr("没有数据");
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
			m_strImpTable += tr(pDllName);
			m_strImpTable += tr("\n\n");

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
					m_strImpTable += QString::asprintf("导入序号:[%d]", pIAT->u1.Ordinal & 0xFFFF);
				}
				else { //函数是以名称导入的
					//字段保存着一个指向IMAGE_IMPORT_BY_NAME结构体的RVA
					DWORD dwFuncNameFOA = rva2foa(pIAT->u1.AddressOfData);
					PIMAGE_IMPORT_BY_NAME pImpName = 
						(PIMAGE_IMPORT_BY_NAME)((ULONG64)m_pFileImageBase + dwFuncNameFOA);

					m_strImpTable += tr("函数名称:[%1]").arg(pImpName->Name);
				}

				pIAT++;
				m_strImpTable += tr("\n");
			}

			pImpTable++;
			m_strImpTable += tr("\n\n");
		}
	}
	else {  //------------64位PE
		//判断是否遍历到了最后一个结构体
		while (pImpTable->Name != 0) {
			//解析出导入的 Dll 的模块名
			DWORD dwNameFOA = rva2foa(pImpTable->Name);
			char *pDllName = (char*)(dwNameFOA + (ULONG64)m_pFileImageBase);
			m_strImpTable += tr(pDllName);
			m_strImpTable += tr("\n\n");

			//解析当前dll的导入函数名称
			//pImpTable->OriginalFirstThunk;  //导入名称表
			//pImpTable->FirstThunk;          //导入地址表
			//上面说的两个表，在文件中保存的内容是完全相同的

			DWORD dwIATfoa = rva2foa(pImpTable->OriginalFirstThunk);
			IMAGE_THUNK_DATA64 *pIAT = (IMAGE_THUNK_DATA64*)((ULONG64)m_pFileImageBase + dwIATfoa);

			//遍历IAT
			//  IAT 是一个 IMAGE_THUNK_DATA 的结构体数组
			while (pIAT->u1.AddressOfData != 0) {
				//判断最高位是否是1
				if (IMAGE_SNAP_BY_ORDINAL64(pIAT->u1.Ordinal)) { //函数是以序号导入的
					// 序号都是WORD类型，所以只取它的低16位
					m_strImpTable += QString::asprintf("导入序号:[%d]", pIAT->u1.Ordinal & 0xFFFF);
				}
				else { //函数是以名称导入的
					//字段保存着一个指向IMAGE_IMPORT_BY_NAME结构体的RVA
					DWORD dwFuncNameFOA = rva2foa(pIAT->u1.AddressOfData);
					PIMAGE_IMPORT_BY_NAME pImpName =
						(PIMAGE_IMPORT_BY_NAME)((ULONG64)m_pFileImageBase + dwFuncNameFOA);

					m_strImpTable += tr("函数名称:[%1]").arg(pImpName->Name);
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
		m_strResTable += tr("没有数据");
		return;
	}

	//获取资源表的FOA
	DWORD dwResTableFOA = rva2foa(pDataDir[2].VirtualAddress);

	//指向第一层目录
	IMAGE_RESOURCE_DIRECTORY *pRoot = 
		(IMAGE_RESOURCE_DIRECTORY*)((ULONG64)m_pFileImageBase + dwResTableFOA);

	IMAGE_RESOURCE_DIRECTORY *pDir2 = NULL;  //指向第二层目录
	IMAGE_RESOURCE_DIRECTORY *pDir3 = NULL;  //指向第三层目录

	IMAGE_RESOURCE_DIRECTORY_ENTRY *pEntry1 = NULL;
	IMAGE_RESOURCE_DIRECTORY_ENTRY *pEntry2 = NULL;
	IMAGE_RESOURCE_DIRECTORY_ENTRY *pEntry3 = NULL;

	IMAGE_RESOURCE_DATA_ENTRY *pDataEntry = NULL;
	IMAGE_RESOURCE_DIR_STRING_U *pIdStr = NULL;

	pEntry1 = (IMAGE_RESOURCE_DIRECTORY_ENTRY *)(pRoot + 1);
	for (int i = 0; i < pRoot->NumberOfIdEntries + pRoot->NumberOfNamedEntries; i++) {
		//获取第一层目录入口的ID(ID就是资源的类型)
		if (pEntry1->NameIsString == 1) {
			//NameOffset这个字段的值是一个偏移
			//  这个偏移是以资源表根目录的地址作为基址
			pIdStr = (IMAGE_RESOURCE_DIR_STRING_U *)(pEntry1->NameOffset + (ULONG64)pRoot);
			TCHAR *pNameBuf = new TCHAR[pIdStr->Length + 1]{};
			StringCchCopy(pNameBuf, pIdStr->Length + 1, pIdStr->NameString);
			m_strResTable += tr("资源类型: %1\n\n").arg(QString::fromWCharArray(pNameBuf));
			delete[] pNameBuf; pNameBuf = nullptr;
		}
		else {
			QString strTypeArr[] = {
				tr(""),          //0
				tr("鼠标指针"),   //1
				tr("位图"),       //2
				tr("图标"),	      //3
				tr("菜单"),       //4
				tr("对话框"),     //5
				tr("字符串列表"),  //6
				tr("字体目录"),    //7
				tr("字体"),       //8
				tr("快捷键"),      //9
				tr("非格式化资源"), //A
				tr("消息列表"),     //B
				tr("鼠标指针数组"),  //C
				tr(""),            //D
				tr("图标组"),       //E
				tr(""),            //F
				tr("版本信息"),     //10
			};

			if (pEntry1->Id > 16) {
				m_strResTable += QString::asprintf("资源类型: %d\n\n", pEntry1->Id);
			}
			else {
				m_strResTable += tr("资源类型: %1").arg(strTypeArr[pEntry1->Id]);
			}
		}

		if (pEntry1->DataIsDirectory == 1) {
			//得到第二层目录的地址
			pDir2 =
				(IMAGE_RESOURCE_DIRECTORY*)(pEntry1->OffsetToDirectory + (ULONG64)pRoot);

			//遍历第二层资源目录的所有目录入口	
			pEntry2 = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pDir2 + 1);
			for (int j = 0; j < pDir2->NumberOfIdEntries + pDir2->NumberOfNamedEntries; j++) {
				//得到资源的ID
				if (pEntry2->NameIsString == 1) {
					pIdStr = (IMAGE_RESOURCE_DIR_STRING_U *)(pEntry2->NameOffset + (ULONG64)pRoot);
					TCHAR *pNameBuf = new TCHAR[pIdStr->Length + 1]{};
					StringCchCopy(pNameBuf, pIdStr->Length + 1, pIdStr->NameString);
					m_strResTable += tr(" +资源ID: %1\n").arg(QString::fromWCharArray(pNameBuf));
					delete[] pNameBuf; pNameBuf = nullptr;
				}
				else {
					m_strResTable += QString::asprintf(" +资源ID: %d\n", (DWORD)pEntry2->Id);
				}

				if (pEntry2->DataIsDirectory == 1) {
					//得到第三层目录的地址
					pDir3 =
						(IMAGE_RESOURCE_DIRECTORY*)(pEntry2->OffsetToDirectory + (ULONG64)pRoot);

					pEntry3 =
						(IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pDir3 + 1);

					m_strResTable += QString::asprintf("   +id: %d\n", pEntry3->Id);

					//得到数据入口
					pDataEntry =
						(IMAGE_RESOURCE_DATA_ENTRY*)(pEntry3->OffsetToData + (ULONG64)pRoot);

					m_strResTable += QString::asprintf("   +资源偏移(RVA): %X (h)\n", pDataEntry->OffsetToData);
					m_strResTable += QString::asprintf("   +资源大小: %X (h)\n\n", pDataEntry->Size);
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
		m_strRelocTable = tr("没有数据");
		return;
	}

	//得到第一个重定位块的数组的FOA
	DWORD dwRelocBlockFOA = rva2foa(pDataDir[5].VirtualAddress);
	
	//得到第一个重定位块的地址
	IMAGE_BASE_RELOCATION *pRelocBlock =
		(IMAGE_BASE_RELOCATION*)((ULONG64)m_pFileImageBase + dwRelocBlockFOA);

	while (true) {
		//判断是否最后一个重定位块
		if (pRelocBlock->SizeOfBlock == 0 && pRelocBlock->VirtualAddress == 0)
			break;

		m_strRelocTable +=
			QString::asprintf("块开始的RVA: %X (h), 字节数: %X (h)\n\n",
			pRelocBlock->VirtualAddress, pRelocBlock->SizeOfBlock);

		//遍历重定位类型和偏移的数据块
		TypeOffset *pTypeOffset =
			(TypeOffset*)((ULONG64)pRelocBlock + sizeof(IMAGE_BASE_RELOCATION));

		DWORD dwCount =
			(pRelocBlock->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(TypeOffset);

		for (DWORD i = 0; i < dwCount; i++) {
			m_strRelocTable += QString::asprintf("偏移: %X (h),  ", pTypeOffset->Offset);

			//得到需要重定位的数据
			DWORD dwRVA = pRelocBlock->VirtualAddress + pTypeOffset[i].Offset;
			DWORD dwFOA = rva2foa(dwRVA);
			DWORD *pRelocData = (DWORD*)(dwFOA + (ULONG64)m_pFileImageBase);

			m_strRelocTable += QString::asprintf("+要修改的地址: %08X (h)\n", *pRelocData);
		}
		m_strRelocTable += tr("\n\n");

		//下一个重定位块
		pRelocBlock =
			(IMAGE_BASE_RELOCATION*)((ULONG64)pRelocBlock + pRelocBlock->SizeOfBlock);
	}
}

//延迟加载导入表
void PETabWidget::getDelayImportTableInfo(PIMAGE_DATA_DIRECTORY pDataDir)
{
	m_strDelayLoadTable.clear();

	if (pDataDir[13].Size == 0) {
		m_strDelayLoadTable = tr("没有数据");
		return;
	}

	DWORD dwDelayTableFOA = rva2foa(pDataDir[13].VirtualAddress);
	IMAGE_DELAYLOAD_DESCRIPTOR *pDelayTable =
		(IMAGE_DELAYLOAD_DESCRIPTOR*)(dwDelayTableFOA + (ULONG64)m_pFileImageBase);

	if (pDelayTable->DllNameRVA == 0x00000300) {
		m_strDelayLoadTable = tr("没有数据");
		return;
	}

	while (pDelayTable->DllNameRVA) {
		//DllName RVA
		m_strDelayLoadTable += QString::asprintf("DllName RVA: %X (h),  ", pDelayTable->DllNameRVA);

		//DllName
		DWORD dwDllNameFOA = rva2foa(pDelayTable->DllNameRVA);
		char *pDllName = (char*)(dwDllNameFOA + (ULONG64)m_pFileImageBase);
		m_strDelayLoadTable += tr("DllName: %1\n").arg(pDllName);

		//导入地址表RVA
		m_strDelayLoadTable += QString::asprintf("IAT(R)RVA: %X (h),  ", pDelayTable->ImportAddressTableRVA);

		//导入名称表RVA
		m_strDelayLoadTable += QString::asprintf("INT(R)RVA: %X (h),  \n", pDelayTable->ImportNameTableRVA);

		m_strDelayLoadTable += tr("\n");

		DWORD dwFuncNameFOA = rva2foa(pDelayTable->ImportNameTableRVA);

		if (m_bPEIs32) {  //--------32位PE
			IMAGE_THUNK_DATA32 *pIAT = (IMAGE_THUNK_DATA32*)((ULONG64)m_pFileImageBase + dwFuncNameFOA);

			//遍历IAT: 是一个IMAGE_THUNK_DATA结构体数组
			while (pIAT->u1.AddressOfData != 0) {
				//判断最高位是否为1
				if (IMAGE_SNAP_BY_ORDINAL32(pIAT->u1.Ordinal)) {
					//函数是以序号导入的
					// 序号都是WORD类型，故只取它的低16位
					m_strDelayLoadTable += QString::asprintf("导入序号: %d\n", pIAT->u1.Ordinal & 0xFFFF);
				}
				else {
					//函数是以名称导入的
					// 字段保存着一个指向 IMAGE_IMPORT_BY_NAME 结构的RVA
					DWORD dwFuncNameFOA = rva2foa(pIAT->u1.AddressOfData);
					IMAGE_IMPORT_BY_NAME *pImportName =
						(IMAGE_IMPORT_BY_NAME*)((ULONG64)m_pFileImageBase + dwFuncNameFOA);

					m_strDelayLoadTable += tr("函数名称: %1").arg(pImportName->Name);
				}

				//下一个结构体
				pIAT++;
				m_strDelayLoadTable += tr("\n");
			}
		}
		else {  //----------64位PE
			IMAGE_THUNK_DATA64 *pIAT = (IMAGE_THUNK_DATA64*)((ULONG64)m_pFileImageBase + dwFuncNameFOA);

			//遍历IAT: 是一个IMAGE_THUNK_DATA结构体数组
			while (pIAT->u1.AddressOfData != 0) {
				//判断最高位是否为1
				if (IMAGE_SNAP_BY_ORDINAL64(pIAT->u1.Ordinal)) {
					//函数是以序号导入的
					// 序号都是WORD类型，故只取它的低16位
					m_strDelayLoadTable += QString::asprintf("导入序号: %d\n", pIAT->u1.Ordinal & 0xFFFF);
				}
				else {
					//函数是以名称导入的
					// 字段保存着一个指向 IMAGE_IMPORT_BY_NAME 结构的RVA
					DWORD dwFuncNameFOA = rva2foa(pIAT->u1.AddressOfData);
					IMAGE_IMPORT_BY_NAME *pImportName =
						(IMAGE_IMPORT_BY_NAME*)((ULONG64)m_pFileImageBase + dwFuncNameFOA);

					m_strDelayLoadTable += tr("函数名称: %1").arg(pImportName->Name);
				}

				//下一个结构体
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
		m_strTLSTable = "没有数据";
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

void PETabWidget::onPsBtnSectionTableClicked()
{
	ui.groupBoxPEData->setTitle(tr("区段表"));
	ui.textBrwPEData->setText(m_strSectionTable);
}

void PETabWidget::onPsBtnResTableClicked()
{
	ui.groupBoxPEData->setTitle(tr("资源表"));
	ui.textBrwPEData->setText(m_strResTable);
}

void PETabWidget::onPsBtnRelocTableClicked()
{
	ui.groupBoxPEData->setTitle(tr("重定位表"));
	ui.textBrwPEData->setText(m_strRelocTable);
}

void PETabWidget::onPsBtnOffsetCounterClicked()
{
	OffsetCounterDlg offsetCounterDlg(m_pNtHeader, this);
	offsetCounterDlg.exec();
}

void PETabWidget::onPsBtnImpTableClicked()
{
	ui.groupBoxPEData->setTitle(tr("导入表"));
	ui.textBrwPEData->setText(m_strImpTable);
}

void PETabWidget::onPsBtnExpTableClicked()
{
	ui.groupBoxPEData->setTitle(tr("导出表"));
	ui.textBrwPEData->setText(m_strExpTable);
}

void PETabWidget::onPsBtnDelayTableClicked()
{
	ui.groupBoxPEData->setTitle(tr("延时加载表"));
	ui.textBrwPEData->setText(m_strDelayLoadTable);
}

void PETabWidget::onPsBtnDataDirTableClicked()
{
	ui.groupBoxPEData->setTitle(tr("数据目录表"));
	ui.textBrwPEData->setText(m_strDataDirTable);
}

void PETabWidget::onPsBtnTLSTableClicked()
{
	ui.groupBoxPEData->setTitle(tr("TLS表"));
	ui.textBrwPEData->setText(m_strTLSTable);
}

void PETabWidget::getFileInfo(QString strFilePath)
{
	//文件路径
	ui.textBrwFileInfo->append(tr("文件路径为: %1").arg(strFilePath));

	//文件MD5
	QString strFileMD5 = md5FileValue((char *)strFilePath.toStdString().c_str());
	ui.textBrwFileInfo->append(tr("文件MD5: %1").arg(strFileMD5));

	//文件信息
	QFileInfo fileInfo(strFilePath);
	if (fileInfo.exists()) {
		ui.textBrwFileInfo->append(tr("创建时间: %1").arg(fileInfo.created().toString("yyyy-MM-dd hh:mm:ss")));
		ui.textBrwFileInfo->append(tr("修改时间: %1").arg(fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss")));
		ui.textBrwFileInfo->append(tr("访问时间: %1").arg(fileInfo.lastRead().toString("yyyy-MM-dd hh:mm:ss")));
		ui.textBrwFileInfo->append(tr("文件大小: %1 字节").arg(fileInfo.size()));
	}

	//文件描述
	QString strFileDesc = getFileDescription(strFilePath);
	if (strFileDesc.isEmpty())
		ui.textBrwFileInfo->append(tr("描述信息: 无"));
	else
		ui.textBrwFileInfo->append(tr("描述信息: %1").arg(strFileDesc));
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
