#include "controller.h"
#include "data.h"
#include "draw_tool.h"
#include "animation.h"
#include "map.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "pillbox.h"

#include <stdlib.h>
#include <conio.h>
#include <time.h>

//ÏÔÊ¾Ö÷²Ëµ¥
void ShowMenu(void)
{
	int bExit = 0;

	MenuItem mainMenu[4] = {
		{ KEY_1, StartGame, "1. ¿ª Ê¼ ÓÎ Ï·" },
		{ KEY_2, LoadGame, "2. ¼Ì Ðø ÓÎ Ï·" },
		{ KEY_3, EditMap, "3. ±à ¼­ µØ Í¼" },
		{ KEY_4, ExitGame, "4. ÍË ³ö ÓÎ Ï·" }
	};

	while (!bExit) {
		ShowStillText();
		for (int i = 0; i < _countof(mainMenu); i++) {
			WriteChar(20, 22 + 2 * i, mainMenu[i].szText, FG_LIGHTGREEN);
		}

		while (1) {
			if (_kbhit()) {
				int key = _getch();
				if (key >= KEY_1 && key <= KEY_4) {
					bExit = mainMenu[key - 48 - 1].func();
					break;
				}
			}

			Sleep(30);
		}
	}
}

//ÏÔÊ¾×Ó²Ëµ¥
int ShowSubMenu(void)
{
	int nRet = 0;

	MenuItem subMenu[3] = {
		{ KEY_1, GoOnGame, "1.¼ÌÐøÓÎÏ·" },
		{ KEY_2, SaveGame, "2.±£´æÓÎÏ·" },
		{ KEY_3, RetMainMenu, "3.·µ¡¡¡¡»Ø" },
	};

	for (int i = 0; i < _countof(subMenu); i++) {
		WriteChar(42, 25 + 2 * i, subMenu[i].szText, FG_LIGHTGREEN);
	}

	while (1) {
		if (_kbhit()) {
			int key = _getch();
			if (key >= KEY_1 && key <= KEY_3) {
				nRet = subMenu[key - 48 - 1].func();
				if (nRet != 2)
					break;
			}
		}

		Sleep(30);
	}

	for (int i = 0; i < _countof(subMenu); i++) {
		WriteChar(42, 25 + 2 * i, "         ", 0);
	}

	return nRet;
}

//×Ó²Ëµ¥£º¼ÌÐøÓÎÏ·
int GoOnGame(void)
{
	return 1;
}

//×Ó²Ëµ¥£º±£´æÓÎÏ·
int SaveGame(void)
{
	//TODO:
	ShowTODO();
	return 2;
}

//×Ó²Ëµ¥£º·µ»ØÖ÷²Ëµ¥
int RetMainMenu(void)
{
	return 3;
}

//½øÈëÓÎÏ·µÄÖ÷Âß¼­µÄÑ­»·
void GoRun(void)
{
	g_startTime = clock();
	int nSubRet = 0;

	ShowMap();
	ShowPillbox(g_pPillbox);
	ShowTank(g_pTankA);
	//ShowTank(g_pTankB);
	ShowEnemies(g_pEnemies);

	while (1) {
		if (!g_pTankA->bDead && !g_pPillbox->bDead) {
			if (g_nEnNum <= 0) { //µÐ¾ü±»È«Ãð
				ShowWinNotice();
				Sleep(3000);
				ClearAllBarrier();
				system("cls");
				goto END;
			}

			//°´¼ü¼ì²â
			switch (g_pTankA->eType)
			{
			case SIGN_TANK_PA:
				if (KEYDOWN('a') || KEYDOWN('A'))
					MoveTank(g_pTankA, DRT_LEFT);
				else if (KEYDOWN('d') || KEYDOWN('D'))
					MoveTank(g_pTankA, DRT_RIGHT);
				else if (KEYDOWN('w') || KEYDOWN('W'))
					MoveTank(g_pTankA, DRT_UP);
				else if (KEYDOWN('s') || KEYDOWN('S'))
					MoveTank(g_pTankA, DRT_DOWN);
				else if (KEYDOWN('j') || KEYDOWN('J'))
					FireBullet(g_pTankA);
				else if (KEYDOWN('')) {
					nSubRet = ShowSubMenu();
					if (nSubRet == 3) {//3±íÊ¾Òª·µ»Øµ½Ö÷²Ëµ¥
						ClearAllBarrier();
						system("cls");
						goto END;
					}
				}

				break;
			default:
				break;
			}
		}
		else { //Íæ¼Ò±»Ãð»òµï±¤±»»÷»Ù
			ClearAllBarrier();
			ShowFailedNotice();
			Sleep(3000);
			system("cls");
			goto END;
		}

		MoveBullets();
		RandomMoveEnemies();

		Sleep(50);
	}

END:
	return;
}

