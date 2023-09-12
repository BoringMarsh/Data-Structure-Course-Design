#pragma once

#include<iostream>
#include<climits>
#include<stdlib.h>
using namespace std;

#define MST_DEFAULT_SIZE 10

template<class _vertex,class _edge>
struct MSTEdgeNode {
	_vertex tail;
	_vertex head;
	_edge key;

	MSTEdgeNode() :key(0) {}

	inline bool operator<(const MSTEdgeNode<_vertex, _edge>& node)
	{
		return (key < node.key);
	}
	inline bool operator<=(const MSTEdgeNode<_vertex, _edge>& node)
	{
		return (key <= node.key);
	}
	inline bool operator>=(const MSTEdgeNode<_vertex, _edge>& node)
	{
		return (key >= node.key);
	}
	inline bool operator>(const MSTEdgeNode<_vertex, _edge>& node)
	{
		return (key > node.key);
	}
};

template<class _vertex,class _edge>
class MinSpanTree {
private:
	int size;
	int maxSize;
	MSTEdgeNode<_vertex, _edge>* edges;
public:
	MinSpanTree(const int sz = MST_DEFAULT_SIZE);

	bool IsEmpty()const;
	void MakeEmpty();
	bool Insert(const MSTEdgeNode<_vertex, _edge>& item);
	void Show()const;
	void Resize(const int sz = MST_DEFAULT_SIZE);
};

//指定大小的构造函数
template<class _vertex, class _edge>
MinSpanTree<_vertex, _edge>::MinSpanTree(const int sz)
{
	maxSize = sz;
	size = 0;

	edges = new MSTEdgeNode<_vertex, _edge>[maxSize];
	if (edges == NULL) {
		cerr << "存储分配错误！" << endl;
		exit(1);
	}
}

//判断最小树是否为空
template<class _vertex,class _edge>
bool MinSpanTree<_vertex, _edge>::IsEmpty()const
{
	return (size == 0);
}

//清空最小树
template<class _vertex, class _edge>
void MinSpanTree<_vertex, _edge>::MakeEmpty()
{
	size = 0;
}

//插入边结点
template<class _vertex, class _edge>
bool MinSpanTree<_vertex, _edge>::Insert(const MSTEdgeNode<_vertex, _edge>& item)
{
	if (size == maxSize)
		return false;

	edges[size++] = item;
	return true;
}

//打印最小生成树
template<class _vertex, class _edge>
void MinSpanTree<_vertex, _edge>::Show()const
{
	for (int i = 0; i < size; i++)
		cout << edges[i].tail << "--" << edges[i].head << ":" << edges[i].key << endl;
}

//扩大内部数组长度
template<class _vertex, class _edge>
void MinSpanTree<_vertex, _edge>::Resize(const int sz)
{
	if (sz <= 0)
		return;

	maxSize = (sz / MST_DEFAULT_SIZE + 1) * MST_DEFAULT_SIZE;
	MSTEdgeNode<_vertex, _edge>* oldData = edges;
	edges = new MSTEdgeNode<_vertex, _edge>[maxSize];

	for (int i = 0; i < size; i++)
		edges[i] = oldData[i];

	delete[] oldData;
}