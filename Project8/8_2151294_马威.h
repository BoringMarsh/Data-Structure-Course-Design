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

#ifndef _GRAPHLIST_H_
#define _GRAPHLIST_H_

/***************************************************************************
  
  本头文件定义的是表示  有权  简单图的  邻接表，使用时请注意！！

***************************************************************************/

#include<iostream>
#include<stdlib.h>
#include<climits>
using namespace std;

#define LIST_VERTICES_DEFAULT_SIZE 20

//IsConnected函数中的节点访问方式（为空）
template<class _vertex>
void IsConnected_visit(_vertex vertex)
{

}

template<class _vertex,class _edge>
struct Edge {
	int dest;
	_edge cost;
	Edge<_vertex, _edge>* link;

	Edge() :dest(-1), link(NULL) {}
	Edge(const int num, const _edge weight, Edge<_vertex, _edge>* first = NULL) :dest(num), cost(weight), link(first) {}

	inline bool operator!=(const Edge<_vertex, _edge>& edge)const
	{
		return (dest != edge.dest);
	}
};

template<class _vertex,class _edge>
struct Vertex {
	_vertex data;
	Edge<_vertex, _edge>* adj;
};

template<class _vertex,class _edge>
class GraphList {
private:
	int maxVertices;
	int numEdges;
	int numVertices;
	Vertex<_vertex, _edge>* NodeTable;
	const bool if_directed;

	typedef void(*_GraphListVisitFunction)(_vertex vertex);

	void DFS_Recursion(const int v, bool visited[], _GraphListVisitFunction visit)const;
	bool IsNonDirectedConnected()const;
public:
	GraphList(const bool ifDirected, const int sz = LIST_VERTICES_DEFAULT_SIZE);
	GraphList(const bool ifDirected, const GraphList<_vertex, _edge>& graph);
	~GraphList();

	int NumOfVertices()const;
	int NumOfEdges()const;
	void ShowList()const;
	bool IsVerticesEmpty()const;
	bool IsEdgesEmpty()const;
	bool IsVerticesFull()const;
	bool IsEdgesFull()const;
	bool IsConnected()const;
	int getVertexPos(_vertex vertex)const;
	bool getVertex(_vertex& vertex, const int pos)const;
	bool getEdge(_edge& edge, const int v1, const int v2)const;
	int getFirstNeighbor(const int v)const;
	int getNextNeighbor(const int v, const int w)const;
	bool insertVertex(const _vertex vertex);
	bool insertEdge(const int v1, const int v2, _edge cost);
	bool removeVertex(const int v);
	bool removeEdge(const int v1, const int v2);

	void DFS_Single(const _vertex v, _GraphListVisitFunction visit)const;
	void DFS_All(_GraphListVisitFunction visit)const;
	void BFS_Single(const _vertex v, _GraphListVisitFunction visit, bool Visited[] = NULL)const;
	void BFS_All(_GraphListVisitFunction visit)const;

	GraphList<_vertex, _edge>& operator=(const GraphList<_vertex, _edge>& graph);

	template<class V, class E>  //不是成员函数，需要有自己的模板
	friend istream& operator>>(istream& in, GraphList<V, E>& Graph);

	template<class V, class E>  //不是成员函数，需要有自己的模板
	friend ostream& operator<<(ostream& out, const GraphList<V, E>& Graph);
};

//重载函数：输入
template<class _vertex, class _edge>
istream& operator>>(istream& in, GraphList<_vertex, _edge>& Graph)
{
	int ver_num, edge_num;
	_vertex v1, v2;
	_edge edge;

	in >> ver_num >> edge_num;

	for (int i = 0; i < ver_num; i++) {
		in >> v1;
		Graph.insertVertex(v1);
	}

	int count = 0;
	while (count < edge_num) {
		in >> v1 >> v2 >> edge;

		int e1 = Graph.getVertexPos(v1);
		int e2 = Graph.getVertexPos(v2);

		if (e1 == -1 || e2 == -1) {
			cout << "边两端点信息输入错误，请重新输入！" << endl;
			cin.clear();
			cin.ignore(INT_MAX, '\n');
		}
			
		else {
			Graph.insertEdge(e1, e2, edge);
			count++;
		}
	}

	return in;
}