//ÏÔÊ¾Ê¤ÀûÌáÊ¾
void ShowWinNotice(void)
{

	WriteChar(7, 7, "©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥", FG_LIGHTRED);
	WriteChar(7, 8, "©§               YOU WIN                  ©§", FG_LIGHTRED);
	WriteChar(7, 9, "©§                                        ©§", FG_LIGHTRED);
	WriteChar(7, 10, "©§       ¹§Ï²Äã£¡Õ½Ê¤ÁËµÐÈË£¡£¡£¡         ©§", FG_LIGHTRED);
	WriteChar(7, 11, "©§                                        ©§", FG_LIGHTRED);
	WriteChar(7, 12, "©§©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©§", FG_LIGHTRED);
}

//ÏÔÊ¾Ê§°ÜÌáÊ¾
void ShowFailedNotice(void)
{
	WriteChar(7, 7, "©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥", FG_LIGHTRED);
	WriteChar(7, 8, "©§               GAME OVER                ©§", FG_LIGHTRED);
	WriteChar(7, 9, "©§                                        ©§", FG_LIGHTRED);
	WriteChar(7, 10, "©§       Ê¤°ÜÄË±ø¼Ò³£ÊÂ, Çë´ÓÍ·ÔÙÀ´£¡     ©§", FG_LIGHTRED);
	WriteChar(7, 11, "©§                                        ©§", FG_LIGHTRED);
	WriteChar(7, 12, "©§©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©¥©§", FG_LIGHTRED);
}

//ÏÔÊ¾¹¦ÄÜÎ´Íê³É
void ShowTODO(void)
{
	WriteChar(41, 32, " ©¥©¥©¥©¥©¥©¥ ", FG_LIGHTTURQUOISE);
	WriteChar(41, 33, "©§¹¦ÄÜÎ´Íê³É©§", FG_LIGHTTURQUOISE);
	WriteChar(41, 34, "©§          ©§", FG_LIGHTTURQUOISE);
	WriteChar(41, 35, "©§ ¾´ÇëÆÚ´ý ©§", FG_LIGHTTURQUOISE);
	WriteChar(41, 36, "©§__________©§", FG_LIGHTTURQUOISE);
	Sleep(1000);
	WriteChar(41, 32, "             ", FG_LIGHTTURQUOISE);
	WriteChar(41, 33, "              ", FG_LIGHTTURQUOISE);
	WriteChar(41, 34, "              ", FG_LIGHTTURQUOISE);
	WriteChar(41, 35, "              ", FG_LIGHTTURQUOISE);
	WriteChar(41, 36, "              ", FG_LIGHTTURQUOISE);
}

//³õÊ¼»¯ÓÎÏ·½ÇÉ«(°üÀ¨Íæ¼Ò¡¢µÐ¾üºÍ×Óµ¯)
void InitGameRoles(void)
{
	g_pPillbox = InitPillbox();
	g_pTankA = CreatePlayer(SIGN_TANK_PA, 12, 36, DRT_UP);
	//g_pTankB = CreatePlayer(SIGN_TANK_PB, 27, 25, DRT_UP);
	g_pBulletBox = InitBulletBox();
	g_pEnemies = InitEnemies();
}

//Ö÷²Ëµ¥£º¿ªÊ¼ÓÎÏ·
int StartGame(void)
{
	system("cls");
	InitGameRoles();
	InitDefaultMap();
	GoRun();
	FreeGameRes();
	return 0;
}

//Ö÷²Ëµ¥£º¼ÌÐøÓÎÏ·
int LoadGame(void)
{
	//TODO:
	ShowTODO();
	return 0;
}

//±à¼­µØÍ¼Ê±µÄ¼üÅÌ¼ì²â´¦Àí
int DrawMapKeyEventProc(KEY_EVENT_RECORD ker)
{
	int bRet = 0;

	if (ker.bKeyDown) { //¼üÅÌ°´ÏÂ
		CHAR keyVal = ker.uChar.AsciiChar;
		if (keyVal == 's') {
			bRet = 1;  //±£´æ±à¼­²¢¿ªÊ¼ÓÎÏ·
		}
		else if (keyVal == 'q') {
			bRet = 2; //·ÅÆú±à¼­²¢·µ»Ø
		}
	}
	else {  //¼üÅÌµ¯Æð

	}

END:
	return bRet;
}

