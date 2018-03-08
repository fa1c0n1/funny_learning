#pragma once

typedef struct {
	int nX;
	int nY;
} Point;

class CAnimation
{
public:
	CAnimation();
	~CAnimation();

	//显示欢迎动画
	void welcomeAnim();
	//返回主菜单时显示静态文字
	void showStillText();

private:
	//动态打印一辆坦克
	void prnStartTank();
	//动态打印文字1: BATTLE
	void prnStartText1();
	//动态打印文字2: CITY
	void prnStartText2();

	static Point m_text1[60];
	static Point m_text2[32];
};

