#pragma once

#include "CBase.h"
#include "CGameMap.h"

class CTank : public CBase
{
public:
	CTank();
	~CTank();

	void drawObject();
	void clearObject();

	//�������ͻ�ȡ̹�˵ĳ����أ����ض�Ӧ��̹��(��������)
	//�ڶ��������ǵо����
	CTank &getTankBirthPlace(int nType, int nEnemyNo=0);
	//�ƶ�̹��
	void moveTank(int nDrt);
	//���õ�ͼ����
	void setMapObj(CGameMap *pGMap);
	void setDead(bool bDead);
	bool isDead();
	void setBlood(int nBlood);
	void setScore(int nScore);
	int getDirection();
	int getPosX();
	int getPosY();
	int getType();
	int getBlood();
	int getScore();

	static int m_nTankShape[4][3][3];
private:
	//�ж�̹����ײ
	bool tankCollision();
	bool tankCollision(int nX, int nY);

	bool m_bDead;
	int m_nX;
	int m_nY;
	int m_nDrt;
	int m_nType;
	int m_nBlood;
	int m_nScore;
	CGameMap *m_pMap;
};



