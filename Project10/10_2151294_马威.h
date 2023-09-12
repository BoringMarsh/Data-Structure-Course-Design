#ifndef _MAXHEAP_H_
#define _MAXHEAP_H_

#include<iostream>
#include<stdlib.h>
using namespace std;

#define MAXHEAP_DEFAULT_SIZE 20

long long Move;
long long Compare;

template<class _class>
class MaxHeap {
private:
	_class* data;
	int size;
	int maxSize;

	void siftDown(const int start, const int m);
	void siftUp(const int start);
	void siftAll();
public:
	MaxHeap(const int sz = MAXHEAP_DEFAULT_SIZE);
	MaxHeap(_class arr[], const int n);
	~MaxHeap();

	bool GetMax(_class& x)const;
	int GetSize()const;
	int GetCapacity()const;
	bool Insert(const _class x);
	bool RemoveMax(_class& x);
	bool IsEmpty()const;
	bool IsFull()const;
	void MakeEmpty();
	void Output()const;

	_class& operator[](const int pos)const;
};

//仅指定大小的构造函数
template<class _class>
MaxHeap<_class>::MaxHeap(const int sz)
{
	maxSize = (MAXHEAP_DEFAULT_SIZE < sz) ? sz : MAXHEAP_DEFAULT_SIZE;

	data = new _class[maxSize];
	if (data == NULL) {
		cerr << "存储分配失败！" << endl;
		exit(1);
	}

	size = 0;
}

//给定数据和大小的构造函数
template<class _class>
MaxHeap<_class>::MaxHeap(_class arr[], const int n)
{
	maxSize = (MAXHEAP_DEFAULT_SIZE < n) ? n : MAXHEAP_DEFAULT_SIZE;

	data = new _class[maxSize];
	if (data == NULL) {
		cerr << "存储分配失败！" << endl;
		exit(1);
	}

	for (int i = 0; i < n; i++) {
		data[i] = arr[i];
		Move++;
	}

	size = n;

	int current = (size - 2) / 2;

	while (current >= 0) {
		siftDown(current, size - 1);
		current--;
	}
}

//析构函数
template<class _class>
MaxHeap<_class>::~MaxHeap()
{
	delete[] data;
}

//从start到m下滑调整成为最大堆
template<class _class>
void MaxHeap<_class>::siftDown(const int start, const int m)
{
	if (start < 0 || start > size || m < 0 || m > size)
		return;

	int i = start, j = 2 * i + 1;
	_class temp = data[i];

	while (j <= m) {
		if (j < m && (Compare++, data[j] < data[j + 1]))
			j++;

		if (Compare++, temp >= data[j])
			break;
		else {
			data[i] = data[j];
			Move++;
			i = j;
			j = 2 * j + 1;
		}
	}

	data[i] = temp;
}

//从start到0上滑调整成为最大堆
template<class _class>
void MaxHeap<_class>::siftUp(const int start)
{
	int j = start, i = (j - 1) / 2;
	_class temp = data[j];

	while (j > 0) {
		if (data[i] >= temp)
			break;
		else {
			data[j] = data[i];
			j = i;
			i = (i - 1) / 2;
		}
	}

	data[j] = temp;
}

//直接调整为最大堆
template<class _class>
void MaxHeap<_class>::siftAll()
{
	if (IsEmpty())
		return;

	int current = (size - 2) / 2;

	while (current >= 0) {
		siftDown(current, size - 1);
		current--;
	}
}

//取堆顶元素，即最大元素
template<class _class>
bool MaxHeap<_class>::GetMax(_class& x)const
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
int MaxHeap<_class>::GetSize()const
{
	return size;
}

//取堆的最大容量
template<class _class>
int MaxHeap<_class>::GetCapacity()const
{
	return maxSize;
}

//将x插入最大堆中
template<class _class>
bool MaxHeap<_class>::Insert(const _class x)
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

//删除堆顶上的最大元素
template<class _class>
bool MaxHeap<_class>::RemoveMax(_class& x)
{
	if (!size) {
		cout << "堆为空！" << endl;
		return false;
	}

	x = data[0];
	data[0] = data[size - 1];
	Move++;

	size--;
	siftDown(0, size - 1);
	return true;
}

//判断堆是否为空
template<class _class>
bool MaxHeap<_class>::IsEmpty()const
{
	return (size == 0);
}

//判断堆是否已满
template<class _class>
bool MaxHeap<_class>::IsFull()const
{
	return(size == maxSize);
}

//置空堆
template<class _class>
void MaxHeap<_class>::MakeEmpty()
{
	size = 0;
}

//按数组内存放顺序输出堆内的元素
template<class _class>
void MaxHeap<_class>::Output()const
{
	for (int i = 0; i < size; i++)
		cout << data[i] << endl;
}

