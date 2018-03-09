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
	//³õÊ¼»¯´°¿Ú
	DrawTool::setWindowSize("Battle City", 116, 40);

	//²¥·Å¿ªÍ·ÒôÀÖ
	PlaySoundA("sound/bgm_welcome.wav", NULL, SND_ASYNC | SND_LOOP);

	//»¶Ó­¶¯»­
	CAnimation anim;
	anim.welcomeAnim();

	//´´½¨AÐÇ¶ÔÏó
	m_gAStar.initMapInfo(40, 40);

MAIN_MENUE:
	//Ö÷²Ëµ¥:
	anim.showStillText();
	showMenu();

	//´¦Àí²Ëµ¥µÄ°´¼üÊÂ¼þ
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
	//²¥·ÅÓÎÏ·½øÐÐÒôÀÖ
	PlaySoundA("sound/90tank.wav", NULL, SND_ASYNC);

	//³õÊ¼»¯Ì¹¿Ë
	initTanks();

	//³õÊ¼»¯×Óµ¯ºÐ
	initBulletbox();
	
	//¶ÁÈ¡µØÍ¼
	m_gMap.setLevel(m_nGameLevel);
	m_gMap.loadMap();

	//»­³öµØÍ¼
	m_gMap.drawMap();

	//»­³öÓÎÏ·ÐÅÏ¢Ãæ°å
	showGameInfoPanel();

	//»­³öµï±¤
	m_gPillbox.setIsDead(false);
	m_gPillbox.setMapObj(&m_gMap);
	m_gPillbox.drawObject();

	//»­³öËùÓÐÌ¹¿Ë
	for (int i = 0; i < m_vtTanks.size(); i++) {
		m_vtTanks[i].setMapObj(&m_gMap);
		m_vtTanks[i].drawObject();
	}

	while (true) {
		//µï±¤±»»÷»Ù£¬»òÕßÍæ¼ÒÌ¹¿Ë±»È«Ãð,½áÊøÓÎÏ·
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

		//µÐ·½±»È«Ãð
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
		else if (m_nEAliveNum == 5 && m_bEnemyCanRevive) { //µÐ¾ü¸´»î
			showNewEnemiesComeNotice();
			for (int i = 0; i < ENEMY_NMAX; i++) {
				m_vtTanks[i + 2].getTankBirthPlace(m_vtTanks[i + 2].getType(), i);
			}

			//Ö»ÄÜ¸´»îÒ»´Î
			m_bEnemyCanRevive = false;
		}

		if (!m_vtTanks[0].isDead()) {
			//´¦ÀíÍæ¼ÒAµÄ°´¼üÊÂ¼þ
			if (KEYDOWN('A')) //°´¼üA
				m_vtTanks[0].moveTank(DRT_LEFT);
			else if (KEYDOWN('D')) //°´¼üD
				m_vtTanks[0].moveTank(DRT_RIGHT);
			else if (KEYDOWN('W')) //°´¼üW
				m_vtTanks[0].moveTank(DRT_UP);
			else if (KEYDOWN('S')) //°´¼üS
				m_vtTanks[0].moveTank(DRT_DOWN);
			else if (KEYDOWN('J')) //°´¼üJ, Íæ¼ÒA·¢Éä×Óµ¯
				CBullet::fireBullet(m_vtBulletbox, m_vtTanks[0]);
		}

		if (!m_vtTanks[1].isDead()) {
			//´¦ÀíÍæ¼ÒBµÄ°´¼üÊÂ¼þ
			if (KEYDOWN(VK_NUMPAD4)) //Ð¡¼üÅÌÊý×Ö4
				m_vtTanks[1].moveTank(DRT_LEFT);
			else if (KEYDOWN(VK_NUMPAD6)) //Ð¡¼üÅÌÊý×Ö6
				m_vtTanks[1].moveTank(DRT_RIGHT);
			else if (KEYDOWN(VK_NUMPAD8)) //Ð¡¼üÅÌÊý×Ö8
				m_vtTanks[1].moveTank(DRT_UP);
			else if (KEYDOWN(VK_NUMPAD5)) //Ð¡¼üÅÌÊý×Ö5
				m_vtTanks[1].moveTank(DRT_DOWN);
			else if (KEYDOWN(VK_NUMPAD0)) { //Ð¡¼üÅÌÊý×Ö0, Íæ¼ÒB·¢ÉäÌ¹¿Ë
				CBullet::fireBullet(m_vtBulletbox, m_vtTanks[1]);
			}
		}

		int nPauseRet;
		if (KEYDOWN('')) {
			//ÔÝÍ£ÓÎÏ·
			nPauseRet = showPauseMenu();
			if (nPauseRet == KEY_3) {
				PlaySoundA(NULL, NULL, SND_ASYNC | SND_LOOP);
				//·µ»ØÖ÷²Ëµ¥
				system("cls");
				m_nGameLevel = 1;
				m_gMap.setLevel(m_nGameLevel);
				freeResources();
				goto MAIN_MENUE;
			}
		}

		//Ëæ»úÒÆ¶¯µÐ¾üÌ¹¿Ë
		randomMoveEnemies();
		//ÒÆ¶¯ËùÓÐ·¢Éä³öÈ¥µÄ×Óµ¯
		moveBullets(m_vtBulletbox, m_vtTanks, m_gPillbox, &m_nEAliveNum);
		updateGameInfo();

		Sleep(50);
	}

