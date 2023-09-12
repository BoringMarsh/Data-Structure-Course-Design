#include<iostream>
#include<climits>
#include<stdlib.h>
#include"6_2151294_马威.h"
using namespace std;

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
  函数名称：print_menu
  功    能：打印菜单
  输入参数：无
  返 回 值：无
  说    明：无
***************************************************************************/
void print_head()
{
	cout << "**                  家谱管理系统                     **\n"
		<< "=======================================================\n"
		<< "**               请选择要执行的操作：                **\n"
		<< "**    A --- 完善家谱          B --- 添加家庭成员     **\n"
		<< "**                                                   **\n"
		<< "**    C --- 解散局部家庭      D --- 更改家庭成员姓名 **\n"
		<< "**                                                   **\n"
		<< "**    E --- 打印全家福        F --- 查找家庭成员     **\n"
		<< "**                                                   **\n"
		<< "**    G --- 展示局部家庭      H --- 保存族谱         **\n"
		<< "**                                                   **\n"
		<< "**                  I --- 退出程序                   **\n"
		<< "=======================================================\n";
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
	char c;
	cout << "请按回车键继续. . . ";

	while (1) {
		c = getchar();

		if (c == '\n' || c == '\r')
			break;
	}
}

/***************************************************************************
  函数名称：results_check
  功    能：在查找到多个结果时，选择一个
  输入参数：const Vector<FamilyMem<String>*>& results：查找结果
  返 回 值：FamilyMem<String>*：指向选择对象的指针
  说    明：无
***************************************************************************/
FamilyMem<String>* results_check(const Vector<FamilyMem<String>*>& results)
{
	if (results.empty())  /*无结果，返回空指针*/
		return NULL;

	else if (results.length() == 1)  /*仅一个结果，返回该结果*/
		return results[0];

	else {
		cout << "查找到多个结果..." << endl;
		Vector<FamilyMem<String>*>::iterator iter = results.begin();  /*游标对象*/

		while (iter != results.end()) {  /*逐个打印结果的详细信息*/
			cout << "结果" << iter - results.begin() + 1 << "：" << endl;
			(*iter)->examine();
			iter++;
		}

		int confirm = 0;

		while (1) {  /*选择一个结果*/
			cout << "请确定您的选择：(1 - " << results.length() << ") ";
			cin >> confirm;

			if (cin.good() && confirm > 0 && confirm <= results.length()) {
				cin.ignore(INT_MAX, '\n');
				break;
			}

			cin.clear();
			cin.ignore(INT_MAX, '\n');
		}

		return results[confirm - 1];
	}
}

/***************************************************************************
  函数名称：family_setup
  功    能：给家庭中一位无子女的成员添加成员
  输入参数：FamilyTree<String>& Family：家族树
  返 回 值：无
  说    明：无
***************************************************************************/
void family_setup(FamilyTree<String>& Family)
{
	String parent;  /*成员姓名*/
	String child;   /*子女姓名*/
	Vector<FamilyMem<String>*> results;  /*查找结果*/
	FamilyMem<String>* Parent;  /*指向该成员的指针*/
	cout << "请输入要建立家庭的人的姓名：";
	cin >> parent;
	cin.ignore(INT_MAX, '\n');

	if (!Family.Find(Family.getAncestor(), parent, results)) {  /*查找成员*/
		cout << "成员不存在！" << endl;
		pause();
		return;
	}

	else
		Parent = results_check(results);  /*选择成员*/

	if (Parent->firstChild) {
		cout << "当前成员已有子女！" << endl;
		pause();
		return;
	}

	cout << "请依次输入" << parent << "的儿女的姓名（以#结尾）：";
	Vector<String> children;

	while (1) {
		cin >> child;

		if (child == "#") {  /*输入#结束，省去输入数量的步骤*/
			cin.ignore(INT_MAX, '\n');
			break;
		}

		else
			children.push_back(child);
	}

	if (children.empty()) {
		cout << "无添加子女数据！" << endl;
		pause();
		return;
	}

	Family.movCurrent(Parent);     /*移动指针*/
	Family.SetupFamily(children);  /*添加子女*/
	Family.ShowSubFamily(Parent);  /*展示局部家庭*/

	pause();
}

