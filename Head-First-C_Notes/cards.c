#include <stdio.h>
#include <stdlib.h>

int main()
{
	char card_name[3];
	int count = 0;

	do {
		puts("输入牌名: ");
		scanf("%2s", card_name);
		int val = 0;
		switch(card_name[0]) {
		case 'K':
		case 'Q':
		case 'J':
			val = 10;
			break;
		case 'A':
			val = 11;
			break;
		case 'X':
			continue;
		default:
			val = atoi(card_name);
			if(!(val >= 1 && val <= 10)) {
				continue;
			}
		}

		if(val > 2 && val < 7) {
			count++;
		} else if(val == 10) {
			count--;
		}
		printf("当前的计数: %i\n", count);

	} while(card_name[0] != 'X');

	return 0;
}


#if 0
//---------P77------------------------
(1) y = x - y  ----->  00 11 21 32 42
(2) y = y + x  ----->  00 11 23 36 410
(3)             
	y = y + 2;
	if(y > 4) {
		y = y - 1;
	}          

			   ----->  02 14 25 36 47


(4)
	x = x + 1;
	y = y + x;

			   ----->  11 34 59

(5)
	if(y < 5) {
		x = x + 1;
		if(y < 3) {
			x = x - 1;
		}
	}
	y = y + 2;

			   ----->  02 14 36 48
#endif

