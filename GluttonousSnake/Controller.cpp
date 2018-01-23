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
	: m_pFood(nullptr), m_pSnake(nullptr), m_bPause(false), m_bExit(false),
	m_eGLevel(GL_EASY), m_eSubOpt(SMOP_RESTART)
{
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

	while (this->m_eSubOpt == SMOP_RESTART)
	{
		this->showMenu();
		this->initGame();
		this->playGame();
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
	DrawTool::SetCursorPosition(7, 27);
	cout << "��ѡ����Ϸ�Ѷ�: " << endl;
	DrawTool::SetCursorPosition(7, 28);
	cout << "<ʹ�����¼�����ѡ�񣬻س�ȷ��>" << endl;

	DrawTool::SetBgColor();
	DrawTool::SetCursorPosition(30, 28);
	cout << "��ģʽ" << endl;

	DrawTool::SetColor(FG_GREEN);
	DrawTool::SetCursorPosition(30, 30);
	cout << "��ͨģʽ" << endl;

	DrawTool::SetCursorPosition(30, 32);
	cout << "����ģʽ" << endl;

	//DrawTool::SetCursorPosition(30, 34);
	//cout << "�˳���Ϸ" << endl;

	this->m_bExit = this->m_bPause = false;

	while (!this->m_bExit)
	{
		if (_kbhit()) {
			int key = _getch();
			if (key == KEY_DIRECTION) {  //�����
				int op = _getch();
				if (op == KEY_UP) { //���ϼ�
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
			else if (key == KEY_ENTER) { //�س���
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
	cout << "��ģʽ" << endl;
	DrawTool::SetCursorPosition(30, 30);
	cout << "��ͨģʽ" << endl;
	DrawTool::SetCursorPosition(30, 32);
	cout << "����ģʽ" << endl;

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
	}
}

void Controller::initGame()
{
	GameMap gMap(this->m_eGLevel);
	gMap.prnMap();

	drawInfoPanel();
}

void Controller::drawInfoPanel()
{
	DrawTool::SetColor(FG_LIGHTTURQUOISE);
	DrawTool::SetCursorPosition(39, 1);
	cout << "��Ϸ��Ϣ" << endl;
	
	drawGameLevelInfo(this->m_eGLevel);
	drawScoreInfo(*this->m_pSnake);

	DrawTool::SetCursorPosition(34, 8);
	cout << "���������� ������ʾ ����������" << endl;
	DrawTool::SetCursorPosition(34, 9);
	cout << "��                          ��" << endl;
	DrawTool::SetCursorPosition(34, 10);
	cout << "��ʹ�����������ĸ�������ƶ���" << endl;
	DrawTool::SetCursorPosition(34, 12);
	cout << "��    �� Esc ����ͣ��Ϸ     ��" << endl;
	DrawTool::SetCursorPosition(34, 11);
	cout << "��                          ��" << endl;
	DrawTool::SetCursorPosition(34, 13);
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

void Controller::drawScoreInfo(Snake &snake)
{
	DrawTool::SetColor(FG_LIGHTTURQUOISE);
	DrawTool::SetCursorPosition(37, 5);
	cout << "��    �֣�";

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
	this->m_pFood->show(*this->m_pSnake);

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
			this->m_pFood->show(*this->m_pSnake);
		}
		this->m_pSnake->m_bDead = this->m_pSnake->checkCrashWall();
		Sleep(150);
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
			if (key == KEY_DIRECTION) { //���·����
				int op = _getch();
				if (op == KEY_UP) { //���ϼ�
					if (this->m_eSubOpt == SMOP_RESTART) {
						this->m_eSubOpt = SMOP_CONTINUE;
						prnSubOptionText(SMOP_CONTINUE);
					}
					else if (this->m_eSubOpt == SMOP_QUIT) {
						this->m_eSubOpt = SMOP_RESTART;
						prnSubOptionText(SMOP_RESTART);
					}
				}
				else if (op == KEY_DOWN) { //���¼�
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
	cout << "������Ϸ" << endl;

	DrawTool::SetCursorPosition(39, 27);
	cout << "���¿�ʼ" << endl;

	DrawTool::SetCursorPosition(39, 29);
	cout << "�˳���Ϸ" << endl;

	switch (opt)
	{
	case SMOP_CONTINUE:
		DrawTool::SetBgColor();
		DrawTool::SetCursorPosition(39, 25);
		cout << "������Ϸ" << endl;
		break;
	case SMOP_RESTART:
		DrawTool::SetBgColor();
		DrawTool::SetCursorPosition(39, 27);
		cout << "���¿�ʼ" << endl;
		break;
	case SMOP_QUIT:
		DrawTool::SetBgColor();
		DrawTool::SetCursorPosition(39, 29);
		cout << "�˳���Ϸ" << endl;
		break;
	}

}

