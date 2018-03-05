#pragma once

#include "CGameMap.h"
#include "CBullet.h"
#include "CTank.h"
#include "CPillbox.h"
#include <vector>

using std::vector;

class CGameController
{
public:
	CGameController();
	~CGameController();

	void launchGame();
	void showMenu();
	void initTanks();
	void initBulletbox();

	void setGameLevel(int nLevel);
	void randomMoveEnemies();
	void moveBullets(vector<CBullet*> &vtBulletBox, vector<CTank> &vtTanks, CPillbox &pillbox);

private:
	int m_nGameLevel;
	int m_nEnemyNum;
	CGameMap m_gMap;
	CPillbox m_gPillbox;
	vector<CBullet *> m_vtBulletbox;  //�ӵ���
	vector<CTank> m_vtTanks;      //̹��Ⱥ,�����ҷ�̹�˺͵з�̹��
};

