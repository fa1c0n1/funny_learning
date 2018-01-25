#include "Controller.h"
#include "Animation.h"
#include "DrawTool.h"
#include "Comm.h"
#include "GameMap.h"
#include "Snake.h"
#include "Food.h"

#include <windows.h>
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <fstream>
#include <exception>

#pragma comment(lib, "winmm.lib")

using namespace std;

Controller::Controller()
	: m_pFood(nullptr), m_pSnake(nullptr), m_pGMap(nullptr), 
	m_bPause(false), m_bExit(false), m_eGLevel(GL_EASY), m_eSubOpt(SMOP_RESTART)
{
}

Controller::~Controller()
{
	this->freeGameRes();
}

void Controller::freeGameRes()
{
	if (this->m_pFood != nullptr) {
		delete this->m_pFood;
		this->m_pFood = nullptr;
	}

	if (this->m_pSnake != nullptr) {
		delete this->m_pSnake;
		this->m_pSnake = nullptr;
	}

	if (this->m_pGMap != nullptr) {
		delete this->m_pGMap;
		this->m_pGMap = nullptr;
	}
}

void Controller::launchGame()
{
	//PlaySoundA("sound/lufeemg.wav", NULL, SND_ASYNC | SND_LOOP);

	this->showAnim();

	while (this->m_eSubOpt == SMOP_RESTART)
	{
		while (this->showMenu());
		this->initGame();
		this->playGame();
		this->freeGameRes();
		DrawTool::SetColor(0);
		system("cls");
	}
}

void Controller::showAnim()
{
	Animation anim;

	DrawTool::SetWindowSize(50, 40);
	DrawTool::SetColor(FG_GREEN);
	anim.welcomeAnim();

	DrawTool::SetCursorPosition(15, 30);
	system("pause");
	DrawTool::SetCursorPosition(15, 30);
	cout << "                                " << endl;
}

bool Controller::showMenu()
{
	DrawTool::SetColor(FG_GREEN);
	DrawTool::SetCursorPosition(7, 27);
	cout << "��ѡ��˵�: " << endl;
	DrawTool::SetCursorPosition(7, 28);
	cout << "<ʹ�����¼�����ѡ�񣬻س�ȷ��>" << endl;

	prnOptionText(this->m_eGLevel);

	while (true)
	{
		if (_kbhit()) {
			int key = _getch();
			if (key == KEY_DIRECTION) {  //�����
				int op = _getch();
				if (op == KEY_UP) { //���ϼ�
					if (this->m_eGLevel == GL_EASY) {
						this->m_eGLevel = GL_RECORD;
					}
					else if (this->m_eGLevel == GL_NORMAL) {
						this->m_eGLevel = GL_EASY;
					}
					else if (this->m_eGLevel == GL_HARD) {
						this->m_eGLevel = GL_NORMAL;
					}
					else if (this->m_eGLevel == GL_RECORD) {
						this->m_eGLevel = GL_HARD;
					}
					prnOptionText(this->m_eGLevel);
				}
				else if (op == KEY_DOWN) {
					if (this->m_eGLevel == GL_RECORD) {
						this->m_eGLevel = GL_EASY;
					}
					else if (this->m_eGLevel == GL_EASY) {
						this->m_eGLevel = GL_NORMAL;
					}
					else if (this->m_eGLevel == GL_NORMAL) {
						this->m_eGLevel = GL_HARD;
					}
					else if (this->m_eGLevel == GL_HARD) {
						this->m_eGLevel = GL_RECORD;
					}
					prnOptionText(this->m_eGLevel);
				}
			}
			else if (key == KEY_ENTER) { //�س���
				break;
			}
		}
		else {
			Sleep(50);
		}
	}

	if (this->m_eGLevel == GL_RECORD) {
		//��ȡ�浵
		return !loadRecord();
	}
	else {
		////// ѯ���û��Ƿ��Զ����ͼ /////
		MapOption mapOp = customMapPrompt();
		switch (mapOp)
		{
		case MAPCUST_RET:
			return true;
		case MAPCUST_YES:
			gotoEditYourMap();
			return false;
		case MAPCUST_NO:
			return false;
		}
	}
}

