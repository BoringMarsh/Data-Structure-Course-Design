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

#ifndef _FAMILYTREE_H_
#define _FAMILYTREE_H_

#include<iostream>
#include<stdlib.h>
#include<fstream>
#include<climits>
using namespace std;

template<class _class>
struct FamilyMem {
	_class data;
	int generation, number;
	FamilyMem<_class>* parent, * firstChild, * nextSibling;
	
	FamilyMem(_class value, int gen, int num, FamilyMem<_class>* pt = NULL, FamilyMem<_class>* fc = NULL, FamilyMem<_class> *ns= NULL)
		:data(value), generation(gen), number(num), parent(pt), firstChild(fc), nextSibling(ns) {}

	void examine();
};

template<class _class>
void FamilyMem<_class>::examine()
{
	cout << "#======================================#" << endl;
	cout << "编号    ：" << number << endl;
	cout << "个人信息：" << data << endl;
	cout << "世代    ：第" << generation << "代" << endl;
	
	if (parent == NULL)
		cout << "老一辈  ：无" << endl;
	else
		cout << "老一辈  ：" << parent->data << endl;

	if (firstChild == NULL)
		cout << "子女    ：无" << endl;
	else {
		cout << "直系子女：";
		FamilyMem<_class>* cur = firstChild;
		
		while (cur != NULL) {
			cout << cur->data << ' ';
			cur = cur->nextSibling;
		}

		cout << endl;
	}

	cout << endl;
}

template<class _class>
class FamilyTree {
private:
	int size;
	int maxGen;
	FamilyMem<_class>* root, * current;

	FamilyMem<_class>* CopySubFamily(FamilyMem<_class>* root);
	void RemoveSubFamily_Recursion(FamilyMem<_class>* root);
	void ShowSubFamily_Recursion(const FamilyMem<_class>* thisAncestor, FamilyMem<_class>* thisRoot, const int gen_gap, bool* stillSibling)const;
	void WriteFamily_Recursion(const FamilyMem<_class>* thisAncestor, FamilyMem<_class>* thisRoot, bool* stillSibling, ofstream& writer)const;

	typedef void (*_FamilyVisitFunction)(FamilyMem<_class>* member);

public:
	FamilyTree();
	FamilyTree(const _class ancestor);
	FamilyTree(const FamilyTree<_class>& family);
	~FamilyTree();

	FamilyTree<_class>& operator=(const FamilyTree<_class>& family);
	int getSize()const;
	int getMaxGen()const;
	FamilyMem<_class>* getAncestor()const;
	FamilyMem<_class>* getCurrent()const;
	void movCurrent(FamilyMem<_class>* dst);

	bool IsEmpty()const;
	bool SetAncestor(const _class ancestor);
	bool ToAncestor();
	bool ToFirstChild();
	bool ToNextSibling();
	bool ToParent();
	bool Find(FamilyMem<_class>* root, const _class target, Vector<FamilyMem<_class>*>& results)const;  //BFS
	bool SetupFamily(Vector<_class>& children);
	bool AddChild(_class child);
	void UpdateGen();  //BFS
	void RemoveSubFamily(FamilyMem<_class>* root);
	void Traversal(FamilyMem<_class>* root, _FamilyVisitFunction visit)const;  //Recursion
	void ShowSubFamily(FamilyMem<_class>* root)const;
	void WriteFamily(FamilyMem<_class>* root)const;
};

//默认构造函数
template<class _class>
FamilyTree<_class>::FamilyTree()
{
	size = 0;
	maxGen = -1;
	root = current = NULL;
}

//设置祖先的构造函数
template<class _class>
FamilyTree<_class>::FamilyTree(const _class ancestor)
{
	size = 1;
	maxGen = 0;

	FamilyMem<_class>* newMem = new FamilyMem<_class>(ancestor, 0, 0);
	if (newMem == NULL) {
		cout << "存储分配错误！" << endl;
		exit(1);
	}

	root = current = newMem;
}

//复制构造函数
template<class _class>
FamilyTree<_class>::FamilyTree(const FamilyTree<_class>& family)
{
	size = family.size;
	maxGen = family.maxGen;
	root = current = CopySubFamily(family.root);
}

//析构函数
template<class _class>
FamilyTree<_class>::~FamilyTree()
{
	RemoveSubFamily(root);
	delete root;
}

//复制以root为根的子树，返回新树的root
template<class _class>
FamilyMem<_class>* FamilyTree<_class>::CopySubFamily(FamilyMem<_class>* root)
{
	if (root == NULL)
		return NULL;

	static const int gen_gap = root->generation;
	static const int num_gap = root->number;

	FamilyMem<_class>* newRoot = new FamilyMem<_class>(root->data, root->generation - gen_gap, root->number - num_gap);
	if (newRoot == NULL) {
		cout << "存储分配错误！" << endl;
		exit(1);
	}

	FamilyMem<_class>* src_cur = root->firstChild, * dst_cur = NULL;
	bool if_first = true;

	while (src_cur != NULL) {
		FamilyMem<_class>* newMem;

		if (src_cur->firstChild) {
			newMem = CopySubFamily(src_cur);
			newMem->parent = newRoot;
		}
		else {
			newMem = new FamilyMem<_class>(src_cur->data, src_cur->generation - gen_gap, src_cur->number - num_gap, newRoot);

			if (newMem == NULL) {
				cout << "存储分配错误！" << endl;
				exit(1);
			}
		}

		if (if_first)
			dst_cur = newRoot->firstChild = newMem;
		else {
			dst_cur->nextSibling = newMem;
			dst_cur = dst_cur->nextSibling;
		}

		if_first = false;
		src_cur = src_cur->nextSibling;
	}

	current = newRoot;
	return newRoot;
}