//重载函数：输出
template<class _vertex, class _edge>
ostream& operator<<(ostream& out, const GraphList<_vertex, _edge>& Graph)
{
	const int ver_num = Graph.NumOfVertices();
	const int edge_num = Graph.NumOfEdges();
	out << "vertices:" << ver_num << endl;
	out << "edges:" << edge_num << endl;

	for (int i = 0; i < ver_num; i++) {
		for (int j = 0; j < ver_num; j++) {  //无向图从i+1开始
			_edge e;
			bool if_get = Graph.getEdge(e, i, j);

			if (if_get) {
				_vertex v1, v2;
				Graph.getVertex(v1, i);
				Graph.getVertex(v2, j);
				out << '<' << v1 << ',' << v2 << '>' << "-->" << e << endl;
			}
		}
	}

	return out;
}

//指定大小的构造函数
template<class _vertex, class _edge>
GraphList<_vertex, _edge>::GraphList(const bool ifDirected, const int sz) : if_directed(ifDirected)
{
	maxVertices = sz;
	numVertices = 0;
	numEdges = 0;

	NodeTable = new Vertex<_vertex, _edge>[maxVertices];
	if (NodeTable == NULL) {
		cerr << "存储分配错误！" << endl;
		exit(1);
	}

	for (int i = 0; i < maxVertices; i++)
		NodeTable[i].adj = NULL;
}

//复制构造函数
template<class _vertex, class _edge>
GraphList<_vertex, _edge>::GraphList(const bool ifDirected, const GraphList<_vertex, _edge>& Graph) : if_directed(ifDirected)
{
	maxVertices = Graph.maxVertices;
	numVertices = Graph.numVertices;
	numEdges = Graph.numEdges;

	NodeTable = new Vertex<_vertex, _edge>[maxVertices];
	if (NodeTable == NULL) {
		cerr << "存储分配失败！" << endl;
		exit(1);
	}

	for (int i = 0; i < numVertices; i++) {
		NodeTable[i].data = Graph.NodeTable[i].data;

		Edge<_vertex, _edge>* src = Graph.NodeTable[i].adj, * newNode, * current = NULL;

		if (src == NULL)
			NodeTable[i].adj = NULL;

		while (src != NULL) {
			newNode = new Edge<_vertex, _edge>(src->dest, src->cost);
			if (newNode == NULL) {
				cerr << "存储分配失败！" << endl;
				exit(1);
			}

			if (src == Graph.NodeTable[i].adj)
				NodeTable[i].adj = newNode;
			else
				current->link = newNode;
			current = newNode;

			src = src->link;
		}
	}
}

//析构函数
template<class _vertex,class _edge>
GraphList<_vertex, _edge>::~GraphList()
{
	for (int i = 0; i < numVertices; i++) {
		Edge<_vertex, _edge>* p = NodeTable[i].adj;

		while (p != NULL) {
			NodeTable[i].adj = p->link;
			delete p;
			p = NodeTable[i].adj;
		}
	}

	delete[] NodeTable;
}

//返回顶点vertex在顶点表中的位置
template<class _vertex, class _edge>
int GraphList<_vertex, _edge>::getVertexPos(_vertex vertex)const
{
	for (int i = 0; i < numVertices; i++) {
		if (NodeTable[i].data == vertex)
			return i;
	}

	return -1;
}

//DFS递归部分
template<class _vertex, class _edge>
void GraphList<_vertex, _edge>::DFS_Recursion(const int v, bool visited[], _GraphListVisitFunction visit)const
{
	_vertex ver;
	getVertex(ver, v);
	visit(ver);

	visited[v] = true;
	int w = getFirstNeighbor(v);

	while (w != -1) {
		if (visited[w] == false)
			DFS_Recursion(w, visited, visit);

		w = getNextNeighbor(v, w);
	}
}

//返回当前顶点数
template<class _vertex, class _edge>
int GraphList<_vertex, _edge>::NumOfVertices()const
{
	return numVertices;
}

