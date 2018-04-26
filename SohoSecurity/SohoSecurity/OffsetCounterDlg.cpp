#include "stdafx.h"
#include "OffsetCounterDlg.h"

OffsetCounterDlg::OffsetCounterDlg(QWidget *parent)
	: QDialog(parent), m_pNtHeader(nullptr), m_bRva2Foa(true)
{
	ui.setupUi(this);
}

OffsetCounterDlg::OffsetCounterDlg(PIMAGE_NT_HEADERS pNtHeader, QWidget *parent)
	: QDialog(parent), m_bRva2Foa(true)
{
	ui.setupUi(this);

	this->setFixedSize(this->width(), this->height());

	m_pNtHeader = pNtHeader;
	ui.leFOA->clear();
	ui.leRVA->clear();
	ui.leSectName->clear();
	ui.leRVA->setReadOnly(false);
	ui.leFOA->setReadOnly(true);
	ui.leRVA->setFocus();

	QRegExp regExp("[A-Fa-f0-9]{8}");
	ui.leRVA->setValidator(new QRegExpValidator(regExp));
	ui.leFOA->setValidator(new QRegExpValidator(regExp));

	connect(ui.leRVA, &QLineEdit::textChanged, this, &OffsetCounterDlg::onText2Upper);
	connect(ui.leFOA, &QLineEdit::textChanged, this, &OffsetCounterDlg::onText2Upper);
}

OffsetCounterDlg::~OffsetCounterDlg()
{
}

void OffsetCounterDlg::onPsBtnRVAClicked()
{
	m_bRva2Foa = true;
	ui.leRVA->setReadOnly(false);
	ui.leRVA->setFocus();
	ui.leFOA->setReadOnly(true);
	ui.leRVA->clear();
	ui.leFOA->clear();
}

void OffsetCounterDlg::onPsBtnFOAClicked()
{
	m_bRva2Foa = false;
	ui.leRVA->setReadOnly(true);
	ui.leFOA->setReadOnly(false);
	ui.leFOA->setFocus();
	ui.leRVA->clear();
	ui.leFOA->clear();
}

void OffsetCounterDlg::onPsBtnExecCountClicked()
{
	disconnect(ui.leRVA, &QLineEdit::textChanged, this, &OffsetCounterDlg::onText2Upper);
	disconnect(ui.leFOA, &QLineEdit::textChanged, this, &OffsetCounterDlg::onText2Upper);

	if (m_bRva2Foa) {
		QString strRVA = ui.leRVA->text();
		DWORD dwRVA = strRVA.toULong(nullptr, 16);
		DWORD dwFOA = rva2foa(dwRVA);
		if (dwFOA != -1) {
			ui.leFOA->setText(QString::asprintf("%08X", dwFOA));
		}
		else {
			ui.leFOA->setText(tr("超出范围"));
		}
	}
	else {
		QString strFOA = ui.leFOA->text();
		DWORD dwFOA = strFOA.toULong(nullptr, 16);
		DWORD dwRVA = foa2rva(dwFOA);
		if (dwRVA != -1) {
			ui.leRVA->setText(QString::asprintf("%08X", dwRVA));
		}
		else {
			ui.leRVA->setText(tr("超出范围"));
		}
	}

	ui.leSectName->setText(m_strSectName);

	connect(ui.leRVA, &QLineEdit::textChanged, this, &OffsetCounterDlg::onText2Upper);
	connect(ui.leFOA, &QLineEdit::textChanged, this, &OffsetCounterDlg::onText2Upper);
}

void OffsetCounterDlg::onText2Upper(const QString &strText)
{
	if (m_bRva2Foa)
		ui.leRVA->setText(strText.toUpper());
	else
		ui.leFOA->setText(strText.toUpper());
}

DWORD OffsetCounterDlg::rva2foa(DWORD dwRva)
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
			char szNameBuf[9] = { 0 };
			memcpy_s(szNameBuf, 9, pSectionHeader[i].Name, 8);
			m_strSectName = tr(szNameBuf);
			DWORD dwFoa = dwRva - pSectionHeader[i].VirtualAddress + pSectionHeader[i].PointerToRawData;
			return dwFoa;
		}
	}
	
	m_strSectName = tr("超出范围");
	return -1;
}

DWORD OffsetCounterDlg::foa2rva(DWORD dwFoa)
{
	//获取区段个数
	DWORD dwSectionNum = m_pNtHeader->FileHeader.NumberOfSections;
	//得到第一个区段
	IMAGE_SECTION_HEADER *pSectionHeader = IMAGE_FIRST_SECTION(m_pNtHeader);

	for (DWORD i = 0; i < dwSectionNum; i++) {
		DWORD dwSectionEndFoa =
			pSectionHeader[i].PointerToRawData + pSectionHeader[i].SizeOfRawData;

		if (dwFoa >= pSectionHeader[i].PointerToRawData && dwFoa < dwSectionEndFoa) {
			//在当前范围内，则返回RVA
			DWORD dwRva = dwFoa + pSectionHeader[i].VirtualAddress - pSectionHeader[i].PointerToRawData;
			char szNameBuf[9] = { 0 };
			memcpy_s(szNameBuf, 9, pSectionHeader[i].Name, 8);
			m_strSectName = tr(szNameBuf);
			return dwRva;
		}
	}

	m_strSectName = tr("超出范围");
	return -1;
}
