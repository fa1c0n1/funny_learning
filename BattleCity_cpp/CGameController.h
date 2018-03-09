#pragma once

#include "CGameMap.h"
#include "CBullet.h"
#include "CTank.h"
#include "CPillbox.h"
#include "CAstar.h"
#include <vector>

using std::vector;

/************************************************************************/
/* CGameController类：游戏主逻辑控制
/************************************************************************/
class CGameController
{
public:
	CGameController();
	~CGameController();

	//进入游戏逻辑主循环
	void launchGame();
	//释放游戏资源
	void freeResources();
	//初始化坦克对象
	void initTanks();
	//初始化子弹盒
	void initBulletbox();
	//处理所有的敌军坦克的移动和发弹
	void randomMoveEnemies();
	//移动所有发出去的子弹
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
	//显示子菜单: 暂停游戏时显示
	int showPauseMenu();
	//显示功能未完成的提示框
	void showTODO();
	//显示新敌军来袭的提示框
	void showNewEnemiesComeNotice();
	//清空键盘缓冲区中的键值
	void clearKeyBuffer();

private:
	bool m_bEnemyCanRevive;           //敌军是否可以重生
	int m_nGameLevel;                 //游戏关卡
	int m_nE0Num;                     //敌军E0坦克的数量
	int m_nE1Num;                     //敌军E1坦克的数量
	int m_nEAliveNum;                 //敌军坦克存活数
	CGameMap m_gMap;                  //地图, 所有坦克对象和子弹对象共用
	CPillbox m_gPillbox;              //碉堡
	CAstar m_gAStar;                  //A星对象
	vector<CBullet *> m_vtBulletbox;  //子弹盒
	vector<CTank> m_vtTanks;          //坦克群,包含我方坦克和敌方坦克
};

