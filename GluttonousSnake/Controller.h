#pragma once

#include "Comm.h"
#include "Food.h"
#include "Snake.h"

class Controller
{
public:
	Controller();
	~Controller();
	void launchGame();
	
	Snake *m_pSnake;
	Food *m_pFood;
	
private:
	void showAnim();
	void showMenu();
	void initGame();
	void playGame();
	void prnOptionText(GameLevel glevel);

	GameLevel m_eGLevel;
};

