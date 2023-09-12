#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<climits>
#include<iomanip>
#include<chrono>
#include<algorithm>
#include"10_2151294_马威.h"
using namespace std;

#define WIDTH 25

/***************************************************************************
  函数名称：cls
  功    能：清空屏幕
  输入参数：无
  返 回 值：无
  说    明：无
***************************************************************************/
void cls()
{
	if (system("clear"))
		system("cls");
}

/***************************************************************************
  函数名称：pause
  功    能：设置一个程序的断点
  输入参数：无
  返 回 值：无
  说    明：本函数只有按回车键才能继续
***************************************************************************/
void pause()
{
	cout << "按回车键继续. . . ";

	while (1) {
		char c = getchar();

		if (c == '\n' || c == '\r')
			break;
	}
}

/***************************************************************************
  函数名称：print_menu
  功    能：打印菜单
  输入参数：无
  返 回 值：无
  说    明：无
***************************************************************************/
void print_menu()
{
	cout << "**              排序算法比较                      **\n"
		<< "====================================================\n"
		<< "**              0 --- 冒泡排序                    **\n"
		<< "**              1 --- 选择排序                    **\n"
		<< "**              2 --- 直接插入排序                **\n"
		<< "**              3 --- 折半插入排序                **\n"
		<< "**              4 --- 希尔排序                    **\n"
		<< "**              5 --- 快速排序                    **\n"
		<< "**              6 --- 改进快速排序                **\n"
		<< "**              7 --- 堆排序                      **\n"
		<< "**              8 --- 归并排序                    **\n"
		<< "**              9 --- 基数程序                    **\n"
		<< "**             10 --- 内置sort、qsort             **\n"
		<< "====================================================\n";
}

/***************************************************************************
  函数名称：cmp
  功    能：比较函数，专为qsort所用
  输入参数：const void* p1：地址1（只读）
           const void* p2：地址2（只读）
  返 回 值：int：两个地址的比较值
  说    明：本比较函数表示为从小到大排序
***************************************************************************/
int cmp(const void* p1, const void* p2)
{
	int* a = (int *)(p1);
	int* b = (int *)(p2);
	return *a - *b;
}

/***************************************************************************
  函数名称：conduct_sort
  功    能：根据排序表进行各类排序
  输入参数：const DataList<int>& data：排序表（只读）
  返 回 值：无
  说    明：本函数对同一个数字序列轮流进行不同排序，同时进行计时并统计比较和移动次数
***************************************************************************/
void conduct_sort(const DataList<int>& data)
{
	bool if_set = false;

	for (int selection = 0; selection < 12; selection++) {  /*12种排序轮流进行*/
		DataList<int> list(data);  /*根据原始序列生成新的排序表*/
		Move = Compare = 0;  /*比较和移动次数*/

		if (!if_set && list.Length() > 1000) {  /*元素数量大于1000时仅进行部分排序，以免效率较低的排序用时过长*/
			selection = 6;
			if_set = true;
		}

		chrono::steady_clock::time_point t1 = chrono::steady_clock::now();  /*计时起点*/
		switch (selection) {
		case 0:
			BubbleSort(list, list.Length());
			cout << setiosflags(ios::left) << setw(WIDTH) << "冒泡排序";
			break;
		case 1:
			SelectSort(list, list.Length());
			cout << setiosflags(ios::left) << setw(WIDTH) << "直接选择排序";
			break;
		case 2:
			InsertSort(list, list.Length());
			cout << setiosflags(ios::left) << setw(WIDTH) << "直接插入排序";
			break;
		case 3:
			BinaryInsertSort(list, list.Length());
			cout << setiosflags(ios::left) << setw(WIDTH) << "折半插入排序";
			break;
		case 4:
			ShellSort(list, list.Length());
			cout << setiosflags(ios::left) << setw(WIDTH) << "希尔排序";
			break;
		case 5:
			QuickSort(list, 0, list.Length() - 1);
			cout << setiosflags(ios::left) << setw(WIDTH) << "快速排序";
			break;
		case 6:
			QuickSortX(list, 0, list.Length() - 1);
			cout << setiosflags(ios::left) << setw(WIDTH) << "改进快速排序";
			break;
		case 7:
			HeapSort(list, list.Length());
			cout << setiosflags(ios::left) << setw(WIDTH) << "堆排序";
			break;
		case 8:
			{
				DataList<int> list2(list);
				MergeSort(list, list2, 0, list.Length() - 1);
				cout << setiosflags(ios::left) << setw(WIDTH) << "归并排序";
			}
			break;
		case 9:
			LSDSort(list, 5);
			cout << setiosflags(ios::left) << setw(WIDTH) << "LSD基数排序";
			break;
		case 10:
			sort(&list[0], &list[list.Length() - 1]);
			cout << setiosflags(ios::left) << setw(WIDTH) << "内置sort";
			break;
		case 11:
			qsort(&list[0],list.Length(), sizeof(int), cmp);
			cout << setiosflags(ios::left) << setw(WIDTH) << "内置qsort";
			break;
		}
		chrono::steady_clock::time_point t2 = chrono::steady_clock::now();  /*计时终点*/
		chrono::duration<double> time_used = chrono::duration_cast<chrono::duration<double>>(t2 - t1);  /*计算时长*/

		cout << setw(WIDTH) << time_used.count() * 1000 << setw(WIDTH) << Compare << setw(WIDTH) << Move << endl;  /*输出比较和移动次数*/
	}
}

int main()
{
	srand((unsigned int)(time(0)));
	bool if_on = true;
	int num = 0;

	while (if_on) {
		cls();
		print_menu();
		cout << endl;

		while (1) {
			cout << "请输入要产生的随机数的个数（输入0退出）：";
			cin >> num;
			cin.ignore(INT_MAX, '\n');

			if (cin.good() && num >= 0 && num <= INT_MAX)
				break;

			cin.clear();
			cin.ignore(INT_MAX, '\n');
		}

		if (!num)
			break;

		DataList<int> data(num);

		for (int times = 0; times < 3; times++) {
			cout << "===================================================================================\n";

			switch (times) {
			case 0:
				cout << "                                       随机序列                                     \n";
				break;
			case 1:
				cout << "                                       升序序列                                     \n";
				break;
			case 2:
				cout << "                                       降序序列                                     \n";
				break;
			}
			cout << "===================================================================================\n";
			cout << "排序算法                 用时(ms)                 比较次数                 移动次数\n";
			data.clear();

			for (int i = 0; i < num; i++) {
				switch (times) {
				case 0:
					data.push_back(rand());  /*生成随机序列*/
					break;
				case 1:
					data.push_back(i);  /*生成正序序列*/
					break;
				case 2:
					data.push_back(num - i);  /*生成倒序序列*/
					break;
				}
			}
			
			conduct_sort(data);  /*序列生成完成后，轮流进行各类排序*/
			cout << "===================================================================================\n\n";
		}

		pause();
	}

	return 0;
}