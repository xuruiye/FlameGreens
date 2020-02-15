#pragma once
#include"Greens.h"

//栈 类
//template<typename T>
class STACK
{
private :
	POINT *stack;
	int stacks;//当前栈尾的下标

public :
	STACK()
	{
		stacks = -1;//空栈
		stack = new POINT[1];
	}
	void EnterStack(int x, int y);//入栈
	void RetreatStack();//退栈
	void EmptyStack();//清空栈
	int StackCount();//返回当前栈的下标
};

//入栈
void STACK::EnterStack(int x, int y)
{
	POINT *temp = stack;
	stack = new POINT[++stacks];
	for (int i = 0; i < stacks; i++)
	{
		stack[i] = temp[i];
	}
	//delete[] temp; //只有用 new 分配的内存才需要delete 掉
	stack[stacks].x = x;
	stack[stacks].y = y;
}

//退栈
void STACK::RetreatStack()
{
	//已为空栈 不允许退栈
	if (stacks == -1)
	{
		return;
	}
	POINT *temp = stack;
	stack = new POINT[--stacks];
	for (int i = 0; i <= stacks; i++)
	{
		stack[i] = temp[i];
	}
	delete[] stack;
}

//清空栈
void STACK::EmptyStack()
{
	while (stacks != -1)
	{
		RetreatStack();
	}
}

//返回当前栈的下标
int STACK::StackCount()
{
	return stacks;
}