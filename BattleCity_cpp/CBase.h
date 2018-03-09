#pragma once

/************************************************************************/
/* CBase类：具有绘制对象和擦除对象功能的抽象类
/************************************************************************/
class CBase
{
public:
	CBase() {}
	virtual ~CBase() {}

	virtual void drawObject() = 0;  //绘制对象
	virtual void clearObject() = 0; //擦除对象
};