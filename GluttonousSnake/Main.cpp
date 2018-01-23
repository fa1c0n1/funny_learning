#include "Controller.h"
#include <cstdlib>
#include <conio.h>

#include <iostream>
#include <windows.h>

using namespace std;

int main(void)
{
	Controller ctrl;
	ctrl.launchGame();

	/*while (true) {
		Sleep(3000);

		if (_kbhit()) {
		cout << "111-down: " << _getch() << endl;
		cout << "222-down: " << _getch() << endl;
		}

		Sleep(50);
		}*/
	
	return 0;
}