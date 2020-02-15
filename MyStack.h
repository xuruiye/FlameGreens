#pragma once
#include"Greens.h"

//ջ ��
//template<typename T>
class STACK
{
private :
	POINT *stack;
	int stacks;//��ǰջβ���±�

public :
	STACK()
	{
		stacks = -1;//��ջ
		stack = new POINT[1];
	}
	void EnterStack(int x, int y);//��ջ
	void RetreatStack();//��ջ
	void EmptyStack();//���ջ
	int StackCount();//���ص�ǰջ���±�
};

//��ջ
void STACK::EnterStack(int x, int y)
{
	POINT *temp = stack;
	stack = new POINT[++stacks];
	for (int i = 0; i < stacks; i++)
	{
		stack[i] = temp[i];
	}
	//delete[] temp; //ֻ���� new ������ڴ����Ҫdelete ��
	stack[stacks].x = x;
	stack[stacks].y = y;
}

//��ջ
void STACK::RetreatStack()
{
	//��Ϊ��ջ ��������ջ
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

//���ջ
void STACK::EmptyStack()
{
	while (stacks != -1)
	{
		RetreatStack();
	}
}

//���ص�ǰջ���±�
int STACK::StackCount()
{
	return stacks;
}