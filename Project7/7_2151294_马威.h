#ifndef _MINHEAP_H_
#define _MINHEAP_H_

#include<iostream>
#include<stdlib.h>
using namespace std;

#define MINHEAP_DEFAULT_SIZE 20

template<class _class>
class MinHeap {
private:
	_class* data;
	int size;
	int maxSize;

	void siftDown(const int start, const int m);
	void siftUp(const int start);
	void siftAll();
public:
	MinHeap(const int sz = MINHEAP_DEFAULT_SIZE);
	MinHeap(_class arr[], const int n);
	~MinHeap();

	bool GetMin(_class& x)const;
	int GetSize()const;
	int GetCapacity()const;
	bool Insert(const _class x);
	bool RemoveMin(_class& x);
	bool IsEmpty()const;
	bool IsFull()const;
	void MakeEmpty();
	void Output()const;

	_class& operator[](const int pos)const;
};

//仅指定大小的构造函数
template<class _class>
MinHeap<_class>::MinHeap(const int sz)
{
	maxSize = (MINHEAP_DEFAULT_SIZE < sz) ? sz : MINHEAP_DEFAULT_SIZE;

	data = new _class[maxSize];
	if (data == NULL) {
		cerr << "存储分配失败！" << endl;
		exit(1);
	}

	size = 0;
}

//给定数据和大小的构造函数
template<class _class>
MinHeap<_class>::MinHeap(_class arr[], const int n)
{
	maxSize = (MINHEAP_DEFAULT_SIZE < n) ? n : MINHEAP_DEFAULT_SIZE;

	data = new _class[maxSize];
	if (data == NULL) {
		cerr << "存储分配失败！" << endl;
		exit(1);
	}

	for (int i = 0; i < n; i++)
		data[i] = arr[i];

	size = n;

	int current = (size - 2) / 2;

	while (current >= 0) {
		siftDown(current, size - 1);
		current--;
	}
}

//析构函数
template<class _class>
MinHeap<_class>::~MinHeap()
{
	delete[] data;
}

//从start到m下滑调整成为最小堆
template<class _class>
void MinHeap<_class>::siftDown(const int start, const int m)
{
	if (start < 0 || start > size || m < 0 || m > size)
		return;

	int i = start, j = 2 * i + 1;
	_class temp = data[i];

	while (j <= m) {
		if (j < m && data[j] > data[j + 1])
			j++;

		if (temp <= data[j])
			break;
		else {
			data[i] = data[j];
			i = j;
			j = 2 * j + 1;
		}
	}

	data[i] = temp;
}

//从start到0上滑调整成为最小堆
template<class _class>
void MinHeap<_class>::siftUp(const int start)
{
	int j = start, i = (j - 1) / 2;
	_class temp = data[j];

	while (j > 0) {
		if (data[i] <= temp)
			break;
		else {
			data[j] = data[i];
			j = i;
			i = (i - 1) / 2;
		}
	}

	data[j] = temp;
}

//直接调整为最小堆
template<class _class>
void MinHeap<_class>::siftAll()
{
	if (IsEmpty())
		return;

	int current = (size - 2) / 2;

	while (current >= 0) {
		siftDown(current, size - 1);
		current--;
	}
}

//取堆顶元素，即最小元素
template<class _class>
bool MinHeap<_class>::GetMin(_class& x)const
{
	if (IsEmpty()) {
		cout << "堆为空！" << endl;
		return false;
	}

	x = data[0];
	return true;
}

//取堆的当前大小
template<class _class>
int MinHeap<_class>::GetSize()const
{
	return size;
}

//取堆的最大容量
template<class _class>
int MinHeap<_class>::GetCapacity()const
{
	return maxSize;
}

//将x插入最小堆中
template<class _class>
bool MinHeap<_class>::Insert(const _class x)
{
	if (size == maxSize) {
		cout << "堆已满！" << endl;
		return false;
	}

	data[size] = x;
	siftUp(size);

	size++;
	return true;
}

//删除堆顶上的最小元素
template<class _class>
bool MinHeap<_class>::RemoveMin(_class& x)
{
	if (!size) {
		cout << "堆为空！" << endl;
		return false;
	}

	x = data[0];
	data[0] = data[size - 1];

	size--;
	siftDown(0, size - 1);
	//siftAll();
	return true;
}

//判断堆是否为空
template<class _class>
bool MinHeap<_class>::IsEmpty()const
{
	return (size == 0);
}

//判断堆是否已满
template<class _class>
bool MinHeap<_class>::IsFull()const
{
	return(size == maxSize);
}

//置空堆
template<class _class>
void MinHeap<_class>::MakeEmpty()
{
	size = 0;
}

//按数组内存放顺序输出堆内的元素
template<class _class>
void MinHeap<_class>::Output()const
{
	for (int i = 0; i < size; i++)
		cout << data[i] << endl;
}

//重载函数：下标访问
template<class _class>
_class& MinHeap<_class>::operator[](const int pos)const
{
	if (pos < 0 || pos >= size) {
		cout << "位置错误！" << endl;
		return data[0];
	}

	return data[pos];
}

#endif