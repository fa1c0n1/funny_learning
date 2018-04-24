#pragma once

#include <QWidget>
#include "ui_PETabWidget.h"

class PETabWidget : public QWidget
{
	Q_OBJECT

public:
	PETabWidget(QWidget *parent = Q_NULLPTR);
	~PETabWidget();

protected:
	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent(QDropEvent *event);

private:
	void getFileInfo(QString strFilePath);
	void getPEInfo(QString strFilePath);
	QString getFileDescription(QString strFilePath);
	void getNtHeaderInfo(PIMAGE_NT_HEADERS pNtHeader);
	void FILE_PE_32();
	void FILE_PE_64();
	void getSectionsTableInfo(PIMAGE_NT_HEADERS32, PIMAGE_NT_HEADERS64);
	void getOptionalInfo(PIMAGE_OPTIONAL_HEADER32, PIMAGE_OPTIONAL_HEADER64);
	void getDataDirTableInfo(PIMAGE_OPTIONAL_HEADER32, PIMAGE_OPTIONAL_HEADER64);
	void getExportTableInfo(PIMAGE_DATA_DIRECTORY);
	void getImportTableInfo(PIMAGE_DATA_DIRECTORY);
	void getResourceTableInfo(PIMAGE_DATA_DIRECTORY);
	void getRelocationeTableInfo(PIMAGE_DATA_DIRECTORY);
	void getDelayImportTableInfo(PIMAGE_DATA_DIRECTORY);
	void getTLSTableInfo(PIMAGE_DATA_DIRECTORY);
	DWORD rva2foa(DWORD dwRva);

public slots:
void onPsBtnSectionTableClicked();
void onPsBtnResTableClicked();
void onPsBtnRelocTableClicked();
void onPsBtnOpenFileClicked();
void onPsBtnOffsetCounterClicked();
void onPsBtnImpTableClicked();
void onPsBtnExpTableClicked();
void onPsBtnDelayTableClicked();
void onPsBtnDataDirTableClicked();

private:
	Ui::PETabWidget ui;
	bool m_bPEIs32;
	BYTE *m_pFileImageBase;
	PIMAGE_NT_HEADERS m_pNtHeader;
	QString m_strSectionTable;
	QString m_strTLSTable;
	QString m_strImpTable;
	QString m_strExpTable;
	QString m_strDelayLoadTable;
	QString m_strRelocTable;
	QString m_strResTable;
	QString m_strDataDirTable;
};