END:
	return;
}

void CGameController::showMenu()
{
	char *menuStr[4] = { "1. ¿ª Ê¼ ÓÎ Ï·", "2. ¼Ì Ðø ÓÎ Ï·", "3. ±à ¼­ µØ Í¼", "4. ÍË ³ö ÓÎ Ï·" };

	for (int i = 0; i < _countof(menuStr); i++) {
		DrawTool::drawText(23, 22 + 2 * i, menuStr[i], FG_LIGHTGREEN);
	}
}

int CGameController::showPauseMenu()
{
	int nRet = KEY_3;
	char *menuStr[3] = { "1.¼ÌÐøÓÎÏ·", "2.±£´æÓÎÏ·", "3.·µ¡¡¡¡»Ø" };
	for (int i = 0; i < _countof(menuStr); i++) {
		DrawTool::drawText(42, 25 + 2 * i, menuStr[i], FG_LIGHTGREEN);
	}

	while (true) {
		if (_kbhit()) {
			int key = _getch();
			if (key == KEY_2) {
				//±£´æÓÎÏ·: TODO
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
	DrawTool::drawText(41, 32, " ©¥©¥©¥©¥©¥©¥ ", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 33, "©§¹¦ÄÜÎ´Íê³É©§", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 34, "©§          ©§", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 35, "©§ ¾´ÇëÆÚ´ý ©§", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 36, "©§__________©§", FG_LIGHTTURQUOISE);
	Sleep(500);
	DrawTool::drawText(41, 32, "             ", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 33, "              ", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 34, "              ", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 35, "              ", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 36, "              ", FG_LIGHTTURQUOISE);
}

void CGameController::showNewEnemiesComeNotice()
{
	DrawTool::drawText(41, 21, "©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥", FG_LIGHTRED);
	DrawTool::drawText(41, 22, "©§     µÐÇéÔ¤¾¯£¡£¡£¡     ©§", FG_LIGHTRED);
	DrawTool::drawText(41, 23, "©§                        ©§", FG_LIGHTRED);
	DrawTool::drawText(41, 24, "©§ ÐÂÒ»²¨µÐ·½´ó¾üÕýÔÚÏ®À´ ©§", FG_LIGHTRED);
	DrawTool::drawText(41, 25, "©§                        ©§", FG_LIGHTRED);
	DrawTool::drawText(41, 26, "©§©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©§", FG_LIGHTRED);
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
	
	//´´½¨ÎÒ·½Ì¹¿Ë
	tmpTank = tmpTank.getTankBirthPlace(SIGN_TANK_PA);
	m_vtTanks.push_back(tmpTank);
	tmpTank = tmpTank.getTankBirthPlace(SIGN_TANK_PB);
	m_vtTanks.push_back(tmpTank);

	//´´½¨µÐ¾üÌ¹¿Ë
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

	//×îºóÒ»¸öÌ¹¿ËÒÔAÐÇÑ°Â·Ëã·¨È¥ÒÆ¶¯
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
	DrawTool::drawText(47, 1, "ÓÎÏ·ÐÅÏ¢", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 3, "¹Ø¿¨: ", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 5, "©¥©¥©¥©¥©¥©¥ Íæ¼ÒA ©¥©¥©¥©¥©¥©¥", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 7, "Ñª Á¿: ", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 9, "·Ö Êý: ", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 11, "©¥©¥©¥©¥©¥©¥ Íæ¼ÒB ©¥©¥©¥©¥©¥©¥", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 13, "Ñª Á¿: ", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 15, "·Ö Êý: ", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 17, "©¥©¥©¥©¥©¥©¥ µÐ¾ü ©¥©¥©¥©¥©¥©¥", FG_LIGHTTURQUOISE);
	DrawTool::drawText(41, 19, "µÐ·½Ì¹¿ËÊýÁ¿: ", FG_LIGHTTURQUOISE);
}

void CGameController::updateGameInfo()
{
	char buf[32] = { 0 };

	//¹Ø¿¨
	sprintf_s(buf, _countof(buf), "%d", m_nGameLevel);
	DrawTool::drawText(44, 3, buf, FG_LIGHTTURQUOISE);

	//Íæ¼ÒA
	sprintf_s(buf, _countof(buf), "%d", m_vtTanks[0].getBlood());
	DrawTool::drawText(45, 7, buf, FG_LIGHTTURQUOISE);
	sprintf_s(buf, _countof(buf), "%d", m_vtTanks[0].getScore());
	DrawTool::drawText(45, 9, buf, FG_LIGHTTURQUOISE);

	//Íæ¼ÒB
	sprintf_s(buf, _countof(buf), "%d", m_vtTanks[1].getBlood());
	DrawTool::drawText(45, 13, buf, FG_LIGHTTURQUOISE);
	sprintf_s(buf, _countof(buf), "%d", m_vtTanks[1].getScore());
	DrawTool::drawText(45, 15, buf, FG_LIGHTTURQUOISE);

	//µÐ¾ü
	sprintf_s(buf, _countof(buf), "%d  ", m_nEAliveNum);
	DrawTool::drawText(48, 19, buf, FG_LIGHTTURQUOISE);
}

void CGameController::showFailedNotice()
{
	DrawTool::drawText(7, 7, "©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥", FG_LIGHTRED);
	DrawTool::drawText(7, 8, "©§               GAME OVER                ©§", FG_LIGHTRED);
	DrawTool::drawText(7, 9, "©§                                        ©§", FG_LIGHTRED);
	DrawTool::drawText(7, 10, "©§       Ê¤°ÜÄË±ø¼Ò³£ÊÂ, Çë´ÓÍ·ÔÙÀ´£¡     ©§", FG_LIGHTRED);
	DrawTool::drawText(7, 11, "©§                                        ©§", FG_LIGHTRED);
	DrawTool::drawText(7, 12, "©§©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©§", FG_LIGHTRED);
}

void CGameController::showWinNotice()
{
	DrawTool::drawText(7, 7, "©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥", FG_LIGHTRED);
	DrawTool::drawText(7, 8, "©§               YOU WIN                  ©§", FG_LIGHTRED);
	DrawTool::drawText(7, 9, "©§                                        ©§", FG_LIGHTRED);
	if (m_nGameLevel == 3)
		DrawTool::drawText(7, 10, "©§       ¹§Ï²Äã£¡È«²¿Í¨¹ØÀ²£¡£¡£¡         ©§", FG_LIGHTRED);
	else
		DrawTool::drawText(7, 10, "©§     Ê¤Àû£¡¼´½«¿ªÆôÏÂÒ»¹Ø£¡£¡£¡         ©§", FG_LIGHTRED);
	DrawTool::drawText(7, 11, "©§                                        ©§", FG_LIGHTRED);
	DrawTool::drawText(7, 12, "©§©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©§", FG_LIGHTRED);
}

void CGameController::clearKeyBuffer()
{
	while (_kbhit())
		_getch();
}
