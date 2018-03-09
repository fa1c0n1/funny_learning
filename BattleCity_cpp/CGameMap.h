#pragma once

#include <windows.h>

/************************************************************************/
/* CGameMap�ࣺ��ͼ��صĹ���
/************************************************************************/
class CGameMap
{
public:
	CGameMap();
	~CGameMap();

	void setLevel(int nLevel);
	//�����ͼ
	void loadMap();
	//���õ�ͼ�ϵ����������
	void setMapValue(int nX, int nY, int nVal);
	//��ȡ��ͼ�ϵ����������
	int getMapValue(int nX, int nY);
	//��ȡ�ݵغ�����ͼ�ϵ����������
	int getMapGrassRiverValue(int nX, int nY);
	//������ͼ
	void drawMap();
	//�༭��ͼ
	void editMap();

private:
	//����༭��ͼ���ܵ�����¼�
	bool editMapMouseEventProc(MOUSE_EVENT_RECORD mer);
	//��ʾ��ͼ�༭�������
	void showMapEditorPanel();
	//���µ�ͼ�༭��������ϵ���Ϣ
	void updateEditorInfo();
	//�����ڱ༭��ͼʱ����յ�ͼ�ϵĿɱ༭����
	void clearEditableArea();
	//�����ڱ༭��ͼ�ǣ�����༭�õĵ�ͼ���ļ�
	void saveMapToFile();
	//��ʾ�����ͼ�ɹ�����ʾ��
	void showSaveMapNotice();
	//�ж��ļ��Ƿ����
	bool isFileExist(const char *pszFileName);
	//��ȡ�ļ�
	void loadFile(const char *pszFileName);

	bool m_bUseDefault;             //�Ƿ�ʹ�õ�ǰ�ؿ���Ĭ�ϵ�ͼ
	int m_nLevel;                   //�ؿ�
	int m_nEditNodeType;            //�༭��ͼʱ,�������Ļ�����༭���ϰ��������
	int m_nArrGrass[40][40];        //������һ������ͼһ���Ķ�ά���鱣���ͼ�ϵĲݵغͺ������
	int m_nArrMap[40][40];          //�洢��ͼ�Ķ�ά����
};