bool Controller::loadRecord()
{
	ifstream infile(RECORD_FILE, ios::in | ios::binary);

	if (!infile) { //ifstream������ ! ���������������ﲻ���ж��Ƿ�Ϊ��
		showLoadErrMsg();
		return false;
	}

	//1. ��ȡ��Ϸ����
	infile.read((char *)&this->m_eGLevel, sizeof(GameLevel));

	//2. ��ȡ Snake (��������ֵ, ����, ����, ����)
	int nTLifeVal = 0,  nTScore = 0, nTBodyLen = 0;
	Direction eTDrt = DRT_UP;

	infile.read((char *)&nTLifeVal, sizeof(int));
	infile.read((char *)&nTScore, sizeof(int));
	infile.read((char *)&nTBodyLen, sizeof(int));
	infile.read((char *)&eTDrt, sizeof(Direction));
	this->m_pSnake = new Snake(false);
	this->m_pSnake->setLifeVal(nTLifeVal);
	this->m_pSnake->setScore(nTScore);
	this->m_pSnake->setDirection(eTDrt);

	for (int i = 0; i < nTBodyLen; i++) {
		Point tPoint(0, 0);
		infile.read((char *)&tPoint, sizeof(Point));
		this->m_pSnake->addBody(tPoint);
	}

	// 3. ��ȡʳ��
	this->m_pFood = new Food;
	infile.read((char *)this->m_pFood, sizeof(Food));

	// 4. ��ȡ��ͼ(����ֻ���ϰ���)

	int nTNum1 = 0, nTNum2 = 0;
	// ��ȡĬ���ϰ���Ŀ���
	infile.read((char *)&nTNum1, sizeof(int));

	DrawTool::SetCursorPosition(15, 35);
	cout << "load: nTNum1=" << nTNum1 << endl;
	system("pause");

	// ��ȡ�Զ����ϰ���Ŀ���
	infile.read((char *)&nTNum2, sizeof(int));

	Barrier *pBarrier = new Barrier;

	for (int i = 0; i < nTNum1; i++) {
		Point tPoint(0, 0);
		infile.read((char *)&tPoint, sizeof(Point));
		pBarrier->addBarrier(tPoint);
	}

	for (int i = 0; i < nTNum2; i++) {
		Point tPoint(0, 0);
		infile.read((char *)&tPoint, sizeof(Point));
		pBarrier->addCustomBarrier(tPoint);
	}

	this->m_pGMap = new GameMap(this->m_eGLevel);
	this->m_pGMap->setBarrier(pBarrier);

	//��ȡ��ϣ��ر��ļ�
	infile.close();

	return true;
}

