#pragma once

#include<iostream>
#include<stdlib.h>
#include<fstream>
#include<climits>
#include"D:/Safe Zone/Tool Kit/Vector/Vector.h"
#include"D:/Safe Zone/Tool Kit/LinkedQueue/LinkedQueue.h"
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