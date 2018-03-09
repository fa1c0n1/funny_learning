#pragma once

#include "CBase.h"
#include "CGameMap.h"

/************************************************************************/
/* CTank�ࣺ̹����صĹ���
/************************************************************************/
class CTank : public CBase
{
public:
	CTank();
	~CTank();

	//����̹��
	void drawObject();
	//����̹��
	void clearObject();

	//�������ͻ�ȡ̹�˵ĳ����أ����ض�Ӧ��̹��(��������), �ڶ��������ǵо����
	CTank &getTankBirthPlace(int nType, int nEnemyNo=0);
	//�ƶ�̹��
	void moveTank(int nDrt);
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
	CGameMap *getMapObj();

	static int m_nTankShape[4][3][3];         //̹�˵���״
private:
	//����̹�˵���ײ�¼������������ײ�򷵻�true����֮����false
	bool tankCollision();
	bool tankCollision(int nX, int nY);

	bool m_bDead;                             //̹���Ƿ�����
	int m_nX;                                 //̹�����Ͻǵ�X����
	int m_nY;                                 //̹�����Ͻǵ�Y����
	int m_nDrt;                               //̹�˵ķ���
	int m_nType;                              //̹�˵�����
	int m_nBlood;                             //̹�˵�Ѫ��
	int m_nScore;                             //̹��ɱ�л�ȡ�ķ���
	CGameMap *m_pMap;                         //��ͼ����
};