void Controller::saveRecord()
{
	ofstream outfile(RECORD_FILE, ios::out | ios::binary);

	//1. ������Ϸ����
	outfile.write((const char *)&this->m_eGLevel, sizeof(GameLevel));

	//2. ���� Snake (��������ֵ������, ����, ����)
	int nTLifeVal = this->m_pSnake->getLifeVal();
	int nTScore = this->m_pSnake->getScore();
	int nTBodyLen = this->m_pSnake->getSnakeLen();
	Direction eTDrt = this->m_pSnake->getDirection();
	outfile.write((const char *)&nTLifeVal, sizeof(int));
	outfile.write((const char *)&nTScore, sizeof(int));
	outfile.write((const char *)&nTBodyLen, sizeof(int));
	outfile.write((const char *)&eTDrt, sizeof(Direction));

	deque<Point> dqSnake = this->m_pSnake->getSnake();
	for (Point &point : dqSnake) {
		outfile.write((const char *)&point, sizeof(Point));
	}

	//3. ����ʳ��
	outfile.write((const char *)this->m_pFood, sizeof(Food));
	
	//4. �����ͼ(ֻ�豣���ϰ���, ��Ϊǽ�ǹ̶�����ģ��ʲ��ñ���ǽ)

	Barrier *pBarrier = this->m_pGMap->getBarrier();

	// ����Ĭ���ϰ���Ŀ���
	int nTNum1 = pBarrier->getBarrierLength();
	outfile.write((const char *)&nTNum1, sizeof(int));

	DrawTool::SetCursorPosition(15, 35);
	cout << "nTNum1=" << nTNum1 << endl;
	system("pause");

	// �����Զ����ϰ���Ŀ���
	int nTNum2 = pBarrier->getCustomBarrierLength();
	outfile.write((const char *)&nTNum2, sizeof(int));

	// ����Ĭ�ϵ��ϰ���
	vector<Point> vtBarriers = pBarrier->getBarriers();
	for (Point &point : vtBarriers) {
		outfile.write((const char *)&point, sizeof(Point));
	}

	// �����Զ�����ϰ���
	set<Point, PointLess> barrierSet = pBarrier->getCustomBarriers();
	for (Point point : barrierSet) {
		outfile.write((const char *)&point, sizeof(Point));
	}

	outfile.close();
	showSaveMsg();
}

void Controller::showLoadErrMsg()
{
	DrawTool::SetColor(FG_LIGHTGREEN);
	DrawTool::SetCursorPosition(7, 30);
	cout << "���������� ���� ����������" << endl;
	DrawTool::SetCursorPosition(7, 31);
	cout << "��                      ��" << endl;
	DrawTool::SetCursorPosition(7, 32);
	cout << "��                      ��" << endl;
	DrawTool::SetCursorPosition(7, 33);
	cout << "��______________________��" << endl;
	DrawTool::SetCursorPosition(8, 32);
	cout << "�޴浵��浵�ļ�����" << endl;

	Sleep(1000);

	DrawTool::SetColor(0);
	DrawTool::SetCursorPosition(7, 30);
	cout << "                         " << endl;
	DrawTool::SetCursorPosition(7, 31);
	cout << "                         " << endl;
	DrawTool::SetCursorPosition(7, 32);
	cout << "                         " << endl;
	DrawTool::SetCursorPosition(7, 33);
	cout << "                         " << endl;
}

void Controller::showSaveMsg()
{
	DrawTool::SetColor(FG_LIGHTTURQUOISE);
	DrawTool::SetCursorPosition(7, 34);
	cout << "���������� ��ʾ ����������" << endl;
	DrawTool::SetCursorPosition(7, 35);
	cout << "��                      ��" << endl;
	DrawTool::SetCursorPosition(7, 36);
	cout << "��                      ��" << endl;
	DrawTool::SetCursorPosition(7, 37);
	cout << "��______________________��" << endl;
	DrawTool::SetCursorPosition(8, 36);
	cout << "       ����ɹ�    " << endl;

	Sleep(1000);

	DrawTool::SetColor(0);
	DrawTool::SetCursorPosition(7, 34);
	cout << "                          " << endl;
	DrawTool::SetCursorPosition(7, 35);
	cout << "                         " << endl;
	DrawTool::SetCursorPosition(7, 36);
	cout << "                         " << endl;
	DrawTool::SetCursorPosition(7, 37);
	cout << "                         " << endl;
	DrawTool::SetCursorPosition(8, 36);
	cout << "                " << endl;
}

