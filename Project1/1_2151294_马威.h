#ifndef _DBLLIST_H_
#define _DBLLIST_H_

#include<iostream>
#include<stdlib.h>
using namespace std;

//指定方向
enum direction { Left, Right };

template<class _class>
class DblNode {
public:
	_class data;
	DblNode<_class>* lLink, * rLink;
	DblNode(DblNode<_class>* left = NULL, DblNode<_class>* right = NULL)
		:lLink(left), rLink(right) {}
	DblNode(_class value, DblNode<_class>* left = NULL, DblNode<_class>* right = NULL)
		:data(value), lLink(left), rLink(right) {}
};

template<class _class>
class DblList {
private:
	DblNode<_class>* first;
	int length;
public:
	DblList();
	DblList(const _class uniqueVal);
	DblList(DblList<_class>& L);
	~DblList();

	void makeEmpty();
	int Length()const;
	DblNode<_class>* getHead()const;
	DblNode<_class>* Search(const _class x)const;
	DblNode<_class>* Locate(const int pos, const direction d)const;
	bool getData(const int pos, _class& x, const direction d)const;
	void setData(const int pos, const _class x, const direction d);
	bool Insert(const int pos, const _class x, const direction d);
	bool Remove(const int pos, _class& x, const direction d);
	bool IsEmpty()const;
	bool IsFull()const;
	void input(_class endTag, const direction d);
	void output(const direction d)const;
	DblList<_class>& operator=(DblList<_class>& L);
};

//默认构造函数
template<class _class>
DblList<_class>::DblList()
{
	first = new DblNode<_class>;

	if (first == NULL) {
		cerr << "存储分配出错！" << endl;
		exit(1);
	}

	first->rLink = first->lLink = first;
	length = 0;
}

//给头结点数据赋值的构造函数
template<class _class>
DblList<_class>::DblList(const _class uniqueVal) {
	first = new DblNode<_class>(uniqueVal);

	if (first == NULL) {
		cerr << "存储分配出错！" << endl;
		exit(1);
	}

	first->rLink = first->lLink = first;
	length = 0;
}

//复制构造函数
template<class _class>
DblList<_class>::DblList(DblList<_class>& L)
{
	_class value;
	DblNode<_class>* srcptr = L.getHead()->rLink;
	DblNode<_class>* dstLast = first = new DblNode<_class>;
	DblNode<_class>* dstCurrent;
	first->rLink = first->lLink = first;

	while (srcptr != L.getHead()) {
		value = srcptr->data;
		dstCurrent = new DblNode<_class>(value);

		if (dstCurrent == NULL) {
			cerr << "存储分配错误！" << endl;
			exit(1);
		}

		dstCurrent->rLink = dstLast->rLink;
		dstLast->rLink = dstCurrent;
		dstCurrent->lLink = dstLast;
		first->lLink = dstCurrent;

		dstLast = dstLast->rLink;
		srcptr = srcptr->rLink;
	}

	length = L.length;
}

//析构函数
template<class _class>
DblList<_class>::~DblList()
{
	makeEmpty();
	delete first;
}

//将链表置为空表
template<class _class>
void DblList<_class>::makeEmpty()
{
	length = 0;
	DblNode<_class>* current;

	while (first->rLink != first) {
		current = first->rLink;
		first->rLink = current->rLink;
		current->rLink->lLink = first;
		delete current;
	}
}

//计算链表的长度
template<class _class>
int DblList<_class>::Length()const
{
	return length;
}

//取附加头结点地址
template<class _class>
DblNode<_class>* DblList<_class>::getHead()const
{
	return first;
}

//在链表中沿后继方向寻找等于给定值x的结点
template<class _class>
DblNode<_class>* DblList<_class>::Search(const _class x)const
{
	DblNode<_class>* current = first->rLink;

	while (current != first && current->data != x)
		current = current->rLink;

	if (current != first)
		return current;
	else
		return NULL;
}

