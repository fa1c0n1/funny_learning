#include "CGameController.h"
#include "DrawTool.h"
#include "Data.h"
#include "CGameMap.h"
#include "CAnimation.h"
#include "CAstar.h"
#include <cstdlib>
#include <conio.h>
#include <ctime>
#include <string>

#pragma comment(lib, "winmm.lib")

using std::string;

CGameController::CGameController() : m_nGameLevel(1), m_bEnemyCanRevive(true)
{
}


CGameController::~CGameController()
{
	freeResources();
}

void CGameController::freeResources()
{
	for (int i = 0; i < m_vtBulletbox.size(); i++) {
		delete m_vtBulletbox[i];
		m_vtBulletbox[i] = nullptr;
	}
	m_vtBulletbox.clear();
	m_vtTanks.clear();
}

void CGameController::launchGame()
{
	//初始化窗口
	DrawTool::setWindowSize("Battle City", 116, 40);

	//播放开头音乐
	PlaySoundA("sound/bgm_welcome.wav", NULL, SND_ASYNC | SND_LOOP);

	//欢迎动画
	CAnimation anim;
	anim.welcomeAnim();

	//创建A星对象
	m_gAStar.initMapInfo(40, 40);

MAIN_MENUE:
	//主菜单:
	anim.showStillText();
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
			case KEY_2:
				showTODO();
				break;
			case KEY_3:
				system("cls");
				m_gMap.editMap();
				system("cls");
				goto MAIN_MENUE;
			case KEY_4:
				goto END;
			default:
				continue;
			}
		}
	}

START_GAME:
	//播放游戏进行音乐
	PlaySoundA("sound/90tank.wav", NULL, SND_ASYNC);

	//初始化坦克
	initTanks();

	//初始化子弹盒
	initBulletbox();
	
	//读取地图
	m_gMap.setLevel(m_nGameLevel);
	m_gMap.loadMap();

	//画出地图
	m_gMap.drawMap();

	//画出游戏信息面板
	showGameInfoPanel();

	//画出碉堡
	m_gPillbox.setIsDead(false);
	m_gPillbox.setMapObj(&m_gMap);
	m_gPillbox.drawObject();

	//画出所有坦克
	for (int i = 0; i < m_vtTanks.size(); i++) {
		m_vtTanks[i].setMapObj(&m_gMap);
		m_vtTanks[i].drawObject();
	}

	while (true) {
		//碉堡被击毁，或者玩家坦克被全灭,结束游戏
		if (m_gPillbox.isDead() || (m_vtTanks[0].isDead() && m_vtTanks[1].isDead())) {
			PlaySoundA("sound/bgm_failed.wav", NULL, SND_ASYNC);
			showFailedNotice();
			Sleep(3000);
			m_nGameLevel = 1;
			m_gMap.setLevel(m_nGameLevel);
			freeResources();
			system("cls");
			clearKeyBuffer();
			goto MAIN_MENUE;
		}

		//敌方被全灭
		if (m_nEAliveNum == 0) {
			PlaySoundA("sound/pal_win.wav", NULL, SND_ASYNC);
			showWinNotice();
			Sleep(3000);
			freeResources();
			system("cls");
			if (m_nGameLevel == 3) {
				m_nGameLevel = 1;
				m_gMap.setLevel(m_nGameLevel);
				clearKeyBuffer();
				goto MAIN_MENUE;
			}
			else {
				m_nGameLevel += 1;
				goto START_GAME;
			}
		}
		else if (m_nEAliveNum == 5 && m_bEnemyCanRevive) { //敌军复活
			showNewEnemiesComeNotice();
			for (int i = 0; i < ENEMY_NMAX; i++) {
				m_vtTanks[i + 2].getTankBirthPlace(m_vtTanks[i + 2].getType(), i);
			}

			//只能复活一次
			m_bEnemyCanRevive = false;
		}

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
			else if (KEYDOWN('J')) //按键J, 玩家A发射子弹
				CBullet::fireBullet(m_vtBulletbox, m_vtTanks[0]);
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
				CBullet::fireBullet(m_vtBulletbox, m_vtTanks[1]);
			}
		}

		int nPauseRet;
		if (KEYDOWN('')) {
			//暂停游戏
			nPauseRet = showPauseMenu();
			if (nPauseRet == KEY_3) {
				PlaySoundA(NULL, NULL, SND_ASYNC | SND_LOOP);
				//返回主菜单
				system("cls");
				m_nGameLevel = 1;
				m_gMap.setLevel(m_nGameLevel);
				freeResources();
				goto MAIN_MENUE;
			}
		}

		//随机移动敌军坦克
		randomMoveEnemies();
		//移动所有发射出去的子弹
		moveBullets(m_vtBulletbox, m_vtTanks, m_gPillbox, &m_nEAliveNum);
		updateGameInfo();

		Sleep(50);
	}