//RemoveSubFamily递归部分
template<class _class>
void FamilyTree<_class>::RemoveSubFamily_Recursion(FamilyMem<_class>* root)
{
	if (root == NULL || root->firstChild == NULL)
		return;

	FamilyMem<_class>* cur = root->firstChild;

	while (cur != NULL) {
		if (cur->firstChild)
			RemoveSubFamily_Recursion(cur);

		FamilyMem<_class>* del = cur;
		cur = cur->nextSibling;

		size--;
		delete del;
	}

	root->firstChild = NULL;
	current = root;
}

//ShowSubFamily递归部分
template<class _class>
void FamilyTree<_class>::ShowSubFamily_Recursion(const FamilyMem<_class>* thisAncestor, FamilyMem<_class>* thisRoot, const int gen_gap, bool* stillSibling)const
{
	if (root == NULL)
		return;

	FamilyMem<_class>* cur = thisRoot;

	while (cur != NULL) {
		for (int i = 0; i < cur->generation - gen_gap - 1; i++) {
			cout << (stillSibling[i] ? "┃  " : "    ");
		}

		if (cur != thisAncestor) {
			if (cur->nextSibling) {
				cout << "┣";
				stillSibling[cur->generation - gen_gap - 1] = true;
			}

			else {
				cout << "┗";
				stillSibling[cur->generation - gen_gap - 1] = false;
			}

			cout << "━" << cur->data << endl;
		}
		else
			cout << cur->data << endl;

		if (cur->firstChild)
			ShowSubFamily_Recursion(thisAncestor, cur->firstChild, gen_gap, stillSibling);

		if (cur->generation - gen_gap)
			cur = cur->nextSibling;
		else
			cur = NULL;
	}
}

//WriteFamily递归部分
template<class _class>
void FamilyTree<_class>::WriteFamily_Recursion(const FamilyMem<_class>* thisAncestor, FamilyMem<_class>* thisRoot, bool* stillSibling, ofstream& writer)const
{
	if (root == NULL)
		return;

	FamilyMem<_class>* cur = thisRoot;

	while (cur != NULL) {
		for (int i = 0; i < cur->generation - 1; i++) {
			writer << (stillSibling[i] ? "┃  " : "    ");
		}

		if (cur != thisAncestor) {
			if (cur->nextSibling) {
				writer << "┣";
				stillSibling[cur->generation - 1] = true;
			}

			else {
				writer << "┗";
				stillSibling[cur->generation - 1] = false;
			}

			writer << "━" << cur->data << endl;
		}
		else
			writer << cur->data << endl;

		if (cur->firstChild)
			WriteFamily_Recursion(thisAncestor, cur->firstChild, stillSibling, writer);

		if (cur->generation)
			cur = cur->nextSibling;
		else
			cur = NULL;
	}
}

//重载函数：复制
template<class _class>
FamilyTree<_class>& FamilyTree<_class>::operator=(const FamilyTree<_class>& family)
{
	size = family.size;
	maxGen = family.maxGen;
	root = current = CopySubFamily(family.root);
}

//取当前家族人数
template<class _class>
int FamilyTree<_class>::getSize()const
{
	return size;
}

//取当前家族最大代数
template<class _class>
int FamilyTree<_class>::getMaxGen()const
{
	return maxGen;
}

//取祖先位置
template<class _class>
FamilyMem<_class>* FamilyTree<_class>::getAncestor()const
{
	return root;
}

//取当前成员位置
template<class _class>
FamilyMem<_class>* FamilyTree<_class>::getCurrent()const
{
	return current;
}

//更改当前成员
template<class _class>
void FamilyTree<_class>::movCurrent(FamilyMem<_class>* dst)
{
	current = dst;
}

//判断家族是否为空
template<class _class>
bool FamilyTree<_class>::IsEmpty()const
{
	return(root == NULL);
}

//建立祖先
template<class _class>
bool FamilyTree<_class>::SetAncestor(const _class ancestor)
{
	if (root != NULL) {
		cout << "家族已有祖先！" << endl;
		return false;
	}

	FamilyMem<_class>* newMem = new FamilyMem<_class>(ancestor, 0, 0);
	if (newMem == NULL) {
		cout << "存储分配错误！" << endl;
		exit(1);
	}

	root = current = newMem;
	return true;
}

//寻找祖先，使其成为当前成员
template<class _class>
bool FamilyTree<_class>::ToAncestor()
{
	if (root == NULL) {
		current = NULL;
		return false;
	}

	else {
		current = root;
		return true;
	}
}

