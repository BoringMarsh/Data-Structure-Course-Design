#include<iostream>
#include<climits>
#include"5_2151294_马威.h"
using namespace std;

#define MAX 1000  /*顾客最大人数*/

int main()
{
	LinkedQueue<int> A, B;  /*A口队列、B口队列*/
	int total, num;   /*顾客总数、顾客编号*/
	int customer[3];  /*三人一组*/

	/*1、输入顾客总人数*/
	while (1) {
		cout << "请输入顾客总人数以及顾客编号，每个数字间用空格分开" << endl;
		cin >> total;

		if (cin.good() && total > 0 && total <= MAX)
			break;

		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "总人数输入错误，请重新输入！" << endl;
	}

	/*2、输入顾客编号*/
	for (int i = 0; i < total; i++) {
		bool if_enqueue = false;

		while (1) {
			cin >> num;

			if (cin.good() && num > 0 && num <= INT_MAX) {
				if_enqueue = true;
				break;
			}

			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << "第" << i + 1 << "位顾客编号输入错误，请重新输入！" << endl;
		}

		if (if_enqueue) {  /*根据编号的奇偶进入不同的队列*/
			if (num % 2 == 0)
				B.EnQueue(num);
			else
				A.EnQueue(num);
		}
	}

	/*3、按要求出队列*/
	while (!A.IsEmpty() || !B.IsEmpty()) {
		if (A.getSize() >= 2) {  /*若A队数量大于2，则直接出队2个*/
			A.DeQueue(customer[0]);
			A.DeQueue(customer[1]);

			if (A.IsEmpty() && B.IsEmpty())
				cout << customer[0] << ' ' << customer[1];
			else
				cout << customer[0] << ' ' << customer[1] << ' ';
		}

		else if (A.getSize() == 1) {  /*若A队数量为1，则直接出队1个*/
			A.DeQueue(customer[0]);

			if (A.IsEmpty() && B.IsEmpty())
				cout << customer[0];
			else
				cout << customer[0] << ' ';
		}

		if (B.getSize()) {  /*若B队不为空，则出队1个*/
			B.DeQueue(customer[0]);

			if (A.IsEmpty() && B.IsEmpty())
				cout << customer[0];
			else
				cout << customer[0] << ' ';
		}
	}

	cout << endl;
	return 0;
}