//返回当前边数
template<class _vertex, class _edge>
int GraphList<_vertex, _edge>::NumOfEdges()const
{
	return numEdges;
}

//展示邻接表
template<class _vertex, class _edge>
void GraphList<_vertex, _edge>::ShowList()const
{
	for (int i = 0; i < numVertices; i++) {
		cout << NodeTable[i].data << "->";
		Edge<_vertex, _edge>* p = NodeTable[i].adj;

		while (p != NULL) {
			if (p->link != NULL)
				cout << p->dest << "->";
			else
			 	cout << p->dest << endl;

			p = p->link;
		}
	}
}

//判断图的顶点数是否为零
template<class _vertex, class _edge>
bool GraphList<_vertex, _edge>::IsVerticesEmpty()const
{
	return (numVertices == 0);
}

//判断图的边数是否为零
template<class _vertex, class _edge>
bool GraphList<_vertex, _edge>::IsEdgesEmpty()const
{
	return (numEdges == 0);
}

//判断图的顶点数是否已满
template<class _vertex, class _edge>
bool GraphList<_vertex, _edge>::IsVerticesFull()const
{
	if (numVertices == maxVertices)
		return true;
	else
		return false;
}

//判断图的边数是否已满
template<class _vertex, class _edge>
bool GraphList<_vertex, _edge>::IsEdgesFull()const
{
	const int limit = if_directed ? numVertices * (numVertices - 1) : numVertices * (numVertices - 1) / 2;

	if (numEdges == limit)
		return true;
	else
		return false;
}

//判断一个无向图是否连通
template<class _vertex,class _edge>
bool GraphList<_vertex, _edge>::IsNonDirectedConnected()const
{
	if (numVertices != 1 && IsEdgesEmpty())
		return false;

	bool* visited = new bool[numVertices];
	if (visited == NULL) {
		cerr << "存储分配失败！" << endl;
		exit(1);
	}

	for (int i = 0; i < numVertices; i++)
		visited[i] = false;

	BFS_Single(NodeTable[0].data, IsConnected_visit<_vertex>, visited);

	for (int i = 0; i < numVertices; i++) {
		if (!visited[i])
			return false;
	}

	delete[] visited;
	return true;
}

//判断一个图是否连通（若为有向图，判断是否为弱连通）
template<class _vertex, class _edge>
bool GraphList<_vertex, _edge>::IsConnected()const
{
	if (!if_directed)
		return IsNonDirectedConnected();

	GraphList<_vertex, _edge> graph(false, numVertices);
	for (int i = 0; i < numVertices; i++)
		graph.insertVertex(NodeTable[i].data);

	for (int i = 0; i < numVertices; i++) {
		Edge<_vertex, _edge>* p = NodeTable[i].adj;

		while (p != NULL) {
			graph.insertEdge(i, p->dest, p->cost);
			p = p->link;
		}
	}

	return graph.IsNonDirectedConnected();
}

//取顶点pos，pos不合理返回false
template<class _vertex, class _edge>
bool GraphList<_vertex, _edge>::getVertex(_vertex& vertex, const int pos)const
{
	if (pos < 0 || pos >= numVertices)
		return false;

	vertex = NodeTable[pos].data;
	return true;
}

//取边<v1,v2>或(v1,v2)上的权值
template<class _vertex, class _edge>
bool GraphList<_vertex, _edge>::getEdge(_edge& edge, const int v1, const int v2)const
{
	if (v1 < 0 || v1 >= numVertices || v2 < 0 || v2 >= numVertices)
		return false;

	Edge<_vertex, _edge>* p = NodeTable[v1].adj;
	
	while (p != NULL && p->dest != v2)
		p = p->link;

	if (p != NULL) {
		edge = p->cost;
		return true;
	}

	return false;
}

//取顶点v的第一个邻接顶点
template<class _vertex, class _edge>
int GraphList<_vertex, _edge>::getFirstNeighbor(const int v)const
{
	if (v >= 0 && v < numVertices) {
		Edge<_vertex, _edge>* p = NodeTable[v].adj;

		if (p != NULL)
			return p->dest;
	}

	return -1;
}