//寻找当前成员的第一个子女，使之成为当前结点
template<class _class>
bool FamilyTree<_class>::ToFirstChild()
{
	if (current == NULL || current->firstChild == NULL) {
		current = NULL;
		return false;
	}

	current = current->firstChild;
	return true;
}

//寻找当前成员的下一个兄弟，使之成为当前结点
template<class _class>
bool FamilyTree<_class>::ToNextSibling()
{
	if (current == NULL || current->nextSibling == NULL) {
		current = NULL;
		return false;
	}

	current = current->nextSibling;
	return true;
}

//寻找当前成员的长辈，使之成为当前结点
template<class _class>
bool FamilyTree<_class>::ToParent()
{
	if (current == NULL || current->parent == NULL) {
		current = NULL;
		return false;
	}

	current = current->parent;
	return true;
}

//在树中搜索含有target的结点（仅以数据域为标准），将所有查找结果压入results中
template<class _class>
bool FamilyTree<_class>::Find(FamilyMem<_class>* root, const _class target, Vector<FamilyMem<_class>*>& results)const
{
	results.clear();
	LinkedQueue<FamilyMem<_class>*> queue;

	if (root != NULL) {
		queue.EnQueue(root);

		while (!queue.IsEmpty()) {
			queue.DeQueue(root);
			
			if (root->data == target)
				results.push_back(root);

			for (root = root->firstChild; root != NULL; root = root->nextSibling)
				queue.EnQueue(root);
		}
	}

	if (results.empty())
		return false;
	else
		return true;
}

//若当前成员无子女，为其添加若干个子女
template<class _class>
bool FamilyTree<_class>::SetupFamily(Vector<_class>& children)
{
	if (current->firstChild || children.empty())
		return false;

	FamilyMem<_class>* parent = current;
	const int length = children.length();

	for (int i = 0; i < length; i++) {
		size++;

		if (parent->generation + 1 > maxGen)
			maxGen = parent->generation + 1;

		FamilyMem<_class>* newMem = new FamilyMem<_class>(children[i], parent->generation + 1, size - 1, parent);
		if (newMem == NULL) {
			cout << "存储分配错误！" << endl;
			exit(1);
		}

		if (i == 0)
			current->firstChild = newMem;
		else
			current->nextSibling = newMem;

		current = newMem;
	}

	return true;
}

//若当前成员有子女，为其添加一个子女
template<class _class>
bool FamilyTree<_class>::AddChild(_class child)
{
	if (current->firstChild == NULL)
		return false;

	FamilyMem<_class>* parent = current;
	current = current->firstChild;

	while (current->nextSibling != NULL)
		current = current->nextSibling;

	size++;

	if (parent->generation + 1 > maxGen)
		maxGen = parent->generation + 1;

	FamilyMem<_class>* newMem = new FamilyMem<_class>(child, parent->generation + 1, size - 1, parent);
	if (newMem == NULL) {
		cout << "存储分配错误！" << endl;
		exit(1);
	}

	current->nextSibling = newMem;
	current = current->nextSibling;
	return true;
}

//更新家族的最大代数
template<class _class>
void FamilyTree<_class>::UpdateGen()
{
	maxGen = 0;
	LinkedQueue<FamilyMem<_class>*> queue;
	FamilyMem<_class>* current = root;

	if (current != NULL) {
		queue.EnQueue(current);

		while (!queue.IsEmpty()) {
			queue.DeQueue(current);

			if (current->generation > maxGen)
				maxGen = current->generation;

			for (current = current->firstChild; current != NULL; current = current->nextSibling)
				queue.EnQueue(current);
		}
	}
}

//解散以root为根的家庭，完成后root为当前成员
template<class _class>
void FamilyTree<_class>::RemoveSubFamily(FamilyMem<_class>* root)
{
	RemoveSubFamily_Recursion(root);
	UpdateGen();
}

//遍历以root为根的家族，并以visit方式访问全部成员
template<class _class>
void FamilyTree<_class>::Traversal(FamilyMem<_class>* root, _FamilyVisitFunction visit)const
{
	if (root == NULL)
		return;

	FamilyMem<_class>* cur = root;

	while (cur != NULL) {
		visit(cur);

		if (cur->firstChild)
			Traversal(cur->firstChild, visit);

		cur = cur->nextSibling;
	}
}

//展示局部家庭（以根结点为祖先，世代为相对世代）
template<class _class>
void FamilyTree<_class>::ShowSubFamily(FamilyMem<_class>* root)const
{
	bool* stillSibling = new bool[maxGen];
	cout << endl;
	ShowSubFamily_Recursion(root, root, root->generation, stillSibling);
	cout << endl;
	delete[] stillSibling;
}

//将族谱写入文件
template<class _class>
void FamilyTree<_class>::WriteFamily(FamilyMem<_class>* root)const
{
	bool* stillSibling = new bool[maxGen];
	if (stillSibling == NULL) {
		cerr << "存储分配错误！" << endl;
		exit(1);
	}

	ofstream writer;
	char filename[32];
	cout << "请输入文件名（输入#取消）：";
	cin >> filename;
	cin.ignore(INT_MAX, '\n');
	if (filename[0] == '#')
		return;

	writer.open(filename, ios::out | ios::binary);
	if (!writer) {
		cerr << "文件" << filename << "打开失败！" << endl;
		return;
	}

	WriteFamily_Recursion(root, root, stillSibling, writer);
	writer.close();
	delete[] stillSibling;
}

