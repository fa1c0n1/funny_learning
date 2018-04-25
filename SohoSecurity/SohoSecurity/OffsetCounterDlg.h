#pragma once

#include <QDialog>
#include "ui_OffsetCounterDlg.h"

class OffsetCounterDlg : public QDialog
{
	Q_OBJECT

public:
	OffsetCounterDlg(QWidget *parent = Q_NULLPTR);
	OffsetCounterDlg(PIMAGE_NT_HEADERS pNtHeader, QWidget *parent = Q_NULLPTR);
	~OffsetCounterDlg();

public slots:
void onPsBtnRVAClicked();
void onPsBtnFOAClicked();
void onPsBtnExecCountClicked();
void onText2Upper(const QString &strText);

private:
	DWORD rva2foa(DWORD dwRva);
	DWORD foa2rva(DWORD dwFoa);

private:
	Ui::OffsetCounterDlg ui;
	bool m_bRva2Foa;
	IMAGE_NT_HEADERS *m_pNtHeader;
	QString m_strSectName;
};