//在链表中定位到指定方向上第i (i >= 0) 个结点
template<class _class>
DblNode<_class>* DblList<_class>::Locate(const int pos, const direction d)const
{
	if (first->rLink == first || pos == 0)
		return first;

	DblNode<_class>* current;
	if (d == Left)
		current = first->lLink;
	else
		current = first->rLink;

	for (int i = 1; i < pos; i++) {
		if (current == first)
			break;
		else if (d == Left)
			current = current->lLink;
		else
			current = current->rLink;
	}
		
	if (current != first)
		return current;
	else
		return NULL;
}

//取出指定方向上第i (i > 0) 个结点的值
template<class _class>
bool DblList<_class>::getData(const int pos, _class& x, const direction d)const
{
	if (pos <= 0)
		return false;

	DblNode<_class>* current = Locate(pos, d);

	if (current == NULL)
		return false;
	else {
		x = current->data;
		return true;
	}
}

//用x修改指定方向上第i (i > 0) 个结点的值
template<class _class>
void DblList<_class>::setData(const int pos, const _class x, const direction d)
{
	if (pos <= 0)
		return;

	DblNode<_class>* current = Locate(pos, d);

	if (current == NULL)
		return;
	else
		current->data = x;
}

//在指定方向上第i (i >= 0) 个结点后插入一个包含有值x的新结点
template<class _class>
bool DblList<_class>::Insert(const int pos, const _class x, const direction d)
{
	DblNode<_class>* current = Locate(pos, d);
	if (current == NULL)
		return false;

	DblNode<_class>* newNode = new DblNode<_class>(x);
	if (newNode == NULL) {
		cerr << "存储分配失败！" << endl;
		exit(1);
	}

	if (d == Left) {
		newNode->lLink = current->lLink;
		current->lLink = newNode;
		newNode->lLink->rLink = newNode;
		newNode->rLink = current;
	}

	else {
		newNode->rLink = current->rLink;
		current->rLink = newNode;
		newNode->rLink->lLink = newNode;
		newNode->lLink = current;
	}

	length++;
	return true;
}

//删除指定方向上第i个结点，x返回其值
template<class _class>
bool DblList<_class>::Remove(const int pos, _class& x, const direction d)
{
	DblNode<_class>* current = Locate(pos, d);
	if (current == NULL)
		return false;

	current->rLink->lLink = current->lLink;
	current->lLink->rLink = current->rLink;
	x = current->data;
	delete current;

	length--;
	return true;
}

//判断链表是否为空
template<class _class>
bool DblList<_class>::IsEmpty()const
{
	return (first->rLink == first);
}

//判断链表是否已满
template<class _class>
bool DblList<_class>::IsFull()const
{
	return false;
}

//按指定方向逐个输入元素
template<class _class>
void DblList<_class>::input(_class endTag, const direction d)
{
	DblNode<_class>* newNode, * last;
	_class value;
	makeEmpty();

	cin >> value;
	last = first;

	while (value != endTag) {
		newNode = new DblNode<_class>(value);

		if (newNode == NULL) {
			cerr << "存储分配错误！" << endl;
			exit(1);
		}

		if (d == Left) {
			newNode->lLink = last->lLink;
			last->lLink = newNode;
			newNode->rLink = last;
			first->rLink = newNode;

			last = last->lLink;
		}

		else {
			newNode->rLink = last->rLink;
			last->rLink = newNode;
			newNode->lLink = last;
			first->lLink = newNode;

			last = last->rLink;
		}

		length++;
		cin >> value;
	}
}

//按指定方向逐个输出元素
template<class _class>
void DblList<_class>::output(const direction d)const
{
	DblNode<_class>* current;
	int count = 0;

	if (d == Left)
		current = first->lLink;
	else
		current = first->rLink;

	while (current != first) {
		cout << '#' << count << ':' << current->data << endl;
		count++;
		
		if (d == Left)
			current = current->lLink;
		else
			current = current->rLink;
	}
}

