#include<iostream>
#include"4_2151294_马威.h"
using namespace std;

/*
特殊运算符的代替：
1、单目 '-'
	单目后是括号 : '@'
	单目后是数字 : '$'
2、单目 '+'
	单目后是数字 : 直接忽略
*/

/***************************************************************************
  函数名称：isp
  功    能：返回一个运算符的栈内优先数
  输入参数：const char ch：待处理的运算符（只读）
  返 回 值：int：运算符对应的栈内优先数
  说    明：无
***************************************************************************/
int isp(const char ch)
{
	int ret = -1;  /*返回值，先置为不可能值-1，以防字符非法*/

	switch (ch) {
	case '#':
		ret = 0;  /*表达式终止符*/
		break;
	case '(':
		ret = 1;  /*左括号*/
		break;
	case '@':
		ret = 7;  /*单目负号（后面是括号）*/
		break;
	case '*':
	case '/':
		ret = 5;  /*乘除号*/
		break;
	case '+':
	case '-':
		ret = 3;  /*加减号*/
		break;
	case ')':
		ret = 8;  /*右括号*/
		break;
	}

	return ret;
}

/***************************************************************************
  函数名称：icp
  功    能：返回一个运算符的栈外优先数
  输入参数：const char ch：待处理的运算符（只读）
  返 回 值：int：运算符对应的栈外优先数
  说    明：无
***************************************************************************/
int icp(const char ch)
{
	int ret = -1;  /*返回值，先置为不可能值-1，以防字符非法*/

	switch (ch) {
	case '#':
		ret = 0;  /*表达式终止符*/
		break;
	case '(':
		ret = 8;  /*左括号*/
		break;
	case '@':
		ret = 6;  /*单目负号（后面是括号）*/
		break;
	case '*':
	case '/':
		ret = 4;  /*乘除号*/
		break;
	case '+':
	case '-':
		ret = 2;  /*加减号*/
		break;
	case ')':
		ret = 1;  /*右括号*/
		break;
	}

	return ret;
}

/***************************************************************************
  函数名称：isDigit
  功    能：判断一个字符是否是用来表示数字的
  输入参数：const char ch：待处理的运算符（只读）
  返 回 值：bool：该字符是否被用于表示数字
  说    明：本函数增加了小数点'.'和处理后的单目负号'$'的判断
***************************************************************************/
bool isDigit(const char ch)
{
	if ((ch >= '0' && ch <= '9') || ch == '.' || ch == '$')
		return true;
	else
		return false;
}

/***************************************************************************
  函数名称：isOp
  功    能：判断一个字符是否是运算符
  输入参数：const char ch：待处理的运算符（只读）
  返 回 值：bool：该字符是否是运算符
  说    明：本函数仅限于加减乘除的判断
***************************************************************************/
bool isOp(const char ch)
{
	switch (ch) {
	case '+':
	case '-':
	case '*':
	case '/':
		return true;
		break;
	default:
		return false;
	}
}

/***************************************************************************
  函数名称：expCheck
  功    能：判断一个表达式是否合法
  输入参数：const String& str：待处理的表达式（只读）
  返 回 值：bool：该表达式是否合法
  说    明：本函数仅在表达式被预处理后进行判断（去除空格与对单目运算符的处理）
***************************************************************************/
bool expCheck(const String& str)
{
	if (*str.last() != '#') {
		cout << "表达式结尾缺少符号#" << endl;
		return false;
	}

	if (*str.begin() == '#') {
		cout << "表达式为空" << endl;
		return false;
	}

	String::iterator iter = str.begin();  /*游标对象*/
	LinkedStack<char> brackets;  /*记录括号情况的栈*/
	int opNum = 0, numNum = 0;  /*操作符（双目）个数，数字个数*/

	while (iter != str.end()) {
		/*1、若扫描到字符用来表示数字，则不停扫描，直到扫描到的字符有其他用途为止*/
		if (isDigit(*iter)) {
			while (isDigit(*iter) && iter != str.end()) {
				if (*iter == '.' && (!isDigit(*(iter - 1)) || !isDigit(*(iter + 1)))) {  /*若小数点前后都不是数字，则非法*/
					cout << "数字输入错误" << endl;
					return false;
				}

				else
					iter++;
			}

			numNum++;  /*一个数字扫描完成，进行计数*/
		}

		if (*iter == '(')  /*若遇到左括号，压入栈中*/
			brackets.push(*iter);
		else if (*iter == ')') {  /*若遇到右括号，栈不为空则弹出一个元素；栈为空则说明右括号多余，非法*/
			if (!brackets.IsEmpty()) {
				char c;
				brackets.pop(c);
			}
			
			else {
				cout << "括号不匹配" << endl;
				return false;
			}
		}
		else if (isOp(*iter))  /*若遇到双目操作符，进行计数*/
			opNum++;

		if (iter != str.end())
			iter++;
		else
			break;
	}

	if (!brackets.IsEmpty()) {  /*若扫描完成栈仍不为空，则左括号多余，非法*/
		cout << "括号不匹配" << endl;
		return false;
	}

	if (numNum != opNum + 1) {  /*若双目运算符个数不等于数字个数+1，则不匹配，非法*/
		cout << "操作符与操作数数量不匹配" << endl;
		return false;
	}
	
	return true;  /*所有标准均符合，则表达式合法*/
}

