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
	void freeResources();
	void initTanks();
	void initBulletbox();
	void setGameLevel(int nLevel);
	void randomMoveEnemies();
	void moveBullets(vector<CBullet*> &vtBulletBox, vector<CTank> &vtTanks, CPillbox &pillbox, int *pEAliveNum);

	//显示主菜单
	void showMenu();
	//显示游戏信息面板
	void showGameInfoPanel();
	//更新游戏信息
	void updateGameInfo();
	//显示我方战败的提示
	void showFailedNotice();
	//显示我方胜利的提示
	void showWinNotice();

private:
	int m_nGameLevel;
	int m_nE0Num;
	int m_nE1Num;
	int m_nEAliveNum;
	CGameMap m_gMap;
	CPillbox m_gPillbox;
	vector<CBullet *> m_vtBulletbox;  //子弹盒
	vector<CTank> m_vtTanks;          //坦克群,包含我方坦克和敌方坦克
};

