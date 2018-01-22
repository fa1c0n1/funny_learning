#include "Controller.h"
#include "Animation.h"
#include "DrawTool.h"
#include "Comm.h"
#include "GameMap.h"
#include "Snake.h"
#include "Food.h"

#include <windows.h>
#include <iostream>
#include <conio.h>

using namespace std;

Controller::Controller()
	: m_pFood(nullptr), m_pSnake(nullptr)
{
	this->m_eGLevel = GL_EASY;
}

Controller::~Controller()
{
	if (this->m_pFood != nullptr) {
		delete this->m_pFood;
		this->m_pFood = nullptr;
	}

	if (this->m_pSnake != nullptr) {
		delete this->m_pSnake;
		this->m_pSnake = nullptr;
	}
}

void Controller::launchGame()
{
	this->showAnim();
	this->showMenu();
	this->initGame();
	this->playGame();
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
	DrawTool::SetCursorPosition(7, 27);
	cout << "请选择游戏难度: " << endl;
	DrawTool::SetCursorPosition(7, 28);
	cout << "<使用上下键进行选择，回车确认>" << endl;

	DrawTool::SetBgColor();
	DrawTool::SetCursorPosition(30, 28);
	cout << "简单模式" << endl;

	DrawTool::SetColor(FG_GREEN);
	DrawTool::SetCursorPosition(30, 30);
	cout << "普通模式" << endl;

	DrawTool::SetCursorPosition(30, 32);
	cout << "困难模式" << endl;

	//DrawTool::SetCursorPosition(30, 34);
	//cout << "退出游戏" << endl;

	bool bExit = false;
	while (!bExit)
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
		} else {
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
	GameMap gMap(this->m_eGLevel);
	gMap.prnMap();
}

void Controller::playGame()
{
	this->m_pSnake = new Snake;
	this->m_pSnake->show();

	this->m_pFood = new Food;
	this->m_pFood->show(*this->m_pSnake);

	while (!this->m_pSnake->m_bDead)
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
		}

		this->m_pSnake->move();
		if (this->m_pSnake->eatFood(*this->m_pFood)) {
			delete this->m_pFood;
			this->m_pFood = nullptr;
			this->m_pFood = new Food;
			this->m_pFood->show(*this->m_pSnake);
		}
		this->m_pSnake->m_bDead = this->m_pSnake->checkCrashWall();
		Sleep(150);
	}
}
