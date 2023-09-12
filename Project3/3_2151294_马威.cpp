#include<iostream>
#include<stdio.h>
#include<climits>
#include"3_2151294_马威.h"
using namespace std;

#define LINE 15
#define ROW  15

#define UP    1
#define DOWN  2
#define LEFT  3
#define RIGHT 4

struct point {
	int line;
	int row;

	point(const int _line = -1, const int _row = -1) :line(_line), row(_row) {}
};

struct maps {
	int waynum;
	bool if_wall;
	bool waymark;

	maps(const bool ifWall = true) :waynum(-1), if_wall(ifWall), waymark(false) {}
};

struct Astar_maps {
	int line;
	int row;
	int direction;
	int F;
	bool if_closed;

	Astar_maps(const int l = -1, const int r = -1, const int d = 0, const int f = 0)
		:line(l), row(r), direction(d), F(f), if_closed(false) {}

	inline bool operator<(const Astar_maps& a)
	{
		return (F < a.F);
	}
	inline bool operator>(const Astar_maps& a)
	{
		return (F > a.F);
	}
	inline bool operator<=(const Astar_maps& a)
	{
		return (F <= a.F);
	}
	inline bool operator>=(const Astar_maps& a)
	{
		return (F >= a.F);
	}
};

struct astar_maps {
	int direction;
	bool if_closed;

	astar_maps(const int d = 0) :direction(d), if_closed(false) {}
};

/***************************************************************************
  函数名称：cls
  功    能：清理屏幕
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
  功    能：程序断点，按回车继续
  输入参数：无
  返 回 值：无
  说    明：本函数属于函数集：通用
***************************************************************************/
void pause()
{
	char c;
	cout << "按回车键继续. . . ";

	while (1) {
		c = getchar();

		if (c == '\n' || c == '\r')
			break;
	}
}

/***************************************************************************
  函数名称：printMaze
  功    能：打印迷宫
  输入参数：const Vector<maps> matrix[]：迷宫（只读）
			const char* way_prompt：路径标志（只读）
  返 回 值：无
  说    明：本函数属于函数集：通用
***************************************************************************/
void printMap(const Vector<maps> matrix[], const char* way_prompt)
{
	for (int i = 0; i < LINE; i++) {
		for (int j = 0; j < ROW; j++) {
			if (i == 1 && j == 0)
				cout << "入";
			else if (i == LINE - 1 && j == ROW - 2)
				cout << "出";
			else if (matrix[i][j].if_wall)
				cout << "口";
			else if (matrix[i][j].waymark)
				cout << way_prompt;
			else
				cout << "  ";
		}

		cout << endl;
	}
}

/***************************************************************************
  函数名称：printway
  功    能：文字显示路径
  输入参数：const Vector<point>& way_coord：路径位置的数组（只读）
			const point start：起点（只读）
  返 回 值：无
  说    明：本函数属于函数集：通用
***************************************************************************/
void printway(const Vector<point>& way_coord, const point start)
{
	cout << "路径：" << "(" << start.line << ',' << start.row << ")-->";
	Vector<point>::iterator iter = way_coord.last();  /*寻路算法原因，一般是倒序存入vector的*/

	while (iter > way_coord.begin()) {
		cout << "(" << (*iter).line << ',' << (*iter).row << ") --> ";
		iter--;
	}

	cout << "(" << (*iter).line << ',' << (*iter).row << ")" << endl;
}

/***************************************************************************
  函数名称：matrix_clear
  功    能：清空内部数组
  输入参数：Vector<maps> matrix[]：迷宫
  返 回 值：无
  说    明：本函数属于函数集：通用
***************************************************************************/
void matrix_clear(Vector<maps> matrix[])
{
	Vector<maps>::iterator iter;

	for (int i = 0; i < LINE; i++) {
		iter = matrix[i].begin();

		while (iter != matrix[i].end()) {
			(*iter).waynum = -1;
			(*iter).waymark = false;
			iter++;
		}
	}
}