//重载函数：下标访问
template<class _class>
_class& MaxHeap<_class>::operator[](const int pos)const
{
	if (pos < 0 || pos >= size) {
		cout << "位置错误！" << endl;
		return data[0];
	}

	return data[pos];
}

#endif

#ifndef _LINKEDQUEUE_H_
#define _LINKEDQUEUE_H_

#include<iostream>
using namespace std;

template<class _class>
struct QueueNode {
	_class data;
	QueueNode<_class>* link;

	QueueNode(QueueNode<_class>* ptr = NULL) :link(ptr) {}
	QueueNode(const _class& item, QueueNode<_class>* ptr = NULL) :data(item), link(ptr) {}
};

template<class _class>
class LinkedQueue {
private:
	QueueNode<_class>* front, * rear;
	int size;
public:
	LinkedQueue() :front(NULL), rear(NULL), size(0) {}
	~LinkedQueue();

	bool EnQueue(const _class x);
	bool DeQueue(_class& x);
	bool getFront(_class& x)const;
	void makeEmpty();
	bool IsEmpty()const;
	int getSize()const;
	void output()const;
	
	friend ostream& operator<<(ostream& ostr, const LinkedQueue<_class>& LQ)
	{
		QueueNode<_class>* p = LQ.front;

		while (p != NULL) {
			ostr << p->data << endl;
			p = p->link;
		}

		return ostr;
	}
};

//析构函数
template<class _class>
LinkedQueue<_class>::~LinkedQueue()
{
	makeEmpty();
}

//入队
template<class _class>
bool LinkedQueue<_class>::EnQueue(const _class x)
{
	if (front == NULL) {
		front = rear = new QueueNode<_class>(x);

		if (front == NULL)
			return false;
	}

	else {
		rear->link = new QueueNode<_class>(x);

		if (rear->link == NULL)
			return false;

		rear = rear->link;
	}

	size++;
	return true;
}

//出队
template<class _class>
bool LinkedQueue<_class>::DeQueue(_class& x)
{
	if (IsEmpty())
		return false;

	QueueNode<_class>* p = front;
	x = front->data;
	front = front->link;

	size--;

	if (!size)
		rear = NULL;

	delete p;
	return true;
}

//取队列第一个元素
template<class _class>
bool LinkedQueue<_class>::getFront(_class& x)const
{
	if (IsEmpty())
		return false;

	x = front->data;
	return true;
}

//清空队列
template<class _class>
void LinkedQueue<_class>::makeEmpty()
{
	QueueNode<_class>* p = front;

	while (front != NULL) {
		p = front;
		front = front->link;
		delete p;
	}

	size = 0;
}

//判断队列是否为空
template<class _class>
bool LinkedQueue<_class>::IsEmpty()const
{
	return (front == NULL);
}

//取队列中元素个数
template<class _class>
int LinkedQueue<_class>::getSize()const
{
	return size;
}

//输出队列中所有元素
template<class _class>
void LinkedQueue<_class>::output()const
{
	QueueNode<_class>* p = front;

	while (p != NULL) {
		cout << p->data << endl;
		p = p->link;
	}
}

#endif

#ifndef _SORT_H_
#define _SORT_H_

#include<iostream>

#define DATALIST_DEFAULT_SIZE 100
#define QUICKSORT_M 25

template<class _class>
class DataList {
private:
	int maxSize;
	int size;
	_class* data;
public:
	DataList(const int sz = DATALIST_DEFAULT_SIZE);
	DataList(_class* datas, const int sz);
	DataList(const DataList<_class>& list);
	~DataList();

	void expand(const int _timesOfExpandingDefaultSize);
	void push_back(const _class item);
	bool pop_back(_class& x);
	void clear();
	void Swap(_class& x1, _class& x2)const;
	int Length()const;

	_class& operator[](const int pos);
	DataList<_class>& operator=(const DataList<_class>& list);
};

//排序表：仅指定大小的构造函数
template<class _class>
DataList<_class>::DataList(const int sz)
{
	maxSize = sz;
	size = 0;

	data = new _class[maxSize];
	if (data == NULL) {
		cerr << "存储分配失败！" << endl;
		exit(1);
	}
}

//排序表：给定数据及大小的构造函数
template<class _class>
DataList<_class>::DataList(_class* datas, const int sz)
{
	size = maxSize = sz;

	data = new _class[maxSize];
	if (data == NULL) {
		cerr << "存储分配失败！" << endl;
		exit(1);
	}

	for (int i = 0; i < sz; i++)
		data[i].key = datas[i];
}

//排序表：复制构造函数
template<class _class>
DataList<_class>::DataList(const DataList<_class>& list)
{
	maxSize = list.maxSize;
	size = list.size;

	data = new _class[maxSize];
	if (data == NULL) {
		cerr << "存储分配错误！" << endl;
		exit(1);
	}

	for (int i = 0; i < size; i++)
		data[i] = list.data[i];
}

