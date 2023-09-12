#include<iostream>
#include<climits>
#include<stdio.h>
#include"7_2151294_马威.h"
using namespace std;

/***************************************************************************
  函数名称：num_input
  功    能：在设置输入提示和正确处理下，输入一个整型数
  输入参数：const char* prompt：输入提示（只读）
			const int min：输入最小值（只读）
			const int max：输入最大值（只读）
  返 回 值：int：输入值
  说    明：无
***************************************************************************/
int num_input(const char* prompt, const int min, const int max)
{
	int ret = 0;

	while (1) {
		cout << prompt;
		cin >> ret;

		if (cin.good() && ret >= min && ret <= max)
			break;

		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "输入错误，请重新输入！" << endl;
	}

	return ret;
}

/***************************************************************************
  函数名称：main
  功    能：主函数
  输入参数：无
  返 回 值：0
  说    明：无
***************************************************************************/
int main()
{
	/*1、输入木头数量*/
	int num = 0;

	while (1) {
		cin >> num;

		if (cin.good() && num >= 1 && num <= 1000)
			break;

		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "输入错误，请重新输入！" << endl;
	}

	int* data = new int[num];
	if (data == NULL) {
		cerr << "存储分配错误！" << endl;
		return 0;
	}

	/*2、输入每块木头的长度*/
	for (int i = 0; i < num; i++) {
		int length = -1;

		while (1) {
			cin >> length;

			if (cin.good() && length >= 1 && length <= INT_MAX) {
				data[i] = length;
				break;
			}

			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << "第" << i + 1 << "块木头长度输入错误，请重新输入" << endl;
		}
	}

	MinHeap<int> woods(data, num);  /*存储每根木头长度的最小堆*/
	int cost = 0;  /*总花费*/
	int minor_child_pos = -1;  /*小子女所在位置*/

	while (woods.GetSize() > 1) {
		if (woods.GetSize() > 2)  /*若最小堆元素大于2，取小子女所在位置*/
			minor_child_pos = (woods[1] > woods[2]) ? 2 : 1;
		else  /*若最小项仅有一个子女，则取该子女位置*/
			minor_child_pos = 1;

		cost += woods[0] + woods[minor_child_pos];  /*将最小项和小子女加到总花费上*/

		int new_wood = woods[minor_child_pos] + woods[0];  /*将最短的两根木头合成为一根*/
		int get = 0;
		woods.RemoveMin(get);  /*从最小堆中移除最小的两个元素*/
		woods.RemoveMin(get);
		woods.Insert(new_wood);  /*将新合成的木头放入最小堆中*/
	}

	cout << cost << endl;  /*输出最小花费*/
	delete[] data;
	return 0;
}