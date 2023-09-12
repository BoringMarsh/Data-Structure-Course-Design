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