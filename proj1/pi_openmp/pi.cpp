#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <windows.h>

using namespace std;


int DifTime(SYSTEMTIME &Time1, SYSTEMTIME &Time2)
{
	int temp, factor, total = 0;

	temp = Time2.wMilliseconds - Time1.wMilliseconds;
	factor = 0;
	if (temp < 0)
	{
		temp += 1000;
		factor = 1;
	}
	total = temp;

	temp = Time2.wSecond - Time1.wSecond - factor;
	factor = 0;
	if (temp < 0)
	{
		temp += 60;
		factor = 1;
	}
	total += temp * 1000;

	temp = Time2.wMinute - Time1.wMinute - factor;
	factor = 0;
	if (temp < 0)
	{
		temp += 60;
		factor = 1;
	}
	total += temp * 60 * 1000;

	temp = Time2.wHour - Time1.wHour - factor;
	if (temp < 0) temp += 60;
	total += temp * 60 * 60 * 1000;

	return(total);
}

int main (int argc, char *argv[])
{

	double area, pi, x;
	int i, n;
	char c;
	char st[100];
	SYSTEMTIME Time1, Time2;
	
	cout << "Numero de processadores: " << omp_get_num_procs() << endl;

	cout << "Numero de divisoes ? "; 
	cin >> n; 
	area = 0.0;

	cout << setprecision(18) << "PI = " << atan(1) * 4 << endl;
	GetLocalTime(&Time1);
	#pragma omp parallel for private(x)
	for (i = 0; i < n; i++) {
	   x = (i+0.5)/n;
	   area[omp_get_thread_num()] += 4.0 / (1.0 + x*x);
	}
	pi = area / n;
	GetLocalTime(&Time2);

	cout << setprecision(18) << "PI = " << pi << endl << endl;
	sprintf(st, "Time: %3.3f seconds\n", ((float)DifTime(Time1, Time2) / 1000.0));
	cout << st;
	//cout << setprecision(18) << "Diferença : " << pi - atan(1) * 4 << endl;

	cout << "Enter para continuar ...";
	cin.get(c);
	cin.get(c);
	
}

