#pragma once

#include "CBase.h"
#include "CGameMap.h"
#include "CPillbox.h"
#include "CTank.h"
#include <vector>

using std::vector;

/************************************************************************/
/* CBullet�ࣺ�ӵ���صĹ���
/************************************************************************/
class CBullet : public CBase
{
public:
	CBullet();
	~CBullet();

	//�����ӵ�
	void drawObject();
	//�����ӵ�
	void clearObject();

	//���ӵ���״̬��λ
	void resetBullet();
	//�ƶ��ӵ�
	void moveBullet(vector<CBullet*> &vtBulletBox, vector<CTank> &vtTanks, CPillbox &pillbox, int *pEnemyNum);
	void setPosX(int nX);
	void setPosY(int nY);
	void setValid(bool bValid);
	void setID(int nID);
	void setDirection(int nDrt);
	void setType(int nType);
	void setOwner(int nOwner);
	void setAtk(int nAtk);
	bool isValid();
	void setMapObj(CGameMap *pGMap);
	int getPosX();
	int getPosY();
	int getType();
	int getOwner();
	int getAtk();
	CGameMap *getGameMap();

	//�����ӵ�
	static void fireBullet(vector<CBullet*> &vtBulletbox, CTank &tank);

private:
	//�����ӵ����е���ײ�����������ײ����true, ��֮����false
	bool bulletCollision(vector<CBullet*> &vtBulletBox, vector<CTank> &vtTanks, CPillbox &pillbox, int *pEAliveNum);
	//�����ӵ�����ӵ��������������ײ, ���������ײ����true, ��֮����false
	bool bulletCollisionWithOthers(int nX, int nY, vector<CTank> &vtTanks, CPillbox &pillbox, int *pEAliveNum);
	//����ұ��з�̹�˵��ӵ�����ʱ�Ĵ���
	void doWhenPlayerBeated(int nX, int nY, vector<CTank> &vtTanks);
	//���з�̹�˵��ӵ�����һ���ʱ�Ĵ���
	void doWhenEnemyBeated(int nX, int nY, vector<CTank> &vtTanks, int *pEAliveNum);
	//���ӵ������ϰ���ʱ�Ĵ���
	bool doWhenBarierBeated(int nX, int nY, CPillbox &pillbox);

	bool m_bValid;    //�ӵ��Ƿ����
	int m_nID;        //�ӵ����
	int m_nX;         //�ӵ���X����
	int m_nY;         //�ӵ���Y����
	int m_nOwner;     //�ӵ�������
	int m_nType;      //�ӵ�������(��������Ӫ)
	int m_nDrt;       //�ӵ��ķ���
	int m_nAtk;       //�ӵ��Ĺ�����
	CGameMap *m_pMap; //��ͼ����
};