//取v邻接顶点w的下一邻接顶点
template<class _vertex, class _edge>
int GraphList<_vertex, _edge>::getNextNeighbor(const int v, const int w)const
{
	if (v >= 0 && v < numVertices && w >= 0 && w < numVertices) {
		Edge<_vertex, _edge>* p = NodeTable[v].adj;

		while (p != NULL && p->dest != w)
			p = p->link;

		if (p != NULL && p->link != NULL)
			return p->link->dest;
	}

	return -1;
}

//插入顶点vertex
template<class _vertex, class _edge>
bool GraphList<_vertex, _edge>::insertVertex(const _vertex vertex)
{
	if (numVertices == maxVertices)
		return false;

	NodeTable[numVertices++].data = vertex;
	return true;
}

//插入边<v1,v2>，权值为cost
template<class _vertex, class _edge>
bool GraphList<_vertex, _edge>::insertEdge(const int v1, const int v2, _edge cost)
{
	if (v1 >= 0 && v1 < numVertices && v2 >= 0 && v2 < numVertices) {
		Edge<_vertex, _edge>* p = NodeTable[v1].adj;

		while (p != NULL && p->dest != v2)
			p = p->link;

		if(p != NULL)
			return false;

		p = new Edge<_vertex, _edge>(v2, cost, NodeTable[v1].adj);
		if (p == NULL) {
			cerr << "存储分配失败！" << endl;
			exit(1);
		}
		NodeTable[v1].adj = p;

		numEdges++;

		if (if_directed)
			return true;
	}

	if (!if_directed && v1 >= 0 && v1 < numVertices && v2 >= 0 && v2 < numVertices) {
		Edge<_vertex, _edge>* p = NodeTable[v2].adj;

		p = new Edge<_vertex, _edge>(v1, cost, NodeTable[v2].adj);
		if (p == NULL) {
			cerr << "存储分配失败！" << endl;
			exit(1);
		}
		NodeTable[v2].adj = p;

		return true;
	}

	return false;
}

//删去顶点v和所有与它相关联的边
template<class _vertex, class _edge>
bool GraphList<_vertex, _edge>::removeVertex(const int v)
{
	if (v < 0 || v >= numVertices)
		return false;

	if (numVertices == 1)
		return false;

	Edge<_vertex, _edge>* p, * s;

	/*1、删除该顶点的出边表*/
	while (NodeTable[v].adj != NULL) {
		p = NodeTable[v].adj;
		NodeTable[v].adj = p->link;
		delete p;
		numEdges--;
	}

	/*2、删除该顶点的入边*/
	for (int i = 0; i < numVertices; i++) {
		if (i == v)
			continue;

		p = NodeTable[i].adj;
		s = NULL;

		while (p != NULL && p->dest != v) {
			s = p;
			p = p->link;
		}

		if (p != NULL) {
			if (s == NULL)
				NodeTable[i].adj = p->link;
			else
			 	s->link = p->link;

			if (if_directed)
				numEdges--;
			
			delete p;
			continue;
		}
	}

	/*3、最后一个顶点顶上删除顶点的位置，并更换数据*/
	NodeTable[v] = NodeTable[numVertices - 1];
	for (int i = 0; i < numVertices; i++) {
		if (i == v)
			continue;

		else if (i == numVertices - 1) {
			NodeTable[i].adj = NULL;
			continue;
		}

		p = NodeTable[i].adj;

		while (p != NULL && p->dest != numVertices - 1)
			p = p->link;

		if (p != NULL) {
			p->dest = v;
			continue;
		}
	}

	numVertices--;
	return true;
}