#endif

#ifndef _STRING_H_
#define _STRING_H_

#include<iostream>
#include<stdlib.h>
#include<stdio.h>
using namespace std;

#define STRING_DEFAULT_SIZE 16
#define STRING_BUFFER_SIZE  65536

//公有函数：求字符串长度
int StrLen(const char* str)
{
	int length = 0;
	
	while (str[length] != '\0')
		length++;

	return length;
}

class String {
private:
	char* data;   //存放数组
	int size;     //字符串当前长度
	int maxSize;  //字符串最大容量

	void expand(const int _timesOfExpandingDefaultSize);  //增加数组空间大小
public:
	String();
	String(const String& str);
	String(const char* str);
	String(const char* str, const int length);
	~String();

	class iterator {
	private:
		char* data;
	public:
		iterator() :data(NULL) {}
		iterator(char* datas) :data(datas) {}

		//读取数据
		char& operator*()const
		{
			return *data;
		}
		//比较运算符：相等
		inline bool operator==(const iterator& s)const
		{
			return (data == s.data);
		}
		//比较运算符：大于
		inline bool operator>(const iterator& s)const
		{
			return (data > s.data);
		}
		//比较运算符：小于
		inline bool operator<(const iterator& s)const
		{
			return (data < s.data);
		}
		//比较运算符：大于等于
		inline bool operator>=(const iterator& s)const
		{
			return (data >= s.data);
		}
		//比较运算符：小于等于
		inline bool operator<=(const iterator& s)const
		{
			return (data <= s.data);
		}
		//比较运算符：不等于
		inline bool operator!=(const iterator& s)const
		{
			return (data != s.data);
		}
		//自增运算符：前置
		String::iterator& operator++()
		{
			data++;
			return *this;
		}
		//自增运算符：后置
		String::iterator operator++(int)
		{
			String::iterator old = *this;
			++(*this);
			return old;
		}
		//自减运算符：前置
		String::iterator& operator--()
		{
			data--;
			return *this;
		}
		//自减运算符：后置
		String::iterator operator--(int)
		{
			String::iterator old = *this;
			--(*this);
			return old;
		}
		//加法：迭代器+数字
		String::iterator operator+(const int num)
		{
			String::iterator old = *this;

			for (int i = 0; i < num; i++)
				old++;

			return old;
		}
		//加法：数字+迭代器
		friend typename String::iterator operator+(const int num, const typename String::iterator s)
		{
			typename String::iterator old = s;

			for (int i = 0; i < num; i++)
				++old;

			return old;
		}
		//减法：迭代器-数字
		String::iterator operator-(const int num)
		{
			String::iterator old = *this;

			for (int i = 0; i < num; i++)
				old--;

			return old;
		}
		//减法：数字-迭代器
		friend typename String::iterator operator-(const int num, const typename String::iterator s)
		{
			typename String::iterator old = s;

			for (int i = 0; i < num; i++)
				--old;

			return old;
		}
		//自加
		String::iterator operator+=(const int num)
		{
			for (int i = 0; i < num; i++)
				++(*this);

			return *this;
		}
		//自减
		String::iterator operator-=(const int num)
		{
			for (int i = 0; i < num; i++)
				--(*this);

			return *this;
		}
		//迭代器相减
		friend int operator-(const String::iterator s1, const String::iterator s2)
		{
			return (s1.data - s2.data);
		}
	};

	inline String::iterator begin();
	inline const String::iterator begin()const;
	inline String::iterator end();
	inline const String::iterator end()const;
	inline String::iterator last();
	inline const String::iterator last()const;

	char& operator[](const int pos)const;
	String& operator=(const String& str);
	String& operator=(const char* str);
	friend istream& operator>>(istream& istr, String& str);
	friend ostream& operator<<(ostream& ostr, const String& str);
	friend bool operator==(const String& Left, const String& Right);
	friend bool operator==(const String& Left, const char* Right);
	friend bool operator==(const char* Left, const String& Right);
	friend bool operator!=(const String& Left, const String& Right);
	friend bool operator!=(const String& Left, const char* Right);
	friend bool operator!=(const char* Left, const String& Right);
	friend String operator+(const String& Left, const String& Right);
	friend String operator+(const String& Left, const char* Right);
	friend String operator+(const char* Left, const String& Right);
	friend String operator+(const String& Left, const char Right);
	friend String operator+(const char Left, const String& Right);
	friend String operator*(const String& Left, const int Right);
	friend String operator*(const int Left, const String& Right);
	friend String& operator+=(String& Left, const String& Right);
	friend String& operator+=(String& Left, const char* Right);
	friend String& operator+=(String& Left, const char Right);
	friend String& operator*=(String& Left, const int Right);
	String operator--();
	String operator--(int);

	void gets();
	void clear();
	const char* c_str()const;
	int length()const;
	int capacity()const;
	bool isEmpty()const;
	bool isFull()const;
	bool isDigit()const;
	bool isOnlyDigit()const;
	bool stringToDouble(double& d)const;
	bool stringToInt(int& i)const;
	char* reverse();
};

