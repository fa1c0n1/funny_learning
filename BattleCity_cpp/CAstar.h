#pragma once

#include <windows.h>
#include <vector>
#include "CTank.h"
#include "CGameMap.h"

using std::vector;

/************************************************************************/
/* CAstar�ࣺA��Ѱ·�㷨
/************************************************************************/
class CAstar
{
public:
	CAstar();
	~CAstar();

	typedef struct _MY_COORD : public COORD
	{
		bool operator==(COORD cod)
		{
			return X == cod.X && Y == cod.Y;
		}

		void operator=(COORD cod)
		{
			X = cod.X;
			Y = cod.Y;
		}

		int nDrt;           
	} MY_COORD;

	//���Ľṹ
	typedef struct {
		int nG;		         //�ƶ����
		int nH;              //���յ����̾���
		int nF;              //nF = nG + nH
		MY_COORD codSelf;    //��������
		MY_COORD codParent;  //���������(��˭��ɢ��)

		void setH_F(COORD cod1, COORD cod2)
		{
			nH = abs(cod1.X - cod2.X) + abs(cod1.Y - cod2.Y);
			nF = nG + nH;
		}
	} NODE_INFO;

	//=====�Ż�����====
	typedef struct _VIR_INFO
	{
		char isOpen : 1; //���Ƿ���Open����
		char isClose : 1; //��ʾ�Ƿ���CLose����
		char receive : 6; //����
	}VIR_INFO, *PVIR_INFO;


	void initMapInfo(int nMapWidth, int nMapHeight);    //��ʼ����ͼ�����Ϣ
	bool findPath(CTank &tank, CTank &target);          //���Ұ������·���ĵ�(�õ�Close��)
	vector<MY_COORD> getPath();               //��ȡ���·��

private:
	bool checkBarrier(int nX, int nY);

	int m_nMapWidth;              //��ͼ�Ŀ��
	int m_nMapHeight;             //��ͼ�ĸ߶�
	CGameMap *m_pMap;             //��ͼ����
	PVIR_INFO m_pVir;             //ָ�������ͼ���׵�ַ
	COORD m_codStart;             //�������
	COORD m_codEnd;               //�յ�����
	vector<NODE_INFO> m_vtOpen;   //����ɢ�ĵ�
	vector<NODE_INFO> m_vtClose;  //��ɢ���ĵ�
	vector<MY_COORD> m_vtPath;    //���·��
};