MapOption Controller::customMapPrompt()  //�Ӳ˵�����ѡ��ʹ�÷�������ֵ����������ѡ���Ƿ�ȡ���Ӳ˵�
{
	MapOption mapOp = MAPCUST_NO;

	DrawTool::SetColor(FG_LIGHTGREEN);
	DrawTool::SetCursorPosition(7, 30);
	cout << "���� �Ƿ��Զ����ͼ ������" << endl;
	DrawTool::SetCursorPosition(7, 31);
	cout << "��                      ��" << endl;
	DrawTool::SetCursorPosition(7, 32);
	cout << "��                      ��" << endl;
	DrawTool::SetCursorPosition(7, 33);
	cout << "��______________________��" << endl;

	prnCustomMapOptionText(mapOp);

	while (true)
	{
		if (_kbhit()) {
			int key = _getch();
			if (key == KEY_DIRECTION) {
				int op = _getch();
				if (op == KEY_LEFT) {
					if (mapOp == MAPCUST_YES) {
						mapOp = MAPCUST_NO;
					}
					else if (mapOp == MAPCUST_NO) {
						mapOp = MAPCUST_YES;
					}
					prnCustomMapOptionText(mapOp);
				}
				else if (op == KEY_RIGHT) {
					if (mapOp == MAPCUST_YES) {
						mapOp = MAPCUST_NO;
					}
					else if (mapOp == MAPCUST_NO) {
						mapOp = MAPCUST_YES;
					}
					prnCustomMapOptionText(mapOp);
				}
			}
			else if (key == KEY_ENTER) {
				DrawTool::SetColor(FG_LIGHTTURQUOISE);
				break;
			}
			else if (key == KEY_EXIT) {
				mapOp = MAPCUST_RET;
				prnCustomMapOptionText(mapOp);
				break;
			}
		}
		else {
			Sleep(50);
		}
	}

	return mapOp;
}

void Controller::prnCustomMapOptionText(MapOption eMapOp)
{
	DrawTool::SetColor(FG_LIGHTGREEN);
	DrawTool::SetCursorPosition(15, 32);
	cout << "��" << endl;
	DrawTool::SetCursorPosition(10, 32);
	cout << "��" << endl;

	DrawTool::SetBgColor();
	switch (eMapOp)
	{
	case MAPCUST_NO:
		DrawTool::SetCursorPosition(15, 32);
		cout << "��" << endl;
		break;
	case MAPCUST_YES:
		DrawTool::SetCursorPosition(10, 32);
		cout << "��" << endl;
		break;
	case MAPCUST_RET:
		DrawTool::SetColor(0);
		DrawTool::SetCursorPosition(7, 30);
		cout << "                          " << endl;
		DrawTool::SetCursorPosition(7, 31);
		cout << "                          " << endl;
		DrawTool::SetCursorPosition(7, 32);
		cout << "                          " << endl;
		DrawTool::SetCursorPosition(7, 33);
		cout << "                          " << endl;
		break;
	}
}

void Controller::mouseEventProc(MOUSE_EVENT_RECORD mer)
{
	DrawTool::SetColor(FG_GRAY);
	Barrier *pBarrier = this->m_pGMap->getBarrier();

	switch (mer.dwEventFlags)
	{
	case 0:
		int nX = mer.dwMousePosition.X / 2;
		int nY = mer.dwMousePosition.Y;

		if (nX >= 2 && nX <= 31 && nY >= 2 && nY <= 31) {
			if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
				pBarrier->addCustomBarrier(Point(nX, nY));
				DrawTool::SetCursorPosition(nX, nY);
				cout << "��";
			}
			else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED) {
				pBarrier->delCustomBarrier(Point(nX, nY));
				DrawTool::SetCursorPosition(nX, nY);
				cout << "  ";
			}

			DrawTool::SetCursorPosition(8, 35);
			cout << "�ϰ��������" << pBarrier->getCustomBarrierLength() << endl;
		}

		break;
	}
}

bool Controller::keyEventProc(KEY_EVENT_RECORD ker)
{
	if (ker.bKeyDown) { //���̰���
		if (ker.uChar.AsciiChar == 's') {
			return true;
		}
		else if (ker.uChar.AsciiChar == 'q') {  
			Barrier *pBarrier = this->m_pGMap->getBarrier();
			pBarrier->clearCustomAll();
			return true;
		}
	}
	else {  //���̵���

	}

	return false;
}

