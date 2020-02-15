#pragma once
#include<graphics.h>

struct ArrayIndex
{
	int one;	//保存一维
	int tow;	//保存二维
	int sign;	//
};


struct Node
{
	ArrayIndex index;	//保存索引下标
	Node * FatherNode;	//父节点
	Node * SonNode;		//子节点
	int VarCount;		//改变方向的节点数
};



//
//---------------------------队列-------------------------------
//
class FIFO
{
private:
	Node * headNode;	//指向头元素
	Node * tailNode;	//指向尾元素
	int NodeNum;		//保存队列元素个数

public:
	FIFO() {};
	void CreateFIFO();
	void INFIFO(ArrayIndex coor);
	Node OUTFIFO();
	Node Gettail();
	void EmptyFIFO();
	bool JudgevoidFIFO();
};

//新建队列
void FIFO::CreateFIFO()
{
	Node* node = new Node();
	headNode = node;
	headNode->FatherNode = NULL;
	headNode->SonNode = NULL;
	tailNode = node;
	tailNode->FatherNode = NULL;
	tailNode->SonNode = NULL;
	NodeNum = 0;
}

//入队
void FIFO::INFIFO(ArrayIndex point)
{ 
	//
	//判断是否队满
	//

	//新建队列元素
	Node* node = new Node();
	node->index = point;
	node->SonNode = new Node();

	//入队到队尾(将元素的头指针指向队尾)
	//node->FatherNode = tailNode;
	/*if (NodeNum == 0)*/
		*tailNode = *node;
	/*else 
		tailNode = node;*/

	//最后 将下次入队元素的地址（即本次入队元素的子节点地址）赋值给队尾指针
	//队尾指针后移一位
	tailNode = tailNode->SonNode;
	NodeNum++;
}

//出队 返回头元素
Node FIFO::OUTFIFO()
{	 
	Node node= *headNode;

	if (--NodeNum != 0)
	{
		headNode = node.SonNode;
	}
	else
	{
		CreateFIFO();
	}

	return node;
}

//清空队列
void FIFO::EmptyFIFO()
{
	headNode = NULL;
	tailNode = NULL;
}

//返回队尾元素
Node FIFO::Gettail()
{
	return *tailNode;
}

//判断是否空队列
bool FIFO:: JudgevoidFIFO()
{
	return NodeNum == 0 ? true : false;
}