//重载函数：赋值
template<class _class>
DblList<_class>& DblList<_class>::operator=(DblList<_class>&L)
{
	_class value;
	DblNode<_class>* srcptr = L.getHead()->rLink;
	DblNode<_class>* dstLast = first;
	DblNode<_class>* dstCurrent;
	makeEmpty();

	while (srcptr != L.getHead()) {
		value = srcptr->data;
		dstCurrent = new DblNode<_class>(value);

		if (dstCurrent == NULL) {
			cerr << "存储分配错误！" << endl;
			exit(1);
		}

		dstCurrent->rLink = dstLast->rLink;
		dstLast->rLink = dstCurrent;
		dstCurrent->lLink = dstLast;
		first->lLink = dstCurrent;

		dstLast = dstLast->rLink;
		srcptr = srcptr->rLink;
	}

	length = L.length;
	return *this;
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

#ifndef _AVLTREE_H_
#define _AVLTREE_H_

#include<iostream>
#include<stdlib.h>
using namespace std;

template<class _data,class _key>
struct AVLNode {
	_data data;
	_key key;
	AVLNode<_data, _key>* left, * right;
	int bf;

	AVLNode() :left(NULL), right(NULL), bf(0) {}
	AVLNode(const _data d, const _key x, AVLNode<_data, _key>* l = NULL, AVLNode<_data, _key>* r = NULL, const int balance = 0)
		:data(d), key(x), left(l), right(r), bf(balance) {}
};

template<class _data,class _key>
class AVLTree {
private:
	AVLNode<_data, _key>* root;
	_key RefValue;
	int size;
	typedef void(* _AVLTreeVisitFunction)(AVLNode<_data, _key>* node);

	AVLNode<_data, _key>* copyTree(const AVLNode<_data, _key>* root);
	void makeEmpty(AVLNode<_data, _key>*& ptr);
	AVLNode<_data, _key>* Search(const _key x, AVLNode<_data, _key>* ptr)const;
	AVLNode<_data, _key>* getMin(AVLNode<_data, _key>* ptr)const;
	AVLNode<_data, _key>* getMax(AVLNode<_data, _key>* ptr)const;
	void RotateL(AVLNode<_data, _key>*& ptr);
	void RotateR(AVLNode<_data, _key>*& ptr);
	void RotateLR(AVLNode<_data, _key>*& ptr);
	void RotateRL(AVLNode<_data, _key>*& ptr);
	bool Insert(AVLNode<_data, _key>*& ptr, const _key x, const _data d);
	bool Remove(AVLNode<_data, _key>*& ptr, const _key x, _data& d);
	int Height(AVLNode<_data, _key>* ptr)const;
	void Traversal(AVLNode<_data, _key>* ptr, _AVLTreeVisitFunction visit)const;
	void Write(AVLNode<_data, _key>* ptr, ostream& ostr)const;
public:
	AVLTree();
	AVLTree(const _key ref);
	AVLTree(const AVLTree<_data, _key>& tree);
	~AVLTree();

	bool IsEmpty()const;
	bool IsFull()const;
	bool getData(const _key x, _data& d)const;
	bool setData(const _key x, const _data d);
	bool setData(const _key oldKey, const _key newKey, const _data d);
	bool Search(const _key x)const;
	void makeEmpty();
	AVLNode<_data, _key>* getRoot()const;
	bool getMin(_data& d)const;
	bool getMax(_data& d)const;
	int getSize()const;
	bool Insert(const _data d, const _key x);
	bool Remove(_key x, _data& d);
	int Height()const;
	void Traversal(_AVLTreeVisitFunction visit)const;
	bool Write()const;
	AVLTree<_data, _key>& operator=(const AVLTree<_data, _key>& tree);
};

//默认构造函数
template<class _data,class _key>
AVLTree<_data, _key>::AVLTree()
{
	root = NULL;
	size = 0;
}

//设置输入结束标志的构造函数
template<class _data, class _key>
AVLTree<_data, _key>::AVLTree(const _key Ref)
{
	root = NULL;
	RefValue = Ref;
	size = 0;
}

//复制构造函数
template<class _data, class _key>
AVLTree<_data, _key>::AVLTree(const AVLTree<_data, _key>& tree)
{
	RefValue = tree.RefValue;
	size = tree.size;
	root = copyTree(tree.root);
}

//析构函数
template<class _data, class _key>
AVLTree<_data, _key>::~AVLTree()
{
	makeEmpty();
}

//复制以root为根的树，并返回新根结点的地址
template<class _data, class _key>
AVLNode<_data, _key>* AVLTree<_data, _key>::copyTree(const AVLNode<_data, _key>* root)
{
	if (root != NULL) {
		AVLNode<_data, _key>* p = new AVLNode<_data, _key>(root->data, root->key, NULL, NULL, root->bf);
		if (p == NULL) {
			cerr << "存储分配错误！" << endl;
			exit(1);
		}

		p->left = copyTree(root->left);
		p->right = copyTree(root->right);
		return p;
	}

	return NULL;
}

//清空AVL树递归部分
template<class _data, class _key>
void AVLTree<_data, _key>::makeEmpty(AVLNode<_data, _key>*& ptr)
{
	if (ptr == NULL)
		return;

	if (ptr->left)
		makeEmpty(ptr->left);
	if (ptr->right)
		makeEmpty(ptr->right);

	delete ptr;
	ptr = NULL;
}

//在AVL树中搜索过程部分
template<class _data, class _key>
AVLNode<_data, _key>* AVLTree<_data, _key>::Search(const _key x, AVLNode<_data, _key>* ptr)const
{
	AVLNode<_data, _key>* current = ptr;

	while (current != NULL) {
		if (x < current->key)
			current = current->left;
		else if (x > current->key)
			current = current->right;
		else
			break;
	}

	if (current != NULL)
		return current;
	else
		return NULL;
}

//取AVL树最小元素过程部分
template<class _data, class _key>
AVLNode<_data, _key>* AVLTree<_data, _key>::getMin(AVLNode<_data, _key>* ptr)const
{
	AVLNode<_data, _key>* current = ptr;
	
	while (current != NULL && current->left != NULL) {
		current = current->left;
	}

	if (current != NULL)
		return current;
	else
		return NULL;
}

//取AVL树最大元素过程部分
template<class _data, class _key>
AVLNode<_data, _key>* AVLTree<_data, _key>::getMax(AVLNode<_data, _key>* ptr)const
{
	AVLNode<_data, _key>* current = ptr;

	while (current != NULL && current->right != NULL) {
		current = current->right;
	}

	if (current != NULL)
		return current;
	else
		return NULL;
}

//将AVL树进行左单旋转
template<class _data, class _key>
void AVLTree<_data, _key>::RotateL(AVLNode<_data, _key>*& ptr)
{
	AVLNode<_data, _key>* subL = ptr;
	ptr = subL->right;
	subL->right = ptr->left;
	ptr->left = subL;
	ptr->bf = subL->bf = 0;
}

//将AVL树进行右单旋转
template<class _data, class _key>
void AVLTree<_data, _key>::RotateR(AVLNode<_data, _key>*& ptr)
{
	AVLNode<_data, _key>* subR = ptr;
	ptr = subR->left;
	subR->left = ptr->right;
	ptr->right = subR;
	ptr->bf = subR->bf = 0;
}

//将AVL树进行先左后右双旋
template<class _data, class _key>
void AVLTree<_data, _key>::RotateLR(AVLNode<_data, _key>*& ptr)
{
	AVLNode<_data, _key>* subR = ptr, * subL = ptr->left;
	ptr = subL->right;
	subL->right = ptr->left;
	ptr->left = subL;

	if (ptr->bf <= 0)
		subL->bf = 0;
	else
		subL->bf = -1;

	subR->left = ptr->right;
	ptr->right = subR;

	if (ptr->bf == -1)
		subR->bf = 1;
	else
		subR->bf = 0;

	ptr->bf = 0;
}

//将AVL树进行先右后左双旋
template<class _data, class _key>
void AVLTree<_data, _key>::RotateRL(AVLNode<_data, _key>*& ptr)
{
	AVLNode<_data, _key>* subL = ptr, * subR = ptr->right;
	ptr = subR->left;
	subR->left = ptr->right;
	ptr->right = subR;

	if (ptr->bf >= 0)
		subR->bf = 0;
	else
		subR->bf = 1;

	subL->right = ptr->left;
	ptr->left = subL;

	if (ptr->bf == 1)
		subL->bf = -1;
	else
		subL->bf = 0;

	ptr->bf = 0;
}

//往AVL树中插入一个元素过程部分
template<class _data, class _key>
bool AVLTree<_data, _key>::Insert(AVLNode<_data, _key>*& ptr, const _key x, const _data d)
{
	AVLNode<_data, _key>* pr = NULL, * p = ptr;
	LinkedStack<AVLNode<_data, _key>* > stack;

	/*1、寻找插入的位置*/
	while (p != NULL) {
		if (x == p->key)
			return false;

		pr = p;
		stack.push(pr);

		if (x < p->key)
			p = p->left;
		else
			p = p->right;
	}

	/*2、创建新结点*/
	p = new AVLNode<_data, _key>(d, x);
	size++;
	if (p == NULL) {
		cerr << "存储分配失败！" << endl;
		exit(1);
	}

	/*3、分配结点位置*/
	if (pr == NULL) {
		ptr = p;
		return true;
	}

	if (x < pr->key)
		pr->left = p;
	else
		pr->right = p;

	/*4、重新调整，使得AVL树平衡*/
	while (!stack.IsEmpty()) {
		stack.pop(pr);

		if (p == pr->left)
			pr->bf--;
		else
			pr->bf++;

		if (pr->bf == 0)
			break;
		if (pr->bf == 1 || pr->bf == -1)
			p = pr;
		else {
			int d = (pr->bf < 0) ? -1 : 1;
			if (p->bf == d) {
				if (d == -1)
					RotateR(pr);
				else
					RotateL(pr);
			}
			else {
				if (d == -1)
					RotateLR(pr);
				else
					RotateRL(pr);
			}
			break;
		}
	}

	/*5、其他调整*/
	if (stack.IsEmpty())
		ptr = pr;
	else {
		AVLNode<_data, _key>* q;
		stack.getTop(q);

		if (q->key > pr->key)
			q->left = pr;
		else
			q->right = pr;
	}

	return true;
}

//从AVL树中删去一个元素过程部分
template<class _data, class _key>
bool AVLTree<_data, _key>::Remove(AVLNode<_data, _key>*& ptr, const _key x, _data& d)
{
	AVLNode<_data, _key>* pr = NULL, * p = ptr;
	LinkedStack<AVLNode<_data, _key>* > stack;

	/*1、寻找删除位置*/
	while (p != NULL) {
		if (x == p->key)
			break;

		pr = p;
		stack.push(pr);

		if (x < p->key)
			p = p->left;
		else
			p = p->right;
	}

	if (p == NULL)  /*未找到待删除元素，返回false*/
		return false;
	else
		d = p->data;

	AVLNode<_data, _key>* q, * ppr = NULL;

	/*2、被删结点有两个子女情况*/
	if (p->left != NULL && p->right != NULL) {
		pr = p;
		stack.push(pr);
		q = p->left;

		while (q->right != NULL) {  /*找中序遍历的前一个结点，把值赋给要删除的节点*/
			pr = q;
			stack.push(pr);
			q = q->right;  /*结束后pr是q的双亲，p不动为了后续赋值*/
		}

		p->data = q->data;
		p->key = q->key;
		p = q;  /*被删结点转化为q*/
	}

	/*3、被删结点最多有一个子女情况*/
	else if (p->left != NULL) {
		pr = p;
		stack.push(pr);
		q = p->left;
		p->data = q->data;
		p->key = q->key;
		p = q;
	}
	else if (p->right != NULL) {
		pr = p;
		stack.push(pr);
		q = p->right;
		p->data = q->data;
		p->key = q->key;
		p = q;
	}
	else
		q = p;

	/*3.2.1、被删结点是根节点*/
	if (pr == NULL)
		ptr = NULL;

	/*3.2.2、被删结点不是根结点*/
	else {
		int d, dd;

		/*3.2.3、重新调整，使得AVL树平衡*/
		while (!stack.IsEmpty()) {
			stack.pop(pr);

			if (pr->right == q)  /*根据要删结点对于pr的位置进行bf调整*/
				pr->bf--;
			else
				pr->bf++;

			if (!stack.IsEmpty()) {
				stack.getTop(ppr);
				dd = (ppr->left == pr) ? -1 : 1;
			}
			else
				dd = 0;

			if (pr->bf == 1 || pr->bf == -1)  /*pr没失衡，结束调整*/
				break;
			if (pr->bf != 0) {  /*pr失衡*/
				if (pr->bf < 0) {  /*pr的bf为负，用d做标记*/
					d = -1;
					q = pr->left;
				}

				else {  /*pr的bf为正，用d做标记*/
					d = 1;
					q = pr->right;
				}

				if (q->bf == 0) {
					if (d == -1) {
						RotateR(pr);
						pr->bf = 1;
						pr->left->bf = -1;
					}

					else {
						RotateL(pr);
						pr->bf = -1;
						pr->right->bf = 1;
					}

					break;
				}

				if (q->bf == d) {
					if (d == -1)
						RotateR(pr);
					else
						RotateL(pr);
				}

				else {
					if (d == -1)
						RotateLR(pr);
					else
						RotateRL(pr);
				}

				if (dd == -1)
					ppr->left = pr;
				else if (dd == 1)
					ppr->right = pr;
			}

			q = pr;
		}

		if (stack.IsEmpty())
			ptr = pr;

		q = ptr;
		while (q != NULL) {
			if (q->left == p) {
				q->left = NULL;
				break;
			}
			else if (q->right == p) {
				q->right = NULL;
				break;
			}

			if (q->key < p->key)
				q = q->right;
			else
				q = q->left;
		}
	}

	delete p;
	size--;

	return true;
}

//求AVL树的高度递归部分（空树为0）
template<class _data, class _key>
int AVLTree<_data, _key>::Height(AVLNode<_data, _key>* ptr)const
{
	if (ptr == NULL)
		return 0;
	
	const int l = (ptr->left) ? Height(ptr->left) : 0;
	const int r = (ptr->right) ? Height(ptr->right) : 0;

	return (l > r) ? (l + 1) : (r + 1);
}

//以visit的访问方式遍历整个AVL树递归部分
template<class _data, class _key>
void AVLTree<_data, _key>::Traversal(AVLNode<_data, _key>* ptr, _AVLTreeVisitFunction visit)const
{
	if (ptr == NULL)
		return;

	Traversal(ptr->left, visit);
	visit(ptr);
	Traversal(ptr->right, visit);
}

//将AVL树中的元素信息写入文件的递归部分
template<class _data, class _key>
void AVLTree<_data, _key>::Write(AVLNode<_data, _key>* ptr, ostream& ostr)const
{
	if (ptr == NULL)
		return;

	Write(ptr->left, ostr);
	ostr << ptr->data;
	Write(ptr->right, ostr);
}

//判断AVL树是否为空
template<class _data, class _key>
bool AVLTree<_data, _key>::IsEmpty()const
{
	return (root == NULL);
}

//判断AVL树是否已满
template<class _data, class _key>
bool AVLTree<_data, _key>::IsFull()const
{
	return false;
}

//从AVL树中取关键码为x的元素
template<class _data, class _key>
bool AVLTree<_data, _key>::getData(const _key x, _data& d)const
{
	AVLNode<_data, _key>* result = Search(x, root);

	if (result == NULL)
		return false;

	d = result->data;
	return true;
}

//设置AVL树中的元素值（不改变关键码）
template<class _data, class _key>
bool AVLTree<_data, _key>::setData(const _key x, const _data d)
{
	AVLNode<_data, _key>* result = Search(x, root);

	if (result == NULL)
		return false;

	result->data = d;
	return true;
}

//设置AVL树中的元素值（改变关键码）
template<class _data, class _key>
bool AVLTree<_data, _key>::setData(const _key oldKey, const _key newKey, const _data d)
{
	_data oldData;
	if (!Remove(oldKey, oldData))
		return false;

	Insert(d, newKey);
	return true;
}

//判断关键码为x的元素在不在AVL树中
template<class _data, class _key>
bool AVLTree<_data, _key>::Search(const _key x)const
{ 
	return (Search(x, root) == NULL);
}

//清空AVL树
template<class _data, class _key>
void AVLTree<_data, _key>::makeEmpty()
{ 
	makeEmpty(root);
	root = NULL;
	size = 0;
}

//取AVL树的根
template<class _data,class _key>
AVLNode<_data, _key>* AVLTree<_data, _key>::getRoot()const
{
	return root;
}

//取AVL树最小元素
template<class _data, class _key>
bool AVLTree<_data, _key>::getMin(_data& d)const
{
	AVLNode<_data, _key>* result = getMin(root);
	if (result == NULL)
		return false;

	d = result->data;
	return true;
}

//取AVL树最大元素
template<class _data, class _key>
bool AVLTree<_data, _key>::getMax(_data& d)const
{
	AVLNode<_data, _key>* result = getMax(root);
	if (result == NULL)
		return false;

	d = result->data;
	return true;
}

//取AVL树元素个数
template<class _data,class _key>
int AVLTree<_data, _key>::getSize()const
{
	return size;
}

//往AVL树中插入一个元素
template<class _data, class _key>
bool AVLTree<_data, _key>::Insert(const _data d, const _key x)
{
	return Insert(root, x, d);
}

//从AVL树中删去一个元素
template<class _data, class _key>
bool AVLTree<_data, _key>::Remove(_key x, _data& d)
{
	return Remove(root, x, d);
}

//求AVL树的高度
template<class _data, class _key>
int AVLTree<_data, _key>::Height()const
{
	return Height(root);
}

//以visit的访问方式遍历整个AVL树（中序遍历）
template<class _data, class _key>
void AVLTree<_data, _key>::Traversal(_AVLTreeVisitFunction visit)const
{
	Traversal(root, visit);
}

//将AVL树中的元素信息写入文件中（中序顺序）
template<class _data, class _key>
bool AVLTree<_data, _key>::Write()const
{
	ofstream writer;
	char filename[32];
	cout << "请输入文件名（输入#取消）：";
	cin >> filename;
	if (filename[0] == '#')
		return false;

	writer.open(filename, ios::out | ios::binary);
	if (!writer) {
		cerr << "文件" << filename << "打开失败！" << endl;
		return false;
	}

	Write(root, writer);
	writer.close();
	return true;
}

//重载函数：赋值
template<class _data, class _key>
AVLTree<_data, _key>& AVLTree<_data, _key>::operator=(const AVLTree<_data, _key>& tree)
{
	makeEmpty();
	RefValue = tree.RefValue;
	size = tree.size;
	root = copyTree(tree.root);

	return *this;
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