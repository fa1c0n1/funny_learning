#include <stdio.h>

void prn_int_arr(int arr[], int len)
{
	int i = 0;
	printf("[");
	for(; i < len; i++) {
		if(i == len - 1) 
			printf("%d]\n", arr[i]);
		else 
			printf("%d, ", arr[i]);
	}
}

void prn_char_arr(char arr[], int len)
{
	int i = 0;
	printf("[");
	arr[0] = 'A';
	for(; i < len; i++) {
		if(i == len - 1) 
			printf("%c]\n", arr[i]);
		else 
			printf("%c, ", arr[i]);
	}	
}