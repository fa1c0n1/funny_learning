#include <stdio.h>
#include <stdlib.h>

#include "controller.h"
#include "draw_tool.h"
#include "data.h"



int main(void)
{
	//printf("■");
	//printf("●");
	//printf("┃______________________┃");
	//printf("▓");

	//while (1) {
	//	if (_kbhit()) {
	//		int key = _getch();
	//		printf("%d\n", key);
	//	}

	//	Sleep(50);
	//}


	LaunchGame();

	//int key = 5;
	//int j = 1;
	//for (int i = 0; i < 10; i++) {
	//	switch (key)
	//	{
	//	case 5: 
	//		j = 4;
	//		//continue;
	//		break;
	//	default:
	//		break;
	//	}

	//	j++;
	//}

	system("pause");
	return 0;
}