#include "CBullet.h"
#include "Data.h"
#include "DrawTool.h"

CBullet::CBullet() : m_bValid(true), m_nX(-2), m_nY(-2), m_nDrt(DRT_NONE)
{
}

CBullet::~CBullet()
{
}

void CBullet::drawObject()
{
	switch (m_pMap->getMapValue(m_nX, m_nY))
	{
	case SIGN_TANK_PA:
	case SIGN_TANK_PB:
	case SIGN_TANK_E0:
	case SIGN_TANK_E1:
		break;
	default:
		m_pMap->setMapValue(m_nX, m_nY, m_nType);
		DrawTool::drawPattern(m_nX, m_nY, m_nType);
		break;
	}
}

void CBullet::clearObject()
{
	switch (m_pMap->getMapValue(m_nX, m_nY))
	{
	case SIGN_TANK_PA:
	case SIGN_TANK_PB:
	case SIGN_TANK_E0:
	case SIGN_TANK_E1:
		break;
	default:
		int nGrassOrRiver = m_pMap->getMapGrassRiverValue(m_nX, m_nY);
		if (nGrassOrRiver == SIGN_GRASS || nGrassOrRiver == SIGN_RIVER) {
			m_pMap->setMapValue(m_nX, m_nY, nGrassOrRiver);
			DrawTool::drawPattern(m_nX, m_nY, nGrassOrRiver);
		}
		else {
			m_pMap->setMapValue(m_nX, m_nY, SIGN_EMPTY);
			DrawTool::drawPattern(m_nX, m_nY, SIGN_EMPTY);
		}
		break;
	} 
}

void CBullet::resetBullet()
{
	m_bValid = true;
	m_pMap->setMapValue(m_nX, m_nY, SIGN_EMPTY);
}

void CBullet::setID(int nID)
{
	m_nID = nID;
}

void CBullet::setPosX(int nX)
{
	m_nX = nX;
}

void CBullet::setPosY(int nY)
{
	m_nY = nY;
}

void CBullet::setType(int nType)
{
	m_nType = nType;
}

void CBullet::setOwner(int nOwner)
{
	m_nOwner = nOwner;
}

void CBullet::setAtk(int nAtk)
{
	m_nAtk = nAtk;
}
	
void CBullet::setValid(bool bValid)
{
	m_bValid = bValid;
}

void CBullet::setDirection(int nDrt)
{
	m_nDrt = nDrt;
}

bool CBullet::isValid()
{
	return m_bValid;
}

void CBullet::setMapObj(CGameMap *pGMap)
{
	m_pMap = pGMap;
}

int CBullet::getPosX()
{
	return m_nX;
}

int CBullet::getPosY()
{
	return m_nY;
}

int CBullet::getType()
{
	return m_nType;
}

int CBullet::getOwner()
{
	return m_nOwner;
}

int CBullet::getAtk()
{
	return m_nAtk;
}

CGameMap *CBullet::getGameMap()
{
	return m_pMap;
}

void CBullet::fireBullet(vector<CBullet*> &vtBulletbox, CTank &tank)
{
	CBullet *pNewBullet = nullptr;
	for (CBullet *pTBullet : vtBulletbox) {
		if (pTBullet->isValid()) {
			pNewBullet = pTBullet;

			int nTX, nTY;
			switch (tank.getDirection())
			{
			case DRT_UP:
				nTX = tank.getPosX() + 1;
				nTY = tank.getPosY() - 1;
				break;
			case DRT_DOWN:
				nTX = tank.getPosX() + 1;
				nTY = tank.getPosY() + 3;
				break;
			case DRT_LEFT:
				nTX = tank.getPosX() - 1;
				nTY = tank.getPosY() + 1;
				break;
			case DRT_RIGHT:
				nTX = tank.getPosX() + 3;
				nTY = tank.getPosY() + 1;
				break;
			}

			if (pNewBullet->getGameMap()->getMapValue(nTX, nTY) == SIGN_WALL0)
				return;

			pNewBullet->setPosX(nTX);
			pNewBullet->setPosY(nTY);
			pNewBullet->setValid(false);
			pNewBullet->setDirection(tank.getDirection());
			pNewBullet->setOwner(tank.getType());

			switch (tank.getType())
			{
			case SIGN_TANK_PA:
			case SIGN_TANK_PB:
				pNewBullet->setType(SIGN_BULLET0);
				break;
			case SIGN_TANK_E0:
				pNewBullet->setAtk(1);
				pNewBullet->setType(SIGN_BULLET1);
				break;
			case SIGN_TANK_E1:
				pNewBullet->setAtk(2);
				pNewBullet->setType(SIGN_BULLET1);
				break;
			}

			pNewBullet->drawObject();
			break;
		}
	}
}

void CBullet::moveBullet(vector<CBullet*> &vtBulletBox, vector<CTank> &vtTanks, CPillbox &pillbox, int *pEAliveNum)
{
	clearObject();

	if (bulletCollision(vtBulletBox, vtTanks, pillbox, pEAliveNum)) {
		clearObject();
		m_bValid = true;
		return;
	}

	switch (m_nDrt)
	{
	case DRT_UP:
		m_nY -= 1;
		break;
	case DRT_DOWN:
		m_nY += 1;
		break;
	case DRT_LEFT:
		m_nX -= 1;
		break;
	case DRT_RIGHT:
		m_nX += 1;
		break;
	}

	drawObject();
}

