#include<iostream>
#include<climits>
#include<stdio.h>
#include"8_2151294_马威.h"
using namespace std;

/***************************************************************************
  函数名称：print_menu
  功    能：打印菜单
  输入参数：无
  返 回 值：无
  说    明：无
***************************************************************************/
void print_menu()
{
	cout << "**                 电网造价模拟系统                  **\n"
		<< "=======================================================\n"
		<< "**               请选择要执行的操作：                **\n"
		<< "**                                                   **\n"
		<< "**    A --- 创建电网顶点      B --- 添加电网的边     **\n"
		<< "**                                                   **\n"
		<< "**    C --- 删除电网的点      D --- 删除电网的边     **\n"
		<< "**                                                   **\n"
		<< "**              E --- 构造最小生成树(Kruskal)        **\n"
		<< "**                                                   **\n"
		<< "**              F --- 构造最小生成树(Prim)           **\n"
		<< "**                                                   **\n"
		<< "**    G --- 显示最小生成树    H --- 清除最小生成树   **\n"
		<< "**                                                   **\n"
		<< "**              I --- 退出程序                       **\n"
		<< "=======================================================\n";
}

/***************************************************************************
  函数名称：cls
  功    能：清空屏幕
  输入参数：无
  返 回 值：无
  说    明：无
***************************************************************************/
void cls()
{
	if (system("clear"))
		system("cls");
}

/***************************************************************************
  函数名称：pause
  功    能：设置一个程序的断点
  输入参数：无
  返 回 值：无
  说    明：本函数只有按回车键才能继续
***************************************************************************/
void pause()
{
	cout << "按回车键继续. . . ";

	while (1) {
		char c = getchar();

		if (c == '\n' || c == '\r')
			break;
	}
}

/***************************************************************************
  函数名称：Kruskal
  功    能：用Kruskal算法生成最小生成树
  输入参数：const GraphList<_vertex, _edge>& graph：图（只读）
            MinSpanTree<_vertex, _edge>& tree：最小生成树
  返 回 值：无
  说    明：无
***************************************************************************/
template<class _vertex, class _edge>
void Kruskal(const GraphList<_vertex, _edge>& graph, MinSpanTree<_vertex, _edge>& tree)
{
	if (!graph.IsConnected()) {
		cout << "图不连通！" << endl;
		return;
	}

	const int ver_num = graph.NumOfVertices();  /*取点数*/
	const int edge_num = graph.NumOfEdges();    /*取边数*/
	MSTEdgeNode<_vertex, _edge> edge;
	MinHeap<MSTEdgeNode<_vertex, _edge> > heap(edge_num);  /*边的最小堆*/
	UFSets set(ver_num);  /*并查集*/

	for (int i = 0; i < ver_num; i++) {
		for (int j = i + 1; j < ver_num; j++) {  /*无向图，从i+1开始*/
			_edge ed;
			const bool if_get = graph.getEdge(ed, i, j);  /*逐个扫描，取边*/

			if (if_get) {
				graph.getVertex(edge.tail, i);
				graph.getVertex(edge.head, j);
				graph.getEdge(edge.key, i, j);
				heap.Insert(edge);  /*插入最小堆*/
			}
		}
	}

	int count = 1;

	while (count < ver_num) {
		heap.RemoveMin(edge);  /*取出最短边*/
		const int i = graph.getVertexPos(edge.tail);
		const int j = graph.getVertexPos(edge.head);
		const int k = set.Find(i);
		const int l = set.Find(j);

		if (k != l) {  /*若两个顶点不在同一连通分量，将该边放入最小生成树*/
			set.Union(k, l);
			tree.Insert(edge);
			count++;
		}
	}
}

