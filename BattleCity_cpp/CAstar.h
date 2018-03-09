#pragma once

#include <windows.h>
#include <vector>
#include "CTank.h"
#include "CGameMap.h"

using std::vector;

/************************************************************************/
/* CAstar类：A星寻路算法
/************************************************************************/
class CAstar
{
public:
	CAstar();
	~CAstar();

	typedef struct _MY_COORD : public COORD
	{
		bool operator==(COORD cod)
		{
			return X == cod.X && Y == cod.Y;
		}

		void operator=(COORD cod)
		{
			X = cod.X;
			Y = cod.Y;
		}

		int nDrt;           
	} MY_COORD;

	//结点的结构
	typedef struct {
		int nG;		         //移动损耗
		int nH;              //距终点的最短距离
		int nF;              //nF = nG + nH
		MY_COORD codSelf;    //自身坐标
		MY_COORD codParent;  //父结点坐标(由谁扩散的)

		void setH_F(COORD cod1, COORD cod2)
		{
			nH = abs(cod1.X - cod2.X) + abs(cod1.Y - cod2.Y);
			nF = nG + nH;
		}
	} NODE_INFO;

	//=====优化处理====
	typedef struct _VIR_INFO
	{
		char isOpen : 1; //表是否在Open表中
		char isClose : 1; //表示是否在CLose表中
		char receive : 6; //保留
	}VIR_INFO, *PVIR_INFO;


	void initMapInfo(int nMapWidth, int nMapHeight);    //初始化地图相关信息
	bool findPath(CTank &tank, CTank &target);          //查找包含最短路径的点(得到Close表)
	vector<MY_COORD> getPath();               //获取最短路径

private:
	bool checkBarrier(int nX, int nY);

	int m_nMapWidth;              //地图的宽度
	int m_nMapHeight;             //地图的高度
	CGameMap *m_pMap;             //地图对象
	PVIR_INFO m_pVir;             //指向虚拟地图的首地址
	COORD m_codStart;             //起点坐标
	COORD m_codEnd;               //终点坐标
	vector<NODE_INFO> m_vtOpen;   //待扩散的点
	vector<NODE_INFO> m_vtClose;  //扩散过的点
	vector<MY_COORD> m_vtPath;    //最短路径
};

