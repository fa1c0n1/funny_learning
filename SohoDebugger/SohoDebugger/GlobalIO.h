#pragma once

#include <QTextStream>

class CGlobalIO
{
public:
	CGlobalIO();
	~CGlobalIO();

	static QTextStream tsIn;
	static QTextStream tsOut;
};