/***************************************************************************
  函数名称：Prim
  功    能：用Prim算法生成最小生成树
  输入参数：const GraphList<_vertex, _edge>& graph：图（只读）
			const _vertex u0：起始顶点（只读）
			MinSpanTree<_vertex, _edge>& tree：最小生成树
  返 回 值：无
  说    明：无
***************************************************************************/
template<class _vertex, class _edge>
void Prim(const GraphList<_vertex, _edge>& graph, const _vertex u0, MinSpanTree<_vertex, _edge>& tree)
{
	if (!graph.IsConnected()) {
		cout << "图不连通！" << endl;
		return;
	}
	
	MSTEdgeNode<_vertex, _edge> edge;
	const int ver_num = graph.NumOfVertices();  /*取点数*/
	const int edge_num = graph.NumOfEdges();    /*取边数*/
	int u = graph.getVertexPos(u0);  /*取起始顶点编号*/
	MinHeap<MSTEdgeNode<_vertex, _edge> > heap(edge_num);  /*边的最小堆*/

	/*标志各点是否已被连接起来的数组，也可看做是集合，false为不在集合中，true为在集合中*/
	bool* Vmset = new bool[ver_num];
	for (int i = 0; i < ver_num; i++)
		Vmset[i] = false;
	Vmset[u] = true;  /*将起始顶点放入集合中*/

	int count = 1;

	while (count < ver_num) {
		int v = graph.getFirstNeighbor(u);  /*取第一个邻接顶点*/

		while (v != -1) {
			if (!Vmset[v]) {  /*若该邻接顶点不在集合中，将连接当前顶点和该邻接顶点的边放入最小堆中*/
				graph.getVertex(edge.tail, u);
				graph.getVertex(edge.head, v);
				graph.getEdge(edge.key, u, v);
				heap.Insert(edge);
			}

			v = graph.getNextNeighbor(u, v);  /*取下一邻接顶点，重复操作*/
		}

		while (!heap.IsEmpty() && count < ver_num) {
			heap.RemoveMin(edge);  /*取出最短边*/

			if (!Vmset[graph.getVertexPos(edge.head)]) {  /*若该边的另一头不在集合中，则将该边放入最小生成树，同时将另一头的顶点放入集合中*/
				tree.Insert(edge);
				u = graph.getVertexPos(edge.head);
				Vmset[u] = true;
				count++;
				break;
			}
		}
	}

	delete[] Vmset;
}

/***************************************************************************
  函数名称：AddVerrtices
  功    能：给图添加顶点
  输入参数：GraphList<_vertex, _edge>& graph：图
  返 回 值：无
  说    明：无
***************************************************************************/
template<class _vertex, class _edge>
void AddVertices(GraphList<_vertex, _edge>& graph)
{
	int count = 0;
	_vertex vertex;

	cout << "请依次输入各顶点的名称（输入#取消）：" << endl;

	while(1) {
		cin >> vertex;

		if (vertex == "#") {
			cin.ignore(INT_MAX, '\n');
			
			if (!count)
				cout << "添加顶点已取消" << endl;  /*直接取消添加的提示*/
			break;
		}

		graph.insertVertex(vertex);
		count++;
	}

	pause();
	cls();
}

/***************************************************************************
  函数名称：AddEdges
  功    能：给图添加边
  输入参数：GraphList<_vertex, _edge>& graph：图
  返 回 值：无
  说    明：无
***************************************************************************/
template<class _vertex, class _edge>
void AddEdges(GraphList<_vertex, _edge>& graph)
{
	_vertex vertex1, vertex2;
	_edge weight;
	int count = 0;
	cout << "请输入边信息以便插入，或输入两个#（空格分隔）以结束" << endl;

	while (1) {
		if (graph.IsEdgesFull()) {  /*实时判断边数，若已满则直接停止输入*/
			cin.ignore(INT_MAX, '\n');
			cout << "图已满，输入结束！" << endl;
			break;
		}

		cout << "请输入第" << count + 1 << "条边的两个顶点及权：";
		cin >> vertex1 >> vertex2;

		if (vertex1 == "#" || vertex2 == "#") {  /*输入两个#取消*/
			cin.ignore(INT_MAX, '\n');
			break;
		}

		while (1) {
			cin >> weight;

			if (cin.good() && weight > 0 && weight <= INT_MAX)
				break;

			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << "第" << count + 1 << "条边权输入错误！" << endl;
		}

		const int v1 = graph.getVertexPos(vertex1);
		const int v2 = graph.getVertexPos(vertex2);

		if (v1 == -1 || v2 == -1) {
			cout << "两顶点信息输入有误！" << endl;  /*顶点输入错误的提示*/
			continue;
		}

		graph.insertEdge(v1, v2, weight);
		count++;
	}

	pause();
	cls();
}

