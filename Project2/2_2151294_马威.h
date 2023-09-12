#ifndef _LINKEDSET_H_
#define _LINKEDSET_H_

#include<iostream>
#include<stdlib.h>
using namespace std;

template<class _class>
struct SetNode {
	_class data;
	SetNode<_class>* link;

	SetNode() :link(NULL) {}
	SetNode(const _class value, SetNode<_class>* next = NULL) :data(value), link(next) {}
};

template<class _class>
class LinkedSet {
private:
	SetNode<_class>* first, * last;

public:
	LinkedSet();
	LinkedSet(const LinkedSet<_class>& LS);
	~LinkedSet();

	void makeEmpty();
	bool IsEmpty()const;
	bool IsFull()const;
	void Input(const _class endTag);
	void Output()const;
	bool addMember(const _class x);
	bool delMember(const _class x);

	LinkedSet<_class>& operator=(const LinkedSet<_class>& LS);
	LinkedSet<_class> operator+(const LinkedSet<_class>& LS);
	LinkedSet<_class> operator*(const LinkedSet<_class>& LS);
	LinkedSet<_class> operator-(const LinkedSet<_class>& LS);

	bool operator==(const LinkedSet<_class>& LS);
	bool Contains(const _class x);
	bool Min(_class& x);
	bool Max(_class& x);
};

//默认构造函数
template<class _class>
LinkedSet<_class>::LinkedSet()
{
	SetNode<_class>* newNode = new SetNode<_class>;
	if (newNode == NULL) {
		cerr << "存储分配错误！" << endl;
		exit(1);
	}

	first = last = newNode;
}

//复制构造函数
template<class _class>
LinkedSet<_class>::LinkedSet(const LinkedSet<_class>& LS)
{
	SetNode<_class>* srcptr = LS.first->link;
	SetNode<_class>* newHead = new SetNode<_class>;
	if (newHead == NULL) {
		cerr << "存储分配错误！" << endl;
		exit(1);
	}

	first = last = newHead;

	while (srcptr != NULL) {
		SetNode<_class>* newNode = new SetNode<_class>(srcptr->data);
		if (newNode == NULL) {
			cerr << "存储分配错误！" << endl;
			exit(1);
		}

		last->link = newNode;
		last = last->link;
		srcptr = srcptr->link;
	}

	last->link = NULL;
}

//析构函数
template<class _class>
LinkedSet<_class>::~LinkedSet()
{
	makeEmpty();
	delete first;
}

//将集合置空
template<class _class>
void LinkedSet<_class>::makeEmpty()
{
	if (IsEmpty())
		return;

	SetNode<_class>* current = first->link, * del;

	while (current != NULL) {
		del = current;
		current = current->link;
		delete del;
	}

	first->link = NULL;
	last = first;
}

//判断集合是否为空集
template<class _class>
bool LinkedSet<_class>::IsEmpty()const
{
	return (first == last);
}

//判断集合是否已满
template<class _class>
bool LinkedSet<_class>::IsFull()const
{
	return false;
}

//用输入方式给当前集合赋值
template<class _class>
void LinkedSet<_class>::Input(const _class endTag)
{
	makeEmpty();
	SetNode<_class>* current;
	_class value;

	while (1) {
		cin >> value;
		current = first;

		if (value == endTag)
			break;

		SetNode<_class>* newNode = new SetNode<_class>(value);
		if (newNode == NULL) {
			cerr << "存储分配错误！" << endl;
			return;
		}

		while (current->link != NULL && current->link->data < value)
			current = current->link;

		newNode->link = current->link;
		current->link = newNode;

		if (current == last)
			last = newNode;
	}
}

//输出当前集合中的所有元素
template<class _class>
void LinkedSet<_class>::Output()const
{
	if (IsEmpty()) {
		cout << "NULL" << endl;
		return;
	}

	SetNode<_class>* current = first->link;

	while (current != NULL) {
		cout << current->data << ' ';
		current = current->link;
	}

	cout << endl;
}

//把新元素x加入到集合之中。若集合中已有此元素，则加入失败，返回false，否则返回true
template<class _class>
bool LinkedSet<_class>::addMember(const _class x)
{
	SetNode<_class>* p = first->link, * pre = first;
	
	while (p != NULL && p->data < x) {
		pre = p;
		p = p->link;
	}

	if (p != NULL && p->data == x)
		return false;

	SetNode<_class>* newNode = new SetNode<_class>(x);
	if (newNode == NULL) {
		cerr << "存储分配失败！" << endl;
		exit(1);
	}

	newNode->link = p;
	pre->link = newNode;

	if (p == NULL)
		last = newNode;
	return true;

}

//把集合中成员x删去。若集合不空且元素x在集合中，删除成功，返回true，否则返回false
template<class _class>
bool LinkedSet<_class>::delMember(const _class x)
{
	SetNode<_class>* p = first->link, * pre = first;

	while (p != NULL && p->data < x) {
		pre = p;
		p = p->link;
	}

	if (p != NULL && p->data == x) {
		pre->link = p->link;

		if (p == last)
			last = pre;

		delete p;
		return true;
	}

	else
		return false;
}