//在图中删去边(v1,v2)或<v1,v2>
template<class _vertex, class _edge>
bool GraphList<_vertex, _edge>::removeEdge(const int v1, const int v2)
{
	if (v1 >= 0 && v1 < numVertices && v2 >= 0 && v2 < numVertices) {
		Edge<_vertex, _edge>* p = NodeTable[v1].adj, * q = NULL, * s = p;

		while (p != NULL && p->dest != v2) {
			q = p;
			p = p->link;
		}

		if (p != NULL) {
			if (p == s)
				NodeTable[v1].adj = p->link;
			else
				q->link = p->link;

			numEdges--;
			delete p;
		}

		else
			return false;

		if (if_directed)
			return true;
			
		p = NodeTable[v2].adj;
		q = NULL;
		s = p;

		while (p->dest != v1) {
			q = p;
			p = p->link;
		}

		if (p == s)
			NodeTable[v2].adj = p->link;
		else
			q->link = p->link;

		delete p;
		return true;
	}

	return false;
}

//深度优先遍历图（若为非连通图，则遍历顶点所在最大连通分量）
template<class _vertex, class _edge>
void GraphList<_vertex, _edge>::DFS_Single(const _vertex v, _GraphListVisitFunction visit)const
{
	const int ver_num = numVertices;
	int loc = getVertexPos(v);

	if (loc == -1) {
		cout << "顶点不存在！" << endl;
		return;
	}

	bool* visited = new bool[ver_num];
	if (visited == NULL) {
		cerr << "存储分配错误！" << endl;
		exit(1);
	}

	for (int i = 0; i < ver_num; i++)
		visited[i] = false;

	DFS_Recursion(loc, visited, visit);
	delete[] visited;
}

//广度优先遍历图（若为非连通图，则遍历顶点所在最大连通子图）
template<class _vertex, class _edge>
void GraphList<_vertex, _edge>::BFS_Single(const _vertex v, _GraphListVisitFunction visit, bool Visited[])const
{
	const int ver_num = numVertices;
	int w;
	int loc = getVertexPos(v);

	if (loc == -1) {
		cout << "顶点不存在！" << endl;
		return;
	}

	bool* visited = NULL;

	if (Visited) {
		visited = Visited;
	}

	else {
		visited = new bool[ver_num];
		if (visited == NULL) {
			cerr << "存储分配错误！" << endl;
			exit(1);
		}

		for (int i = 0; i < ver_num; i++)
			visited[i] = false;
	}

	visit(v);
	visited[loc] = true;

	LinkedQueue<int> queue;
	queue.EnQueue(loc);

	while (!queue.IsEmpty()) {
		queue.DeQueue(loc);
		w = getFirstNeighbor(loc);

		while (w != -1) {
			if (!visited[w]) {
				_vertex vertex;
				getVertex(vertex, w);

				visit(vertex);
				visited[w] = true;
				queue.EnQueue(w);
			}

			w = getNextNeighbor(loc, w);
		}
	}

	if (Visited == NULL)
		delete[] visited;
}

//深度优先遍历图（若为非连通图，则逐个遍历连通分量）
template<class _vertex,class _edge>
void GraphList<_vertex, _edge>::DFS_All(_GraphListVisitFunction visit)const
{
	const int ver_num = numVertices;

	bool* visited = new bool[ver_num];
	if (visited == NULL) {
		cerr << "存储分配失败！" << endl;
		exit(1);
	}

	for (int i = 0; i < ver_num; i++)
		visited[i] = false;

	for (int i = 0; i < ver_num; i++) {
		if (!visited[i]) {
			DFS_Recursion(i, visited, visit);
			cout << endl;
		}
	}

	delete[] visited;
}

//广度优先遍历图（若为非连通图，则逐个遍历连通分量）
template<class _vertex, class _edge>
void GraphList<_vertex, _edge>::BFS_All(_GraphListVisitFunction visit)const
{
	const int ver_num = numVertices;

	bool* visited = new bool[ver_num];
	if (visited == NULL) {
		cerr << "存储分配错误！" << endl;
		exit(1);
	}

	for (int i = 0; i < ver_num; i++)
		visited[i] = false;

	for (int i = 0; i < ver_num; i++) {
		if (!visited[i]) {
			_vertex vertex;
			getVertex(vertex, i);

			BFS_Single(vertex, visit, visited);
			cout << endl;
		}
	}

	delete[] visited;
}

