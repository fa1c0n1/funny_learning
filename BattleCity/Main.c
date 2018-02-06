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
	/*srand(time(NULL));
	while (1) {
	int num = rand() % 4;
	printf("%d\n", num);
	system("pause");
	}*/

	/*int key = 5;
	int j = 1;
	for (int i = 0; i < 10; i++) {
		switch (key)
		{
		case 5: 
			for (int m = 0; m < 2; m++) {
				j = 4;
				goto END;
			}
			continue;
			break;
		default:
			break;
		}

		j++;
END:
		j += 4;
	}*/

	return 0;
}