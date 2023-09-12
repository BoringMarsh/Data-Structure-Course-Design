#include<iostream>
#include<stdio.h>
#include<iomanip>
#include<climits>
#include<fstream>
#include"1_2151294_马威.h"
using namespace std;

const int maxSize = 1000000;  /*最大人数*/

//存储考生信息的结构体
struct student {
	String num;     //考号
	String name;    //姓名
	String gender;  //性别
	int age;        //年龄
	String type;    //报考类别

	student() :num(""), name(""), gender(""), age(-1), type("") {}  //构造函数
};

//存储某个报考类别信息的结构体
struct typeinfo {
	String type;  //报考类别名称
	int count;    //数量

	typeinfo(String kind = "", const int num = -1) :type(kind), count(num) {}  //构造函数

	friend ostream& operator<<(ostream& out, const typeinfo& t)  //重载函数：输出
	{
		out << t.type << endl;
		out << t.count << "人" << endl << endl;
		return out;
	}
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
  说    明：无
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
  函数名称：(operator>>)
  功    能：重载student结构体的输入
  输入参数：istream& in：输入流对象
			student& stu：要输入信息的student结构体
  返 回 值：istream&：输入流对象
  说    明：本函数已对输入做错误处理
***************************************************************************/
istream& operator>>(istream& in, student& stu)
{
	while (1) {
		in >> stu.num >> stu.name >> stu.gender >> stu.age >> stu.type;
		in.ignore(INT_MAX, '\n');

		bool ok = true;  /*输入是否正确的标志*/

		if (!stu.num.isOnlyDigit()) {  /*判断考号*/
			cout << "考号输入错误！" << endl;
			ok = false;
		}

		if (!(stu.gender == "男") && !(stu.gender == "女")) {  /*判断性别*/
			cout << "性别输入错误！" << endl;
			ok = false;
		}

		if (!cin.good() || stu.age <= 0) {  /*判断年龄*/
			cout << "年龄输入错误！" << endl;
			ok = false;
		}

		if (ok)  /*输入正确，跳出循环*/
			break;
		else {  /*输入有误，重新输入*/
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			cout << "请重新输入该生所有信息" << endl;
		}
	}

	return in;
}

/***************************************************************************
  函数名称：(operator<<)
  功    能：重载student结构体的输出
  输入参数：ostream& out：输出流对象
			student& stu：要输出信息的student结构体
  返 回 值：ostream&：输出流对象
  说    明：本函数已对输出做格式调整
***************************************************************************/
ostream& operator<<(ostream& out, student& stu)
{
	out << setiosflags(ios::left)
	    << setw(8) << stu.num
		<< setw(10) << stu.name
		<< setw(6) << stu.gender
		<< setw(6) << stu.age
		<< stu.type << endl;

	return out;
}

/***************************************************************************
  函数名称：num_input
  功    能：在设置输入提示和正确处理下，输入一个整型数
  输入参数：const char* prompt：输入提示（只读）
			const int min：输入最小值（只读）
			const int max：输入最大值（只读）
  返 回 值：int：输入值
  说    明：无
***************************************************************************/
int num_input(const char* prompt, const int min, const int max)
{
	int i;

	while (1) {
		cout << prompt;
		cin >> i;
		cin.ignore(INT_MAX, '\n');

		if (cin.good() && i >= min && i <= max)
			break;

		cin.clear();
		cin.ignore(INT_MAX, '\n');
		cout << "输入错误，请重新输入！" << endl;
	}

	return i;
}

/***************************************************************************
  函数名称：store
  功    能：将AVL树内的元素按中序序列装入链表中（递归部分）
  输入参数：const AVLNode<student, int>* ptr：指向当前AVL树结点的指针（只读）
			DblList<student>& list：装载元素的链表
			const int num：装载数量最大值（只读）
			int& count：当前已装载数量
  返 回 值：无
  说    明：参数num、count用于限制装载数量，提高运行效率
***************************************************************************/
void store(const AVLNode<student, int>* ptr, DblList<student>& list, const int num, int& count)
{
	if (ptr == NULL || count >= num)
		return;

	store(ptr->left, list, num, count);  /*装载左子树*/

	if (count >= num)
		return;

	list.Insert(0, ptr->data, Left);  /*装载当前结点*/
	count++;  /*计数器自增*/

	if (count >= num)
		return;

	store(ptr->right, list, num, count);  /*装载右子树*/
}

/***************************************************************************
  函数名称：store
  功    能：将AVL树内的元素按中序序列装入链表中
  输入参数：const AVLTree<student, int>& System：数据来源的AVL树（只读）
			DblList<student>& list：装载元素的链表
			const int num：装载数量最大值（只读）
  返 回 值：无
  说    明：参数num用于限制装载数量，提高运行效率
***************************************************************************/
void store(const AVLTree<student, int>& System, DblList<student>& list, const int num = -1)
{
	int count = 0;  /*当前已装载数量，先设为0*/
	store(System.getRoot(), list, num, count);  /*执行递归部分*/
}

/***************************************************************************
  函数名称：Info_InsertType
  功    能：对新插入考生的报考类别做统计
  输入参数：DblList<typeinfo>& Info：统计链表
			const String& newType：新插入考生的报考类别（只读）
  返 回 值：无
  说    明：无
***************************************************************************/
void Info_InsertType(DblList<typeinfo>& Info, const String& newType)
{
	DblNode<typeinfo>* current = Info.getHead()->rLink;

	while (current != Info.getHead()) {  /*寻找新插入考生的报考类别是否存在*/
		if (current->data.type == newType)
			break;

		current = current->rLink;
	}

	if (current != Info.getHead()) {  /*若已找到，则对应类别数量自增*/
		(current->data.count)++;
		return;
	}

	Info.Insert(0, typeinfo(newType, 1), Left);  /*若未找到，则在尾部新插入一个结点，对应数量为1*/
}

/***************************************************************************
  函数名称：Info_RemoveType
  功    能：对新删除考生的报考类别做统计
  输入参数：DblList<typeinfo>& Info：统计链表
			const String& Type：新删除考生的报考类别（只读）
  返 回 值：无
  说    明：无
***************************************************************************/
void Info_RemoveType(DblList<typeinfo>& Info, const String& Type)
{
	DblNode<typeinfo>* current = Info.getHead()->rLink;
	int pos = 1;

	while (current != NULL) {  /*寻找新删除考生的报考类别是否存在*/
		if (current->data.type == Type)
			break;

		current = current->rLink;
		pos++;
	}

	if (current != NULL) {  /*若已找到，则对应类别数量自减*/
		(current->data.count)--;

		if (!(current->data.count)) {  /*若自减后数量为0，则删去对应结点*/
			typeinfo t;
			Info.Remove(pos, t, Right);
		}

		return;
	}
}

/***************************************************************************
  函数名称：System_Read
  功    能：从文件中读取考生信息
  输入参数：AVLTree<student, int>& System：存放考生信息的AVL树
            DblList<typeinfo>& Info：统计链表
			ifstream& reader：文件读对象
  返 回 值：无
  说    明：无
***************************************************************************/
void System_Read(AVLTree<student, int>& System, DblList<typeinfo>& Info, ifstream& reader)
{
	int count = 0;
	reader.seekg(count, ios::beg);

	while (reader.peek() != EOF) {
		student stu;  /*存放每轮读取信息的结构体*/
		stu.age = 0;  /*年龄先置为0*/

		for (int i = 0; i < 5; i++) {
			while (reader.peek() != ' ' && reader.peek() != '\n' && reader.peek() != EOF) {  /*读到空格或换行符停止*/
				switch (i) {
				case 0:
					stu.num += reader.peek();  /*读考号*/
					break;
				case 1:
					stu.name += reader.peek();  /*读姓名*/
					break;
				case 2:
					stu.gender += reader.peek();  /*读性别*/
					break;
				case 3:
					stu.age = stu.age * 10 + reader.peek() - '0';  /*读年龄*/
					break;
				case 4:
					stu.type += reader.peek();  /*读报考类别*/
					break;
				}
				
				reader.seekg(++count, ios::beg);
			}

			while (reader.peek() == ' ' || reader.peek() == '\n')  /*跳过多余空格或换行符*/
				reader.seekg(++count, ios::beg);
		}

		int key;
		stu.num.stringToInt(key);  /*取关键码*/
		System.Insert(stu, key);   /*插入AVL树中*/
		Info_InsertType(Info, stu.type);   /*对报考类别进行统计*/
	}
}

/***************************************************************************
  函数名称：System_Statistic
  功    能：统计所有学生信息，并打印出来
  输入参数：const AVLTree<student, int>& System：存放考生信息的AVL树（只读）
  返 回 值：无
  说    明：总人数超过15时，取前15个考生信息打印，并作出提示
***************************************************************************/
void System_Statistic(const AVLTree<student, int>& System)
{
	if (System.IsEmpty()) {
		cout << "\n当前系统内考生信息为空" << endl;
		return;
	}

	cout << "考号    姓名      性别  年龄  报考类别\n";
	DblList<student> list;
	store(System, list, 15);  /*装载前15个*/
	DblNode<student>* current = list.getHead()->rLink;

	while (current != list.getHead()) {  /*格式化打印考生信息*/
		cout << current->data;
		current = current->rLink;
	}

	if (System.getSize() > 15)  /*总人数超过15的提示*/
		cout << "共 " << System.getSize() << " 条记录，仅显示前 15 条" << endl;
}

/***************************************************************************
  函数名称：System_Statistic
  功    能：统计所有学生报考类别，并打印出来
  输入参数：const AVLTree<student, int>& System：存放考生信息的AVL树（只读）
			const DblList<typeinfo>& Info：统计链表（只读）
  返 回 值：无
  说    明：无
***************************************************************************/
void System_Statistic(const AVLTree<student, int>& System, const DblList<typeinfo>& Info)
{
	if (Info.IsEmpty())
		cout << "\n当前系统内考生信息为空" << endl;

	else {
		cls();
		cout << "报考类别及数量统计如下：\n";
		DblNode<typeinfo>* current = Info.getHead()->rLink;
		
		while (current != Info.getHead()) {  /*格式化打印各类报考类别的情况*/
			cout << current->data;
			current = current->rLink;
		}
	}

	pause();
	cls();
	System_Statistic(System);
}

/***************************************************************************
  函数名称：System_Insert
  功    能：插入一个学生的信息
  输入参数：AVLTree<student, int>& System：存放考生信息的AVL树
			DblList<typeinfo>& Info：统计链表
  返 回 值：无
  说    明：无
***************************************************************************/
void System_Insert(AVLTree<student, int>& System, DblList<typeinfo>& Info)
{
	cout << "请输入你要插入的考生的考号，姓名，性别，年龄及报考类别！" << endl;
	student stu;  /*存放输入的考生信息的结构体*/
	cin >> stu;   /*输入信息*/

	int key = 0;
	stu.num.stringToInt(key);  /*取关键码*/

	System.Insert(stu, key);  /*插入AVL树中*/
	Info_InsertType(Info, stu.type);  /*对报考类别进行统计*/
	cls();
	System_Statistic(System);
}

/***************************************************************************
  函数名称：System_Insert
  功    能：删除一个学生的信息
  输入参数：AVLTree<student, int>& System：存放考生信息的AVL树
			DblList<typeinfo>& Info：统计链表
  返 回 值：无
  说    明：无
***************************************************************************/
void System_Remove(AVLTree<student, int>& System, DblList<typeinfo>& Info)
{
	if (System.IsEmpty())
		cout << "系统内无任何考生信息，请至少输入一个考生的信息！" << endl;

	else {
		String num;
		int key = 0;

		do {
			cout << "请输入要删除的考生的考号：";
			cin >> num;
			cin.ignore(INT_MAX, '\n');
		} while (!num.isOnlyDigit());

		num.stringToInt(key);
		student stu;  /*存放删除考生信息的结构体*/

		if (System.getData(key, stu) && stu.num == num) {  /*若该考生存在，删除并进行统计*/
			cout << "你删除的考生信息是：\n" << stu;
			System.Remove(key, stu);
			Info_RemoveType(Info, stu.type);
		}
		else  /*若该考生不存在，输出提示*/
			cout << "该考生不存在！" << endl;
	}
	
	pause();
	cls();
	System_Statistic(System);
}

/***************************************************************************
  函数名称：System_Search
  功    能：搜索一个学生的信息
  输入参数：AVLTree<student, int>& System：存放考生信息的AVL树
  返 回 值：无
  说    明：无
***************************************************************************/
void System_Search(AVLTree<student, int>& System)
{
	if (System.IsEmpty())
		cout << "系统内无任何考生信息，无法查询！" << endl;

	else {
		String num;
		int key = 0;

		do {
			cout << "请输入要搜索的考生的考号：";
			cin >> num;
			cin.ignore(INT_MAX, '\n');
		} while (!num.isOnlyDigit());

		num.stringToInt(key);
		student stu;  /*存放考生信息的结构体*/

		if (System.getData(key, stu))  /*若该考生存在，输出信息*/
			cout << "查询结果：\n" << stu;
		else  /*若该考生不存在，输出提示*/
			cout << "该考生不存在！" << endl;
	}

	pause();
	cls();
	System_Statistic(System);
}

/***************************************************************************
  函数名称：System_Revise
  功    能：更改一个学生的信息
  输入参数：AVLTree<student, int>& System：存放考生信息的AVL树
			DblList<typeinfo>& Info：统计链表
  返 回 值：无
  说    明：无
***************************************************************************/
void System_Revise(AVLTree<student, int>& System, DblList<typeinfo>& Info)
{
	if (System.IsEmpty())
		cout << "系统内无任何考生信息，无法修改！" << endl;

	else {
		String num;
		int oldKey = 0, newKey = 0;  /*新旧关键码*/

		do {
			cout << "请输入要修改的考生的考号：";
			cin >> num;
			cin.ignore(INT_MAX, '\n');
		} while (!num.isOnlyDigit());

		num.stringToInt(oldKey);
		student stu;  /*存放考生原信息的结构体*/
		String oldType, newType;  /*新旧报考类别*/

		if (System.getData(oldKey, stu) && stu.num == num) {
			cout << "考生原信息：\n" << stu;  /*输出考生原信息*/
			oldType = stu.type;  /*取旧报考类别*/

			cout << "请输入考生新信息：" << endl;
			cin >> stu;  /*输入新信息*/
			newType = stu.type;  /*取新报考类别*/
			stu.num.stringToInt(newKey);  /*取新关键码*/

			/*根据是否改变了关键码进行更改操作*/
			newKey == oldKey ? System.setData(newKey, stu) : System.setData(oldKey, newKey, stu);
			//if (newKey == oldKey)  /*根据是否改变了关键码进行更改操作*/
				//System.setData(newKey, stu);
			//else
				//System.setData(oldKey, newKey, stu);

			if (newType != oldType) {  /*根据是否改变了报考类别进行统计操作*/
				Info_RemoveType(Info, oldType);
				Info_InsertType(Info, newType);
			}

			cout << "考生信息已修改！" << endl;
		}

		else
			cout << "该考生不存在！" << endl;
	}

	pause();
	cls();
	System_Statistic(System);
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
	AVLTree<student, int> System;  /*存放考生信息的AVL树*/
	DblList<typeinfo> Info;  /*统计链表*/

	cls();
	cout << "首先请建立考生信息系统！\n"
		<< "请选择建立方式：\n"
		<< "1.从文件读取\n"
		<< "2.手动输入\n"
		<< "您的选择是：";

	char way;

	/*选择建立方式*/
	while (1) {
		way = getchar();

		if (way >= '1' && way <= '2')
			break;
	}

	/*从文件读取*/
	if (way == '1') {
		while (1) {
			cout << "\n请输入文件名：";
			String filename;
			cin >> filename;
			cin.ignore(INT_MAX, '\n');

			ifstream reader;
			reader.open(filename.c_str(), ios::in);
			if (!reader) {
				cerr << "文件" << filename << "打开失败！是否重新输入(y/n)";
				char choice;

				while (1) {
					choice = getchar();

					if (choice == 'y' || choice == 'Y' || choice == 'n' || choice == 'N')
						break;
				}

				if (choice == 'n' || choice == 'N') {
					cout << "停止文件读取，已自动转向手动输入" << endl;
					way = '2';
					break;
				}
			}

			else {
				System_Read(System, Info, reader);
				cout << "读取完成！" << endl;
				reader.close();
				pause();
				cls();
				System_Statistic(System);
				break;
			}
		}
	}

	/*手动输入*/
	if (way == '2') {
		int num = num_input("\n请输入考生人数：", 1, maxSize);

		cout << "请依次输入考生的考号，姓名，性别，年龄及报考类别！" << endl;
		for (int i = 0; i < num; i++) {
			student stu;
			cin >> stu;

			int key = 0;
			stu.num.stringToInt(key);

			System.Insert(stu, key);
			Info_InsertType(Info, stu.type);
		}

		cls();
		System_Statistic(System);
	}
	
	bool if_end = false;

	while (!if_end) {
		cout << "\n请选择您要进行的操作（1为插入，2为删除，3为查找，4为修改，5为统计，6为保存，0为取消操作）：";
		char selection;
		
		/*选择操作*/
		while (1) {
			cin >> selection;
			cin.ignore(INT_MAX, '\n');

			if (selection >= '0' && selection <= '6')
				break;
		}

		switch (selection) {
		case '0':
			if_end = true;
			break;
		case '1':
			System_Insert(System, Info);
			break;
		case '2':
			System_Remove(System, Info);
			break;
		case '3':
			System_Search(System);
			break;
		case '4':
			System_Revise(System, Info);
			break;
		case '5':
			System_Statistic(System, Info);
			break;
		case '6':
			if (System.Write())
				cout << "保存成功！" << endl;
			else
				cout << "保存未成功" << endl;

			pause();
			cls();
			System_Statistic(System);
			break;
		}
	}
	
	cls();
	cout << "考生系统已关闭！" << endl;
	return 0;
}
