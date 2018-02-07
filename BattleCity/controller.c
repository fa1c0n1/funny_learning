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

//��ʾ���˵�
void ShowMenu(void)
{
	int bExit = 0;

	MenuItem mainMenu[4] = {
		{ KEY_1, StartGame, "1. �� ʼ �� Ϸ" },
		{ KEY_2, LoadGame, "2. �� �� �� Ϸ" },
		{ KEY_3, EditMap, "3. �� �� �� ͼ" },
		{ KEY_4, ExitGame, "4. �� �� �� Ϸ" }
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

//��ʾ�Ӳ˵�
int ShowSubMenu(void)
{
	int nRet = 0;

	MenuItem subMenu[3] = {
		{ KEY_1, GoOnGame, "1.������Ϸ" },
		{ KEY_2, SaveGame, "2.������Ϸ" },
		{ KEY_3, RetMainMenu, "3.��������" },
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

//�Ӳ˵���������Ϸ
int GoOnGame(void)
{
	return 1;
}

//�Ӳ˵���������Ϸ
int SaveGame(void)
{
	//TODO:
	ShowTODO();
	return 2;
}

//�Ӳ˵����������˵�
int RetMainMenu(void)
{
	return 3;
}

//������Ϸ�����߼���ѭ��
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
			if (g_nEnNum <= 0) { //�о���ȫ��
				ShowWinNotice();
				Sleep(3000);
				ClearAllBarrier();
				system("cls");
				goto END;
			}

			//�������
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
					if (nSubRet == 3) {//3��ʾҪ���ص����˵�
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
		else { //��ұ����ﱤ������
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

//��ʾʤ����ʾ
void ShowWinNotice(void)
{

	WriteChar(7, 7, "��������������������������������������������", FG_LIGHTRED);
	WriteChar(7, 8, "��               YOU WIN                  ��", FG_LIGHTRED);
	WriteChar(7, 9, "��                                        ��", FG_LIGHTRED);
	WriteChar(7, 10, "��       ��ϲ�㣡սʤ�˵��ˣ�����         ��", FG_LIGHTRED);
	WriteChar(7, 11, "��                                        ��", FG_LIGHTRED);
	WriteChar(7, 12, "��������������������������������������������", FG_LIGHTRED);
}

//��ʾʧ����ʾ
void ShowFailedNotice(void)
{
	WriteChar(7, 7, "��������������������������������������������", FG_LIGHTRED);
	WriteChar(7, 8, "��               GAME OVER                ��", FG_LIGHTRED);
	WriteChar(7, 9, "��                                        ��", FG_LIGHTRED);
	WriteChar(7, 10, "��       ʤ���˱��ҳ���, ���ͷ������     ��", FG_LIGHTRED);
	WriteChar(7, 11, "��                                        ��", FG_LIGHTRED);
	WriteChar(7, 12, "��������������������������������������������", FG_LIGHTRED);
}

//��ʾ����δ���
void ShowTODO(void)
{
	WriteChar(41, 32, " ������������ ", FG_LIGHTTURQUOISE);
	WriteChar(41, 33, "������δ��ɩ�", FG_LIGHTTURQUOISE);
	WriteChar(41, 34, "��          ��", FG_LIGHTTURQUOISE);
	WriteChar(41, 35, "�� �����ڴ� ��", FG_LIGHTTURQUOISE);
	WriteChar(41, 36, "��__________��", FG_LIGHTTURQUOISE);
	Sleep(1000);
	WriteChar(41, 32, "             ", FG_LIGHTTURQUOISE);
	WriteChar(41, 33, "              ", FG_LIGHTTURQUOISE);
	WriteChar(41, 34, "              ", FG_LIGHTTURQUOISE);
	WriteChar(41, 35, "              ", FG_LIGHTTURQUOISE);
	WriteChar(41, 36, "              ", FG_LIGHTTURQUOISE);
}

//��ʼ����Ϸ��ɫ(������ҡ��о����ӵ�)
void InitGameRoles(void)
{
	g_pPillbox = InitPillbox();
	g_pTankA = CreatePlayer(SIGN_TANK_PA, 12, 36, DRT_UP);
	//g_pTankB = CreatePlayer(SIGN_TANK_PB, 27, 25, DRT_UP);
	g_pBulletBox = InitBulletBox();
	g_pEnemies = InitEnemies();
}

//���˵�����ʼ��Ϸ
int StartGame(void)
{
	system("cls");
	InitGameRoles();
	InitDefaultMap();
	GoRun();
	FreeGameRes();
	return 0;
}

//���˵���������Ϸ
int LoadGame(void)
{
	//TODO:
	ShowTODO();
	return 0;
}

//�༭��ͼʱ�ļ��̼�⴦��
int DrawMapKeyEventProc(KEY_EVENT_RECORD ker)
{
	int bRet = 0;

	if (ker.bKeyDown) { //���̰���
		CHAR keyVal = ker.uChar.AsciiChar;
		if (keyVal == 's') {
			bRet = 1;  //����༭����ʼ��Ϸ
		}
		else if (keyVal == 'q') {
			bRet = 2; //�����༭������
		}
	}
	else {  //���̵���

	}

END:
	return bRet;
}

//�༭��ͼʱ������⴦��
void DrawMapMouseEventProc(MOUSE_EVENT_RECORD mer)
{
	int nX = mer.dwMousePosition.X / 2;
	int nY = mer.dwMousePosition.Y;

	switch (mer.dwEventFlags)
	{
	case 0:
		if (nX == 46) {  //����ϰ����л�����
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
			if (nX >= 15 && nX <= 21) { //�ﱤ�����ܱ༭
				if (nY >= 34 && nY <= 38)
					return;
			}

			if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) { //����������
				switch (g_eNodeType)
				{
				case SIGN_WALL0:
					WriteChar(nX, nY, "��", FG_HIGHWHITE);
					break;
				case SIGN_WALL1:
					WriteChar(nX, nY, "��", FG_YELLOW);
					break;
				case SIGN_RIVER:
					WriteChar(nX, nY, "��", FG_LIGHTBLUE);
					break;
				case SIGN_GRASS:
					WriteChar(nX, nY, "��", FG_GREEN);
					break;
				}

				g_Map[nY][nX] = g_eNodeType;
			}
			else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED) { //��������Ҽ�
				g_Map[nY][nX] = SIGN_EMPTY;
				WriteChar(nX, nY, "  ", 0);
			}
		}

		break;
	}
}

//�༭��ͼ
int DrawMapResult(void)
{
	int bSave = 0;

	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD stcRecord = { 0 };
	DWORD dwRead;
	SetConsoleMode(hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

	WriteChar(42, 1, "�֡�ǽ", FG_WHITE);
	WriteChar(46, 1, "��", FG_HIGHWHITE);
	WriteChar(42, 3, "ˮ��ǽ", FG_WHITE);
	WriteChar(46, 3, "��", FG_YELLOW);
	WriteChar(42, 5, "�ӡ���", FG_WHITE);
	WriteChar(46, 5, "��", FG_LIGHTBLUE);
	WriteChar(42, 7, "�ݡ���", FG_WHITE);
	WriteChar(46, 7, "��", FG_GREEN);

	while (1)
	{
		// �ȴ��¼�
		ReadConsoleInput(hStdin, &stcRecord, 1, &dwRead);
		// �����¼�
		if (stcRecord.EventType == KEY_EVENT)
		{
			// �����¼�
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

//���˵����༭��ͼ
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

//���˵����˳���Ϸ
int ExitGame(void)
{
	FreeGameRes();
	return 1;
}


//�ͷ���Ϸ��Դ
void FreeGameRes(void)
{
	DestroyObject(&g_pTankA);
	//DestroyObject(&g_pTankB);
	DestroyObject(&g_pEnemies);
	DestroyObject(&g_pBulletBox);
	DestroyObject(&g_pPillbox);
}

//������Ϸ(��main��������)
void LaunchGame(void)
{
	//��ʼ������
	SetWindowSize("Battle City", 100, 40);

	WelcomeAnim();
	ShowMenu();
}
