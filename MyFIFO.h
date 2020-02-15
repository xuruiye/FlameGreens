#pragma once
#include<graphics.h>

struct ArrayIndex
{
	int one;	//����һά
	int tow;	//�����ά
	int sign;	//
};


struct Node
{
	ArrayIndex index;	//���������±�
	Node * FatherNode;	//���ڵ�
	Node * SonNode;		//�ӽڵ�
	int VarCount;		//�ı䷽��Ľڵ���
};



//
//---------------------------����-------------------------------
//
class FIFO
{
private:
	Node * headNode;	//ָ��ͷԪ��
	Node * tailNode;	//ָ��βԪ��
	int NodeNum;		//�������Ԫ�ظ���

public:
	FIFO() {};
	void CreateFIFO();
	void INFIFO(ArrayIndex coor);
	Node OUTFIFO();
	Node Gettail();
	void EmptyFIFO();
	bool JudgevoidFIFO();
};

//�½�����
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

//���
void FIFO::INFIFO(ArrayIndex point)
{ 
	//
	//�ж��Ƿ����
	//

	//�½�����Ԫ��
	Node* node = new Node();
	node->index = point;
	node->SonNode = new Node();

	//��ӵ���β(��Ԫ�ص�ͷָ��ָ���β)
	//node->FatherNode = tailNode;
	/*if (NodeNum == 0)*/
		*tailNode = *node;
	/*else 
		tailNode = node;*/

	//��� ���´����Ԫ�صĵ�ַ�����������Ԫ�ص��ӽڵ��ַ����ֵ����βָ��
	//��βָ�����һλ
	tailNode = tailNode->SonNode;
	NodeNum++;
}

//���� ����ͷԪ��
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

//��ն���
void FIFO::EmptyFIFO()
{
	headNode = NULL;
	tailNode = NULL;
}

//���ض�βԪ��
Node FIFO::Gettail()
{
	return *tailNode;
}

//�ж��Ƿ�ն���
bool FIFO:: JudgevoidFIFO()
{
	return NodeNum == 0 ? true : false;
}