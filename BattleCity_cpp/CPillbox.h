#pragma once

#include "CBase.h"
#include "CGameMap.h"

/************************************************************************/
/* CPillbox类：碉堡相关的功能
/************************************************************************/
class CPillbox : public CBase
{
public:
	CPillbox();
	~CPillbox();

	//画出碉堡
	void drawObject();
	//擦除碉堡
	void clearObject();

	void setMapObj(CGameMap *pGMap);
	void setIsDead(bool bDead);
	bool isDead();

private:
	bool m_bDead;                     //碉堡是否被击毁
	int m_nX;                         //碉堡的X坐标
	int m_nY;                         //碉堡的Y坐标
	int m_nType;                      //表示碉堡的点的类型
	CGameMap *m_pMap;                 //地图对象
	static int m_nPillboxShape[3][3]; //碉堡的形状
};

