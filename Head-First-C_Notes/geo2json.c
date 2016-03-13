#include <stdio.h>
#include <string.h>

int main(void)
{
	float latitude;
	float longitude;
	char info[80];
	int started = 0;

	puts("data=[");
	while(scanf("%f,%f,%79[^\n]", &latitude, &longitude, info) == 3) {
		if(started) {
			printf(",\n");
		} else {
			started = 1;
		}

		//check
		if(latitude < -90 || latitude > 90) {
			fprintf(stderr, "Invalid latitude: %f\n", latitude);
			return 2;
		}
		if(longitude < -180 || longitude > 180) {
			fprintf(stderr, "Invalid longtitude: %f\n", longitude);
			return 2;
		}
		printf("{latitude: %f, longitude: %f, info: '%s'}", latitude, longitude, info);
	}
	puts("\n]");

	return 0;
}