//默认构造函数
String::String()
{
	maxSize = STRING_DEFAULT_SIZE;
	size = 0;

	data = new char[maxSize];
	data[0] = '\0';

	if (data == NULL) {
		cout << "存储分配错误！" << endl;
		exit(1);
	}
}

//复制构造函数：从String对象复制
String::String(const String& str)
{
	maxSize = str.maxSize;
	size = str.size;

	data = new char[maxSize];
	if (data == NULL) {
		cout << "存储分配错误！" << endl;
		exit(1);
	}

	for (int i = 0; i < size; i++)
		data[i] = str.data[i];

	data[size] = '\0';
}

//复制构造函数：从const char*变量复制
String::String(const char* str)
{
	int length = StrLen(str);

	size = length;
	maxSize = (length / STRING_DEFAULT_SIZE + 1) * STRING_DEFAULT_SIZE;
	data = new char[maxSize];
	if (data == NULL) {
		cerr << "存储分配失败！" << endl;
		exit(1);
	}

	for (int i = 0; i < size; i++)
		data[i] = str[i];
	data[size] = '\0';
}

//从const char*变量复制，同时指定长度的构造函数（长度取两者最小值）
String::String(const char* str, const int length)
{
	int strLength = StrLen(str);
	maxSize = (strLength / STRING_DEFAULT_SIZE + 1) * STRING_DEFAULT_SIZE;

	if (strLength >= length)
		size = length;
	else
		size = strLength;

	data = new char[maxSize];
	if (data == NULL) {
		cerr << "存储分配失败！" << endl;
		exit(1);
	}

	for (int i = 0; i < size; i++)
		data[i] = str[i];
	data[size] = '\0';
}

//析构函数
String::~String()
{
	delete[] data;
}

//返回字符串的起始位置
inline typename String::iterator String::begin()
{
	return size > 0 ? typename String::iterator(&data[0]) : typename String::iterator();
}

//返回字符串的起始位置
inline const typename String::iterator String::begin()const
{
	return size > 0 ? typename String::iterator(&data[0]) : typename String::iterator();
}

//返回字符串末尾的后一个位置
inline typename String::iterator String::end()
{
	return size > 0 ? ++(typename String::iterator(&data[size - 1])) : typename String::iterator();
}

//返回字符串末尾的后一个位置
inline const typename String::iterator String::end()const
{
	return size > 0 ? ++(typename String::iterator(&data[size - 1])) : typename String::iterator();
}

//返回字符串的末尾位置
inline typename String::iterator String::last()
{
	return size > 0 ? typename String::iterator(&data[size - 1]) : typename String::iterator();
}

//返回字符串的末尾位置
inline const typename String::iterator String::last()const
{
	return size > 0 ? typename String::iterator(&data[size - 1]) : typename String::iterator();
}

//重载函数：下标访问
char& String::operator[](const int pos)const
{
	if (pos < 0 || pos >= size) {
		cout << "位置非法！已返回首个字母" << endl;
		return data[0];
	}

	return data[pos];
}

//重载函数：复制（从String对象复制）
String& String::operator=(const String& str)
{
	if (*this == str)
		return *this;

	delete[] data;

	maxSize = str.maxSize;
	size = str.size;

	data = new char[maxSize];
	if (data == NULL) {
		cout << "存储分配错误！" << endl;
		exit(1);
	}

	for (int i = 0; i <= size; i++)
		data[i] = str.data[i];

	return *this;
}

//重载函数：复制（从const char*变量复制）
String& String::operator=(const char* str)
{
	delete[] data;

	int length = StrLen(str);

	size = length;
	maxSize = (length / STRING_DEFAULT_SIZE + 1) * STRING_DEFAULT_SIZE;

	data = new char[maxSize];
	if (data == NULL) {
		cout << "存储分配错误！" << endl;
		exit(1);
	}

	for (int i = 0; i <= size; i++)
		data[i] = str[i];

	return *this;
}

//输入字符串（支持空格输入）
void String::gets()
{
	char* buffer = new char[STRING_BUFFER_SIZE];
	if (buffer == NULL) {
		cerr << "存储分配错误！" << endl;
		exit(1);
	}

	fgets(buffer, STRING_BUFFER_SIZE, stdin);

	const int buffer_len = StrLen(buffer);
	
	if (buffer_len >= maxSize) {
		maxSize = (buffer_len / STRING_DEFAULT_SIZE + 1) * STRING_DEFAULT_SIZE;
		char* newData = new char[maxSize];
		if (newData == NULL) {
			cerr << "存储分配失败！" << endl;
			exit(1);
		}

		for (int i = 0; i <= buffer_len; i++)
			newData[i] = buffer[i];

		char* oldData = data;
		data = newData;
		delete[] oldData;
	}

	else {
		for (int i = 0; i < buffer_len; i++)
			data[i] = buffer[i];

		data[buffer_len] = '\0';
	}

	size = buffer_len;
	delete[] buffer;
}

//将字符串置为空
void String::clear()
{
	delete[] data;

	maxSize = STRING_DEFAULT_SIZE;
	size = 0;

	data = new char[maxSize];
	if (data == NULL) {
		cout << "存储分配错误！" << endl;
		exit(1);
	}

	data[size] = '\0';
}

