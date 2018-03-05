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
	void moveBullet(vector<CBullet*> &vtBulletBox, vector<CTank> &vtTanks, CPillbox &pillbox);
	void setPosX(int nX);
	void setPosY(int nY);
	void setValid(bool bValid);
	void setID(int nID);
	void setDirection(int nDrt);
	void setType(int nType);
	bool isValid();
	void setMapObj(CGameMap *pGMap);
	int getPosX();
	int getPosY();
	int getType();
	CGameMap *getGameMap();

private:
	bool bulletCollision(vector<CBullet*> &vtBulletBox, vector<CTank> &vtTanks, CPillbox &pillbox);
	bool bulletCollisionWithOthers(int nX, int nY, vector<CTank> &vtTanks, CPillbox &pillbox);
	void doWhenPlayerBeated(int nX, int nY, vector<CTank> &vtTanks);
	void doWhenEnemyBeated(int nX, int nY, vector<CTank> &vtTanks);
	bool doWhenBarierBeated(int nX, int nY, CPillbox &pillbox);

	bool m_bValid;    //子弹是否可用
	int m_nID;        //子弹编号
	int m_nX;         //子弹的X坐标
	int m_nY;         //子弹的Y坐标
	int m_nType;      //子弹的类型(即所属阵营)
	int m_nDrt;       //子弹的方向
	int m_nAtk;       //子弹的攻击力
	CGameMap *m_pMap; //地图对象
};