//排序表：析构函数
template<class _class>
DataList<_class>::~DataList()
{
	delete[] data;
}

//扩大数组空间
template<class _class>
void DataList<_class>::expand(const int _timesOfExpandingDefaultSize)
{
	if (_timesOfExpandingDefaultSize <= 0)
		return;

	maxSize += _timesOfExpandingDefaultSize * DATALIST_DEFAULT_SIZE;
	_class* oldData = data;
	
	data = new _class[maxSize];
	if (data == NULL) {
		cerr << "存储分配错误！" << endl;
		exit(1);
	}

	for (int i = 0; i < size; i++) {
		data[i] = oldData[i];
	}

	delete[] oldData;
}

//排序表：向末尾插入一个元素
template<class _class>
void DataList<_class>::push_back(const _class item)
{
	if (size == maxSize)
		expand(1);

	data[size++] = item;
}

//排序表：移走末尾最后一个元素
template<class _class>
bool DataList<_class>::pop_back(_class& x)
{
	if (size == 0)
		return false;

	x = data[--size];
	return true;
}

//排序表：清空排序表
template<class _class>
void DataList<_class>::clear()
{
	size = 0;
}

//排序表：交换两个元素
template<class _class>
void DataList<_class>::Swap(_class& x1, _class& x2)const
{
	_class temp = x1;
	x1 = x2;
	x2 = temp;
}

//排序表：取表的长度
template<class _class>
int DataList<_class>::Length()const
{
	return size;
}

//在表前端、尾端与中间点三者取中值，交换到尾端（改进快速排序专用）
template<class _class>
_class& Partition_median3(DataList<_class>& list, const int left, const int right)
{
	int mid = (left + right) / 2;
	int k = left;

	if (Compare++, list[mid] < list[k])
		k = mid;
	if (Compare++, list[right] < list[k])
		k = right;

	if (k != left) {
		list.Swap(list[k], list[left]);
		Move += 3;
	}
	if (mid != right && (Compare++, list[mid] < list[right])) {
		list.Swap(list[mid], list[right]);
		Move += 3;
	}

	return list[right];
}

//划分（快速排序专用）
template<class _class>
int Partition(DataList<_class>& list, const int low, const int high)
{
	int pivotpos = low;
	_class pivot = list[low];

	for (int i = low + 1; i <= high; i++) {
		if (Compare++, list[i] < pivot) {
			pivotpos++;

			if(pivotpos != i) {
				list.Swap(list[pivotpos], list[i]);
				Move += 3;
			}
		}
	}

	list[low] = list[pivotpos];
	list[pivotpos] = pivot;
	Move += 2;
	return pivotpos;
}

//划分（改进快速排序专用）
template<class _class>
int PartitionX(DataList<_class>& list, const int left, const int right)
{
	int i = left, j = right - 1;

	if (left < right) {
		_class pivot = Partition_median3(list, left, right);

		while (1) {
			while (i < j && (Compare++, list[i] < pivot))
				i++;
			while (i < j && (Compare++, pivot < list[j]))
				j--;

			if (i < j) {
				list.Swap(list[i], list[j]);
				Move += 3;
				i++; j--;
			}
			else
				break;
		}

		if (Compare++, list[i] > pivot) {
			list[right] = list[i];
			list[i] = pivot;
			Move += 2;
		}
	}

	return i;
}

//重载函数：下标访问
template<class _class>
_class& DataList<_class>::operator[](const int pos)
{
	if (pos < 0 || pos >= size)
		return data[0];

	return data[pos];
}

//重载函数：赋值
template<class _class>
DataList<_class>& DataList<_class>::operator=(const DataList<_class>& list)
{
	delete[] data;
	maxSize = list.maxSize;
	size = list.size;

	data = new _class[maxSize];
	if (data == NULL) {
		cerr << "存储分配错误！" << endl;
		exit(1);
	}

	for (int i = 0; i < size; i++)
		data[i] = list.data[i];
}

//冒泡排序
template<class _class>
void BubbleSort(DataList<_class>& list, const int num)
{
	for (int i = 0; i < num; i++) {
		for (int j = num - 1; j >= i; j--) {
			if (Compare++, list[j - 1] > list[j]) {
				list.Swap(list[j - 1], list[j]);
				Move += 3;
			}
		}
	}
}

//选择排序
template<class _class>
void SelectSort(DataList<_class>& list, const int num)
{
	for (int i = 0; i < num - 1; i++) {
		int k = i;

		for (int j = i + 1; j < num; j++)
			if (Compare++, list[j] < list[k]) {
				k = j;
			}

		if (i != k) {
			list.Swap(list[i], list[k]);
			Move += 3;
		}
	}
}