bool CBullet::bulletCollision(vector<CBullet*> &vtBulletBox, vector<CTank> &vtTanks, CPillbox &pillbox, int *pEAliveNum)
{
	bool bRet = false;

	//�ж��ӵ�֮�����ײ
	for (int i = 0; i < vtBulletBox.size(); i++) {
		if (i == m_nID)
			continue;
		if (!vtBulletBox[i]->isValid()) {
			if (m_nX == vtBulletBox[i]->getPosX() && m_nY == vtBulletBox[i]->getPosY()) {
				if (m_nType != vtBulletBox[i]->getType()) { //�ҷ��ӵ���з��ӵ���ײ
					vtBulletBox[i]->setValid(true);
					bRet = true;
					goto END;
				}
			}
		}
	}

	int nTX, nTY;
	switch (m_nDrt)
	{
	case DRT_UP:
		nTX = m_nX;
		nTY = m_nY - 1;
		break;
	case DRT_DOWN:
		nTX = m_nX;
		nTY = m_nY + 1;
		break;
	case DRT_LEFT:
		nTX = m_nX - 1;
		nTY = m_nY;
		break;
	case DRT_RIGHT:
		nTX = m_nX + 1;
		nTY = m_nY;
		break;
	}

	bRet = bulletCollisionWithOthers(nTX, nTY, vtTanks, pillbox, pEAliveNum);
END:
	return bRet;
}

bool CBullet::bulletCollisionWithOthers(int nX, int nY, vector<CTank> &vtTanks, CPillbox &pillbox, int *pEAliveNum)
{
	bool bRet = false;

	if (m_nType == SIGN_BULLET0) { //�ҷ����ӵ�
		if (m_pMap->getMapValue(nX, nY) == SIGN_TANK_E0 || m_pMap->getMapValue(nX, nY) == SIGN_TANK_E1) {
			//�ӵ����ео�
			doWhenEnemyBeated(nX, nY, vtTanks, pEAliveNum);
			bRet = true;
			goto END;
		}
	}
	else if (m_nType == SIGN_BULLET1) { //�з����ӵ�
		if (m_pMap->getMapValue(nX, nY) == SIGN_TANK_PA || m_pMap->getMapValue(nX, nY) == SIGN_TANK_PB) {
			//�ӵ��������
			doWhenPlayerBeated(nX, nY, vtTanks);
			bRet = true;
			goto END;
		}
	}

	if (doWhenBarierBeated(nX, nY, pillbox)) {
		bRet = true;
		goto END;
	}

END:
	return bRet;
}

void CBullet::doWhenEnemyBeated(int nX, int nY, vector<CTank> &vtTanks, int *pEAliveNum)
{
	for (int i = 0; i < ENEMY_NMAX; i++) {
		if (!vtTanks[i + 2].isDead()) {
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					if (vtTanks[i + 2].getPosX() + m == nX && vtTanks[i + 2].getPosY() + n == nY) {

						//��ȥ�����еĵз�̹�˵�����ֵ, �ж��Ƿ�����������Ӧ����
						vtTanks[i + 2].setBlood(vtTanks[i + 2].getBlood() - 1);
						if (vtTanks[i + 2].getBlood() <= 0) {
							//Ѫ��Ϊ0������, �о�������1
							vtTanks[i + 2].setDead(true);
							vtTanks[i + 2].clearObject();
							*pEAliveNum -= 1;
						}
						else {
							//��Ѫ��������
							vtTanks[i + 2].clearObject();
							Sleep(200);
							vtTanks[i + 2].drawObject();
						}

						//���ݴ��в�ͬ���͵ĵз�̹�ˣ������ҷ���Ӧ������ӷ���
						int nDeltaScore = 10;
						if (vtTanks[i + 2].getType() == SIGN_TANK_E1)
							nDeltaScore = 20;
						
						if (m_nOwner == SIGN_TANK_PA)
							vtTanks[0].setScore(vtTanks[0].getScore() + nDeltaScore);
						else if (m_nOwner == SIGN_TANK_PB)
							vtTanks[1].setScore(vtTanks[1].getScore() + nDeltaScore);

						goto END;
					}
				}
			}
		}
	}
END:
	return;
}

void CBullet::doWhenPlayerBeated(int nX, int nY, vector<CTank> &vtTanks)
{
	for (int i = 0; i < 2; i++) {
		if (!vtTanks[i].isDead()) {
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					if (vtTanks[i].getPosX() + m == nX && vtTanks[i].getPosY() + n == nY) {
						//��Ҽ�Ѫ��
						vtTanks[i].setBlood(vtTanks[i].getBlood() - m_nAtk);
						if (vtTanks[i].getBlood() <= 0) {
							//Ѫ��Ϊ0, ��ұ���
							vtTanks[i].setDead(true);
							vtTanks[i].setBlood(0);
							vtTanks[i].clearObject();
						}
						else {
							//��Ҽ�Ѫ��������
							vtTanks[i].clearObject();
							Sleep(200);
							vtTanks[i].drawObject();
						}

						goto END;
					}
				}
			}
		}
	}
END:
	return;
}

bool CBullet::doWhenBarierBeated(int nX, int nY, CPillbox &pillbox)
{
	bool bRet = false;

	if (m_pMap->getMapValue(nX, nY) == SIGN_WALL0) { //�ӵ����и�ǽ
		bRet = true;
	}
	else if (m_pMap->getMapValue(nX, nY) == SIGN_WALL1) { //�ӵ�����ˮ��ǽ
		m_pMap->setMapValue(nX, nY, SIGN_EMPTY);
		DrawTool::drawPattern(nX, nY, SIGN_EMPTY);
		bRet = true;
	}
	else if (m_pMap->getMapValue(nX, nY) == SIGN_PILLBOX) { //�ӵ����еﱤ
		pillbox.setIsDead(true);
		pillbox.drawObject();
		bRet = true;
	}

	return bRet;
}
