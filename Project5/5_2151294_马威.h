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