#include<iostream>
#include"2_2151294_马威.h"
using namespace std;

int main()
{
	LinkedSet<int> a, b;  /*a、b两个链式集合*/
	a.Input(-1);  /*输入a*/
	b.Input(-1);  /*输入b*/
	(a * b).Output();  /*求a与b的交，并输出出来*/
	return 0;
}