END:
	return;
}

void CGameController::showMenu()
{
	char *menuStr[4] = { "1. 开 始 游 戏", "2. 继 续 游 戏", "3. 编 辑 地 图", "4. 退 出 游 戏" };

	for (int i = 0; i < _countof(menuStr); i++) {
		DrawTool::drawText(23, 22 + 2 * i, menuStr[i], FG_LIGHTGREEN);
	}
}

int CGameController::showPauseMenu()
{
	int nRet = KEY_3;
	char *menuStr[3] = { "1.继续游戏", "2.保存游戏", "3.返　　回" };
	for (int i = 0; i < _countof(menuStr); i++) {
		DrawTool::drawText(42, 25 + 2 * i, menuStr[i], FG_LIGHTGREEN);
	}

	while (true) {
		if (_kbhit()) {
			int key = _getch();
			if (key == KEY_2) {
				//保存游戏: TODO
				showTODO();
			} else if (key == KEY_1 || key == KEY_3) {
				nRet = key;
				break;
			}
		}
		else {
			Sleep(30);
		}
	}

	for (int i = 0; i < _countof(menuStr); i++) {
		DrawTool::drawText(42, 25 + 2 * i, "         ", 0);
	}

	return nRet;
}

void CGameController::showTODO()
{
	DrawTool::drawText(41, 32, " ━━━━━━ ", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 33, "┃功能未完成┃", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 34, "┃          ┃", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 35, "┃ 敬请期待 ┃", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 36, "┃__________┃", FG_LIGHTTURQUOISE);
	Sleep(500);
	DrawTool::drawText(41, 32, "             ", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 33, "              ", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 34, "              ", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 35, "              ", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 36, "              ", FG_LIGHTTURQUOISE);
}