/***************************************************************************
  函数名称：if_complete
  功    能：判断当前位置周围是否完成标记
  输入参数：const Vector<maps> matrix[]：迷宫（只读）
			const point current：当前位置（只读）
  返 回 值：bool：当前位置是否标记完成
  说    明：本函数属于函数集：BFS
***************************************************************************/
bool if_complete(const Vector<maps> matrix[], const point current)
{
	/*若当前位置上下左右四个位置是墙或已记录步数，则标记完成，否则未完成*/
	if (current.line > 1 && (matrix[current.line - 1][current.row].if_wall || matrix[current.line - 1][current.row].waynum != -1)
		&& current.line < LINE - 1 && (matrix[current.line + 1][current.row].if_wall || matrix[current.line + 1][current.row].waynum != -1)
		&& current.row > 1 && (matrix[current.line][current.row - 1].if_wall || matrix[current.line][current.row - 1].waynum != -1)
		&& current.row < ROW - 1 && (matrix[current.line][current.row + 1].if_wall || matrix[current.line][current.row + 1].waynum != -1))
		return true;
	else
		return false;
}

/***************************************************************************
  函数名称：countway_single_scan
  功    能：对当前位置进行处理
  输入参数：Vector<maps> matrix[]：迷宫
			LinkedQueue<point>& countway_points：存放下一轮统计位置的队列
			const point current：当前位置（只读）
  返 回 值：无
  说    明：本函数属于函数集：BFS
***************************************************************************/
void countway_single_scan(Vector<maps> matrix[], LinkedQueue<point>& countway_points, const point current)
{
	point p;

	/*检查上下左右四个元素，若该元素存在、位置为空且移动步数情况未统计，则进行统计。若该元素标记未完成，使其入队*/
	/*1、检查正上方元素*/
	if (current.line > 1 && !matrix[current.line - 1][current.row].if_wall && matrix[current.line - 1][current.row].waynum == -1) {
		matrix[current.line - 1][current.row].waynum = matrix[current.line][current.row].waynum + 1;  /*上方元素步数多1*/
		p = point(current.line - 1, current.row);

		if (!if_complete(matrix, p))  /*上方元素标记未完成，入队*/
			countway_points.EnQueue(p);
	}

	/*2、检查正下方元素*/
	if (current.line < LINE - 1 && !matrix[current.line + 1][current.row].if_wall && matrix[current.line + 1][current.row].waynum == -1) {
		matrix[current.line + 1][current.row].waynum = matrix[current.line][current.row].waynum + 1;  /*下方元素步数多1*/
		p = point(current.line + 1, current.row);

		if (!if_complete(matrix, p))  /*下方元素标记未完成，入队*/
			countway_points.EnQueue(p);
	}

	/*3、检查左侧元素*/
	if (current.row > 1 && !matrix[current.line][current.row - 1].if_wall && matrix[current.line][current.row - 1].waynum == -1) {
		matrix[current.line][current.row - 1].waynum = matrix[current.line][current.row].waynum + 1;  /*左侧元素步数多1*/
		p = point(current.line, current.row - 1);

		if (!if_complete(matrix, p))  /*左侧元素标记未完成，入队*/
			countway_points.EnQueue(p);
	}

	/*4、检查右侧元素*/
	if (current.row < ROW - 1 && !matrix[current.line][current.row + 1].if_wall && matrix[current.line][current.row + 1].waynum == -1) {
		matrix[current.line][current.row + 1].waynum = matrix[current.line][current.row].waynum + 1;  /*右侧元素步数多1*/
		p = point(current.line, current.row + 1);

		if (!if_complete(matrix, p))  /*右侧元素标记未完成，入队*/
			countway_points.EnQueue(p);
	}

	countway_points.DeQueue(p);  /*当前位置已标记完成，直接在此处出队*/
}

/***************************************************************************
  函数名称：countway
  功    能：从起点开始，将迷宫中所有位置都统计步数
  输入参数：Vector<maps> matrix[]：迷宫
			const point start：起点（只读）
			const point dst：终点（只读）
  返 回 值：无
  说    明：本函数属于函数集：BFS
***************************************************************************/
void countway(Vector<maps> matrix[], const point start, const point dst)
{
	if (start.line == dst.line && start.row == dst.row)  /*起点与终点相同，不做任何操作*/
		return;

	LinkedQueue<point> countway_points;  /*存储待处理位置的队列*/

	matrix[start.line][start.row].waynum = 0;  /*起点处的移动步数为0*/
	countway_points.EnQueue(point(start.line, start.row));  /*起点入队*/
	point p;

	while (!countway_points.IsEmpty()) {  /*重复操作至队列为空*/
		countway_points.getFront(p);  /*取队列头部元素*/

		if (if_complete(matrix, p))  /*若其标记完成，出队*/
			countway_points.DeQueue(p);

		else  /*若其未标记完成，则对该位置进行处理*/
			countway_single_scan(matrix, countway_points, p);
	}
}

