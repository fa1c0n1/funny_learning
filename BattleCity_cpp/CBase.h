#pragma once

class CBase
{
public:
	CBase() {}
	virtual ~CBase() {}

	virtual void drawObject() = 0;  //���ƶ���
	virtual void clearObject() = 0; //��������
};