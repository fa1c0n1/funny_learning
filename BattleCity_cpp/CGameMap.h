#pragma once

#include <windows.h>

/************************************************************************/
/* CGameMap类：地图相关的功能
/************************************************************************/
class CGameMap
{
public:
	CGameMap();
	~CGameMap();

	void setLevel(int nLevel);
	//载入地图
	void loadMap();
	//设置地图上单个点的类型
	void setMapValue(int nX, int nY, int nVal);
	//获取地图上单个点的类型
	int getMapValue(int nX, int nY);
	//获取草地河流地图上单个点的类型
	int getMapGrassRiverValue(int nX, int nY);
	//画出地图
	void drawMap();
	//编辑地图
	void editMap();

private:
	//处理编辑地图功能的鼠标事件
	bool editMapMouseEventProc(MOUSE_EVENT_RECORD mer);
	//显示地图编辑功能面板
	void showMapEditorPanel();
	//更新地图编辑功能面板上的信息
	void updateEditorInfo();
	//用于在编辑地图时，清空地图上的可编辑区域
	void clearEditableArea();
	//用于在编辑地图是，保存编辑好的地图到文件
	void saveMapToFile();
	//显示保存地图成功的提示框
	void showSaveMapNotice();
	//判断文件是否存在
	bool isFileExist(const char *pszFileName);
	//读取文件
	void loadFile(const char *pszFileName);

	bool m_bUseDefault;             //是否使用当前关卡的默认地图
	int m_nLevel;                   //关卡
	int m_nEditNodeType;            //编辑地图时,鼠标在屏幕上所编辑的障碍物的类型
	int m_nArrGrass[40][40];        //单独用一个跟地图一样的二维数组保存地图上的草地和河流结点
	int m_nArrMap[40][40];          //存储地图的二维数组
};

