// ch2_pract.cpp -- chapter2 programming practice

#include <iostream>

using namespace std;

void pract_1()
{
	cout << "My name is plathon," << endl;
	cout << "And my address is No.2371 in backer stree in London" << endl;
}

void pract_2()
{
	long distance;
	cout << "Please enter the distance: ";
	cin >> distance;
	cout << "The distance in MA is " << distance + 220 << ". " << endl;
}

void p_3_1()
{
	cout << "Three blind mice" << endl;
}

void p_3_2()
{
	cout << "See how they run" << endl;
}

void pract_3()
{
	p_3_1();
	p_3_1();
	p_3_2();
	p_3_2();
}

void pract_4()
{
	int age;
	cout << "Please enter your age: ";
	cin >> age;
	cout << "The age includes " << age * 12 << " months." << endl;
}

double temp_convert_c_2_f(double);

void pract_5()
{
	double c_temp;
	cout << "Please enter a Celsius value: ";
	cin >> c_temp;
	cout << c_temp << " degrees Celsius is " << temp_convert_c_2_f(c_temp) << " degrees Fahrenheit." << endl;
}

double temp_convert_c_2_f(double celsius)
{
	return celsius * 1.8 + 32;
}

void pract_6()
{
	double light_years;
	cout << "Enter the number of light years: ";
	cin >> light_years;
	cout << light_years << " light years = " << light_years * 63240 << " astronomical units." << endl;
}

void pract_7()
{
	int hour, minute;
	cout << "Enter the number of hours: ";
	cin >> hour;
	cout << "Enter the number of minutes: ";
	cin >> minute;
	cout << "Time: " << hour << ":" << minute << endl;
}

int main()
{
	pract_7();

	return 0;
}

