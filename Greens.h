#pragma once
#include<graphics.h>

struct COOD
{
	int x;
	int y;
	bool sign;
};

class Greens
{
public:
	Greens() {};
	Greens(int xx, int yy, IMAGE img,int num);
	void loadImageAndNumber(LPCSTR URL, int num);
	void loadImageAndNumber(IMAGE image, int num);
	void setimageAndNumber(IMAGE image,int num);
	void setsign(int s);
	int getx();
	int gety();	
	int getsign();
	int getnumber();
	IMAGE getimage();
	void putGreens();
	void border(COLORREF color, int borderwidth);
	~Greens() {}
private:
	int x;				//�߲�����
	int y;				//
	int number;			//���
	int sign;			//�߲�״̬
	IMAGE greensimg;	//�߲�ͼƬ
};

//���캯��
Greens::Greens(int xx, int yy, IMAGE img,int num)
{
	x = xx;
	y = yy;
	greensimg = img;
	number = num;
}

//���� ͼƬ·�� �Լ� ������
void Greens::loadImageAndNumber(LPCSTR URL,int num)
{
	loadimage(&greensimg, URL);
	number = num;
}

//���ö�����߲�ͼƬ�Ͷ�����
void Greens::setimageAndNumber(IMAGE image, int num)
{
	greensimg = image;
	number = num;
}

//�����߲˵�״̬
void Greens::setsign(int s)
{
	sign = s;
}

//��ȡx����
int Greens::getx()
{
	return x;
}

//��ȡy����
int Greens::gety()
{
	return y;
}

//��ȡ�߲�״̬
int Greens::getsign()
{
	return sign;
}

//���� ͼƬ �Լ� ������
void Greens::loadImageAndNumber(IMAGE image, int num)
{
	greensimg = image;
	number = num;
}

//����ͼƬ
void Greens::putGreens()
{
	putimage(x, y, &greensimg);
}

//�����߿�
void Greens::border(COLORREF color = GREEN, int borderwidth = 1)
{
	setcolor(color);
	for (int i = 0; i < borderwidth; i++)
	{
		rectangle(x - i, y - i, x + greensimg.getwidth() + i, y + greensimg.getheight() + i);
	}
}

//��ȡ���
int Greens::getnumber()
{
	return number;
}

//��ȡͼƬ
IMAGE Greens::getimage()
{
	return greensimg;
}

