#pragma once

#include "CBase.h"
#include "CGameMap.h"

class CPillbox : public CBase
{
public:
	CPillbox();
	~CPillbox();

	void drawObject();
	void clearObject();

	void setMapObj(CGameMap *pGMap);
	void setIsDead(bool bDead);
	bool isDead();

private:
	bool m_bDead;
	int m_nX;
	int m_nY;
	int m_nType;
	CGameMap *m_pMap;
	static int m_nPillboxShape[3][3];
};

