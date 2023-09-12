#ifndef _VECTOR_H_
#define _VECTOR_H_

#include<iostream>
#include<stdlib.h>
using namespace std;

#define VECTOR_DEFAULT_SIZE 32

template<class _class>
class Vector {
private:
	int size;
	int maxSize;
	_class* data;

	_class* rangeCopy(_class* data, const int low, const int high, int sz = 0);
	void expand(const int _timesOfExpandingDefaultSize);
public:
	Vector();
	Vector(const int sz);
	Vector(const Vector& v);
	Vector(const int sz, const _class item);
	~Vector();

	class iterator {
	private:
		_class* data;
	public:
		iterator() :data(NULL) {}
		iterator(_class* datas) :data(datas) {}
		
		//读取数据
		_class& operator*()const
		{
			return *data;
		}
		//比较运算符：相等
		inline bool operator==(const iterator& v)const
		{
			return (data == v.data);
		}
		//比较运算符：大于
		inline bool operator>(const iterator& v)const
		{
			return (data > v.data);
		}
		//比较运算符：小于
		inline bool operator<(const iterator& v)const
		{
			return (data < v.data);
		}
		//比较运算符：大于等于
		inline bool operator>=(const iterator& v)const
		{
			return (data >= v.data);
		}
		//比较运算符：小于等于
		inline bool operator<=(const iterator& v)const
		{
			return (data <= v.data);
		}
		//比较运算符：不等于
		inline bool operator!=(const iterator& v)const
		{
			return (data != v.data);
		}
		//自增运算符：前置
		Vector<_class>::iterator& operator++()
		{
			data++;
			return *this;
		}
		//自增运算符：后置
		Vector<_class>::iterator operator++(int)
		{
			Vector<_class>::iterator old = *this;
			++(*this);
			return old;
		}
		//自减运算符：前置
		Vector<_class>::iterator& operator--()
		{
			data--;
			return *this;
		}
		//自减运算符：后置
		Vector<_class>::iterator operator--(int)
		{
			Vector<_class>::iterator old = *this;
			--(*this);
			return old;
		}
		//加法：迭代器+数字
		Vector<_class>::iterator operator+(const int num)
		{
			Vector<_class>::iterator old = *this;

			for (int i = 0; i < num; i++)
				old++;

			return old;
		}
		//加法：数字+迭代器
		friend typename Vector<_class>::iterator operator+(const int num, const typename Vector<_class>::iterator v)
		{
			typename Vector<_class>::iterator old = v;

			for (int i = 0; i < num; i++)
				++old;

			return old;
		}
		//减法：迭代器-数字
		Vector<_class>::iterator operator-(const int num)
		{
			Vector<_class>::iterator old = *this;

			for (int i = 0; i < num; i++)
				old--;

			return old;
		}
		//减法：数字-迭代器
		friend typename Vector<_class>::iterator operator-(const int num, const typename Vector<_class>::iterator v)
		{
			typename Vector<_class>::iterator old = v;

			for (int i = 0; i < num; i++)
				--old;

			return old;
		}
		//自加
		Vector<_class>::iterator operator+=(const int num)
		{
			for (int i = 0; i < num; i++)
				++(*this);

			return *this;
		}
		//自减
		Vector<_class>::iterator operator-=(const int num)
		{
			for (int i = 0; i < num; i++)
				--(*this);

			return *this;
		}
		//迭代器相减
		friend int operator-(const Vector<_class>::iterator v1, const Vector<_class>::iterator v2)
		{
			return (v1.data - v2.data);
		}
	};

	int length()const;
	int capacity()const;
	bool empty()const;
	void push_back(const _class item);
	bool pop_back(_class& x);
	bool insert(const Vector<_class>::iterator place, const _class item);
	bool erase(const Vector<_class>::iterator place);
	void fill(const int sz, const _class item);
	void clear();
	
	inline Vector<_class>::iterator begin();
	inline const Vector<_class>::iterator begin()const;
	inline Vector<_class>::iterator end();
	inline const Vector<_class>::iterator end()const;
	inline Vector<_class>::iterator last();
	inline const Vector<_class>::iterator last()const;

	_class& operator[](const int pos)const;
	Vector<_class>& operator=(const Vector<_class>& v);

	void reSize(const int sz);
	void output()const;
};

//从data中的[low,high)区域复制一段长为sz的数据
template<class _class>
_class* Vector<_class>::rangeCopy(_class* data, const int low, const int high, int sz)
{
	if (sz == 0)
		sz = high - low;

	_class* newData = new _class[sz];
	if (newData == NULL) {
		cerr << "存储分配错误！" << endl;
		exit(1);
	}

	for (int i = low; i < high; i++)
		newData[i] = data[low + i];

	return newData;
}