/***************************************************************************
  函数名称：BFS_findway
  功    能：用BFS算法寻找一条路径
  输入参数：Vector<maps> matrix[]：迷宫
			Vector<point>& way_coord：路径位置的数组
			const point start：起点（只读）
			const point dst：终点（只读）
  返 回 值：bool：路径是否找到
  说    明：本函数属于函数集：BFS
***************************************************************************/
bool BFS_findway(Vector<maps> matrix[], Vector<point>& way_coord, const point start, const point dst)
{
	countway(matrix, start, dst);  /*统计所有可移动到的位置到起点的最短步数*/

	if (matrix[dst.line][dst.row].waynum == -1)  /*若统计完成后终点步数仍为-1，则路径不存在，返回假*/
		return false;

	int line = dst.line;
	int row = dst.row;

	for (int i = matrix[dst.line][dst.row].waynum; i > 0; i--) {  /*每一次循环寻找到路径的其中一步，步数由终点位置元素的移动步数来决定*/

		/*检查上下左右四个元素，若该元素存在、移动步数比当前位置少1且不为零，
		则进行路径标记，并将当前位置记录到数组way_coord[]中，同时将指针移动到下一步*/
		/*1、检查正上方元素*/
		if (line && matrix[line - 1][row].waynum == matrix[line][row].waynum - 1 && matrix[line][row].waynum) {
			way_coord.push_back(point(line, row));  //压入当前位置坐标
			matrix[line][row].waymark = true;
			line--;
			continue;
		}

		/*2、检查正下方元素*/
		if (line < LINE + 1 && matrix[line + 1][row].waynum == matrix[line][row].waynum - 1 && matrix[line][row].waynum) {
			way_coord.push_back(point(line, row));  //压入当前位置坐标
			matrix[line][row].waymark = true;
			line++;
			continue;
		}

		/*3、检查左侧元素*/
		if (row && matrix[line][row - 1].waynum == matrix[line][row].waynum - 1 && matrix[line][row].waynum) {
			way_coord.push_back(point(line, row));  //压入当前位置坐标
			matrix[line][row].waymark = true;
			row--;
			continue;
		}

		/*4、检查右侧元素*/
		if (row < ROW + 1 && matrix[line][row + 1].waynum == matrix[line][row].waynum - 1 && matrix[line][row].waynum) {
			way_coord.push_back(point(line, row));  //压入当前位置坐标
			matrix[line][row].waymark = true;
			row++;
			continue;
		}
	}

	return true;
}

/***************************************************************************
  函数名称：direction_count
  功    能：统计当前位置可以继续行进的方向并保存
  输入参数：const Vector<maps> matrix[]：迷宫（只读）
			Vector<point>& next：下一步可行位置的数组
			const point me：行进到的当前位置（只读）
  返 回 值：int：可行方向数
  说    明：本函数属于函数集：DFS
***************************************************************************/
int direction_count(const Vector<maps> matrix[], Vector<point>& next, const point me)
{
	int count = 0;
	next.clear();   /*清空原有记录*/

	/*检查上下左右四个元素，若该元素存在、位置为空且未被标记，则进行统计*/
	/*1、检查上方元素*/
	if (me.line > 1 && !matrix[me.line - 1][me.row].if_wall && !matrix[me.line - 1][me.row].waymark) {
		next.push_back(point(me.line - 1, me.row));
		count++;
	}

	/*2、检查下方元素*/
	if (me.line < LINE - 1 && !matrix[me.line + 1][me.row].if_wall && !matrix[me.line + 1][me.row].waymark) {
		next.push_back(point(me.line + 1, me.row));
		count++;
	}

	/*3、检查左侧元素*/
	if (me.row > 1 && !matrix[me.line][me.row - 1].if_wall && !matrix[me.line][me.row - 1].waymark) {
		next.push_back(point(me.line, me.row - 1));
		count++;
	}

	/*4、检查右侧元素*/
	if (me.row < ROW - 1 && !matrix[me.line][me.row + 1].if_wall && !matrix[me.line][me.row + 1].waymark) {
		next.push_back(point(me.line, me.row + 1));
		count++;
	}

	return count;
}

