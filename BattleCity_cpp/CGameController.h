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

	//��ʾ���˵�
	void showMenu();
	//��ʾ��Ϸ��Ϣ���
	void showGameInfoPanel();
	//������Ϸ��Ϣ
	void updateGameInfo();
	//��ʾ�ҷ�ս�ܵ���ʾ
	void showFailedNotice();
	//��ʾ�ҷ�ʤ������ʾ
	void showWinNotice();

private:
	int m_nGameLevel;
	int m_nE0Num;
	int m_nE1Num;
	int m_nEAliveNum;
	CGameMap m_gMap;
	CPillbox m_gPillbox;
	vector<CBullet *> m_vtBulletbox;  //�ӵ���
	vector<CTank> m_vtTanks;          //̹��Ⱥ,�����ҷ�̹�˺͵з�̹��
};