//默认构造函数
template<class _class>
Vector<_class>::Vector()
{
	maxSize = VECTOR_DEFAULT_SIZE;
	size = 0;

	data = new _class[maxSize];
	if (data == NULL) {
		cerr << "存储分配错误！" << endl;
		exit(1);
	}
}

//指定长度的构造函数
template<class _class>
Vector<_class>::Vector(const int sz)
{
	maxSize = sz;
	size = 0;

	data = new _class[maxSize];
	if (data == NULL) {
		cerr << "存储分配错误！" << endl;
		exit(1);
	}
}

//复制构造函数
template<class _class>
Vector<_class>::Vector(const Vector<_class>& v)
{
	maxSize = v.maxSize;
	size = v.size;

	data = rangeCopy(v.data, 0, size, maxSize);
}

//指定长度和所有元素初始值的构造函数
template<class _class>
Vector<_class>::Vector(const int sz, const _class item)
{
	if (sz < 0) {
		maxSize = VECTOR_DEFAULT_SIZE;
		size = 0;
	}

	else {
		maxSize = (sz / VECTOR_DEFAULT_SIZE + 1) * VECTOR_DEFAULT_SIZE;
		size = sz;
	}

	data = new _class[maxSize];
	if (data == NULL) {
		cerr << "存储分配错误！" << endl;
		exit(1);
	}

	for (int i = 0; i < sz; i++)
		data[i] = item;
}

//析构函数
template<class _class>
Vector<_class>::~Vector()
{
	delete[] data;
}

//返回容器内部数组的长度
template<class _class>
int Vector<_class>::length()const
{
	return size;
}

//返回容器最大容量
template<class _class>
int Vector<_class>::capacity()const
{
	return maxSize;
}

//扩大数组空间
template<class _class>
void Vector<_class>::expand(const int _timesOfExpandingDefaultSize)
{
	if (_timesOfExpandingDefaultSize <= 0)
		return;

	maxSize += _timesOfExpandingDefaultSize * VECTOR_DEFAULT_SIZE;
	
	_class* oldData = data;
	data = rangeCopy(data, 0, size, maxSize);

	delete[] oldData;
}

//判断容器是否为空
template<class _class>
bool Vector<_class>::empty()const
{
	return (size == 0);
}

//向容器末尾压入一个元素
template<class _class>
void Vector<_class>::push_back(const _class item)
{
	if (size == maxSize)
		expand(1);

	data[size++] = item;
}

//弹出容器末尾元素
template<class _class>
bool Vector<_class>::pop_back(_class& x)
{
	if (size == 0)
		return false;

	x = data[--size];
	return true;
}

//在迭代器位置place上插入元素item，此位置及其后续元素后移一位
template<class _class>
bool Vector<_class>::insert(const Vector<_class>::iterator place, const _class item)
{
	const int index = place - begin();

	if (index < 0 || index > size)
		return false;
	else if (index == size) {
		push_back(item);
		return true;
	}

	if (size == maxSize)
		expand(1);

	for (int i = size - 1; i >= index; i--)
		data[i + 1] = data[i];

	data[index] = item;
	size++;
	
	return true;
}

//删除迭代器位置place上的元素
template<class _class>
bool Vector<_class>::erase(const Vector<_class>::iterator place)
{
	int index = place - begin();

	if (index < 0 || index >= size)
		return false;

	while (index < size - 1) {
		data[index] = data[index + 1];
		index++;
	}

	size--;
	return true;
}

//指定容器数组长度，并用元素item进行填充
template<class _class>
void Vector<_class>::fill(const int sz, const _class item)
{
	delete[] data;

	if (sz < 0) {
		maxSize = VECTOR_DEFAULT_SIZE;
		size = 0;
	}

	else {
		maxSize = (sz / VECTOR_DEFAULT_SIZE + 1) * VECTOR_DEFAULT_SIZE;
		size = sz;
	}

	data = new _class[maxSize];
	if (data == NULL) {
		cerr << "存储分配错误！" << endl;
		exit(1);
	}

	for (int i = 0; i < sz; i++)
		data[i] = item;
}

//将容器置为空
template<class _class>
void Vector<_class>::clear()
{
	delete[] data;

	maxSize = VECTOR_DEFAULT_SIZE;
	size = 0;

	data = new _class[maxSize];
	if (data == NULL) {
		cerr << "存储分配错误！" << endl;
		exit(1);
	}
}