/***************************************************************************
  函数名称：count_check
  功    能：对某一位置的可行方向数做相应处理
  输入参数：const int count：可行方向数（只读）
			LinkedStack<point>& intersections：存放各路口位置的栈
			LinkedStack<point>& marks：存放走过位置的栈
			point& me：行进到的当前位置
  返 回 值：bool：是否仍有路可走
  说    明：本函数属于函数集：DFS
***************************************************************************/
bool count_check(const int count, LinkedStack<point>& intersections, LinkedStack<point>& marks, point& me, const point start)
{
	if (!count) {
		point last;
		bool if_get = intersections.pop(last);  /*取上一个路口位置*/

		if (if_get) {
			me = last;  /*回到上一个路口位置*/
			point p;

			while (p.line != last.line || p.row != last.row)  /*将上一个路口之后走过的位置全部出栈*/
				marks.pop(p);

			if (p.line != start.line || p.row != start.row)  /*只记录除起点外的其他位置*/
				marks.push(p);
		}

		else  /*若栈已空，说明无路可走（不存在通向终点的路径），返回假*/
			return false;
	}

	else if (count > 1)  /*若可通行方向数大于1，满足路口条件，将该位置压入栈中*/
		intersections.push(point(me.line, me.row));

	return true;
}

/***************************************************************************
  函数名称：walk
  功    能：随机往前走一步
  输入参数：Vector<maps> matrix[]：迷宫
			Vector<point>& next：下一步可行位置的数组
			LinkedStack<point>& marks：存放走过位置的栈
			point& me：行进到的当前位置
  返 回 值：无
  说    明：本函数属于函数集：DFS
***************************************************************************/
void walk(Vector<maps> matrix[], Vector<point>& next, LinkedStack<point>& marks, point& me)
{
	if (next.empty())  /*若下一步无路可走，则不做任何操作*/
		return;

	int direction = rand() % next.length();  /*随机选择一个位置走过，将其位置压入栈中并做标记*/
	me = next[direction];
	marks.push(me);

	matrix[me.line][me.row].waymark = true;
}

/***************************************************************************
  函数名称：DFS_findway
  功    能：用BFS算法寻找一条路径
  输入参数：Vector<maps> matrix[]：迷宫
			Vector<point>& way_coord：路径位置的数组
			const point start：起点（只读）
			const point dst：终点（只读）
  返 回 值：bool：路径是否找到
  说    明：本函数属于函数集：DFS
***************************************************************************/
bool DFS_findway(Vector<maps> matrix[], Vector<point>& way_coord, const point start, const point dst)
{
	Vector<point> next;  /*下一步可行位置的数组*/
	LinkedStack<point> intersections;  /*存放各路口位置的栈*/
	LinkedStack<point> marks;  /*存放走过位置的栈*/

	matrix[start.line][start.row].waymark = true;  /*给起点做上标记并将其压入走过位置的栈*/
	marks.push(start);

	point me(start.line, start.row);  /*模拟一个正在行进的对象，初始位置是起点*/
	do {
		const int count = direction_count(matrix, next, me);  /*统计可行进方向*/
		const bool ret = count_check(count, intersections, marks, me, start);  /*对统计结果进行处理*/

		if (ret)  /*若结果正常，随机向前走一步*/
			walk(matrix, next, marks, me);
		else  /*若无路可走，停止寻路，返回假*/
			return false;

	} while (me.line != dst.line || me.row != dst.row);  /*走到终点时停止*/

	while (!marks.IsEmpty()) {  /*将栈中的元素转移到Vector中，并标记路径位置*/
		point p;
		marks.pop(p);
		way_coord.push_back(p);
		matrix[p.line][p.row].waymark = true;
	}

	return true;
}

/***************************************************************************
  函数名称：Fabs
  功    能：求某整型数的绝对值
  输入参数：const int num：待处理的数（只读）
  返 回 值：int：该数的绝对值
  说    明：本函数属于函数集：A*
***************************************************************************/
int Fabs(const int num)
{
	return (num >= 0) ? num : -1 * num;
}

