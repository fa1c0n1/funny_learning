#pragma once

#include "CBase.h"
#include "CGameMap.h"

/************************************************************************/
/* CPillbox�ࣺ�ﱤ��صĹ���
/************************************************************************/
class CPillbox : public CBase
{
public:
	CPillbox();
	~CPillbox();

	//�����ﱤ
	void drawObject();
	//�����ﱤ
	void clearObject();

	void setMapObj(CGameMap *pGMap);
	void setIsDead(bool bDead);
	bool isDead();

private:
	bool m_bDead;                     //�ﱤ�Ƿ񱻻���
	int m_nX;                         //�ﱤ��X����
	int m_nY;                         //�ﱤ��Y����
	int m_nType;                      //��ʾ�ﱤ�ĵ������
	CGameMap *m_pMap;                 //��ͼ����
	static int m_nPillboxShape[3][3]; //�ﱤ����״
};

