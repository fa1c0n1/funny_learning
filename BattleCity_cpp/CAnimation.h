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

	//��ʾ��ӭ����
	void welcomeAnim();
	//�������˵�ʱ��ʾ��̬����
	void showStillText();

private:
	//��̬��ӡһ��̹��
	void prnStartTank();
	//��̬��ӡ����1: BATTLE
	void prnStartText1();
	//��̬��ӡ����2: CITY
	void prnStartText2();

	static Point m_text1[60];
	static Point m_text2[32];
};

