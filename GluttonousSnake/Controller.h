#pragma once

#include "Comm.h"
#include "Food.h"
#include "Snake.h"
#include "GameMap.h"

#include <windows.h>

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
	bool showMenu();
	void initGame();
	void playGame();
	void prnOptionText(GameLevel glevel);
	void drawInfoPanel();
	void drawGameLevelInfo(GameLevel glevel);
	void drawSnakeInfo(Snake &snake);
	void showSubMenu();
	void prnSubOptionText(SubMenuOption opt);
	void showGameOverPrompt(Snake &snake);
	void prnGameOverOptionText(SubMenuOption opt);
	void eraseSubMenu();
	void freeGameRes();
	MapOption customMapPrompt();
	void prnCustomMapOptionText(MapOption eMapOp);
	void gotoEditYourMap();
	void mouseEventProc(MOUSE_EVENT_RECORD mer);
	bool keyEventProc(KEY_EVENT_RECORD ker);

	GameLevel m_eGLevel;
	SubMenuOption m_eSubOpt;
	bool m_bPause;
	bool m_bExit;
};