//重载函数：输入
istream& operator>>(istream& istr, String& str)
{
	char* buffer = new char[STRING_BUFFER_SIZE];
	if (buffer == NULL) {
		cerr << "存储分配错误！" << endl;
		exit(1);
	}

	istr >> buffer;

	const int buffer_len = StrLen(buffer);
	
	if (buffer_len >= str.maxSize) {
		str.maxSize = (buffer_len / STRING_DEFAULT_SIZE + 1) * STRING_DEFAULT_SIZE;
		char* newData = new char[str.maxSize];
		if (newData == NULL) {
			cerr << "存储分配失败！" << endl;
			exit(1);
		}

		for (int i = 0; i <= buffer_len; i++)
			newData[i] = buffer[i];

		char* oldData = str.data;
		str.data = newData;
		delete[] oldData;
	}

	else {
		for (int i = 0; i < buffer_len; i++)
			str.data[i] = buffer[i];

		str.data[buffer_len] = '\0';
	}

	str.size = buffer_len;
	delete[] buffer;
	return istr;
}

//重载函数：输出
ostream& operator<<(ostream& ostr, const String& str)
{
	ostr << str.c_str();
	return ostr;
}

//重载函数：等于（与一个String对象相比）
bool operator==(const String& Left, const String& Right)
{
	if (Left.size != Right.size)
		return false;

	for (int i = 0; i < Left.size; i++) {
		if (Left.data[i] != Right.data[i])
			return false;
	}

	return true;
}

//重载函数：等于（与一个const char*变量相比）
bool operator==(const String&Left,const char* Right)
{
	if (Left.size != StrLen(Right))
		return false;

	for (int i = 0; i < Left.size; i++) {
		if (Left.data[i] != Right[i])
			return false;
	}

	return true;
}

//重载函数：等于
bool operator==(const char* Left, const String& Right)
{
	if (StrLen(Left) != Right.size)
		return false;

	for (int i = 0; i < Right.size; i++) {
		if (Left[i] != Right.data[i])
			return false;
	}

	return true;
}

//重载函数：不等于（与一个String对象相比）
bool operator!=(const String& Left, const String& Right)
{
	if (Left.size != Right.size)
		return true;

	for (int i = 0; i < Left.size; i++) {
		if (Left.data[i] != Right.data[i])
			return true;
	}

	return false;
}

//重载函数：不等于（与一个const char*变量相比）
bool operator!=(const String& Left, const char* Right)
{
	if (Left.size != StrLen(Right))
		return true;

	for (int i = 0; i < Left.size; i++) {
		if (Left.data[i] != Right[i])
			return true;
	}

	return false;
}

//重载函数：不等于
bool operator!=(const char* Left, const String& Right)
{
	if (StrLen(Left) != Right.size)
		return true;

	for (int i = 0; i < Right.size; i++) {
		if (Left[i] != Right.data[i])
			return true;
	}

	return false;
}

//重载函数：右加上一个String对象
String operator+(const String&Left,const String& Right)
{
	String Str;

	Str.maxSize = ((Left.size + Right.size) / STRING_DEFAULT_SIZE + 1) * STRING_DEFAULT_SIZE;
	char* oldStr = Str.data;
	Str.data = new char[Str.maxSize];
	if (Str.data == NULL) {
		cerr << "存储分配错误！" << endl;
		exit(1);
	}

	for (int i = 0; i < Left.size; i++)
		Str.data[i] = Left.data[i];

	delete[] oldStr;

	Str.size = Left.size + Right.size;  //构造函数在字符串长度和指定长度间优先取最小值，故在此重新设置size值
	for (int i = Left.size; i < Str.size; i++)
		Str.data[i] = Right.data[i - Left.size];
	Str.data[Str.size] = '\0';

	return Str;
}

//重载函数：右加上一个const char* str变量
String operator+(const String& Left, const char* Right)
{
	const int length = StrLen(Right);
	String Str;

	Str.maxSize = ((Left.size + length) / STRING_DEFAULT_SIZE + 1) * STRING_DEFAULT_SIZE;
	char* oldStr = Str.data;
	Str.data = new char[Str.maxSize];
	if (Str.data == NULL) {
		cerr << "存储分配错误！" << endl;
		exit(1);
	}

	for (int i = 0; i < Left.size; i++)
		Str.data[i] = Left.data[i];

	delete[] oldStr;

	Str.size = Left.size + length;  //构造函数在字符串长度和指定长度间优先取最小值，故在此重新设置size值
	for (int i = Left.size; i < Str.size; i++)
		Str.data[i] = Right[i - Left.size];
	Str.data[Str.size] = '\0';

	return Str;
}

//友元函数：const char*变量+String对象
String operator+(const char* Left, const String& Right)
{
	const int length = StrLen(Left);
	String Str;

	Str.maxSize = ((length + Right.size) / STRING_DEFAULT_SIZE + 1) * STRING_DEFAULT_SIZE;
	char* oldStr = Str.data;

	Str.data = new char[Str.maxSize];
	if (Str.data == NULL) {
		cerr << "存储分配错误！" << endl;
		exit(1);
	}

	for (int i = 0; i < length; i++)
		Str.data[i] = Left[i];

	delete[] oldStr;

	Str.size = length + Right.size;  //构造函数在字符串长度和指定长度间优先取最小值，故在此重新设置size值
	for (int i = length; i < Str.size; i++)
		Str.data[i] = Right[i - length];
	Str.data[Str.size] = '\0';

	return Str;
}