/***************************************************************************
  函数名称：postfix_prep
  功    能：对一个表达式进行预处理
  输入参数：String& str：待处理的表达式
  返 回 值：无
  说    明：无
***************************************************************************/
void postfix_prep(String& str)
{
	if (str.isEmpty())
		return;

	String ostr;  /*存放处理结果的辅助字符串*/
	String::iterator iter = str.begin();  /*游标对象*/

	while (iter != str.end()) {
		if (*iter == '+') {  /*去除单目正号*/
			if (iter != str.begin() && *(iter - 1) != '(')  /*若该正号不在表达式开头且前面不是左括号，则是双目正号，需放进结果字符串；否则忽略*/
				ostr += *iter;
		}

		else if (*iter != ' ') {  /*去除空格*/
			ostr += *iter;
		}

		iter++;
	}

	iter = ostr.begin();/*准备进行第二次扫描*/

	while (iter != ostr.end()) {
		if (*iter == '-') {  /*标记单目负号*/

			/*若该负号在表达式开头，根据后面的字符做对应更改*/
			if (iter == ostr.begin()) {
				if (isDigit(*(iter + 1)))
					*iter = '$';
				else
					*iter = '@';
			}
			
			/*若该负号不在表达式开头，则当它前面不是数字、不是运算符、不是右括号时，它就是单目负号。根据后面的字符做对应更改*/
			else if (!isDigit(*(iter - 1)) && !isOp(*(iter - 1)) && *(iter - 1) != ')') {
				if (isDigit(*(iter + 1)))
					*iter = '$';
				else
					*iter = '@';
			}
		}
		iter++;
	}

	ostr += '#';  /*添加结束符号*/
	*(ostr.end()) = '\0';  /*手动添加尾零*/
	str = ostr;  /*将结果赋值给原来的字符串*/
}

/***************************************************************************
  函数名称：postfix_adj
  功    能：调整表达式中的辅助符号
  输入参数：String& str：待处理的表达式
  返 回 值：无
  说    明：无
***************************************************************************/
void postfix_adj(String& str)
{
	String::iterator iter = str.begin();  /*游标对象*/

	while (iter != str.end()) {
		if (*iter == '$')  /*将负数的负号转换回来*/
			*iter = '-';

		if (*iter == '#')  /*去掉末尾的'#'*/
			*iter = '\0';

		iter++;
	}
}

/***************************************************************************
  函数名称：postfix
  功    能：调整表达式为后缀表达式
  输入参数：String& str：待处理的表达式
  返 回 值：无
  说    明：无
***************************************************************************/
void postfix(String& src)
{
	LinkedStack<char> stack;  /*存放运算符的栈*/
	String::iterator iter = src.begin();  /*游标对象*/
	String result;  /*存放转换结果的字符串*/

	char ch = '#', ch1, op;  /*当前字符、栈顶字符、弹出字符*/
	bool cut = false;  /*是否添加空格的标志*/

	stack.push(ch);  /*将#压入栈中，使栈不为空*/
	ch = *iter;  /*读入第一个字符*/
	iter++;

	while (!stack.IsEmpty()) {

		/*1、当前字符用来表示数字*/
		if (isDigit(ch)) {
			if (ch == '$') {  /*若为单目负号，则不需要分隔*/
				cut = false;

				if (result.length())  /*若当前字符串为空，则仅添加一个$；若不为空，则添加一个空格和$*/
					result += " $";
				else
					result += '$';

				ch = *iter;  /*读入下一个字符*/
				iter++;
			}

			else if (cut) {  /*若需要分隔，则添加一个空格，标志置为假*/
				cut = false;
				result += ' ';
			}

			else if (result.last() != result.begin() && !isDigit(*(result.last())))  /*若字符串不止一个字符且最后一个字符不用来表示数字，则添加一个空格*/
				result += ' ';

			result += ch;  /*添加当前字符*/
			ch = *iter;    /*读入下一个字符*/
			iter++;
		}

		/*2、当前字符是括号或运算符*/
		else {
			if (result.length())  /*若当前字符串不为空，则分隔标志设为真*/
				cut = true;
			stack.getTop(ch1);  /*取栈顶运算符*/

			if (isp(ch1) < icp(ch)) {  /*若当前字符（栈外）优先级大于栈顶运算符（栈内）的优先级，则将当前字符压入栈中*/
				stack.push(ch);
				ch = *iter;  /*读入下一个字符*/
				iter++;
			}

			else if (isp(ch1) > icp(ch)) {  /*若当前字符（栈外）优先级小于栈顶运算符（栈内）的优先级，则弹出栈顶元素，并添加空格和栈顶元素*/
				stack.pop(op);

				result += ' ';
				result += op;
			}

			else {  /*若两者优先级相等，则弹出栈顶元素*/
				stack.pop(op);

				if (op == '(') {  /*若栈顶元素是左括号，直接读入下一个字符*/
					ch = *iter;
					iter++;
				}
			}
		}
	}

	src = result;  /*将结果赋值给原来的字符串*/
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
	String source;  /*待处理的字符串*/
	cin >> source;
	
	postfix_prep(source);  /*做预处理工作*/

	if (expCheck(source)) {  /*若表达式合法，则转换为后缀表达式输出*/
		postfix(source);
		postfix_adj(source);
		cout << source << endl;
	}

	return 0;
}
