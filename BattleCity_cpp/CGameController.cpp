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
	//初始化窗口
	DrawTool::setWindowSize("Battle City", 120, 40);

	//菜单: TODO
	showMenu();

	//处理菜单的按键事件
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
	//初始化坦克
	initTanks();

	//初始化子弹盒
	initBulletbox();
	
	//读取地图
	m_gMap.loadMap();

	//画出地图
	m_gMap.drawMap();

	//画出碉堡
	m_gPillbox.setMapObj(&m_gMap);
	m_gPillbox.drawObject();

	//画出所有坦克
	for (int i = 0; i < m_vtTanks.size(); i++) {
		m_vtTanks[i].setMapObj(&m_gMap);
		m_vtTanks[i].drawObject();
	}

	while (true) {
		if (!m_vtTanks[0].isDead()) {
			//处理玩家A的按键事件
			if (KEYDOWN('A')) //按键A
				m_vtTanks[0].moveTank(DRT_LEFT);
			else if (KEYDOWN('D')) //按键D
				m_vtTanks[0].moveTank(DRT_RIGHT);
			else if (KEYDOWN('W')) //按键W
				m_vtTanks[0].moveTank(DRT_UP);
			else if (KEYDOWN('S')) //按键S
				m_vtTanks[0].moveTank(DRT_DOWN);
			else if (KEYDOWN('J')) { //按键J, 玩家A发射子弹
				CBullet tmpBullet;
				tmpBullet.fireBullet(m_vtBulletbox, m_vtTanks[0]);
			}
		}

		if (!m_vtTanks[1].isDead()) {
			//处理玩家B的按键事件
			if (KEYDOWN(VK_NUMPAD4)) //小键盘数字4
				m_vtTanks[1].moveTank(DRT_LEFT);
			else if (KEYDOWN(VK_NUMPAD6)) //小键盘数字6
				m_vtTanks[1].moveTank(DRT_RIGHT);
			else if (KEYDOWN(VK_NUMPAD8)) //小键盘数字8
				m_vtTanks[1].moveTank(DRT_UP);
			else if (KEYDOWN(VK_NUMPAD5)) //小键盘数字5
				m_vtTanks[1].moveTank(DRT_DOWN);
			else if (KEYDOWN(VK_NUMPAD0)) { //小键盘数字0, 玩家B发射坦克
				CBullet tmpBullet;
				tmpBullet.fireBullet(m_vtBulletbox, m_vtTanks[1]);
			}
		}

		//随机移动敌军坦克
		randomMoveEnemies();
		//移动所有发射出去的子弹
		moveBullets(m_vtBulletbox, m_vtTanks, m_gPillbox);

		Sleep(50);
	}
}

void CGameController::showMenu()
{
	char *menuStr[4] = { "1. 开 始 游 戏", "2. 继 续 游 戏", "3. 编 辑 地 图", "4. 退 出 游 戏" };

	for (int i = 0; i < _countof(menuStr); i++) {
		DrawTool::drawText(20, 22 + 2 * i, menuStr[i], FG_LIGHTGREEN);
	}
}
	
void CGameController::initTanks()
{
	CTank tmpTank;
	
	//创建我方坦克
	tmpTank = tmpTank.getTankBirthPlace(SIGN_TANK_PA);
	m_vtTanks.push_back(tmpTank);
	tmpTank = tmpTank.getTankBirthPlace(SIGN_TANK_PB);
	m_vtTanks.push_back(tmpTank);

	//创建敌军坦克
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