//返回容器的起始位置
template<class _class>
inline typename Vector<_class>::iterator Vector<_class>::begin()
{
	return size > 0 ? typename Vector<_class>::iterator(&data[0]) : typename Vector<_class>::iterator();
}

//返回容器的起始位置
template<class _class>
inline const typename Vector<_class>::iterator Vector<_class>::begin()const
{
	return size > 0 ? typename Vector<_class>::iterator(&data[0]) : typename Vector<_class>::iterator();
}

//返回容器末尾的后一个位置
template<class _class>
inline typename Vector<_class>::iterator Vector<_class>::end()
{
	return size > 0 ? ++(typename Vector<_class>::iterator(&data[size - 1])) : typename Vector<_class>::iterator();
}

//返回容器末尾的后一个位置
template<class _class>
inline const typename Vector<_class>::iterator Vector<_class>::end()const
{
	return size > 0 ? ++(typename Vector<_class>::iterator(&data[size - 1])) : typename Vector<_class>::iterator();
}

//返回容器的末尾位置
template<class _class>
inline typename Vector<_class>::iterator Vector<_class>::last()
{
	return size > 0 ? typename Vector<_class>::iterator(&data[size - 1]) : typename Vector<_class>::iterator();
}

//返回容器的末尾位置
template<class _class>
inline const typename Vector<_class>::iterator Vector<_class>::last()const
{
	return size > 0 ? typename Vector<_class>::iterator(&data[size - 1]) : typename Vector<_class>::iterator();
}

//重载函数：下标访问
template<class _class>
_class& Vector<_class>::operator[](const int pos)const
{
	if (pos < 0 || pos >= size) {
		cerr << "位置错误！" << endl;
		return data[0];
	}

	return data[pos];
}

//重载函数：赋值
template<class _class>
Vector<_class>& Vector<_class>::operator=(const Vector<_class>& v)
{
	delete[] data;

	maxSize = v.maxSize;
	size = v.size;

	data = new _class[maxSize];
	if (data == NULL) {
		cerr << "存储分配错误！" << endl;
		exit(1);
	}

	for (int i = 0; i < size; i++)
		data[i] = v.data[i];

	return *this;
}

//调整容器的数组长度
template<class _class>
void Vector<_class>::reSize(const int sz)
{
	while (size < sz) {
		if (size == maxSize)
			expand(1);

		size++;
	}

	while (size > sz)
		size--;
}

//输出容器内元素的值
template<class _class>
void Vector<_class>::output()const
{
	for (int i = 0; i < size; i++)
		cout << data[i] << endl;
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

#ifndef _LINKEDSTACK_H_
#define _LINKEDSTACK_H_

#include<iostream>
#include<assert.h>
using namespace std;

template<class _class>
struct StackNode {
	_class data;
	StackNode<_class>* link;

	StackNode(StackNode<_class>* ptr = NULL) :link(ptr) {}
	StackNode(const _class& item, StackNode<_class>* ptr = NULL) :data(item), link(ptr) {}
};

template<class _class>
class LinkedStack {
private:
	StackNode<_class>* top;  //栈顶指针，即链头指针
	int size;
public:
	LinkedStack() :top(NULL), size(0) {}  //默认构造函数
	~LinkedStack();

	void push(const _class x);
	bool pop(_class& x);
	bool getTop(_class& x)const;
	bool IsEmpty()const;
	int getSize()const;
	void makeEmpty();
};

//析构函数
template<class _class>
LinkedStack<_class>::~LinkedStack()
{
	makeEmpty();
}

//进栈
template<class _class>
void LinkedStack<_class>::push(const _class x)
{
	top = new StackNode<_class>(x, top);
	assert(top != NULL);
	size++;
}

//出栈
template<class _class>
bool LinkedStack<_class>::pop(_class& x)
{
	if (IsEmpty())
		return false;

	StackNode<_class>* p = top;
	top = top->link;
	x = p->data;

	size--;
	delete p;
	return true;
}

//取栈顶元素
template<class _class>
bool LinkedStack<_class>::getTop(_class& x)const
{
	if (IsEmpty())
		return false;

	x = top->data;
	return true;
}

//判栈空否
template<class _class>
bool LinkedStack<_class>::IsEmpty()const
{
	return (top == NULL);
}

//返回栈元素个数
template<class _class>
int LinkedStack<_class>::getSize()const
{
	return size;
}

//清空栈的内容
template<class _class>
void LinkedStack<_class>::makeEmpty()
{
	StackNode<_class>* p;

	while (top != NULL) {
		p = top;
		top = top->link;
		delete p;
	}

	size = 0;
}

#endif

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