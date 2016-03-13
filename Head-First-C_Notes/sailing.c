#include <stdio.h>

void go_south_east(int *lat, int *lon)
{
	*lat = *lat - 1;
	*lon = *lon + 1;
}

int main()
{
	// int latitude = 32;
	// int longtitude = -64;

	// go_south_east(&latitude, &longtitude);
	// printf("停! 当前位置: [%d, %d]\n", latitude, longtitude);

	char arr[10] = {'a', 'b', 'c'};
	char arr2[10] = "abcde";
	printf("arr.len=%ld\n", sizeof(arr));
	printf("arr2.len=%ld\n", sizeof(arr2));
	printf("%c\n", arr[8]);
}