/***************************************************************************
  函数名称：Astar_check
  功    能：将某个位置周围可以通行且不在closelist内的位置存入openlist中
  输入参数：const Vector<maps> matrix[]：迷宫（只读）
			Vector<astar_maps> Astar_map[]：辅助地图
			const point p：当前位置（只读）
			const point dst：终点（只读）
			MinHeap<Astar_maps>& openlist：待处理位置的最小堆
  返 回 值：bool：终点是否存入了openlist
  说    明：本函数属于函数集：A*
***************************************************************************/
bool Astar_check(const Vector<maps> matrix[], Vector<astar_maps> Astar_map[], const point p, const point dst, MinHeap<Astar_maps>& openlist)
{
	/*检查上下左右四个元素，若该元素存在、位置为空、方向未被记录且不在closelist中，则计算出其权值后放入openlist中*/
	/*1、检查上方*/
	if (p.line > 1 && !matrix[p.line - 1][p.row].if_wall
		&& Astar_map[p.line - 1][p.row].direction == 0
		&& !Astar_map[p.line - 1][p.row].if_closed) {
		openlist.Insert(Astar_maps(p.line - 1, p.row, UP, 1 + Fabs(p.line - 1 - dst.line) + Fabs(p.row - dst.row)));
		Astar_map[p.line - 1][p.row].direction = UP;
	}

	/*2、检查下方*/
	if (p.line < LINE - 1 && !matrix[p.line + 1][p.row].if_wall
		&& Astar_map[p.line + 1][p.row].direction == 0
		&& !Astar_map[p.line + 1][p.row].if_closed) {
		openlist.Insert(Astar_maps(p.line + 1, p.row, DOWN, 1 + Fabs(p.line + 1 - dst.line) + Fabs(p.row - dst.row)));
		Astar_map[p.line + 1][p.row].direction = DOWN;
	}

	/*3、检查左侧*/
	if (p.row > 1 && !matrix[p.line][p.row - 1].if_wall
		&& Astar_map[p.line][p.row - 1].direction == 0
		&& !Astar_map[p.line][p.row - 1].if_closed) {
		openlist.Insert(Astar_maps(p.line, p.row - 1, LEFT, 1 + Fabs(p.line - dst.line) + Fabs(p.row - 1 - dst.row)));
		Astar_map[p.line][p.row - 1].direction = LEFT;
	}

	/*4、检查右侧*/
	if (p.row < ROW - 1 && !matrix[p.line][p.row + 1].if_wall
		&& Astar_map[p.line][p.row + 1].direction == 0
		&& !Astar_map[p.line][p.row + 1].if_closed) {
		openlist.Insert(Astar_maps(p.line, p.row + 1, RIGHT, 1 + Fabs(p.line - dst.line) + Fabs(p.row + 1 - dst.row)));
		Astar_map[p.line][p.row + 1].direction = RIGHT;
	}

	/*5、终点被加入openlist了，说明路径找到，return*/
	if (dst.line == p.line - 1 && dst.row == p.row) {
		Astar_map[dst.line][dst.row].direction = UP;
		return true;
	}
	else if (dst.line == p.line + 1 && dst.row == p.row) {
		Astar_map[dst.line][dst.row].direction = DOWN;
		return true;
	}
	else if (dst.line == p.line && dst.row == p.row - 1) {
		Astar_map[dst.line][dst.row].direction = LEFT;
		return true;
	}
	else if (dst.line == p.line && dst.row == p.row + 1) {
		Astar_map[dst.line][dst.row].direction = RIGHT;
		return true;
	}

	return false;
}