//重载函数：右加上一个字符
String operator+(const String& Left, const char Right)
{
	String Str;

	if (Left.size + 1 >= Str.maxSize) {
		Str.maxSize = ((Left.size + 1) / STRING_DEFAULT_SIZE + 1) * STRING_DEFAULT_SIZE;
		char* oldStr = Str.data;
		Str.data = new char[Str.maxSize];
		if (Str.data == NULL) {
			cerr << "存储分配失败！" << endl;
			exit(1);
		}

		delete[] oldStr;
	}

	for (int i = 0; i < Left.size; i++)
		Str.data[i] = Left.data[i];

	Str.data[Left.size] = Right;
	Str.data[Left.size + 1] = '\0';
	Str.size = Left.size + 1;

	return Str;
}

//友元函数：字符+String对象
String operator+(const char Left, const String& Right)
{
	String Str;

	if (Right.size + 1 >= Str.maxSize) {
		Str.maxSize = ((Right.size + 1) / STRING_DEFAULT_SIZE + 1) * STRING_DEFAULT_SIZE;
		char* oldStr = Str.data;
		Str.data = new char[Str.maxSize];
		if (Str.data == NULL) {
			cerr << "存储分配失败！" << endl;
			exit(1);
		}

		Str.data[0] = Left;

		for (int i = 0; i <= Right.size; i++)
			Str.data[i + 1] = Right.data[i];

		delete[] oldStr;
	}

	else {
		Str.data[0] = Left;
		for (int i = 1; i < Right.size + 1; i++)
			Str.data[i] = Right.data[i - 1];

		Str.data[Right.size + 1] = '\0';
	}

	Str.size = Right.size + 1;
	return Str;
}

//重载函数：乘法
String operator*(const String& Left, const int Right)
{
	String Str;

	if (Right <= 0)
		return Str;

	Str.maxSize = (Left.size * Right / STRING_DEFAULT_SIZE + 1) * STRING_DEFAULT_SIZE;
	char* oldStr = Str.data;
	Str.data = new char[Str.maxSize];
	if (Str.data == NULL) {
		cerr << "存储分配错误！" << endl;
		exit(1);
	}

	delete[] oldStr;

	for (int i = 0; i < Right; i++) {
		for (int j = 0; j < Left.size; j++) {
			Str.data[i * Left.size + j] = Left[j];
		}
	}

	Str.size = Left.size * Right;
	Str.data[Right * Left.size] = '\0';

	return Str;
}

//重载函数：乘法
String operator*(const int Left, const String& Right)
{
	String Str;

	if (Left <= 0)
		return Str;

	Str.maxSize = (Right.size * Left / STRING_DEFAULT_SIZE + 1) * STRING_DEFAULT_SIZE;
	char* oldStr = Str.data;
	Str.data = new char[Str.maxSize];
	if (Str.data == NULL) {
		cerr << "存储分配错误！" << endl;
		exit(1);
	}

	delete[] oldStr;

	for (int i = 0; i < Left; i++) {
		for (int j = 0; j < Right.size; j++) {
			Str.data[i * Right.size + j] = Right[j];
		}
	}

	Str.size = Right.size * Left;
	Str.data[Left * Right.size] = '\0';

	return Str;
}

//重载函数：自增一个String对象
String& operator+=(String& Left, const String& Right)
{
	Left.maxSize = ((Left.size + Right.size) / STRING_DEFAULT_SIZE + 1) * STRING_DEFAULT_SIZE;
	char* oldStr = Left.data;
	Left.data = new char[Left.maxSize];
	if (Left.data == NULL) {
		cerr << "存储分配失败！" << endl;
		exit(1);
	}

	for (int i = 0; i < Left.size; i++)
		Left.data[i] = oldStr[i];

	Left.size = Left.size + Right.size;

	for (int i = Left.size - Right.size; i < Left.size; i++)
		Left.data[i] = Right.data[i - Left.size + Right.size];

	Left.data[Left.size] = '\0';
	delete[] oldStr;

	return Left;
}

//重载函数：自增一个const char*变量
String& operator+=(String& Left, const char* Right)
{
	int length = StrLen(Right);
	Left.maxSize = ((Left.size + length) / STRING_DEFAULT_SIZE + 1) * STRING_DEFAULT_SIZE;
	char* oldStr = Left.data;
	Left.data = new char[Left.maxSize];
	if (Left.data == NULL) {
		cerr << "存储分配失败！" << endl;
		exit(1);
	}

	for (int i = 0; i < Left.size; i++)
		Left.data[i] = oldStr[i];

	Left.size = Left.size + length;

	for (int i = Left.size - length; i < Left.size; i++)
		Left.data[i] = Right[i - Left.size + length];

	Left.data[Left.size] = '\0';
	delete[] oldStr;

	return Left;
}

