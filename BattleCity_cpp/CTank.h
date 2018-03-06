#pragma once

#include "CBase.h"
#include "CGameMap.h"
#include <vector>

using std::vector;

class CTank : public CBase
{
public:
	CTank();
	~CTank();

	void drawObject();
	void clearObject();

	//根据类型获取坦克的出生地，返回对应的坦克(用于重生)
	//第二个参数是敌军编号
	CTank &getTankBirthPlace(int nType, int nEnemyNo=0);
	//移动坦克
	void moveTank(int nDrt);
	//判断碰撞
	bool isCollision();
	//设置地图对象
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

private:
	bool m_bDead;
	int m_nX;
	int m_nY;
	int m_nDrt;
	int m_nType;
	int m_nBlood;
	int m_nScore;
	CGameMap *m_pMap;
	static int m_nTankShape[4][3][3];
};



