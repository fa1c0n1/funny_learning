#pragma once

#include "CBase.h"
#include "CGameMap.h"
#include "CPillbox.h"
#include "CTank.h"
#include <vector>

using std::vector;

/************************************************************************/
/* CBullet类：子弹相关的功能
/************************************************************************/
class CBullet : public CBase
{
public:
	CBullet();
	~CBullet();

	//画出子弹
	void drawObject();
	//擦除子弹
	void clearObject();

	//将子弹的状态复位
	void resetBullet();
	//移动子弹
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

	//发射子弹
	static void fireBullet(vector<CBullet*> &vtBulletbox, CTank &tank);

private:
	//处理子弹所有的碰撞，如果发生碰撞返回true, 反之返回false
	bool bulletCollision(vector<CBullet*> &vtBulletBox, vector<CTank> &vtTanks, CPillbox &pillbox, int *pEAliveNum);
	//处理子弹与除子弹外其他事物的碰撞, 如果发生碰撞返回true, 反之返回false
	bool bulletCollisionWithOthers(int nX, int nY, vector<CTank> &vtTanks, CPillbox &pillbox, int *pEAliveNum);
	//当玩家被敌方坦克的子弹击中时的处理
	void doWhenPlayerBeated(int nX, int nY, vector<CTank> &vtTanks);
	//当敌方坦克的子弹被玩家击中时的处理
	void doWhenEnemyBeated(int nX, int nY, vector<CTank> &vtTanks, int *pEAliveNum);
	//当子弹击中障碍物时的处理
	bool doWhenBarierBeated(int nX, int nY, CPillbox &pillbox);

	bool m_bValid;    //子弹是否可用
	int m_nID;        //子弹编号
	int m_nX;         //子弹的X坐标
	int m_nY;         //子弹的Y坐标
	int m_nOwner;     //子弹的属主
	int m_nType;      //子弹的类型(即所属阵营)
	int m_nDrt;       //子弹的方向
	int m_nAtk;       //子弹的攻击力
	CGameMap *m_pMap; //地图对象
};