/***************************************************************************
  函数名称：family_add
  功    能：给家庭中一位有子女的成员添加成员
  输入参数：FamilyTree<String>& Family：家族树
  返 回 值：无
  说    明：无
***************************************************************************/
void family_add(FamilyTree<String>& Family)
{
	String parent;  /*成员姓名*/
	String child;   /*子女姓名*/
	Vector<FamilyMem<String>*> results;  /*查找结果*/
	FamilyMem<String>* Parent;  /*指向该成员的指针*/
	cout << "请输入要添加儿子（或女儿）的人的姓名：";
	cin >> parent;
	cin.ignore(INT_MAX, '\n');

	if (!Family.Find(Family.getAncestor(), parent, results)) {  /*查找成员*/
		cout << "成员不存在！" << endl;
		pause();
		return;
	}

	else
		Parent = results_check(results);  /*选择成员*/

	if (Parent->firstChild == NULL) {
		cout << "当前成员未有子女！" << endl;
		pause();
		return;
	}

	cout << "请输入" << parent << "新添加的儿子（或女儿）的姓名，或输入#取消：";
	cin >> child;
	cin.ignore(INT_MAX, '\n');

	if (child == "#") {  /*输入#结束*/
		cout << "取消成功" << endl;
		pause();
		return;
	}

	Family.movCurrent(Parent);     /*移动指针*/
	Family.AddChild(child);     /*添加子女*/
	Family.ShowSubFamily(Parent);  /*展示局部家庭*/

	pause();
}

/***************************************************************************
  函数名称：family_remove
  功    能：解散以一位成员为祖先的局部家庭
  输入参数：FamilyTree<String>& Family：家族树
  返 回 值：无
  说    明：无
***************************************************************************/
void family_remove(FamilyTree<String>& Family)
{
	String parent;  /*成员姓名*/
	Vector<FamilyMem<String>*> results;  /*查找结果*/
	FamilyMem<String>* Parent;  /*指向该成员的指针*/
	cout << "请输入要解散家庭的人的姓名：";
	cin >> parent;
	cin.ignore(INT_MAX, '\n');

	if (!Family.Find(Family.getAncestor(), parent, results)) {  /*查找成员*/
		cout << "成员不存在！" << endl;
		pause();
		return;
	}

	else
		Parent = results_check(results);  /*选择成员*/

	Family.movCurrent(Parent);  /*移动指针*/
	cout << parent << "的家庭已解散，原家庭情况为：" << endl;  /*展示局部家庭*/
	Family.ShowSubFamily(Parent);
	Family.RemoveSubFamily(Family.getCurrent());  /*删除局部家庭*/

	pause();
}

/***************************************************************************
  函数名称：family_revise
  功    能：给一位家庭成员改名
  输入参数：FamilyTree<String>& Family：家族树
  返 回 值：无
  说    明：无
***************************************************************************/
void family_revise(FamilyTree<String>& Family)
{
	String member, newName;  /*成员姓名，新姓名*/
	Vector<FamilyMem<String>*> results;  /*查找结果*/
	FamilyMem<String>* Member;  /*指向该成员的指针*/
	cout << "请输入要更改姓名的人的目前姓名：";
	cin >> member;
	cin.ignore(INT_MAX, '\n');

	if (!Family.Find(Family.getAncestor(), member, results)) {  /*查找成员*/
		cout << "成员不存在！" << endl;
		pause();
		return;
	}

	else
		Member = results_check(results);  /*选择成员*/

	cout << "请输入更改后的姓名：";
	cin >> newName;
	cin.ignore(INT_MAX, '\n');
	Member->data = newName;
	cout << member << "已更名为" << newName << endl;

	pause();
}