void CGameController::showNewEnemiesComeNotice()
{
	DrawTool::drawText(41, 21, "━━━━━━━━━━━━━━", FG_LIGHTRED);
	DrawTool::drawText(41, 22, "┃     敌情预警！！！     ┃", FG_LIGHTRED);
	DrawTool::drawText(41, 23, "┃                        ┃", FG_LIGHTRED);
	DrawTool::drawText(41, 24, "┃ 新一波敌方大军正在袭来 ┃", FG_LIGHTRED);
	DrawTool::drawText(41, 25, "┃                        ┃", FG_LIGHTRED);
	DrawTool::drawText(41, 26, "┃━━━━━━━━━━━━┃", FG_LIGHTRED);
	Sleep(1500);
	DrawTool::drawText(41, 21, "                           ", 0);
	DrawTool::drawText(41, 22, "                           ", 0);
	DrawTool::drawText(41, 23, "                           ", 0);
	DrawTool::drawText(41, 24, "                           ", 0);
	DrawTool::drawText(41, 25, "                           ", 0);
	DrawTool::drawText(41, 26, "                           ", 0);
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
	if (m_nGameLevel == 1 || m_nGameLevel == 2)
		m_nE0Num = 4;
	else if (m_nGameLevel == 3)
		m_nE0Num = 3;

	m_nEAliveNum = ENEMY_NMAX * 2;
	m_nE1Num = ENEMY_NMAX - m_nE0Num;
	for (int i = 0; i < m_nE0Num; i++) {
		tmpTank = tmpTank.getTankBirthPlace(SIGN_TANK_E0, i);
		m_vtTanks.push_back(tmpTank);
	}

	for (int i = m_nE0Num; i < ENEMY_NMAX; i++) {
		tmpTank = tmpTank.getTankBirthPlace(SIGN_TANK_E1, i);
		m_vtTanks.push_back(tmpTank);
	}

	m_bEnemyCanRevive = true;
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

void CGameController::randomMoveEnemies()
{
	int nDrtArr[4] = { DRT_UP, DRT_DOWN, DRT_LEFT, DRT_RIGHT };
	srand(time(NULL));
	clock_t endTime;

	for (int i = 0; i < ENEMY_NMAX - 1; i++) {
		if (!m_vtTanks[i + 2].isDead()) {
			endTime = clock();
			if (endTime % 150 + 150 > 200)
				m_vtTanks[i + 2].moveTank(nDrtArr[rand() % 4]);

			if (endTime % 150 + 150 > (296 - m_nGameLevel * 2))
				CBullet::fireBullet(m_vtBulletbox, m_vtTanks[i + 2]);
		}
	}

	//最后一个坦克以A星寻路算法去移动
	CTank &aStarTank = m_vtTanks.back();
	if (!aStarTank.isDead()) {
		bool bFind = false;
		if (!m_vtTanks[0].isDead())
			bFind = m_gAStar.findPath(aStarTank, m_vtTanks[0]);
		else if (!m_vtTanks[1].isDead())
			bFind = m_gAStar.findPath(aStarTank, m_vtTanks[1]);

		if (bFind) {
			vector<CAstar::MY_COORD> vtAStarPath = m_gAStar.getPath();
			for (int i = vtAStarPath.size() - 1; i >= 0; i--) {
				if (vtAStarPath[i].X > aStarTank.getPosX())
					aStarTank.moveTank(DRT_RIGHT);
				else if (vtAStarPath[i].X < aStarTank.getPosX())
					aStarTank.moveTank(DRT_LEFT);

				if (vtAStarPath[i].Y > aStarTank.getPosY())
					aStarTank.moveTank(DRT_DOWN);
				else if (vtAStarPath[i].Y < aStarTank.getPosY())
					aStarTank.moveTank(DRT_UP);

				break;
			}
		}
		else {
			aStarTank.moveTank(nDrtArr[rand() % 4]);
		}

		if (endTime % 150 + 150 > (290 - m_nGameLevel * 2))
			CBullet::fireBullet(m_vtBulletbox, aStarTank);
	}
}

void CGameController::moveBullets(vector<CBullet*> &vtBulletBox, vector<CTank> &vtTanks, CPillbox &pillbox, int *pEAliveNum)
{
	for (CBullet *pTBullet : m_vtBulletbox) {
		if (!pTBullet->isValid())
			pTBullet->moveBullet(vtBulletBox, vtTanks, pillbox, pEAliveNum);
	}
}

void CGameController::showGameInfoPanel()
{
	DrawTool::drawText(47, 1, "游戏信息", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 3, "关卡: ", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 5, "━━━━━━ 玩家A ━━━━━━", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 7, "血 量: ", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 9, "分 数: ", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 11, "━━━━━━ 玩家B ━━━━━━", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 13, "血 量: ", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 15, "分 数: ", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 17, "━━━━━━ 敌军 ━━━━━━", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 19, "敌方坦克数量: ", FG_LIGHTTURQUOISE);
}

void CGameController::updateGameInfo()
{
	char buf[32] = { 0 };

	//关卡
	sprintf_s(buf, _countof(buf), "%d", m_nGameLevel);
	DrawTool::drawText(44, 3, buf, FG_LIGHTTURQUOISE);

	//玩家A
	sprintf_s(buf, _countof(buf), "%d", m_vtTanks[0].getBlood());
	DrawTool::drawText(45, 7, buf, FG_LIGHTTURQUOISE);
	sprintf_s(buf, _countof(buf), "%d", m_vtTanks[0].getScore());
	DrawTool::drawText(45, 9, buf, FG_LIGHTTURQUOISE);

	//玩家B
	sprintf_s(buf, _countof(buf), "%d", m_vtTanks[1].getBlood());
	DrawTool::drawText(45, 13, buf, FG_LIGHTTURQUOISE);
	sprintf_s(buf, _countof(buf), "%d", m_vtTanks[1].getScore());
	DrawTool::drawText(45, 15, buf, FG_LIGHTTURQUOISE);

	//敌军
	sprintf_s(buf, _countof(buf), "%d  ", m_nEAliveNum);
	DrawTool::drawText(48, 19, buf, FG_LIGHTTURQUOISE);
}

void CGameController::showFailedNotice()
{
	DrawTool::drawText(7, 7, "━━━━━━━━━━━━━━━━━━━━━━", FG_LIGHTRED);
	DrawTool::drawText(7, 8, "┃               GAME OVER                ┃", FG_LIGHTRED);
	DrawTool::drawText(7, 9, "┃                                        ┃", FG_LIGHTRED);
	DrawTool::drawText(7, 10, "┃       胜败乃兵家常事, 请从头再来！     ┃", FG_LIGHTRED);
	DrawTool::drawText(7, 11, "┃                                        ┃", FG_LIGHTRED);
	DrawTool::drawText(7, 12, "┃━━━━━━━━━━━━━━━━━━━━┃", FG_LIGHTRED);
}

void CGameController::showWinNotice()
{
	DrawTool::drawText(7, 7, "━━━━━━━━━━━━━━━━━━━━━━", FG_LIGHTRED);
	DrawTool::drawText(7, 8, "┃               YOU WIN                  ┃", FG_LIGHTRED);
	DrawTool::drawText(7, 9, "┃                                        ┃", FG_LIGHTRED);
	if (m_nGameLevel == 3)
		DrawTool::drawText(7, 10, "┃       恭喜你！全部通关啦！！！         ┃", FG_LIGHTRED);
	else
		DrawTool::drawText(7, 10, "┃     胜利！即将开启下一关！！！         ┃", FG_LIGHTRED);
	DrawTool::drawText(7, 11, "┃                                        ┃", FG_LIGHTRED);
	DrawTool::drawText(7, 12, "┃━━━━━━━━━━━━━━━━━━━━┃", FG_LIGHTRED);
}

void CGameController::clearKeyBuffer()
{
	while (_kbhit())
		_getch();
}