/***************************************************************************
  函数名称：RemoveVertices
  功    能：给图删除顶点
  输入参数：GraphList<_vertex, _edge>& graph：图
  返 回 值：无
  说    明：无
***************************************************************************/
template<class _vertex, class _edge>
void RemoveVertices(GraphList<_vertex, _edge>& graph)
{
	int count = 0;
	_vertex vertex;

	cout << "请依次输入各顶点的名称（输入#取消）：" << endl;

	while (1) {
		cin >> vertex;

		if (vertex == "#") {
			cin.ignore(INT_MAX, '\n');

			if (!count)
				cout << "删除顶点已取消" << endl;  /*直接取消删除的提示*/
			break;
		}

		int pos = graph.getVertexPos(vertex);

		if (pos == -1)
			cout << "顶点" << vertex << "不存在，删除失败！" << endl;
		else
			graph.removeVertex(pos);

		count++;
	}

	pause();
	cls();
}

/***************************************************************************
  函数名称：RemoveVertices
  功    能：给图删除边
  输入参数：GraphList<_vertex, _edge>& graph：图
  返 回 值：无
  说    明：无
***************************************************************************/
template<class _vertex, class _edge>
void RemoveEdges(GraphList<_vertex, _edge>& graph)
{
	_vertex vertex1, vertex2;
	int count = 0;
	cout << "请输入边信息以便删除，或输入两个#（空格分隔）以结束" << endl;

	while (1) {
		if (!graph.NumOfEdges()) {  /*实时判断边数，若已空则停止继续输入*/
			cin.ignore(INT_MAX, '\n');
			cout << "图已空，输入结束！" << endl;
			break;
		}

		cout << "请输入要删除边的两个顶点：";
		cin >> vertex1 >> vertex2;

		if (vertex1 == "#" && vertex2 == "#") {  /*输入两个#取消*/
			cin.ignore(INT_MAX, '\n');

			if (!count)
				cout << "删除边已取消" << endl;

			break;
		}

		const int v1 = graph.getVertexPos(vertex1);
		const int v2 = graph.getVertexPos(vertex2);

		if (v1 == -1 || v2 == -1) {
			cout << "两顶点信息输入有误！" << endl;
			continue;
		}

		if (!graph.removeEdge(v1, v2))
			cout << "边(" << vertex1 << ',' << vertex2 << ")不存在，删除失败！" << endl;
		count++;
	}

	pause();
	cls();
}

/***************************************************************************
  函数名称：SpanKruskal
  功    能：在正确情况下，用Kruskal算法给图生成最小生成树
  输入参数：const GraphList<_vertex, _edge>& graph：图（只读）
            MinSpanTree<_vertex, _edge>& tree：最小生成树
  返 回 值：无
  说    明：本函数对于最小树的生成进行了“错误处理”
***************************************************************************/
template<class _vertex,class _edge>
void SpanKruskal(const GraphList<_vertex, _edge>& graph, MinSpanTree<_vertex, _edge>& tree)
{
	if (graph.NumOfVertices() == 1)  /*若图只有一个顶点，则输出提示，不做任何操作*/
		cout << "只有一个顶点，不需生成最小生成树！" << endl;

	else if (!graph.IsConnected())  /*若图不连通，则输出提示，不做任何操作*/
		cout << "图不连通！" << endl;

	else if (tree.IsEmpty()) {  /*若最小树未生成过，图为空则不生成，图不为空则生成*/
		if (graph.IsEdgesEmpty())
			cout << "当前图为空！" << endl;

		else {
			tree.Resize(graph.NumOfEdges());
			Kruskal(graph, tree);
			cout << "最小树生成成功！" << endl;
		}
	}

	else  /*若最小树已生成过，则不生成*/
		cout << "最小树已生成过，请先清空！" << endl;

	pause();
	cls();
}