//重载函数：自增一个字符
String& operator+=(String& Left, const char Right)
{
	if (Left.size + 1 >= Left.maxSize) {
		Left.maxSize = ((Left.size + 1) / STRING_DEFAULT_SIZE + 1) * STRING_DEFAULT_SIZE;
		char* oldStr = Left.data;

		Left.data = new char[Left.maxSize];
		if (Left.data == NULL) {
			cerr << "存储分配失败！" << endl;
			exit(1);
		}

		for (int i = 0; i < Left.size; i++)
			Left.data[i] = oldStr[i];

		delete[] oldStr;
	}
	
	Left.data[Left.size] = Right;
	Left.data[Left.size + 1] = '\0';
	Left.size++;

	return Left;
}

//重载函数：自增若干倍
String& operator*=(String& Left, const int Right)
{
	if (Right <= 0)
		return Left;

	Left.maxSize = (Left.size * Right / STRING_DEFAULT_SIZE + 1) * STRING_DEFAULT_SIZE;
	char* oldStr = Left.data;
	Left.data = new char[Left.maxSize];
	if (Left.data == NULL) {
		cerr << "存储分配错误！" << endl;
		exit(1);
	}

	for (int i = 0; i < Right; i++) {
		for (int j = 0; j < Left.size; j++) {
			Left.data[i * Left.size + j] = oldStr[j];
		}
	}

	delete[] oldStr;
	Left.data[Right * Left.size] = '\0';
	Left.size = Left.size * Right;

	return Left;
}

//减少一个字符（前缀）
String String::operator--()
{
	if (size == 0)
		return *this;

	String old = *this;
	data[size - 1] = '\0';
	size--;
	return old;
}

//减少一个字符（后缀）
String String::operator--(int)
{
	if (size == 0)
		return *this;

	data[size - 1] = '\0';
	size--;
	return *this;
}

//返回字符串首地址（以const char*形式返回）
const char* String::c_str()const
{
	return data;
}

//返回字符串长度
int String::length()const
{
	return StrLen(data);
}

//返回字符串最大容量
int String::capacity()const
{
	return maxSize;
}

//判断字符串是否为空
bool String::isEmpty()const
{
	return (size == 0);
}

//判断字符串是否已满
bool String::isFull()const
{
	return (size == maxSize - 1);
}

//将字符串反转
char* String::reverse()
{
	String::iterator front = begin();
	String::iterator back = last();
	char temp;

	while (front < back) {
		temp = *front;
		*front = *back;
		*back = temp;

		front++;
		back--;
	}

	return data;
}

//判断字符串是否表示一个数值
bool String::isDigit()const
{
	String::iterator iter = begin();
	bool if_point = false;

	if (*iter == '\0')
		return false;
	else if (*iter == '-') {
		if (*(iter + 1) < '0' || *(iter + 1) > '9')
			return false;

		else
			iter++;
	}

	while (iter != end()) {
		if (*iter == '.') {
			if (!if_point) {
				if (*(iter - 1) < '0' || *(iter - 1) > '9' || *(iter + 1) < '0' || *(iter + 1) > '9')
					return false;

				else
					if_point = true;
			}

			else
				return false;
		}

		else if (*iter < '0' || *iter > '9')
			return false;

		iter++;
	}

	return true;
}

//判断字符串是否仅有数字
bool String::isOnlyDigit()const
{
	String::iterator iter = begin();

	while (iter != end()) {
		if (*iter < '0' || *iter>'9')
			return false;

		iter++;
	}

	return true;
}

//将字符串转成双精度数
bool String::stringToDouble(double& d)const
{
	if (!isDigit())
		return false;

	String::iterator iter = begin();
	bool if_minus = false, if_point = false;
	double result = 0.0;
	int count = 1;

	if (*iter == '-') {
		if_minus = true;
		iter++;
	}

	while (iter != end()) {
		if (*iter == '.') {
			if_point = true;
			iter++;
		}

		if (if_point) {
			double item = double(*iter) - '0';

			for (int i = 0; i < count; i++)
				item = item / 10;

			result += item;
			count++;
		}

		else
			result = result * 10 + *iter - '0';

		iter++;
	}

	if (if_minus)
		d = -1.0 * result;
	else
		d = result;

	return true;
}

//将字符串转成整型数
bool String::stringToInt(int& i)const
{
	if (!isDigit())
		return false;

	String::iterator iter = begin();
	bool if_minus = false;
	int result = 0;

	if (*iter == '-') {
		if_minus = true;
		iter++;
	}

	while (iter != end()) {
		if (*iter == '.')
			break;

		result = result * 10 + *iter - '0';
		iter++;
	}

	if (if_minus)
		i = -1 * result;
	else
		i = result;
	return true;
}

//增加数组空间大小
void String::expand(const int _timesOfExpandingDefaultSize)
{
	if (_timesOfExpandingDefaultSize <= 0)
		return;

	maxSize += _timesOfExpandingDefaultSize * STRING_DEFAULT_SIZE;
	char* oldData = data;
	data = new char[maxSize];
	if (data == NULL) {
		cerr << "存储分配失败！" << endl;
		exit(1);
	}

	for (int i = 0; i < size; i++)
		data[i] = oldData[i];
	data[size] = '\0';

	delete[] oldData;
}

#endif