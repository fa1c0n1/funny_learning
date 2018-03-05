#include "CGameController.h"
#include "DrawTool.h"
#include "Data.h"
#include "CGameMap.h"

#include <cstdlib>
#include <conio.h>
#include <ctime>

CGameController::CGameController() : m_nGameLevel(1), m_nEnemyNum(ENEMY_NMAX)
{
}


CGameController::~CGameController()
{
	for (int i = 0; i < m_vtBulletbox.size(); i++) {
		delete m_vtBulletbox[i];
		m_vtBulletbox[i] = nullptr;
	}
	m_vtBulletbox.clear();
}

void CGameController::launchGame()
{
	//��ʼ������
	DrawTool::setWindowSize("Battle City", 120, 40);

	//�˵�: TODO
	showMenu();

	//����˵��İ����¼�
	while (true) {
		if (_kbhit()) {
			int key = _getch();
			switch (key)
			{
			case KEY_1:
				system("cls");
				goto START_GAME;
				break;
			case KEY_2:
				break;
			case KEY_3:
				break;
			default:
				continue;
			}
		}
	}

START_GAME:
	//��ʼ��̹��
	initTanks();

	//��ʼ���ӵ���
	initBulletbox();
	
	//��ȡ��ͼ
	m_gMap.loadMap();

	//������ͼ
	m_gMap.drawMap();

	//�����ﱤ
	m_gPillbox.setMapObj(&m_gMap);
	m_gPillbox.drawObject();

	//��������̹��
	for (int i = 0; i < m_vtTanks.size(); i++) {
		m_vtTanks[i].setMapObj(&m_gMap);
		m_vtTanks[i].drawObject();
	}

	while (true) {
		if (!m_vtTanks[0].isDead()) {
			//�������A�İ����¼�
			if (KEYDOWN('A')) //����A
				m_vtTanks[0].moveTank(DRT_LEFT);
			else if (KEYDOWN('D')) //����D
				m_vtTanks[0].moveTank(DRT_RIGHT);
			else if (KEYDOWN('W')) //����W
				m_vtTanks[0].moveTank(DRT_UP);
			else if (KEYDOWN('S')) //����S
				m_vtTanks[0].moveTank(DRT_DOWN);
			else if (KEYDOWN('J')) { //����J, ���A�����ӵ�
				CBullet tmpBullet;
				tmpBullet.fireBullet(m_vtBulletbox, m_vtTanks[0]);
			}
		}

		if (!m_vtTanks[1].isDead()) {
			//�������B�İ����¼�
			if (KEYDOWN(VK_NUMPAD4)) //С��������4
				m_vtTanks[1].moveTank(DRT_LEFT);
			else if (KEYDOWN(VK_NUMPAD6)) //С��������6
				m_vtTanks[1].moveTank(DRT_RIGHT);
			else if (KEYDOWN(VK_NUMPAD8)) //С��������8
				m_vtTanks[1].moveTank(DRT_UP);
			else if (KEYDOWN(VK_NUMPAD5)) //С��������5
				m_vtTanks[1].moveTank(DRT_DOWN);
			else if (KEYDOWN(VK_NUMPAD0)) { //С��������0, ���B����̹��
				CBullet tmpBullet;
				tmpBullet.fireBullet(m_vtBulletbox, m_vtTanks[1]);
			}
		}

		//����ƶ��о�̹��
		randomMoveEnemies();
		//�ƶ����з����ȥ���ӵ�
		moveBullets(m_vtBulletbox, m_vtTanks, m_gPillbox);

		Sleep(50);
	}
}

void CGameController::showMenu()
{
	char *menuStr[4] = { "1. �� ʼ �� Ϸ", "2. �� �� �� Ϸ", "3. �� �� �� ͼ", "4. �� �� �� Ϸ" };

	for (int i = 0; i < _countof(menuStr); i++) {
		DrawTool::drawText(20, 22 + 2 * i, menuStr[i], FG_LIGHTGREEN);
	}
}
	
void CGameController::initTanks()
{
	CTank tmpTank;
	
	//�����ҷ�̹��
	tmpTank = tmpTank.getTankBirthPlace(SIGN_TANK_PA);
	m_vtTanks.push_back(tmpTank);
	tmpTank = tmpTank.getTankBirthPlace(SIGN_TANK_PB);
	m_vtTanks.push_back(tmpTank);

	//�����о�̹��
	int nE0Num = ENEMY_NMAX;
	if (m_nGameLevel == 2)
		nE0Num = 4;
	else if (m_nGameLevel == 3)
		nE0Num = 3;

	for (int i = 0; i < nE0Num; i++) {
		tmpTank = tmpTank.getTankBirthPlace(SIGN_TANK_E0, i);
		m_vtTanks.push_back(tmpTank);
	}

	for (int i = nE0Num; i < ENEMY_NMAX; i++) {
		tmpTank = tmpTank.getTankBirthPlace(SIGN_TANK_E1, i);
		m_vtTanks.push_back(tmpTank);
	}
}

void CGameController::initBulletbox()
{
	CBullet *tmpBullet = nullptr;
	for (int i = 0; i < BOX_CAPACITY; i++) {
		tmpBullet = new CBullet;
		tmpBullet->setMapObj(&m_gMap);
		tmpBullet->setID(i);
		m_vtBulletbox.push_back(tmpBullet);
	}
}

void CGameController::setGameLevel(int nLevel)
{
	m_nGameLevel = nLevel;
}
	
void CGameController::randomMoveEnemies()
{
	int nDrtArr[4] = { DRT_UP, DRT_DOWN, DRT_LEFT, DRT_RIGHT };
	srand(time(NULL));
	clock_t endTime;

	CBullet tmpBullet;
	for (int i = 0; i < ENEMY_NMAX; i++) {
		if (!m_vtTanks[i + 2].isDead()) {
			endTime = clock();
			if (endTime % 150 + 150 > 200)
				m_vtTanks[i + 2].moveTank(nDrtArr[rand() % 4]);
			
			if (endTime % 150 + 150 > 290)
				tmpBullet.fireBullet(m_vtBulletbox, m_vtTanks[i + 2]);
		}
	}
}

void CGameController::moveBullets(vector<CBullet*> &vtBulletBox, vector<CTank> &vtTanks, CPillbox &pillbox)
{
	for (CBullet *pTBullet : m_vtBulletbox) {
		if (!pTBullet->isValid())
			pTBullet->moveBullet(vtBulletBox, vtTanks, pillbox);
	}
}