/***************************************************************************
  函数名称：Astar_findway
  功    能：用A*算法寻找一条路径
  输入参数：Vector<maps> matrix[]：迷宫
			Vector<point>& way_coord：路径位置的数组
			const point start：起点（只读）
			const point dst：终点（只读）
  返 回 值：bool：路径是否找到
  说    明：本函数属于函数集：A*
***************************************************************************/
bool Astar_findway(const Vector<maps> matrix[], Vector<point>& way_coord, const point start, const point dst)
{
	Vector<astar_maps> Astar_map[LINE];  /*辅助地图，仅记录每个位置的方向和是否在closelist中*/
	for (int i = 0; i < LINE; i++)
		Astar_map[i].fill(ROW, astar_maps());

	MinHeap<Astar_maps> openlist(4 * (LINE - 1) * (ROW - 1));  /*存放待处理位置的最小堆*/
	bool if_end = false;  /*终点是否存入openlist*/
	openlist.Insert(Astar_maps(start.line, start.row));  /*将起点存入openlist（权值设为0，保证从起点开始搜索）*/

	while (!openlist.IsEmpty() && !if_end) {
		Astar_maps am(-1, -1);
		openlist.RemoveMin(am);  /*取权值最小元素*/

		if_end = Astar_check(matrix, Astar_map, point(am.line, am.row), dst, openlist);  /*将该元素周围位置存入openlist中*/
		Astar_map[am.line][am.row].if_closed = true;  /*将该元素存入closelist中*/

		if (if_end)  /*终点在openlist中，说明路径已找到，返回真*/
			break;
		else if (openlist.IsEmpty())  /*若openlist已空而终点还没存入过openlist中，则路径不存在，返回假*/
			return false;
	}

	point current(dst.line, dst.row);  /*模拟对象，初始位置为终点*/

	/*根据每个位置上的方向，找出该位置的直接前驱，同时记录下路径位置，直到回到起点为止*/
	while (current.line != start.line || current.row != start.row) {
		way_coord.push_back(current);
		matrix[current.line][current.row].waymark = true;

		switch (Astar_map[current.line][current.row].direction) {
		case UP:
			current.line++;
			break;
		case DOWN:
			current.line--;
			break;
		case LEFT:
			current.row++;
			break;
		case RIGHT:
			current.row--;
			break;
		}
	}

	return true;
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
    bool oMatrix[15][15] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                            0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,
                            1,0,0,0,1,0,1,0,1,0,0,0,0,0,1,
                            1,0,0,1,0,0,1,0,1,1,0,0,0,0,1,
                            1,1,0,1,0,1,0,0,0,1,0,0,0,0,1,
                            1,0,0,1,0,1,1,1,1,0,0,0,0,0,1,
                            1,1,0,1,0,0,0,0,0,1,0,0,0,0,1,
                            1,0,0,1,0,1,0,1,0,0,1,0,0,0,1,
                            1,0,1,0,0,1,1,0,1,0,0,0,0,0,1,
                            1,0,1,1,1,0,0,0,0,0,1,0,0,0,1,
                            1,0,0,0,0,0,1,0,1,0,0,0,0,0,1,
                            1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                            1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                            1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                            1,1,1,1,1,1,1,1,1,1,1,1,1,0,1};

    Vector<maps> matrix[15];  /*存储地图信息的向量数组*/
    Vector<point> way_coord;  /*存储路径信息的向量*/
    const point start = point(1, 1);  /*起点*/
    const point dst = point(13, 13);  /*终点*/
    bool if_on = true;
	bool if_get = true;

    for (int i = 0; i < 15; i++) {  /*用地图形状初始化向量数组*/
        for (int j = 0; j < 15; j++)
            matrix[i].push_back(maps(oMatrix[i][j]));
    }

    while (if_on) {
		cls();
		printMap(matrix, "  ");  /*打印地图*/

		cout << "1.BFS查找展示\n"
			<< "2.DFS查找展示\n"
			<< "3.A*查找展示\n"
			<< "0.退出程序\n"
			<< "\n请选择要进行的操作: ";

		char selection;
        while (1) {
            cin >> selection;
            cin.ignore(INT_MAX, '\n');

            if (selection >= '0' && selection <= '3')
                break;
        }

		switch (selection) {
		case '1':
			if_get = BFS_findway(matrix, way_coord, start, dst);  /*用广度优先搜索寻路*/
            break;
		case '2':
			if_get = DFS_findway(matrix, way_coord, start, dst);  /*用深度优先搜索寻路*/
			break;
		case '3':
			if_get = Astar_findway(matrix, way_coord, start, dst);  /*用A*算法寻路*/
			break;
		case '0':
			if_on = false;  /*退出程序*/
			break;
		}

		if (if_get && selection != '0') {
			cls();
			matrix[start.line][start.row].waymark = true;  /*起点标记，使得后续显示的时候起点也显示出来*/

			switch (selection) {  /*图形化打印路径*/
			case '1':
				printMap(matrix, "。");
				break;
			case '2':
				printMap(matrix, "。");
				break;
			case '3':
				printMap(matrix, "。");
				break;
			}
		
			printway(way_coord, start);  /*用坐标表示路径*/
			way_coord.clear();  /*清空向量*/
			matrix_clear(matrix);  /*地图初始化*/
			pause();
		}
		
		else if (selection != '0') {
			cout << "路径不存在！" << endl;
			pause();
		}
	}

	cls();
	cout << "程序已结束！" << endl;
	return 0;
}