//±à¼­µØÍ¼Ê±µÄÊó±ê¼ì²â´¦Àí
void DrawMapMouseEventProc(MOUSE_EVENT_RECORD mer)
{
	int nX = mer.dwMousePosition.X / 2;
	int nY = mer.dwMousePosition.Y;

	switch (mer.dwEventFlags)
	{
	case 0:
		if (nX == 46) {  //µã»÷ÕÏ°­ÎïÇÐ»»ÇøÓò
			if (nY == 1)
				g_eNodeType = SIGN_WALL0;
			else if (nY == 3)
				g_eNodeType = SIGN_WALL1;
			else if (nY == 5)
				g_eNodeType = SIGN_RIVER;
			else if (nY == 7)
				g_eNodeType = SIGN_GRASS;

			return;
		}

		if (nX >= 1 && nX <= 38 && nY >= 1 && nY <= 38) {
			if (nX >= 15 && nX <= 21) { //µï±¤ÇøÓò²»ÄÜ±à¼­
				if (nY >= 34 && nY <= 38)
					return;
			}

			if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) { //°´ÏÂÊó±ê×ó¼ü
				switch (g_eNodeType)
				{
				case SIGN_WALL0:
					WriteChar(nX, nY, "¡ö", FG_HIGHWHITE);
					break;
				case SIGN_WALL1:
					WriteChar(nX, nY, "¡ö", FG_YELLOW);
					break;
				case SIGN_RIVER:
					WriteChar(nX, nY, "¨ˆ", FG_LIGHTBLUE);
					break;
				case SIGN_GRASS:
					WriteChar(nX, nY, "¨ˆ", FG_GREEN);
					break;
				}

				g_Map[nY][nX] = g_eNodeType;
			}
			else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED) { //°´ÏÂÊó±êÓÒ¼ü
				g_Map[nY][nX] = SIGN_EMPTY;
				WriteChar(nX, nY, "  ", 0);
			}
		}

		break;
	}
}

//±à¼­µØÍ¼
int DrawMapResult(void)
{
	int bSave = 0;

	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD stcRecord = { 0 };
	DWORD dwRead;
	SetConsoleMode(hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

	WriteChar(42, 1, "¸Ö¡¡Ç½", FG_WHITE);
	WriteChar(46, 1, "¡ö", FG_HIGHWHITE);
	WriteChar(42, 3, "Ë®ÄàÇ½", FG_WHITE);
	WriteChar(46, 3, "¡ö", FG_YELLOW);
	WriteChar(42, 5, "ºÓ¡¡Á÷", FG_WHITE);
	WriteChar(46, 5, "¨ˆ", FG_LIGHTBLUE);
	WriteChar(42, 7, "²Ý¡¡µØ", FG_WHITE);
	WriteChar(46, 7, "¨ˆ", FG_GREEN);

	while (1)
	{
		// µÈ´ýÊÂ¼þ
		ReadConsoleInput(hStdin, &stcRecord, 1, &dwRead);
		// ´¦ÀíÊÂ¼þ
		if (stcRecord.EventType == KEY_EVENT)
		{
			// ¼üÅÌÊÂ¼þ
			int bRet = DrawMapKeyEventProc(stcRecord.Event.KeyEvent);
			if (bRet) {
				WriteChar(42, 1, "      ", FG_WHITE);
				WriteChar(46, 1, "  ", FG_HIGHWHITE);
				WriteChar(42, 3, "      ", FG_WHITE);
				WriteChar(46, 3, "  ", FG_YELLOW);
				WriteChar(42, 5, "      ", FG_WHITE);
				WriteChar(46, 5, "   ", FG_LIGHTBLUE);
				WriteChar(42, 7, "      ", FG_WHITE);
				WriteChar(46, 7, "  ", FG_GREEN);
				
				bSave = (bRet == 1 ? 1 : 0);
				break;
			}
		}
		else if (stcRecord.EventType == MOUSE_EVENT) {
			DrawMapMouseEventProc(stcRecord.Event.MouseEvent);
		}
	}

	return bSave;
}

//Ö÷²Ëµ¥£º±à¼­µØÍ¼
int EditMap(void)
{
	system("cls");

	ClearAllBarrier();
	ShowMap();

	if (DrawMapResult()) {
		InitGameRoles();
		GoRun();
		FreeGameRes();
	}
	else {
		ClearAllBarrier();
		system("cls");
	}

	return 0;
}

//Ö÷²Ëµ¥£ºÍË³öÓÎÏ·
int ExitGame(void)
{
	FreeGameRes();
	return 1;
}


//ÊÍ·ÅÓÎÏ·×ÊÔ´
void FreeGameRes(void)
{
	DestroyObject(&g_pTankA);
	//DestroyObject(&g_pTankB);
	DestroyObject(&g_pEnemies);
	DestroyObject(&g_pBulletBox);
	DestroyObject(&g_pPillbox);
}

//Æô¶¯ÓÎÏ·(±»mainº¯Êýµ÷ÓÃ)
void LaunchGame(void)
{
	//³õÊ¼»¯´°¿Ú
	SetWindowSize("Battle City", 100, 40);

	WelcomeAnim();
	ShowMenu();
}
