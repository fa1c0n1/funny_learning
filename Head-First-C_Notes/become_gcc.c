#include <stdio.h>

float mercury_day_in_earth_days();
int hours_in_an_earth_day();

int main()
{
    float length_of_day = mercury_day_in_earth_days();
    int hours = hours_in_an_earth_day();
    float day = length_of_day * hours;
    printf("水星上一天有 %f 小时\n", day);
    return 0;
}

float mercury_day_in_earth_days() {
    return 58.65;
}

int hours_in_an_earth_day() {
    return 24;
}

