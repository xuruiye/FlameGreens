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
	int x;				//蔬菜坐标
	int y;				//
	int number;			//编号
	int sign;			//蔬菜状态
	IMAGE greensimg;	//蔬菜图片
};

//构造函数
Greens::Greens(int xx, int yy, IMAGE img,int num)
{
	x = xx;
	y = yy;
	greensimg = img;
	number = num;
}

//加载 图片路径 以及 对象编号
void Greens::loadImageAndNumber(LPCSTR URL,int num)
{
	loadimage(&greensimg, URL);
	number = num;
}

//设置对象的蔬菜图片和对象编号
void Greens::setimageAndNumber(IMAGE image, int num)
{
	greensimg = image;
	number = num;
}

//设置蔬菜的状态
void Greens::setsign(int s)
{
	sign = s;
}

//获取x坐标
int Greens::getx()
{
	return x;
}

//获取y坐标
int Greens::gety()
{
	return y;
}

//获取蔬菜状态
int Greens::getsign()
{
	return sign;
}

//加载 图片 以及 对象编号
void Greens::loadImageAndNumber(IMAGE image, int num)
{
	greensimg = image;
	number = num;
}

//画出图片
void Greens::putGreens()
{
	putimage(x, y, &greensimg);
}

//画出边框
void Greens::border(COLORREF color = GREEN, int borderwidth = 1)
{
	setcolor(color);
	for (int i = 0; i < borderwidth; i++)
	{
		rectangle(x - i, y - i, x + greensimg.getwidth() + i, y + greensimg.getheight() + i);
	}
}

//获取编号
int Greens::getnumber()
{
	return number;
}

//获取图片
IMAGE Greens::getimage()
{
	return greensimg;
}