//直接插入排序
template<class _class>
void InsertSort(DataList<_class>& list, const int num)
{
	_class temp;

	for (int i = 1; i < num; i++) {
		if (Compare++, list[i] < list[i - 1]) {
			temp = list[i];
			Move++;
			int j = i - 1;

			do {
				list[j + 1] = list[j];
				Move++;
				j--;
			} while (j >= 0 && (Compare++, temp < list[j]));

			list[j + 1] = temp;
			Move++;
		}
	}
}

//折半插入排序
template<class _class>
void BinaryInsertSort(DataList<_class>& list, const int num)
{
	_class temp;

	for (int i = 1; i < num; i++) {  /*从1开始：后num-1个数排好，第1个数也排好*/
		temp = list[i];
		Move++;
		int low = 0, high = i - 1;

		while (low <= high) {
			int middle = (low + high) / 2;

			if (Compare++, temp < list[middle])
				high = middle - 1;
			else
			 	low = middle + 1;
		}

		for (int j = i - 1; j >= low; j--) {
			list[j + 1] = list[j];
			Move++;
		}

		list[low] = temp;
		Move++;
	}
}

//希尔排序
template<class _class>
void ShellSort(DataList<_class>& list, const int num)
{
	int gap = num;
	_class temp;

	do {
		gap = gap / 3 + 1;  /*缩小gap的方式为gap=[gap/3+1]*/

		for (int i = gap; i <= num - 1; i++) {
			if (Compare++, list[i] < list[i - gap]) {
				temp = list[i];
				Move++;
				int j = i - gap;

				do {
					list[j + gap] = list[j];
					Move++;
					j = j - gap;
				} while (j >= 0 && (Compare++, temp < list[j]));

				list[j + gap] = temp;
				Move++;
			}
		}
	} while (gap > 1);  /*gap等于1时停止循环，排序结束*/
}

//快速排序
template<class _class>
void QuickSort(DataList<_class>& list, const int left, const int right)
{
	if (left < right) {
		int pivotpos = Partition(list, left, right);
		QuickSort(list, left, pivotpos - 1);
		QuickSort(list, pivotpos + 1, right);
	}
}


//改进快速排序
template<class _class>
void QuickSortX(DataList<_class>& list, const int left, const int right)
{
	if (left < right) {
		int pivotpos = PartitionX(list, left, right);
		QuickSortX(list, left, pivotpos - 1);
		QuickSortX(list, pivotpos + 1, right);
	}
}

//堆排序
template<class _class>
void HeapSort(DataList<_class>& list, const int num)
{
	MaxHeap<_class> heap(&list[0], num);
	_class _max;
	const int size = heap.GetSize();

	for (int i = 0; i < size; i++) {
		heap.RemoveMax(_max);
		list[num - 1 - i] = _max;
		Move++;
	}
}

//归并排序：将两路进行归并
template<class _class>
void Merge(DataList<_class>& L1, DataList<_class>& L2, const int left, const int mid, const int right)
{
	for (int k = left; k <= right; k++) {
		L2[k] = L1[k];
		Move++;
	}

	int s1 = left, s2 = mid + 1, t = left;

	while (s1 <= mid && s2 <= right) {
		if (Compare++, L2[s1] <= L2[s2]) {
			L1[t++] = L2[s1++];
			Move++;
		}
		else {
			L1[t++] = L2[s2++];
			Move++;
		}
	}

	while (s1 <= mid) {
		L1[t++] = L2[s1++];
		Move++;
	}

	while (s2 <= right) {
		L1[t++] = L2[s2++];
		Move++;
	}
}

//归并排序
template<class _class>
void MergeSort(DataList<_class>& L1, DataList<_class>& L2, const int left, const int right)
{
	if (left >= right)
		return;

	int mid = (left + right) / 2;
	MergeSort(L1, L2, left, mid);
	MergeSort(L1, L2, mid + 1, right);
	Merge(L1, L2, left, mid, right);
}

//取元素的某一位数（0为最低位）
template<class _class>
int getDigit(_class x, const int i)
{
	int num = 1;

	for (int j = 0; j < i; j++)
		num *= 10;

	if (x < num)
		return 0;

	return (x - x % num) / num % 10;
}

//LSD基数排序
template<class _class>
void LSDSort(DataList<_class>& list, int d)
{
	LinkedQueue<_class> queues[10];

	for (int i = 0; i < d; i++) {
		for (int j = 0; j < list.Length(); j++) {
			const int k = getDigit(list[j], i);
			queues[k].EnQueue(list[j]);
			Move++;
		}

		int current = 0;

		for (int j = 0; j < 10; j++) {
			while (!queues[j].IsEmpty()) {
				queues[j].DeQueue(list[current]);
				Move++;
				current++;
			}
		}
	}
}

#endif