#ifndef _LINKEDSTACK_H_
#define _LINKEDSTACK_H_

#include<iostream>
#include<assert.h>
using namespace std;

template<class _class>
struct StackNode {
	_class data;
	StackNode<_class>* link;

	StackNode(StackNode<_class>* ptr = NULL) :link(ptr) {}
	StackNode(const _class& item, StackNode<_class>* ptr = NULL) :data(item), link(ptr) {}
};

template<class _class>
class LinkedStack {
private:
	StackNode<_class>* top;  //栈顶指针，即链头指针
	int size;
public:
	LinkedStack() :top(NULL), size(0) {}  //默认构造函数
	~LinkedStack();

	void push(const _class x);
	bool pop(_class& x);
	bool getTop(_class& x)const;
	bool IsEmpty()const;
	int getSize()const;
	void makeEmpty();
};

//析构函数
template<class _class>
LinkedStack<_class>::~LinkedStack()
{
	makeEmpty();
}

//进栈
template<class _class>
void LinkedStack<_class>::push(const _class x)
{
	top = new StackNode<_class>(x, top);
	assert(top != NULL);
	size++;
}

//出栈
template<class _class>
bool LinkedStack<_class>::pop(_class& x)
{
	if (IsEmpty())
		return false;

	StackNode<_class>* p = top;
	top = top->link;
	x = p->data;

	size--;
	delete p;
	return true;
}

//取栈顶元素
template<class _class>
bool LinkedStack<_class>::getTop(_class& x)const
{
	if (IsEmpty())
		return false;

	x = top->data;
	return true;
}

//判栈空否
template<class _class>
bool LinkedStack<_class>::IsEmpty()const
{
	return (top == NULL);
}

//返回栈元素个数
template<class _class>
int LinkedStack<_class>::getSize()const
{
	return size;
}

//清空栈的内容
template<class _class>
void LinkedStack<_class>::makeEmpty()
{
	StackNode<_class>* p;

	while (top != NULL) {
		p = top;
		top = top->link;
		delete p;
	}

	size = 0;
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