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
	case SIGN_GRASS:
	case SIGN_RIVER:
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
	case SIGN_GRASS:
	case SIGN_RIVER:
	case SIGN_TANK_PA:
	case SIGN_TANK_PB:
	case SIGN_TANK_E0:
	case SIGN_TANK_E1:
		break;
	default:
		m_pMap->setMapValue(m_nX, m_nY, SIGN_EMPTY);
		DrawTool::drawPattern(m_nX, m_nY, SIGN_EMPTY);
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

			switch (tank.getType())
			{
			case SIGN_TANK_PA:
			case SIGN_TANK_PB:
				pNewBullet->setType(SIGN_BULLET0);
				break;
			case SIGN_TANK_E0:
			case SIGN_TANK_E1:
				pNewBullet->setType(SIGN_BULLET1);
				break;
			}

			pNewBullet->drawObject();
			break;
		}
	}
}

void CBullet::moveBullet(vector<CBullet*> &vtBulletBox, vector<CTank> &vtTanks, CPillbox &pillbox)
{
	clearObject();

	if (bulletCollision(vtBulletBox, vtTanks, pillbox)) {
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

bool CBullet::bulletCollision(vector<CBullet*> &vtBulletBox, vector<CTank> &vtTanks, CPillbox &pillbox)
{
	bool bRet = false;

	//判断子弹之间的碰撞
	for (int i = 0; i < vtBulletBox.size(); i++) {
		if (i == m_nID)
			continue;
		if (!vtBulletBox[i]->isValid()) {
			if (m_nX == vtBulletBox[i]->getPosX() && m_nY == vtBulletBox[i]->getPosY()) {
				if (m_nType != vtBulletBox[i]->getType()) { //我方子弹与敌方子弹碰撞
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

	bRet = bulletCollisionWithOthers(nTX, nTY, vtTanks, pillbox);
END:
	return bRet;
}

bool CBullet::bulletCollisionWithOthers(int nX, int nY, vector<CTank> &vtTanks, CPillbox &pillbox)
{
	bool bRet = false;

	if (m_nType == SIGN_BULLET0) { //我方的子弹
		if (m_pMap->getMapValue(nX, nY) == SIGN_TANK_E0 || m_pMap->getMapValue(nX, nY) == SIGN_TANK_E1) {
			//子弹打中敌军
			doWhenEnemyBeated(nX, nY, vtTanks);
			bRet = true;
			goto END;
		}
	}
	else if (m_nType == SIGN_BULLET1) { //敌方的子弹
		if (m_pMap->getMapValue(nX, nY) == SIGN_TANK_PA || m_pMap->getMapValue(nX, nY) == SIGN_TANK_PB) {
			//子弹打中玩家
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

void CBullet::doWhenEnemyBeated(int nX, int nY, vector<CTank> &vtTanks)
{
	for (int i = 0; i < ENEMY_NMAX; i++) {
		if (!vtTanks[i + 2].isDead()) {
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					if (vtTanks[i + 2].getPosX() + m == nX && vtTanks[i + 2].getPosY() + n == nY) {
						vtTanks[i + 2].setDead(true);
						vtTanks[i + 2].clearObject();
						break;
					}
				}
			}
		}
	}
}

void CBullet::doWhenPlayerBeated(int nX, int nY, vector<CTank> &vtTanks)
{
	for (int i = 0; i < 2; i++) {
		if (!vtTanks[i].isDead()) {
			for (int m = 0; m < 3; m++) {
				for (int n = 0; n < 3; n++) {
					if (vtTanks[i].getPosX() + m == nX && vtTanks[i].getPosY() + n == nY) {
						vtTanks[i].setDead(true);
						vtTanks[i].clearObject();
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

	if (m_pMap->getMapValue(nX, nY) == SIGN_WALL0) { //子弹打中钢墙
		bRet = true;
	}
	else if (m_pMap->getMapValue(nX, nY) == SIGN_WALL1) { //子弹打中水泥墙
		m_pMap->setMapValue(nX, nY, SIGN_EMPTY);
		DrawTool::drawPattern(nX, nY, SIGN_EMPTY);
		bRet = true;
	}
	else if (m_pMap->getMapValue(nX, nY) == SIGN_PILLBOX) { //子弹打中碉堡
		pillbox.setIsDead(true);
		pillbox.drawObject();
		bRet = true;
	}

	return bRet;
}
