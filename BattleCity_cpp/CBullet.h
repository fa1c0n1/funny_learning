#pragma once

#include "CBase.h"
#include "CGameMap.h"
#include "CPillbox.h"
#include "CTank.h"
#include <vector>

using std::vector;

class CBullet : public CBase
{
public:
	CBullet();
	~CBullet();

	void drawObject();
	void clearObject();

	void fireBullet(vector<CBullet*> &vtBulletbox, CTank &tank);
	void resetBullet();
	void moveBullet(vector<CBullet*> &vtBulletBox, vector<CTank> &vtTanks, CPillbox &pillbox, int *pEnemyNum);
	void setPosX(int nX);
	void setPosY(int nY);
	void setValid(bool bValid);
	void setID(int nID);
	void setDirection(int nDrt);
	void setType(int nType);
	void setOwner(int nOwner);
	void setAtk(int nAtk);
	bool isValid();
	void setMapObj(CGameMap *pGMap);
	int getPosX();
	int getPosY();
	int getType();
	int getOwner();
	int getAtk();
	CGameMap *getGameMap();

private:
	bool bulletCollision(vector<CBullet*> &vtBulletBox, vector<CTank> &vtTanks, CPillbox &pillbox, int *pEAliveNum);
	bool bulletCollisionWithOthers(int nX, int nY, vector<CTank> &vtTanks, CPillbox &pillbox, int *pEAliveNum);
	void doWhenPlayerBeated(int nX, int nY, vector<CTank> &vtTanks);
	void doWhenEnemyBeated(int nX, int nY, vector<CTank> &vtTanks, int *pEAliveNum);
	bool doWhenBarierBeated(int nX, int nY, CPillbox &pillbox);

	bool m_bValid;    //�ӵ��Ƿ����
	int m_nID;        //�ӵ����
	int m_nX;         //�ӵ���X����
	int m_nY;         //�ӵ���Y����
	int m_nOwner;     //�ӵ�������
	int m_nType;      //�ӵ�������(��������Ӫ)
	int m_nDrt;       //�ӵ��ķ���
	int m_nAtk;       //�ӵ��Ĺ�����
	CGameMap *m_pMap; //��ͼ����
};

