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
	cout << "���    ��" << number << endl;
	cout << "������Ϣ��" << data << endl;
	cout << "����    ����" << generation << "��" << endl;
	
	if (parent == NULL)
		cout << "��һ��  ����" << endl;
	else
		cout << "��һ��  ��" << parent->data << endl;

	if (firstChild == NULL)
		cout << "��Ů    ����" << endl;
	else {
		cout << "ֱϵ��Ů��";
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

//Ĭ�Ϲ��캯��
template<class _class>
FamilyTree<_class>::FamilyTree()
{
	size = 0;
	maxGen = -1;
	root = current = NULL;
}

//�������ȵĹ��캯��
template<class _class>
FamilyTree<_class>::FamilyTree(const _class ancestor)
{
	size = 1;
	maxGen = 0;

	FamilyMem<_class>* newMem = new FamilyMem<_class>(ancestor, 0, 0);
	if (newMem == NULL) {
		cout << "�洢�������" << endl;
		exit(1);
	}

	root = current = newMem;
}

//���ƹ��캯��
template<class _class>
FamilyTree<_class>::FamilyTree(const FamilyTree<_class>& family)
{
	size = family.size;
	maxGen = family.maxGen;
	root = current = CopySubFamily(family.root);
}

//��������
template<class _class>
FamilyTree<_class>::~FamilyTree()
{
	RemoveSubFamily(root);
	delete root;
}

//������rootΪ��������������������root
template<class _class>
FamilyMem<_class>* FamilyTree<_class>::CopySubFamily(FamilyMem<_class>* root)
{
	if (root == NULL)
		return NULL;

	static const int gen_gap = root->generation;
	static const int num_gap = root->number;

	FamilyMem<_class>* newRoot = new FamilyMem<_class>(root->data, root->generation - gen_gap, root->number - num_gap);
	if (newRoot == NULL) {
		cout << "�洢�������" << endl;
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
				cout << "�洢�������" << endl;
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

//RemoveSubFamily�ݹ鲿��
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

//ShowSubFamily�ݹ鲿��
template<class _class>
void FamilyTree<_class>::ShowSubFamily_Recursion(const FamilyMem<_class>* thisAncestor, FamilyMem<_class>* thisRoot, const int gen_gap, bool* stillSibling)const
{
	if (root == NULL)
		return;

	FamilyMem<_class>* cur = thisRoot;

	while (cur != NULL) {
		for (int i = 0; i < cur->generation - gen_gap - 1; i++) {
			cout << (stillSibling[i] ? "��  " : "    ");
		}

		if (cur != thisAncestor) {
			if (cur->nextSibling) {
				cout << "��";
				stillSibling[cur->generation - gen_gap - 1] = true;
			}

			else {
				cout << "��";
				stillSibling[cur->generation - gen_gap - 1] = false;
			}

			cout << "��" << cur->data << endl;
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

//WriteFamily�ݹ鲿��
template<class _class>
void FamilyTree<_class>::WriteFamily_Recursion(const FamilyMem<_class>* thisAncestor, FamilyMem<_class>* thisRoot, bool* stillSibling, ofstream& writer)const
{
	if (root == NULL)
		return;

	FamilyMem<_class>* cur = thisRoot;

	while (cur != NULL) {
		for (int i = 0; i < cur->generation - 1; i++) {
			writer << (stillSibling[i] ? "��  " : "    ");
		}

		if (cur != thisAncestor) {
			if (cur->nextSibling) {
				writer << "��";
				stillSibling[cur->generation - 1] = true;
			}

			else {
				writer << "��";
				stillSibling[cur->generation - 1] = false;
			}

			writer << "��" << cur->data << endl;
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

//���غ���������
template<class _class>
FamilyTree<_class>& FamilyTree<_class>::operator=(const FamilyTree<_class>& family)
{
	size = family.size;
	maxGen = family.maxGen;
	root = current = CopySubFamily(family.root);
}

//ȡ��ǰ��������
template<class _class>
int FamilyTree<_class>::getSize()const
{
	return size;
}

//ȡ��ǰ����������
template<class _class>
int FamilyTree<_class>::getMaxGen()const
{
	return maxGen;
}

//ȡ����λ��
template<class _class>
FamilyMem<_class>* FamilyTree<_class>::getAncestor()const
{
	return root;
}

//ȡ��ǰ��Աλ��
template<class _class>
FamilyMem<_class>* FamilyTree<_class>::getCurrent()const
{
	return current;
}

//���ĵ�ǰ��Ա
template<class _class>
void FamilyTree<_class>::movCurrent(FamilyMem<_class>* dst)
{
	current = dst;
}

//�жϼ����Ƿ�Ϊ��
template<class _class>
bool FamilyTree<_class>::IsEmpty()const
{
	return(root == NULL);
}

//��������
template<class _class>
bool FamilyTree<_class>::SetAncestor(const _class ancestor)
{
	if (root != NULL) {
		cout << "�����������ȣ�" << endl;
		return false;
	}

	FamilyMem<_class>* newMem = new FamilyMem<_class>(ancestor, 0, 0);
	if (newMem == NULL) {
		cout << "�洢�������" << endl;
		exit(1);
	}

	root = current = newMem;
	return true;
}

//Ѱ�����ȣ�ʹ���Ϊ��ǰ��Ա
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

//Ѱ�ҵ�ǰ��Ա�ĵ�һ����Ů��ʹ֮��Ϊ��ǰ���
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

//Ѱ�ҵ�ǰ��Ա����һ���ֵܣ�ʹ֮��Ϊ��ǰ���
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

//Ѱ�ҵ�ǰ��Ա�ĳ�����ʹ֮��Ϊ��ǰ���
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

//��������������target�Ľ�㣨����������Ϊ��׼���������в��ҽ��ѹ��results��
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

//����ǰ��Ա����Ů��Ϊ��������ɸ���Ů
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
			cout << "�洢�������" << endl;
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

//����ǰ��Ա����Ů��Ϊ�����һ����Ů
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
		cout << "�洢�������" << endl;
		exit(1);
	}

	current->nextSibling = newMem;
	current = current->nextSibling;
	return true;
}

//���¼����������
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

//��ɢ��rootΪ���ļ�ͥ����ɺ�rootΪ��ǰ��Ա
template<class _class>
void FamilyTree<_class>::RemoveSubFamily(FamilyMem<_class>* root)
{
	RemoveSubFamily_Recursion(root);
	UpdateGen();
}

//������rootΪ���ļ��壬����visit��ʽ����ȫ����Ա
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

//չʾ�ֲ���ͥ���Ը����Ϊ���ȣ�����Ϊ���������
template<class _class>
void FamilyTree<_class>::ShowSubFamily(FamilyMem<_class>* root)const
{
	bool* stillSibling = new bool[maxGen];
	cout << endl;
	ShowSubFamily_Recursion(root, root, root->generation, stillSibling);
	cout << endl;
	delete[] stillSibling;
}

//������д���ļ�
template<class _class>
void FamilyTree<_class>::WriteFamily(FamilyMem<_class>* root)const
{
	bool* stillSibling = new bool[maxGen];
	if (stillSibling == NULL) {
		cerr << "�洢�������" << endl;
		exit(1);
	}

	ofstream writer;
	char filename[32];
	cout << "�������ļ���������#ȡ������";
	cin >> filename;
	cin.ignore(INT_MAX, '\n');
	if (filename[0] == '#')
		return;

	writer.open(filename, ios::out | ios::binary);
	if (!writer) {
		cerr << "�ļ�" << filename << "��ʧ�ܣ�" << endl;
		return;
	}

	WriteFamily_Recursion(root, root, stillSibling, writer);
	writer.close();
	delete[] stillSibling;
}