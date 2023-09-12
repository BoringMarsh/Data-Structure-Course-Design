#include<iostream>
#include<stdlib.h>
#include<climits>
#include"9_2151294_马威.h"
using namespace std;

/***************************************************************************
  函数名称：TopologicalSort
  功    能：对图进行拓扑排序
  输入参数：const GraphList<_vertex, _edge>& graph：图（只读）
			int vertexOrder[]：顶点的拓扑序列
            int count[]：各顶点的入度表
  返 回 值：bool：图是否为拓扑有序
  说    明：无
***************************************************************************/
template<class _vertex, class _edge>
bool TopologicalSort(const GraphList<_vertex, _edge>& graph, int vertexOrder[], int count[])
{
	const int ver_num = graph.NumOfVertices();
	int top = -1;

	for (int i = 0; i < ver_num; i++) {
		if (!count[i]) {  /*将入度为零的顶点压入栈中*/
			count[i] = top;
			top = i;
		}
	}

	for (int i = 0; i < ver_num; i++) {
		if (top == -1) {  /*若还没经过n趟出栈后栈就为空，则有有向回路，图不为拓扑有序，返回假*/
			cout << '0' << endl;
			return false;
		}

		else {
			int v = -1;
			v = top;  /*从栈弹出一个入度为0顶点*/
			top = count[top];

			vertexOrder[i] = v;     /*将该顶点插入拓扑序列中*/
			
			int w = graph.getFirstNeighbor(v);

			while (w != -1) {
				if (--count[w] == 0) {  /*让v的所有邻接顶点入度-1，若入度变为0，压入栈中*/
					count[w] = top;
					top = w;
				}

				w = graph.getNextNeighbor(v, w);
			}
		}
	}

	return true;
}

/***************************************************************************
  函数名称：CrititicalPath
  功    能：求图的关键路径
  输入参数：const GraphList<_vertex, _edge>& graph：图（只读）
			const int vertexOrder[]：拓扑序列（只读）
  返 回 值：无
  说    明：无
***************************************************************************/
template<class _vertex,class _edge>
void CriticalPath(const GraphList<_vertex, _edge>& graph, const int vertexOrder[])
{
	const int ver_num = graph.NumOfVertices();
	_edge Ae, Al;

	/*1、创建Ve、Vl数组*/
	_edge* Ve = new _edge[ver_num];
	_edge* Vl = new _edge[ver_num];
	if (Ve == NULL || Vl == NULL) {
		cerr << "存储分配错误！" << endl;
		exit(1);
	}

	for (int i = 0; i < ver_num; i++)  /*Ve都初始化为0*/
		Ve[i] = 0;

	/*2、拓扑正序递推求解Ve*/
	for (int i = 0; i < ver_num; i++) {
		int tail = graph.getFirstNeighbor(vertexOrder[i]);  /*按拓扑序列求解*/

		while (tail != -1) {
			_edge e;
			graph.getEdge(e, vertexOrder[i], tail);

			if (Ve[vertexOrder[i]] + e > Ve[tail])  /*Ve[tail] = max{Ve[head] + dur(<head,tail>)}*/
				Ve[tail] = Ve[vertexOrder[i]] + e;

			tail = graph.getNextNeighbor(vertexOrder[i], tail);
		}
	}

	/*3、将Vl数组都初始化为Ve[n-1]，原因如下：
		(1)Vl[n-1]=Ve[n-1]
		(2)网络可能有多个终点，它们的Vl都为Ve[n-1]但下标不一定为n-1，这样做将这类特殊情况考虑到了
		(3)后续取最小值时有比较基准了（否则未初始化时，Vl都是一个绝对值很大的负数，比较会出现错误）
	*/
	for (int i = 0; i < ver_num; i++)
		Vl[i] = Ve[vertexOrder[ver_num - 1]];
	cout << Ve[vertexOrder[ver_num - 1]] << endl;    /*输出整个项目完成所需时间*/

	/*4、拓扑逆序递推求解Vl*/
	for (int i = ver_num - 2; i >= 0; i--) {
		int tail = graph.getFirstNeighbor(vertexOrder[i]);  /*按拓扑序列求解*/

		while (tail != -1) {
			_edge e;
			graph.getEdge(e, vertexOrder[i], tail);

			if (Vl[tail] - e < Vl[vertexOrder[i]])  /*Vl[head] = min{Vl[tail] - dur(<head,tail>)}*/
				Vl[vertexOrder[i]] = Vl[tail] - e;

			tail = graph.getNextNeighbor(vertexOrder[i], tail);
		}
	}

	/*4、求解关键活动*/
	for (int i = 0; i < ver_num; i++) {  /*为符合要求输出，直接从序号检查*/
		int j = graph.getFirstNeighbor(i);

		while (j != -1) {
			Ae = Ve[i];  /*计算Ae*/

			_edge e;
			graph.getEdge(e, i, j);
			Al = Vl[j] - e;  /*计算Al*/

			if (Al == Ae) {  /*若Al=Ae，则为关键活动，输出*/
				_vertex v1, v2;
				graph.getVertex(v1, i);
				graph.getVertex(v2, j);

				cout << v1 << "->" << v2 << endl;
			}

			j = graph.getNextNeighbor(i, j);
		}
	}

	delete[] Ve;
	delete[] Vl;
}

/***************************************************************************
  函数名称：main
  功    能：主函数
  输入参数：无
  返 回 值：0
  说    明：无
***************************************************************************/
int main()
{
	int ver_num = 0, edge_num = 0;

	/*1、输入顶点数和边数*/
	while (1) {
		cin >> ver_num >> edge_num;

		if (cin.good() && ver_num > 0 && ver_num <= 100 && edge_num >= 0 && edge_num <= ver_num * (ver_num - 1))
			break;

		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "顶点数或边数输入有误，请重新输入！" << endl;
	}

	/*2、构建图，插入顶点*/
	GraphList<int, int> graph(true, ver_num);
	for (int i = 0; i < ver_num; i++)
		graph.insertVertex(i + 1);

	/*3、构建顶点入度记录数组*/
	int* count = new int[ver_num];
	if (count == NULL) {
		cerr << "存储分配错误！" << endl;
		exit(1);
	}

	for (int i = 0; i < ver_num; i++)
		count[i] = 0;

	/*4、输入边信息，同时统计各点入度*/
	for (int times = 0; times < edge_num; times++) {
		int i, j, num = 0;
		int cost;

		while (1) {
			cin >> i >> j >> cost;

			//适应题目输入要求，已对范围进行调整
			if (cin.good() && i > 0 && i <= ver_num && j > 0 && j <= ver_num && cost >= 0 && cost <= INT_MAX) {
				graph.insertEdge(i - 1, j - 1, cost);
				num++;
				count[j - 1]++;
				break;
			}

			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << "第" << num + 1 << "条边信息有误，请重新输入！" << endl;
		}
	}

	/*5、为拓扑序列开辟空间*/
	int* vertexOrder = new int[ver_num];
	if (vertexOrder == NULL) {
		cerr << "存储分配错误！" << endl;
		exit(1);
	}

	/*6、若图不连通，则方案不可行，输出0*/
	if (!graph.IsConnected())
		cout << '0' << endl;

	/*7、进行拓扑排序，若拓扑有序，则计算关键路径*/
	else if (TopologicalSort(graph, vertexOrder, count))
		CriticalPath(graph, vertexOrder);

	delete[] count;
	delete[] vertexOrder;
	return 0;
}