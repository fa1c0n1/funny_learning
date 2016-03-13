/*
[2] = Q
[1] = J
[0] = Q
[2] = J
[1] = K


cards = "QKJ"*/

#include <stdio.h>

int main(void)
{
	const char *cards = "JQK";
	// char cards[] = "JQK";
	cards[2] = cards[1];
	printf("%c\n", cards[2]);
	// cards[1] = cards[0];
	// printf("%c", cards[1]);
	// cards[0] = cards[2];
	// printf("%c", cards[0]);
	// cards[2] = cards[1];
	// printf("%c", cards[2]);
	// cards[1] = a_card;
	// printf("%c", cards[1]);
	// puts(cards);

	return 0;
}