/***************************************************************************
  函数名称：SpanPrim
  功    能：在正确情况下，用Prim算法给图生成最小生成树
  输入参数：const GraphList<_vertex, _edge>& graph：图（只读）
			MinSpanTree<_vertex, _edge>& tree：最小生成树
  返 回 值：无
  说    明：本函数对于最小树的生成进行了“错误处理”
***************************************************************************/
template<class _vertex, class _edge>
void SpanPrim(const GraphList<_vertex, _edge>& graph, MinSpanTree<_vertex, _edge>& tree)
{
	if (graph.NumOfVertices() == 1)  /*若图只有一个顶点，则输出提示，不做任何操作*/
		cout << "只有一个顶点，不需生成最小生成树！" << endl;

	else if (!graph.IsConnected())  /*若图不连通，则输出提示，不做任何操作*/
		cout << "图不连通！" << endl;

	else if (tree.IsEmpty()) {  /*若最小树未生成过，图为空则不生成，图不为空则生成*/
		if (graph.IsEdgesEmpty())
			cout << "当前图为空！" << endl;

		else {  /*正确输入起始顶点后生成树*/
			String vertex;

			while (1) {
				cout << "请输入起始顶点：";
				cin >> vertex;
				cin.ignore(INT_MAX, '\n');

				if (graph.getVertexPos(vertex) != -1)
					break;

				cout << "顶点不存在！" << endl;
			}

			tree.Resize(graph.NumOfEdges());
			Prim(graph, vertex, tree);
			cout << "最小树生成成功！" << endl;
		}
	}

	else  /*若最小树已生成过，则不生成*/
		cout << "最小树已生成过，请先清空！" << endl;

	pause();
	cls();
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
	GraphList<String, int> graph(false);  /*图*/
	MinSpanTree<String, int> tree;   /*最小生成树*/
	bool if_on = true;

	while (if_on) {
		cls();
		print_menu();  /*打印菜单*/

		cout << "\n当前状态：" << graph.NumOfVertices() << "个顶点，"  /*打印当前状态*/
			<< graph.NumOfEdges() << "条边，"
			<< "最小树：" << (!tree.IsEmpty() ? "已生成" : "未生成") << endl;

		cout << "\n请选择操作：";
		char selection = '\0';

		while (1) {
			cin >> selection;
			cin.ignore(INT_MAX, '\n');

			if ((selection >= 'A' && selection <= 'I') || (selection >= 'a' && selection <= 'i'))
				break;

			cout << "请重新选择：";
		}

		switch (selection) {
		case 'A':
		case 'a':
			AddVertices(graph);
			break;
		case 'B':
		case 'b':
			AddEdges(graph);
			break;
		case 'C':
		case 'c':
			RemoveVertices(graph);
			break;
		case 'D':
		case 'd':
			RemoveEdges(graph);
			break;
		case 'E':
		case 'e':
			SpanKruskal(graph, tree);
			break;
		case 'F':
		case 'f':
			SpanPrim(graph, tree);
			break;
		case 'G':
		case 'g':
			if (!tree.IsEmpty())
				tree.Show();
			else
				cout << "最小树未生成过，请先生成！" << endl;

			pause();
			cls();
			break;
		case 'H':
		case 'h':
			tree.MakeEmpty();
			cout << "最小树清除成功！" << endl;

			pause();
			cls();
			break;
		case 'I':
		case 'i':
			if_on = false;
			cls();
			print_menu();
			cout << "\n系统已经关闭！" << endl;
			break;
		}
	}

	return 0;
}