//复制集合LS到当前集合
template<class _class>
LinkedSet<_class>& LinkedSet<_class>::operator=(const LinkedSet<_class>& LS)
{
	makeEmpty();
	delete first;

	SetNode<_class>* pb = LS.first->link;
	SetNode<_class>* newNode = new SetNode<_class>;
	if (newNode == NULL) {
		cerr << "存储分配失败！" << endl;
		exit(1);
	}

	SetNode<_class>* pa = first = newNode;

	while (pb != NULL) {
		SetNode<_class>* newNode = new SetNode<_class>(pb->data);
		if (newNode == NULL) {
			cerr << "存储分配失败！" << endl;
			exit(1);
		}

		pa->link = newNode;
		pa = pa->link;
		pb = pb->link;
	}

	pa->link = NULL;
	last = pa;
	return *this;
}

//求当前集合与LS的并
template<class _class>
LinkedSet<_class> LinkedSet<_class>::operator+(const LinkedSet<_class>& LS)
{
	SetNode<_class>* pa = first->link, * pb = LS.first->link;
	LinkedSet<_class> temp;
	SetNode<_class>* p, * pc = temp.first;

	while (pa != NULL && pb != NULL) {
		if (pa->data == pb->data) {
			SetNode<_class>* newNode = new SetNode<_class>(pa->data);
			if (newNode == NULL) {
				cerr << "存储分配失败！" << endl;
				exit(1);
			}

			pc->link = newNode;
			pa = pa->link;
			pb = pb->link;
		}

		else if (pa->data < pb->data) {
			SetNode<_class>* newNode = new SetNode<_class>(pa->data);
			if (newNode == NULL) {
				cerr << "存储分配失败！" << endl;
				exit(1);
			}

			pc->link = newNode;
			pa = pa->link;
		}

		else {
			SetNode<_class>* newNode = new SetNode<_class>(pb->data);
			if (newNode == NULL) {
				cerr << "存储分配失败！" << endl;
				exit(1);
			}

			pc->link = newNode;
			pb = pb->link;
		}

		pc = pc->link;
	}

	if (pa != NULL)
		p = pa;
	else
		p = pb;

	while (p != NULL) {
		SetNode<_class>* newNode = new SetNode<_class>(p->data);
		if (newNode == NULL) {
			cerr << "存储分配失败！" << endl;
			exit(1);
		}

		pc->link = newNode;
		pc = pc->link;
		p = p->link;
	}

	pc->link = NULL;
	temp.last = pc;
	return temp;
}

//求当前集合与LS的交
template<class _class>
LinkedSet<_class> LinkedSet<_class>::operator*(const LinkedSet<_class>& LS)
{
	SetNode<_class>* pa = first->link, * pb = LS.first->link;
	LinkedSet<_class> temp;
	SetNode<_class>* pc = temp.first;

	while (pa != NULL && pb != NULL) {
		if (pa->data == pb->data) {
			SetNode<_class>* newNode = new SetNode<_class>(pa->data);
			if (newNode == NULL) {
				cerr << "存储分配失败！" << endl;
				exit(1);
			}

			pc->link = newNode;
			pc = pc->link;
			pa = pa->link;
			pb = pb->link;
		}

		else if (pa->data < pb->data)
			pa = pa->link;
		else
			pb = pb->link;
	}

	pc->link = NULL;
	temp.last = pc;
	return temp;
}

//求当前集合与LS的差
template<class _class>
LinkedSet<_class> LinkedSet<_class>::operator-(const LinkedSet<_class>& LS)
{
	SetNode<_class>* pa = first->link, * pb = LS.first->link;
	LinkedSet<_class> temp;
	SetNode<_class>* pc = temp.first;

	while (pa != NULL && pb != NULL) {
		if (pa->data == pb->data) {
			pa = pa->link;
			pb = pb->link;
		}

		else if (pa->data < pb->data) {
			SetNode<_class>* newNode = new SetNode<_class>(pa->data);
			if (newNode == NULL) {
				cerr << "存储分配失败！" << endl;
				exit(1);
			}

			pc->link = newNode;
			pc = pc->link;
			pa = pa->link;
		}

		else
			pb = pb->link;
	}

	while (pa != NULL) {
		SetNode<_class>* newNode = new SetNode<_class>(pa->data);
		if (newNode == NULL) {
			cerr << "存储分配失败！" << endl;
			exit(1);
		}

		pc->link = newNode;
		pc = pc->link;
		pa = pa->link;
	}

	pc->link = NULL;
	temp.last = pc;
	return temp;
}

//判断当前集合与LS是否相等
template<class _class>
bool LinkedSet<_class>::operator==(const LinkedSet<_class>& LS)
{
	SetNode<_class>* pa = first->link, * pb = LS.first->link;

	while (pa != NULL && pb != NULL) {
		if (pa->data == pb->data) {
			pa = pa->link;
			pb = pb->link;
		}

		else
			return false;
	}

	if (pa != NULL || pb != NULL)
		return false;

	return true;
}

//判断x是否为当前集合的成员
template<class _class>
bool LinkedSet<_class>::Contains(const _class x)
{
	SetNode<_class>* ptr = first->link;

	while (ptr != NULL && ptr->data < x)
		ptr = ptr->link;

	if (ptr != NULL && ptr->data == x)
		return true;
	else
		return false;
}

//取集合中的最小元素
template<class _class>
bool LinkedSet<_class>::Min(_class& x)
{
	if (IsEmpty())
		return false;

	x = first->link->data;
	return true;
}

//取集合中的最大元素
template<class _class>
bool LinkedSet<_class>::Max(_class& x)
{
	if (IsEmpty())
		return false;

	x = last->data;
	return true;
}

#endif