/***************************************************************************
  函数名称：family_show
  功    能：展示全家福
  输入参数：FamilyTree<String>& Family：家族树
  返 回 值：无
  说    明：无
***************************************************************************/
void family_show(FamilyTree<String>& Family)
{
	Family.ShowSubFamily(Family.getAncestor());
	pause();
}

/***************************************************************************
  函数名称：family_search
  功    能：按名称搜索成员
  输入参数：FamilyTree<String>& Family：家族树
  返 回 值：无
  说    明：无
***************************************************************************/
void family_search(FamilyTree<String>& Family)
{
	String member;  /*成员姓名*/
	Vector<FamilyMem<String>*> results;  /*查找结果*/
	cout << "请输入要查找的家庭成员的姓名：";
	cin >> member;
	cin.ignore(INT_MAX, '\n');

	if (!Family.Find(Family.getAncestor(), member, results)) {  /*查找成员*/
		cout << "成员不存在！" << endl;
		pause();
		return;
	}

	if (results.length() == 1) {  /*仅搜索到一个结果*/
		cout << "查找成功，信息如下：" << endl << endl;
		results[0]->examine();
	}

	else {  /*搜索到多个结果*/
		cout << "查找到多个结果..." << endl << endl;
		Vector<FamilyMem<String>*>::iterator iter = results.begin();

		while (iter != results.end()) {
			cout << "结果" << iter - results.begin() + 1 << "：" << endl;
			(*iter)->examine();
			cout << endl;
			iter++;
		}
	}

	pause();
}

/***************************************************************************
  函数名称：family_show_branch
  功    能：展示局部家庭
  输入参数：FamilyTree<String>& Family：家族树
  返 回 值：无
  说    明：无
***************************************************************************/
void family_show_branch(FamilyTree<String>& Family)
{
	String ancestor;  /*祖先姓名*/
	Vector<FamilyMem<String>*> results;  /*查找结果*/
	FamilyMem<String>* Ancestor;  /*指向当前祖先的指针*/
	cout << "请输入要展示的局部家庭的祖先姓名：";
	cin >> ancestor;
	cin.ignore(INT_MAX, '\n');

	if (!Family.Find(Family.getAncestor(), ancestor, results)) {  /*查找成员*/
		cout << "成员不存在！" << endl;
		pause();
		return;
	}

	else
		Ancestor = results_check(results);  /*选择成员*/

	cout << "该局部家庭如下：" << endl;
	Family.ShowSubFamily(Ancestor);

	pause();
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
	String ancestor;
	bool if_on = true;

	cls();
	print_head();

	cout << "首先建立一个家谱！\n"
		<< "请输入祖先的姓名：";
	cin >> ancestor;
	cin.ignore(INT_MAX, '\n');
	cout << "此家谱的祖先是：" << ancestor << endl;

	FamilyTree<String> Family(ancestor);

	while (if_on) {
		/*显示实时人数和世代*/
		cout << "\n当前情况：人数" << Family.getSize() << "人，共" << Family.getMaxGen() << "代" << endl;
		cout << "\n请选择要执行的操作：";
		char selection;

		while (1) {
			selection = getchar();
			cin.ignore(INT_MAX, '\n');

			if ((selection >= 'A' && selection <= 'I') || (selection >= 'a' && selection <= 'i'))
				break;
		}

		switch (selection) {
		case 'A':
		case 'a':
			family_setup(Family);
			break;
		case 'B':
		case 'b':
			family_add(Family);
			break;
		case 'C':
		case 'c':
			family_remove(Family);
			break;
		case 'D':
		case 'd':
			family_revise(Family);
			break;
		case 'E':
		case 'e':
			family_show(Family);
			break;
		case 'F':
		case 'f':
			family_search(Family);
			break;
		case 'G':
		case 'g':
			family_show_branch(Family);
			break;
		case 'H':
		case 'h':
			Family.WriteFamily(Family.getAncestor());
			break;
		case 'I':
		case 'i':
			if_on = false;
			break;
		}

		cls();
		print_head();
	}

	cout << "\n家谱管理系统已关闭！" << endl;
	return 0;
}