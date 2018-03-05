#pragma once

class CBase
{
public:
	CBase() {}
	virtual ~CBase() {}

	virtual void drawObject() = 0;  //绘制对象
	virtual void clearObject() = 0; //擦除对象
};