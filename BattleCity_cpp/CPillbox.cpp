#include <windows.h>
#include "CPillbox.h"
#include "Data.h"
#include "DrawTool.h"

int CPillbox::m_nPillboxShape[3][3] = {
	{ 0, 1, 0 }, { 1, 1, 1 }, { 0, 1, 0 }
};

CPillbox::CPillbox() : m_bDead(false), m_nX(17), m_nY(36), m_nType(SIGN_PILLBOX)
{
}


CPillbox::~CPillbox()
{
}

void CPillbox::drawObject()
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			m_pMap->setMapValue(m_nX + j, m_nY + i, SIGN_PILLBOX);
			if (m_nPillboxShape[i][j] == 1)
				DrawTool::drawPattern(m_nX + j, m_nY + i, m_nType);
		}
	}
}

void CPillbox::clearObject()
{
}

void CPillbox::setMapObj(CGameMap *pGMap)
{
	m_pMap = pGMap;
}

void CPillbox::setIsDead(bool bDead)
{
	m_nType = bDead ? SIGN_PILLBOX_HIT : SIGN_PILLBOX;
	m_bDead = bDead;
}

bool CPillbox::isDead()
{
	return m_bDead;
}