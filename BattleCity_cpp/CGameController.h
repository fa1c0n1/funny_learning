#pragma once

#include "CGameMap.h"
#include "CBullet.h"
#include "CTank.h"
#include "CPillbox.h"
#include "CAstar.h"
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
	//��ʾ�Ӳ˵�: ��ͣ��Ϸʱ��ʾ
	int showPauseMenu();
	//��ʾ����δ��ɵ���ʾ��
	void showTODO();
	//��ʾ�µо���Ϯ����ʾ��
	void showNewEnemiesComeNotice();
	//��ռ��̻������еļ�ֵ
	void clearKeyBuffer();

private:
	bool m_bEnemyCanRevive;
	int m_nGameLevel;                 //��Ϸ�ؿ�
	int m_nE0Num;                     //�о�E0̹�˵�����
	int m_nE1Num;                     //�о�E1̹�˵�����
	int m_nEAliveNum;                 //�о�̹�˴����
	CGameMap m_gMap;                  //��ͼ, ����̹�˶�����ӵ�������
	CPillbox m_gPillbox;              //�ﱤ
	CAstar m_gAStar;                  //A�Ƕ���
	vector<CBullet *> m_vtBulletbox;  //�ӵ���
	vector<CTank> m_vtTanks;          //̹��Ⱥ,�����ҷ�̹�˺͵з�̹��
};

