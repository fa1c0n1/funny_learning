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
	this->showAnim();

	while (this->m_eSubOpt == SMOP_RESTART)
	{
		this->showMenu();
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

void Controller::showMenu()
{
	DrawTool::SetColor(FG_GREEN);
	DrawTool::SetCursorPosition(7, 27);
	cout << "请选择游戏难度: " << endl;
	DrawTool::SetCursorPosition(7, 28);
	cout << "<使用上下键进行选择，回车确认>" << endl;

	this->m_eGLevel = GL_EASY;
	prnOptionText(this->m_eGLevel);

	while (true)
	{
		if (_kbhit()) {
			int key = _getch();
			if (key == KEY_DIRECTION) {  //方向键
				int op = _getch();
				if (op == KEY_UP) { //按上键
					if (this->m_eGLevel == GL_EASY)
						continue;
					else if (this->m_eGLevel == GL_NORMAL) {
						this->m_eGLevel = GL_EASY;
						prnOptionText(GL_EASY);
					}
					else if (this->m_eGLevel == GL_HARD) {
						this->m_eGLevel = GL_NORMAL;
						prnOptionText(GL_NORMAL);
					}
				}
				else if (op == KEY_DOWN) {
					if (this->m_eGLevel == GL_HARD)
						continue;
					else if (this->m_eGLevel == GL_EASY) {
						this->m_eGLevel = GL_NORMAL;
						prnOptionText(GL_NORMAL);
					}
					else if (this->m_eGLevel == GL_NORMAL) {
						this->m_eGLevel = GL_HARD;
						prnOptionText(GL_HARD);
					}
				}
			}
			else if (key == KEY_ENTER) { //回车键
				DrawTool::SetColor(FG_LIGHTTURQUOISE);
				system("cls");
				break;
			}
		}
		else {
			Sleep(50);
		}
	}
}


void Controller::prnOptionText(GameLevel glevel)
{
	DrawTool::SetColor(FG_GREEN);
	DrawTool::SetCursorPosition(30, 28);
	cout << "简单模式" << endl;
	DrawTool::SetCursorPosition(30, 30);
	cout << "普通模式" << endl;
	DrawTool::SetCursorPosition(30, 32);
	cout << "困难模式" << endl;

	switch (glevel)
	{
	case GL_EASY:
		DrawTool::SetBgColor();
		DrawTool::SetCursorPosition(30, 28);
		cout << "简单模式" << endl;
		break;
	case GL_NORMAL:
		DrawTool::SetBgColor();
		DrawTool::SetCursorPosition(30, 30);
		cout << "普通模式" << endl;
		break;
	case GL_HARD:
		DrawTool::SetBgColor();
		DrawTool::SetCursorPosition(30, 32);
		cout << "困难模式" << endl;
		break;
	}
}

void Controller::initGame()
{
	this->m_bExit = this->m_bPause = false;

	this->m_pGMap = new GameMap(this->m_eGLevel);
	this->m_pGMap->prnMap();

	drawInfoPanel();
}

void Controller::drawInfoPanel()
{
	DrawTool::SetColor(FG_LIGHTTURQUOISE);
	DrawTool::SetCursorPosition(39, 1);
	cout << "游戏信息" << endl;
	
	drawGameLevelInfo(this->m_eGLevel);
	drawScoreInfo(*this->m_pSnake);

	DrawTool::SetCursorPosition(34, 8);
	cout << "————— 操作提示 —————" << endl;
	DrawTool::SetCursorPosition(34, 9);
	cout << "┃                          ┃" << endl;
	DrawTool::SetCursorPosition(34, 10);
	cout << "┃使用上下左右四个方向键移动┃" << endl;
	DrawTool::SetCursorPosition(34, 12);
	cout << "┃    按 Esc 键暂停游戏     ┃" << endl;
	DrawTool::SetCursorPosition(34, 11);
	cout << "┃                          ┃" << endl;
	DrawTool::SetCursorPosition(34, 13);
	cout << "┃__________________________┃" << endl;
}

void Controller::drawGameLevelInfo(GameLevel glevel)
{
	DrawTool::SetColor(FG_LIGHTTURQUOISE);
	DrawTool::SetCursorPosition(37, 3);
	cout << "难度级别：";

	switch (glevel)
	{
	case GL_EASY:
		cout << "   简 单";
		break;
	case GL_NORMAL:
		cout << "   普 通";
		break;
	case GL_HARD:
		cout << "   困 难";
		break;
	}
}

void Controller::drawScoreInfo(Snake &snake)
{
	DrawTool::SetColor(FG_LIGHTTURQUOISE);
	DrawTool::SetCursorPosition(37, 5);
	cout << "得    分：";

	if (&snake == nullptr)
		cout << setw(8) << 0 << endl;
	else
		cout << setw(8) << snake.getScore() << endl;
}

void Controller::playGame()
{
	this->m_pSnake = new Snake;
	this->m_pSnake->show();

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
					// 显示子菜单
					showSubMenu();

					if (this->m_eSubOpt == SMOP_RESTART || this->m_eSubOpt == SMOP_QUIT) {
						this->m_bExit = true;
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
			drawScoreInfo(*this->m_pSnake);
			delete this->m_pFood;
			this->m_pFood = nullptr;
			this->m_pFood = new Food;
			this->m_pFood->show(*this->m_pSnake, *this->m_pGMap);
		}

		this->m_pSnake->m_bDead = this->m_pSnake->checkCrashWall(*this->m_pGMap);
		if (this->m_pSnake->m_bDead) {
			showGameOverPrompt(*this->m_pSnake);
			break;
		}

		Sleep((4 - (int)this->m_eGLevel) * 50);
	}
}

