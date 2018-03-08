#include "CAstar.h"
#include "Data.h"

CAstar::CAstar() : m_pVir(nullptr)
{
}

CAstar::~CAstar()
{
	if (m_pVir) {
		delete[] m_pVir;
		m_pVir = nullptr;
	}
}

void CAstar::initMapInfo(int nMapWidth, int nMapHeight)
{
	m_nMapWidth = nMapWidth;
	m_nMapHeight = nMapHeight;
	m_pVir = new VIR_INFO[m_nMapWidth * m_nMapHeight]{};
}

bool CAstar::findPath(CTank &tank, CTank &target)
{
	//清空Open表和Close表
	m_vtOpen.clear();
	m_vtClose.clear();

	//============清空 m_pVir 的原有标识=================
	ZeroMemory(m_pVir, m_nMapWidth * m_nMapHeight);
	//=================================================

	//设置起点和终点
	m_codStart.X = tank.getPosX();
	m_codStart.Y = tank.getPosY();
	m_codEnd.X = target.getPosX();
	m_codEnd.Y = target.getPosY();

	//设置游戏地图对象
	m_pMap = tank.getMapObj();

	NODE_INFO nodeStar;
	nodeStar.codSelf = m_codStart;
	nodeStar.nG = 0;
	nodeStar.setH_F(m_codStart, m_codEnd);

	m_vtOpen.push_back(nodeStar);

	//开始循环查找
	while (true) {
		//Open表是否为空
		if (m_vtOpen.empty())
			return false;

		//找Open表中F值最小的点
		int nIdx = 0;
		for (int i = 1; i < m_vtOpen.size(); i++) {
			if (m_vtOpen[nIdx].nF > m_vtOpen[i].nF)
				nIdx = i;
		}

		//扩散
		NODE_INFO nodeNew[4];

		//上
		nodeNew[0].codSelf.X = m_vtOpen[nIdx].codSelf.X;
		nodeNew[0].codSelf.Y = m_vtOpen[nIdx].codSelf.Y - 1;

		//下
		nodeNew[1].codSelf.X = m_vtOpen[nIdx].codSelf.X;
		nodeNew[1].codSelf.Y = m_vtOpen[nIdx].codSelf.Y + 1;

		//左
		nodeNew[2].codSelf.X = m_vtOpen[nIdx].codSelf.X - 1;
		nodeNew[2].codSelf.Y = m_vtOpen[nIdx].codSelf.Y;

		//右
		nodeNew[3].codSelf.X = m_vtOpen[nIdx].codSelf.X + 1;
		nodeNew[3].codSelf.Y = m_vtOpen[nIdx].codSelf.Y;

		for (int i = 0; i < 4; i++) {
			nodeNew[i].codParent = m_vtOpen[nIdx].codSelf;
			nodeNew[i].nG = m_vtOpen[nIdx].nG + 1;
			nodeNew[i].setH_F(nodeNew[i].codSelf, m_codEnd);
			nodeNew[i].codSelf.nDrt = i;
		}

		//=================================
		int x1 = m_vtOpen[nIdx].codSelf.X;
		int y1 = m_vtOpen[nIdx].codSelf.Y;
		m_pVir[y1 * m_nMapWidth + x1].isClose = true;
		m_pVir[y1 * m_nMapWidth + x1].isOpen = false;
		//=================================

		//把扩散过的点保存到Close表，并从Open表中删除
		m_vtClose.push_back(m_vtOpen[nIdx]);
		m_vtOpen.erase(m_vtOpen.begin() + nIdx);

		//检测扩散出来的点是否合格，是否是终点
		for (int i = 0; i < 4; i++) {
			int nTX = nodeNew[i].codSelf.X;
			int nTY = nodeNew[i].codSelf.Y;

			//是不是终点
			if (nodeNew[i].codSelf == m_codEnd) {
				m_vtClose.push_back(nodeNew[i]);
				return true;
			}

			//检测是否是障碍物
			bool bBlock = false;
			if (i == 0) { //上
				for (int m = 0; m < 3; m++) {
					if (checkBarrier(nTX + m, nTY)) {
						bBlock = true;
						break;
					}
				}

				if (bBlock)
					continue;
			}
			else if (i == 1) { //下
				for (int m = 0; m < 3; m++) {
					if (checkBarrier(nTX + m, nTY + 2)) {
						bBlock = true;
						break;
					}
				}

				if (bBlock)
					continue;
			}
			else if (i == 2) { //左
				for (int m = 0; m < 3; m++) {
					if (checkBarrier(nTX, nTY + m)) {
						bBlock = true;
						break;
					}
				}

				if (bBlock)
					continue;
			}
			else if (i == 3) { //右
				for (int m = 0; m < 3; m++) {
					if (checkBarrier(nTX + 2, nTY + m)) {
						bBlock = true;
						break;
					}
				}

				if (bBlock)
					continue;
			}

			//判断是否在Open表和Close表中
			if (m_pVir[nTY * m_nMapWidth + nTX].isClose || m_pVir[nTY * m_nMapWidth + nTX].isOpen)
				continue;

			//将合格的点保存到Open表中
			m_vtOpen.push_back(nodeNew[i]);

			//====================================
			m_pVir[nTY * m_nMapWidth + nTX].isOpen = true;
			//====================================
		}
	} //while
}

vector<CAstar::MY_COORD> CAstar::getPath()
{
	//回溯父结点
	m_vtPath.clear();    //清空原来的路径
	int nSize = m_vtClose.size();
	int nIdx = nSize - 1;

	while (true) {
		for (int i = 0; i < nSize; i++) {
			if (m_vtClose[nIdx].codParent == m_codStart)
				goto END;

			if (m_vtClose[nIdx].codParent == m_vtClose[i].codSelf) {
				m_vtPath.push_back(m_vtClose[i].codSelf);
				nIdx = i;
				break;
			}
		}
	}

END:
	return m_vtPath;
}

bool CAstar::checkBarrier(int nX, int nY)
{
	return m_pMap->getMapValue(nX, nY) != SIGN_EMPTY && m_pMap->getMapValue(nX, nY) != SIGN_GRASS
		&& m_pMap->getMapValue(nX, nY) != SIGN_BULLET0 && m_pMap->getMapValue(nX, nY) != SIGN_BULLET1
		&& m_pMap->getMapValue(nX, nY) != m_pMap->getMapValue(m_codEnd.X, m_codEnd.Y);
}
