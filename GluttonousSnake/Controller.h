#pragma once

#include "Comm.h"
#include "Food.h"
#include "Snake.h"
#include "GameMap.h"

class Controller
{
public:
	Controller();
	~Controller();
	void launchGame();
	
	Snake *m_pSnake;
	Food *m_pFood;
	GameMap *m_pGMap;
	
private:
	void showAnim();
	void showMenu();
	void initGame();
	void playGame();
	void prnOptionText(GameLevel glevel);
	void drawInfoPanel();
	void drawGameLevelInfo(GameLevel glevel);
	void drawScoreInfo(Snake &snake);
	void showSubMenu();
	void prnSubOptionText(SubMenuOption opt);
	void showGameOverPrompt(Snake &snake);
	void prnGameOverOptionText(SubMenuOption opt);
	void eraseSubMenu();
	void freeGameRes();

	GameLevel m_eGLevel;
	SubMenuOption m_eSubOpt;
	bool m_bPause;
	bool m_bExit;
};