void Controller::gotoEditYourMap()
{
	this->m_pGMap = new GameMap(this->m_eGLevel);
	this->m_pGMap->prnOnlyWall();

	//------ ��ʾ������ʾ -------------------------
	DrawTool::SetCursorPosition(34, 9);
	cout << "���������� ������ʾ ����������" << endl;
	DrawTool::SetCursorPosition(34, 10);
	cout << "��                           ��" << endl;
	DrawTool::SetCursorPosition(34, 11);
	cout << "�� 1. ��굥����������ϰ��� ��" << endl;
	DrawTool::SetCursorPosition(34, 12);
	cout << "��                           ��" << endl;
	DrawTool::SetCursorPosition(34, 13);
	cout << "�� 2. ��굥���Ҽ�ȡ���ϰ��� ��" << endl;
	DrawTool::SetCursorPosition(34, 14);
	cout << "��                           ��" << endl;
	DrawTool::SetCursorPosition(34, 15);
	cout << "�� 3. ����ĸs����ɲ���ʼ��Ϸ��" << endl;
	DrawTool::SetCursorPosition(34, 16);
	cout << "��                           ��" << endl;
	DrawTool::SetCursorPosition(34, 17);
	cout << "�� 4. ����ĸq��ȡ������ʼ��Ϸ��" << endl;
	DrawTool::SetCursorPosition(34, 18);
	cout << "��___________________________��" << endl;
	//--------------------------------------------

	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	INPUT_RECORD stcRecord = { 0 };
	DWORD dwRead;
	SetConsoleMode(hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

	while (true)
	{
		// �ȴ��¼�
		ReadConsoleInput(hStdin, &stcRecord, 1, &dwRead);
		if (stcRecord.EventType == MOUSE_EVENT) {
			mouseEventProc(stcRecord.Event.MouseEvent);
		}
		else if (stcRecord.EventType == KEY_EVENT) {
			if (keyEventProc(stcRecord.Event.KeyEvent))
				break;
		}
	}
}

void Controller::prnOptionText(GameLevel glevel)
{
	DrawTool::SetColor(FG_GREEN);
	DrawTool::SetCursorPosition(30, 28);
	cout << "��ģʽ" << endl;
	DrawTool::SetCursorPosition(30, 30);
	cout << "��ͨģʽ" << endl;
	DrawTool::SetCursorPosition(30, 32);
	cout << "����ģʽ" << endl;
	DrawTool::SetCursorPosition(30, 34);
	cout << "��ȡ�浵" << endl;

	switch (glevel)
	{
	case GL_EASY:
		DrawTool::SetBgColor();
		DrawTool::SetCursorPosition(30, 28);
		cout << "��ģʽ" << endl;
		break;
	case GL_NORMAL:
		DrawTool::SetBgColor();
		DrawTool::SetCursorPosition(30, 30);
		cout << "��ͨģʽ" << endl;
		break;
	case GL_HARD:
		DrawTool::SetBgColor();
		DrawTool::SetCursorPosition(30, 32);
		cout << "����ģʽ" << endl;
		break;
	case GL_RECORD:
		DrawTool::SetBgColor();
		DrawTool::SetCursorPosition(30, 34);
		cout << "��ȡ�浵" << endl;
		break;
	}
}

void Controller::initGame()
{
	this->m_bExit = this->m_bPause = false;

	if (this->m_pGMap == nullptr)
		this->m_pGMap = new GameMap(this->m_eGLevel);

	this->m_pGMap->prnMap();
	drawInfoPanel();
}

void Controller::drawInfoPanel()
{
	DrawTool::SetColor(FG_LIGHTTURQUOISE);
	DrawTool::SetCursorPosition(39, 1);
	cout << "��Ϸ��Ϣ" << endl;
	
	drawGameLevelInfo(this->m_eGLevel);
	drawSnakeInfo(*this->m_pSnake);

	DrawTool::SetCursorPosition(34, 9);
	cout << "���������� ������ʾ ����������" << endl;
	DrawTool::SetCursorPosition(34, 10);
	cout << "��                          ��" << endl;
	DrawTool::SetCursorPosition(34, 12);
	cout << "��                          ��" << endl;
	DrawTool::SetCursorPosition(34, 11);
	cout << "��ʹ�����������ĸ�������ƶ���" << endl;
	DrawTool::SetCursorPosition(34, 13);
	cout << "��    �� Esc ����ͣ��Ϸ     ��" << endl;
	DrawTool::SetCursorPosition(34, 14);
	cout << "��                          ��" << endl;
	DrawTool::SetCursorPosition(34, 15);
	cout << "��__________________________��" << endl;
}

void Controller::drawGameLevelInfo(GameLevel glevel)
{
	DrawTool::SetColor(FG_LIGHTTURQUOISE);
	DrawTool::SetCursorPosition(37, 3);
	cout << "�Ѷȼ���";

	switch (glevel)
	{
	case GL_EASY:
		cout << "   �� ��";
		break;
	case GL_NORMAL:
		cout << "   �� ͨ";
		break;
	case GL_HARD:
		cout << "   �� ��";
		break;
	}
}

void Controller::drawSnakeInfo(Snake &snake)
{
	DrawTool::SetColor(FG_LIGHTTURQUOISE);
	DrawTool::SetCursorPosition(37, 5);
	cout << "��    �֣�";

	if (&snake == nullptr)
		cout << setw(8) << 0 << endl;
	else
		cout << setw(8) << snake.getScore() << endl;

	DrawTool::SetCursorPosition(37, 7);
	cout << "�� �� ֵ��";
	if (&snake == nullptr)
		cout << setw(8) << SNAKE_INIT_LIFE << endl;
	else
		cout << setw(8) << snake.getLifeVal() << endl;
}

void Controller::playGame()
{
	if (this->m_pSnake == nullptr)
		this->m_pSnake = new Snake;
	this->m_pSnake->show();

	if (this->m_pFood == nullptr)
		this->m_pFood = new Food;
	this->m_pFood->show(*this->m_pSnake, *this->m_pGMap);

	while (!this->m_pSnake->m_bDead && !this->m_bExit)
	{
		if (_kbhit()) {
			int key = _getch();
			if (key == KEY_DIRECTION) {
				int op = _getch();
				if (op == KEY_UP) {
					if (this->m_pSnake->getDirection() == DRT_LEFT || this->m_pSnake->getDirection() == DRT_RIGHT)
						this->m_pSnake->setDirection(DRT_UP);
				}
				else if (op == KEY_DOWN) {
					if (this->m_pSnake->getDirection() == DRT_LEFT || this->m_pSnake->getDirection() == DRT_RIGHT)
						this->m_pSnake->setDirection(DRT_DOWN);
				}
				else if (op == KEY_LEFT) {
					if (this->m_pSnake->getDirection() == DRT_UP || this->m_pSnake->getDirection() == DRT_DOWN)
						this->m_pSnake->setDirection(DRT_LEFT);
				}
				else if (op == KEY_RIGHT) {
					if (this->m_pSnake->getDirection() == DRT_UP || this->m_pSnake->getDirection() == DRT_DOWN)
						this->m_pSnake->setDirection(DRT_RIGHT);
				}
			}
			else if (key == KEY_EXIT) {
				this->m_bPause = true;
				while (this->m_bPause) {
					// ��ʾ�Ӳ˵�
					showSubMenu();

					if (this->m_eSubOpt == SMOP_RESTART || this->m_eSubOpt == SMOP_QUIT) {
						this->m_bExit = true;
					}
					else if (this->m_eSubOpt == SMOP_SAVE) {
						saveRecord();
						this->m_bPause = true;
						continue;
					}
					this->m_bPause = false;
					eraseSubMenu();
				}
			}
		}

		if (this->m_bExit)
			break;

		this->m_pSnake->move();
		if (this->m_pSnake->eatFood(*this->m_pFood)) {
			drawSnakeInfo(*this->m_pSnake);
			delete this->m_pFood;
			this->m_pFood = nullptr;
			this->m_pFood = new Food;
			this->m_pFood->show(*this->m_pSnake, *this->m_pGMap);
		}

		if (this->m_pSnake->checkCrashBarrier(*this->m_pGMap))
			drawSnakeInfo(*this->m_pSnake);

		this->m_pSnake->m_bDead = this->m_pSnake->getLifeVal() <= 0 ? true : false;
		if (this->m_pSnake->m_bDead)
			goto DEAD;
		this->m_pSnake->m_bDead = this->m_pSnake->checkCrashWall();

DEAD:
		if (this->m_pSnake->m_bDead) {
			drawSnakeInfo(*this->m_pSnake);
			showGameOverPrompt(*this->m_pSnake);
			break;
		}

		Sleep((4 - (int)this->m_eGLevel) * 50);
	}
}

void Controller::showSubMenu()
{
	prnSubOptionText(this->m_eSubOpt);

	while (true)
	{
		if (_kbhit()) {
			int key = _getch();
			if (key == KEY_DIRECTION) { //���·����
				int op = _getch();
				if (op == KEY_UP) { //���ϼ�
					if (this->m_eSubOpt == SMOP_CONTINUE) {
						this->m_eSubOpt = SMOP_QUIT;
					}
					else if (this->m_eSubOpt == SMOP_RESTART) {
						this->m_eSubOpt = SMOP_CONTINUE;
					}
					else if (this->m_eSubOpt == SMOP_SAVE) {
						this->m_eSubOpt = SMOP_RESTART;
					}
					else if (this->m_eSubOpt == SMOP_QUIT) {
						this->m_eSubOpt = SMOP_SAVE;
					}
					prnSubOptionText(this->m_eSubOpt);
				}
				else if (op == KEY_DOWN) { //���¼�
					if (this->m_eSubOpt == SMOP_CONTINUE) {
						this->m_eSubOpt = SMOP_RESTART;
					}
					else if (this->m_eSubOpt == SMOP_RESTART) {
						this->m_eSubOpt = SMOP_SAVE;
					}
					else if (this->m_eSubOpt == SMOP_SAVE) {
						this->m_eSubOpt = SMOP_QUIT;
					}
					else if (this->m_eSubOpt == SMOP_QUIT) {
						this->m_eSubOpt = SMOP_CONTINUE;
					}
					prnSubOptionText(this->m_eSubOpt);
				}
			}
			else if (key == KEY_ENTER) {
				break;
			}
		}
		else {
			Sleep(50);
		}
			
	}
}

void Controller::eraseSubMenu()
{
	DrawTool::SetColor(0);
	DrawTool::SetCursorPosition(39, 23);
	cout << "         " << endl;
	DrawTool::SetCursorPosition(39, 25);
	cout << "         " << endl;
	DrawTool::SetCursorPosition(39, 27);
	cout << "         " << endl;
	DrawTool::SetCursorPosition(39, 29);
	cout << "         " << endl;
}

void Controller::prnSubOptionText(SubMenuOption opt)
{
	DrawTool::SetColor(FG_LIGHTGREEN);
	DrawTool::SetCursorPosition(39, 23);
	cout << "������Ϸ" << endl;

	DrawTool::SetCursorPosition(39, 25);
	cout << "���¿�ʼ" << endl;

	DrawTool::SetCursorPosition(39, 27);
	cout << "������Ϸ" << endl;

	DrawTool::SetCursorPosition(39, 29);
	cout << "�˳���Ϸ" << endl;

	switch (opt)
	{
	case SMOP_CONTINUE:
		DrawTool::SetBgColor();
		DrawTool::SetCursorPosition(39, 23);
		cout << "������Ϸ" << endl;
		break;
	case SMOP_RESTART:
		DrawTool::SetBgColor();
		DrawTool::SetCursorPosition(39, 25);
		cout << "���¿�ʼ" << endl;
		break;
	case SMOP_SAVE:
		DrawTool::SetBgColor();
		DrawTool::SetCursorPosition(39, 27);
		cout << "������Ϸ" << endl;
		break;
	case SMOP_QUIT:
		DrawTool::SetBgColor();
		DrawTool::SetCursorPosition(39, 29);
		cout << "�˳���Ϸ" << endl;
		break;
	}
}

void Controller::showGameOverPrompt(Snake &snake)
{
	DrawTool::SetColor(FG_LIGHTRED);
	DrawTool::SetCursorPosition(7, 7);
	cout << "��������������������������������������������";
	DrawTool::SetCursorPosition(7, 8);
	cout << "��               GAME OVER                ��";
	DrawTool::SetCursorPosition(7, 9);
	cout << "��                                        ��";
	DrawTool::SetCursorPosition(7, 10);
	cout << "��       ʤ���˱��ҳ���, ���ͷ������     ��";
	DrawTool::SetCursorPosition(7, 11);
	cout << "��                                        ��";
	DrawTool::SetCursorPosition(7, 12);
	cout << "��          ��ķ���Ϊ�� " << setw(8) << snake.getScore() << "         ��";
	DrawTool::SetCursorPosition(7, 13);
	cout << "��                                        ��";
	DrawTool::SetCursorPosition(7, 14);
	cout << "��     �Ƿ��ٴ���ս��                     ��";
	DrawTool::SetCursorPosition(7, 15);
	cout << "��                                        ��";
	DrawTool::SetCursorPosition(7, 16);
	cout << "��                                        ��";
	DrawTool::SetCursorPosition(7, 17);
	cout << "��                                        ��";
	DrawTool::SetCursorPosition(7, 18);
	cout << "��                                        ��";
	DrawTool::SetCursorPosition(7, 19);
	cout << "��                                        ��";
	DrawTool::SetCursorPosition(7, 20);
	cout << "��������������������������������������������";

	this->m_eSubOpt = SMOP_RESTART;
	prnGameOverOptionText(this->m_eSubOpt);
	
	while (true)
	{
		if (_kbhit()) {
			int key = _getch();
			if (key == KEY_DIRECTION) {
				int op = _getch();
				if (op == KEY_LEFT) {
					if (this->m_eSubOpt == SMOP_QUIT) {
						this->m_eSubOpt = SMOP_RESTART;
					}
					else if (this->m_eSubOpt == SMOP_RESTART) {
						this->m_eSubOpt = SMOP_QUIT;
					}
				}
				else if (op == KEY_RIGHT) {
					if (this->m_eSubOpt == SMOP_RESTART) {
						this->m_eSubOpt = SMOP_QUIT;
					}
					else if (this->m_eSubOpt == SMOP_QUIT) {
						this->m_eSubOpt = SMOP_RESTART;
					}
				}
				prnGameOverOptionText(this->m_eSubOpt);
			}
			else if (key == KEY_ENTER) {
				break;
			}
		}
		else {
			Sleep(50);
		}
	}
}

void Controller::prnGameOverOptionText(SubMenuOption opt)
{
	DrawTool::SetColor(FG_LIGHTRED);
	DrawTool::SetCursorPosition(11, 17);
	cout << "�ǣ�������ս" << endl;
	DrawTool::SetCursorPosition(19, 17);
	cout << "���˳���Ϸ" << endl;

	switch (opt)
	{
	case SMOP_RESTART:
		DrawTool::SetBgColor();
		DrawTool::SetCursorPosition(11, 17);
		cout << "�ǣ�������ս" << endl;
		break;
	case SMOP_QUIT:
		DrawTool::SetBgColor();
		DrawTool::SetCursorPosition(19, 17);
		cout << "���˳���Ϸ" << endl;
		break;
	}
}

