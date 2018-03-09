#pragma once

#include "CBase.h"
#include "CGameMap.h"

/************************************************************************/
/* CTank类：坦克相关的功能
/************************************************************************/
class CTank : public CBase
{
public:
	CTank();
	~CTank();

	//画出坦克
	void drawObject();
	//擦除坦克
	void clearObject();

	//根据类型获取坦克的出生地，返回对应的坦克(用于重生), 第二个参数是敌军编号
	CTank &getTankBirthPlace(int nType, int nEnemyNo=0);
	//移动坦克
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

	static int m_nTankShape[4][3][3];         //坦克的形状
private:
	//处理坦克的碰撞事件，如果发生碰撞则返回true，反之返回false
	bool tankCollision();
	bool tankCollision(int nX, int nY);

	bool m_bDead;                             //坦克是否死亡
	int m_nX;                                 //坦克左上角的X坐标
	int m_nY;                                 //坦克左上角的Y坐标
	int m_nDrt;                               //坦克的方向
	int m_nType;                              //坦克的类型
	int m_nBlood;                             //坦克的血量
	int m_nScore;                             //坦克杀敌获取的分数
	CGameMap *m_pMap;                         //地图对象
};