void Controller::showSubMenu()
{
	this->m_eSubOpt = SMOP_CONTINUE;
	prnSubOptionText(this->m_eSubOpt);

	while (true)
	{
		if (_kbhit()) {
			int key = _getch();
			if (key == KEY_DIRECTION) { //按下方向键
				int op = _getch();
				if (op == KEY_UP) { //按上键
					if (this->m_eSubOpt == SMOP_RESTART) {
						this->m_eSubOpt = SMOP_CONTINUE;
						prnSubOptionText(SMOP_CONTINUE);
					}
					else if (this->m_eSubOpt == SMOP_QUIT) {
						this->m_eSubOpt = SMOP_RESTART;
						prnSubOptionText(SMOP_RESTART);
					}
				}
				else if (op == KEY_DOWN) { //按下键
					if (this->m_eSubOpt == SMOP_CONTINUE) {
						this->m_eSubOpt = SMOP_RESTART;
						prnSubOptionText(SMOP_RESTART);
					}
					else if (this->m_eSubOpt == SMOP_RESTART) {
						this->m_eSubOpt = SMOP_QUIT;
						prnSubOptionText(SMOP_QUIT);
					}
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
	DrawTool::SetCursorPosition(39, 25);
	cout << "继续游戏" << endl;

	DrawTool::SetCursorPosition(39, 27);
	cout << "重新开始" << endl;

	DrawTool::SetCursorPosition(39, 29);
	cout << "退出游戏" << endl;

	switch (opt)
	{
	case SMOP_CONTINUE:
		DrawTool::SetBgColor();
		DrawTool::SetCursorPosition(39, 25);
		cout << "继续游戏" << endl;
		break;
	case SMOP_RESTART:
		DrawTool::SetBgColor();
		DrawTool::SetCursorPosition(39, 27);
		cout << "重新开始" << endl;
		break;
	case SMOP_QUIT:
		DrawTool::SetBgColor();
		DrawTool::SetCursorPosition(39, 29);
		cout << "退出游戏" << endl;
		break;
	}
}

void Controller::showGameOverPrompt(Snake &snake)
{
	DrawTool::SetColor(FG_LIGHTRED);
	DrawTool::SetCursorPosition(7, 7);
	cout << "━━━━━━━━━━━━━━━━━━━━━━";
	DrawTool::SetCursorPosition(7, 8);
	cout << "┃               GAME OVER                ┃";
	DrawTool::SetCursorPosition(7, 9);
	cout << "┃                                        ┃";
	DrawTool::SetCursorPosition(7, 10);
	cout << "┃       胜败乃兵家常事, 请从头再来！     ┃";
	DrawTool::SetCursorPosition(7, 11);
	cout << "┃                                        ┃";
	DrawTool::SetCursorPosition(7, 12);
	cout << "┃          你的分数为： " << setw(8) << snake.getScore() << "         ┃";
	DrawTool::SetCursorPosition(7, 13);
	cout << "┃                                        ┃";
	DrawTool::SetCursorPosition(7, 14);
	cout << "┃     是否再次挑战？                     ┃";
	DrawTool::SetCursorPosition(7, 15);
	cout << "┃                                        ┃";
	DrawTool::SetCursorPosition(7, 16);
	cout << "┃                                        ┃";
	DrawTool::SetCursorPosition(7, 17);
	cout << "┃                                        ┃";
	DrawTool::SetCursorPosition(7, 18);
	cout << "┃                                        ┃";
	DrawTool::SetCursorPosition(7, 19);
	cout << "┃                                        ┃";
	DrawTool::SetCursorPosition(7, 20);
	cout << "┃━━━━━━━━━━━━━━━━━━━━┃";

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
						prnGameOverOptionText(SMOP_RESTART);
					}
				}
				else if (op == KEY_RIGHT) {
					if (this->m_eSubOpt == SMOP_RESTART) {
						this->m_eSubOpt = SMOP_QUIT;
						prnGameOverOptionText(SMOP_QUIT);
					}
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

void Controller::prnGameOverOptionText(SubMenuOption opt)
{
	DrawTool::SetColor(FG_LIGHTRED);
	DrawTool::SetCursorPosition(11, 17);
	cout << "是，继续挑战" << endl;
	DrawTool::SetCursorPosition(19, 17);
	cout << "否，退出游戏" << endl;

	switch (opt)
	{
	case SMOP_RESTART:
		DrawTool::SetBgColor();
		DrawTool::SetCursorPosition(11, 17);
		cout << "是，继续挑战" << endl;
		break;
	case SMOP_QUIT:
		DrawTool::SetBgColor();
		DrawTool::SetCursorPosition(19, 17);
		cout << "否，退出游戏" << endl;
		break;
	}
}