//重载函数：赋值
template<class _vertex,class _edge>
GraphList<_vertex, _edge>& GraphList<_vertex, _edge>::operator=(const GraphList<_vertex, _edge>& graph)
{
	for (int i = 0; i < numVertices; i++) {
		Edge<_vertex, _edge>* p = NodeTable[i].adj;

		while (p != NULL) {
			NodeTable[i].adj = p->link;
			delete p;
			p = NodeTable[i].adj;
		}
	}

	delete[] NodeTable;

	maxVertices = graph.maxVertices;
	numVertices = graph.numVertices;
	numEdges = graph.numEdges;

	NodeTable = new Vertex<_vertex, _edge>[maxVertices];
	if (NodeTable == NULL) {
		cerr << "存储分配失败！" << endl;
		exit(1);
	}

	for (int i = 0; i < numVertices; i++) {
		NodeTable[i].data = graph.NodeTable[i].data;

		Edge<_vertex, _edge>* src = graph.NodeTable[i].adj, * newNode, * current = NULL;

		if (src == NULL)
			NodeTable[i].adj = NULL;

		while (src != NULL) {
			newNode = new Edge<_vertex, _edge>(src->dest, src->cost);
			if (newNode == NULL) {
				cerr << "存储分配失败！" << endl;
				exit(1);
			}

			if (src == graph.NodeTable[i].adj)
				NodeTable[i].adj = newNode;
			else
				current->link = newNode;
			current = newNode;

			src = src->link;
		}
	}

	return *this;
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

#ifndef _UFSETS_H_
#define _UFSETS_H_

#include<iostream>
#include<stdlib.h>
#include<climits>
using namespace std;

#define UFSETS_DEFAULT_SIZE 10

class UFSets {
private:
	int* parents;
	int size;
public:
	UFSets(const int sz = UFSETS_DEFAULT_SIZE);
	UFSets(const UFSets& UFSet);
	~UFSets();

	UFSets& operator=(const UFSets& UFSet);

	void Union(const int Root1, const int Root2);
	int Find(const int x);
	void Show()const;
};

//构造函数
UFSets::UFSets(const int sz)
{
	size = sz;
	parents = NULL;
	
	parents = new int[size];
	if (parents == NULL) {
		cerr << "存储分配失败！" << endl;
		exit(1);
	}

	for (int i = 0; i < size; i++)
		parents[i] = -1;
}

//复制构造函数
UFSets::UFSets(const UFSets& UFSet)
{
	size = UFSet.size;
	parents = NULL;

	parents = new int[size];
	if (parents == NULL) {
		cerr << "存储分配失败！" << endl;
		exit(1);
	}

	for (int i = 0; i < size; i++)
		parents[i] = UFSet.parents[i];
}

//析构函数
UFSets::~UFSets()
{
	delete[] parents;
}

//重载函数：赋值
UFSets& UFSets::operator=(const UFSets& UFSet)
{
	size = UFSet.size;
	delete[] parents;
	parents = NULL;

	parents = new int[size];
	if (parents == NULL) {
		cerr << "存储分配失败！" << endl;
		exit(1);
	}

	for (int i = 0; i < size; i++)
		parents[i] = UFSet.parents[i];

	return *this;
}

//搜寻集合x的根
int UFSets::Find(const int x)
{
	if (x < 0 || x >= size)
		return INT_MIN;

	int current = x;

	while (parents[current] >= 0)
		current = parents[current];

	return current;
}

//两个子集合合并
void UFSets::Union(const int Root1, const int Root2)
{
	if (Root1 < 0 || Root1 >= size || Root2 < 0 || Root2 >= size)
		return;

	const int r1 = Find(Root1), r2 = Find(Root2);
	int temp;

	if (r1 != r2) {
		temp = parents[r1] + parents[r2];

		if (parents[r2] < parents[r1]) {
			parents[r1] = r2;
			parents[r2] = temp;
		}

		else {
			parents[r2] = r1;
			parents[r1] = temp;
		}
	}
}

//显示并查集中的元素状况
void UFSets::Show()const
{
	for (int i = 0; i < size; i++)
		cout << parents[i] << ' ';
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