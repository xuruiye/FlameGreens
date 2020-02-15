#include<graphics.h>
#include<time.h>
#include<conio.h>
#include<mmsystem.h>			//添加头文件  播放音乐
#pragma comment(lib,"winmm.lib")//导入库
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
//#pragma comment( linker, "/subsystem:/ "windows/ " /entry:/ "mainCRTStartup/ " " )
#include"Greens.h"    
#include"MyStack.h"
#include"MyFIFO.h"

#define Height 495	//窗口大小
#define Width  864
#define HClee 6		//游戏边上格子数
#define WClee 9

//全局 变量
Node node[HClee + 2][WClee + 2];
FIFO path;
ArrayIndex old;//x,y保存上次点击格子的小标 以及sign判断是否是“第二次”点击 
Greens greens[HClee + 2][WClee + 2];//蔬菜对象

//函数声明处
void Gamestart();	//游戏开始界面
void GameInit();	//初始化
void Lefttop(MOUSEMSG m);
void InitGreens(IMAGE image[]);	//初始化蔬菜对象 画出外边框以及蔬菜的边框
void nodeInit();
void ShowGreens(int sleep = 10);
void ShowBack();
void ShowMarginBorder(int sleep = 200);
ArrayIndex returnNode(int one, int tow, int sign);
void RandomGreens();
void GreensExchange(Greens *g1, Greens *g2);//蔬菜交换
bool SearchConnectionPath(ArrayIndex index1, ArrayIndex index2);
bool infifo(int fatherone, int fathertow, int sonone, int sontow);

/*#pragma comment( linker, "/subsystem:\"windows\" /entry:\"maincrtstartup\"")*/ 
//主函数
int main()
{

	Gamestart();
	return 0;
}

//游戏开始界面
void Gamestart()
{
	//播放音乐
	mciSendString("open music/startback.mp3 alias mdgd", 0, 0, 0);
	mciSendString("play mdgd repeat", 0, 0, 0);

	initgraph(Width,Height);

	IMAGE startback[4];//背景图数组
	loadimage(&startback[0], "imag/index0.jpg");//背景图
	loadimage(&startback[1], "imag/index1.jpg");
	loadimage(&startback[2], "imag/index2.jpg");
	loadimage(&startback[3], "imag/index1.jpg");

	int i = 0;
	MOUSEMSG m;
	
	while (true) 
	{
		while (MouseHit())//r如果有鼠标消息
		{	
			m = GetMouseMsg();
			//点击开始按钮
			if (m.uMsg== WM_LBUTTONUP &&m.x > 444 && m.x < 518 && m.y>394 && m.y < 464)
			{
				mciSendString("close mdgd", NULL, 0, NULL);//关闭音乐
				GameInit();//游戏数据初始化
			}
		}
		BeginBatchDraw();
		Sleep(100);
		putimage(0, 0, &startback[++i%4]);//画背景
		EndBatchDraw();
	}
}

//游戏数据初始化   获取图片  格子坐标
void GameInit()
{
	ShowBack();	//画背景

	//背景音乐
	mciSendString("open music/backmusic.mp3 alias bm", 0, 0, 0);
	mciSendString("play bm repeat", 0, 0, 0);

	IMAGE image[9];
	//将蔬菜图片读入程序
	loadimage(&image[0], "imag/0.jpg");
	loadimage(&image[1], "imag/1.jpg");
	loadimage(&image[2], "imag/2.jpg");
	loadimage(&image[3], "imag/3.jpg");
	loadimage(&image[4], "imag/4.jpg");
	loadimage(&image[5], "imag/5.jpg");
	loadimage(&image[6], "imag/6.jpg");
	loadimage(&image[7], "imag/7.jpg");
	loadimage(&image[8], "imag/8.jpg");

	//初始化蔬菜对象 画出外边框以及蔬菜的边框
	InitGreens(image);
	//初始化node节点
	nodeInit();

	MOUSEMSG m;
	while (true)//死循环
	{
		while (MouseHit())//如果有鼠标消息
		{
			m = GetMouseMsg();
			//如果左键弹起 并且点击范围在蔬菜区域
			if (m.uMsg == WM_LBUTTONUP)
			{
				Lefttop(m);
			}
		}
	}
}

//初始化蔬菜对象 画出外边框和蔬菜以及蔬菜的边框
void InitGreens(IMAGE image[])
{
	int imageH = image[0].getheight();
	int imageW = image[0].getwidth();

	//给蔬菜初始化
	greens[0][0] = Greens(0, 0, NULL, 0);
	greens[0][0].setsign(0);

	//先给第一行赋值
	for (int i = 1; i < WClee + 2; i++)
	{
		greens[0][i] = Greens(greens[0][i - 1].getx() + imageW + 2,	//获取左边对象的x+图片宽度
							  greens[0][i - 1].gety(),				//获取左边对象的y
							  NULL,NULL);
		greens[0][i].setsign(0);
	}

	//按照第一行给其他行赋值
	for (int i = 0; i < WClee + 2; i++)//列
	{
		for (int j = 1; j < HClee + 2; j++)//行
		{
			greens[j][i] = Greens(greens[j - 1][i].getx(),				//获取上边对象的x
								  greens[j - 1][i].gety() + imageH + 2,	//获取上边对象的高+图片高度
								  NULL, NULL);
			greens[j][i].setsign(0);
		}
	}

	//从第二行第二列开始 到 倒数第二行倒数第二列 给对象加载图片和编号
	int k = 0;
	for (int i = 1; i < WClee + 1; i++)//列
	{
		for (int j = 1; j < HClee + 1; j++)//行
		{
			greens[j][i].setimageAndNumber(image[k % 9], k % 9);
			greens[j][i].setsign(1);
			k++;
		}
	}

	//随机打乱蔬菜对象的图片和编号
	RandomGreens();

	//画外边框
	ShowMarginBorder();
	//画蔬菜
	ShowGreens();	
}

//随机打乱蔬菜图片
void RandomGreens()
{
	srand(unsigned(time(0)));
	for (int i = 1; i < WClee + 1; i++)//列
	{
		for (int j = 1; j < HClee + 1; j++)//行
		{
			GreensExchange(&greens[j][i], &greens[(rand() % 6) + 1][(rand() % 9) + 1]);
		}
	}
}

//蔬菜交换
void GreensExchange(Greens *g1,Greens *g2)
{
	//将指针类型转换为 对象实例
	Greens greens1 = *g1;
	Greens greens2 = *g2;

	//开始交换
	Greens Temp = *g1;
	greens1.setimageAndNumber(greens2.getimage(), greens2.getnumber());
	greens2.setimageAndNumber(Temp.getimage(), Temp.getnumber());

	//将交换后的临时对象实例 重新赋值给指针表示的 对象
	*g1 = greens1;
	*g2 = greens2;
}

//画背景
void ShowBack()
{
	IMAGE backgame;
	loadimage(&backgame, "imag/bakcgame.jpg");
	putimage(0, 0, &backgame);
}

//画蔬菜
void ShowGreens(int sleep)
{
	for (int i = 1; i < WClee + 1; i++)//行
	{
		for (int j = 1; j < HClee + 1; j++)//列
		{
			if (greens[j][i].getsign() == 1)
			{
				//画蔬菜
				greens[j][i].putGreens();
				//画内框
				greens[j][i].border();
				Sleep(sleep);
			}
		}
	}
}

//画外边框
void ShowMarginBorder(int sleep)
{
	setcolor(GREEN);
	for (int i = 0; i < 5; i++)
	{
		rectangle(greens[1][1].getx() - i - 2,
			greens[1][1].gety() - i - 2,
			greens[HClee + 1][WClee + 1].getx() + i,
			greens[HClee + 1][WClee + 1].gety() + i);
		Sleep(sleep);
	}
}

//节点node 初始化
void nodeInit()
{
	for (int i = 0; i < WClee + 2; i++)//列
	{
		for (int j = 0; j < HClee + 2; j++)//行
		{
			node[j][i].index = returnNode(j, i ,0);
			node[j][i].FatherNode = NULL;
			node[j][i].SonNode = NULL;
			node[j][i].VarCount = 0;
		}
	}
}

//将下标转换为ArrayIndex（二维下标类型）
ArrayIndex returnNode(int one,int tow,int sign)
{
	ArrayIndex index;
	index.one = one;
	index.tow = tow;
	index.sign = 0;
	return index;
}

//左键弹起
void Lefttop(MOUSEMSG m)
{
	int imageH = greens[1][1].getimage().getheight();
	int imageW = greens[1][1].getimage().getwidth();

	//获取每个蔬菜对象的宽高
	int greensW = greens[0][1].getx() - greens[0][0].getx();
	int greensH = greens[1][0].gety() - greens[0][0].gety();
	int TowW = m.x / greensW;		//把鼠标点击的坐标 转化为格子的下标
	int FirstW = m.y / greensH;		//FirstW：一维 TowW：二维
	
	//点击范围无效
	if (TowW<1 || FirstW<1 || TowW>WClee + 1 || FirstW>HClee + 1 || greens[FirstW][TowW].getsign()==0)
	{
		if (old.sign)//如果本次点击为第二次点击
		{
			//将上次点击重置为绿色
			greens[old.one][old.tow].border();
			old.sign = 0;//将下次点击设置为第一次点击
		}
		return;
	}

	ArrayIndex news;	//保存当前鼠标点击位置的格子的下标
	news.one = FirstW;
	news.tow = TowW;

	//将本次点击位置点亮
	greens[news.one][news.tow].border(RED);

	//如果本次点击为第二次点击  
	if (old.sign == 1)
	{
		//点击的不是同一类型图片
		if (greens[old.one][old.tow].getnumber() != greens[FirstW][TowW].getnumber())
		{
			greens[old.one][old.tow].border();
			greens[news.one][news.tow].border();
			old.sign = 0;
			return;
		}
		else if (old.one == news.one && old.tow == news.tow)
		{ 
			//点击的是同一张图片
			old.sign = 0;
			greens[news.one][news.tow].border();
			return;
		}
		//算法搜索 连接路径
		if (SearchConnectionPath(old, news))
		{
			//找到了
			ArrayIndex index1 = node[old.one][old.tow].index;
			ArrayIndex index2 = node[old.one][old.tow].FatherNode->index;

			//直到连接完全
			//fillroundrect()
			setfillcolor(GREEN);
			while (1)
			{
				//直线连接相邻的两个格子
				solidroundrect(greens[index1.one][index1.tow].getx() + imageW / 2 - 1,
							greens[index1.one][index1.tow].gety() + imageW / 2 - 1,
							greens[index2.one][index2.tow].getx() + imageW / 2 +1,
							greens[index2.one][index2.tow].gety() + imageW / 2 + 1,
							4,4);
				//solidcircle(greens[index1.one][index1.tow].getx() + imageW / 2,
					//greens[index1.one][index1.tow].gety() + imageW / 2,1);
				if (node[index2.one][index2.tow].FatherNode == NULL)
					break;
				Sleep(100);
				//返回本身的父元素
				index1 = index2;
				index2 = node[index2.one][index2.tow].FatherNode->index;
			}

			BeginBatchDraw();	//开始批量绘图

			path.EmptyFIFO();	//清空队列
			nodeInit();			//本次搜索记录清除
			ShowBack();			//画背景
			ShowMarginBorder(0);//画外边框  0延迟
			ShowGreens(0);		//画蔬菜  0延迟

			EndBatchDraw();//结束批量绘图

			old.sign = 0;		
			return;
		}
		else
		{
			//没找到
			old.sign = 0;	//将下次点击设为第一次点击
			greens[old.one][old.tow].border();
			greens[news.one][news.tow].border();
			nodeInit();
			return;
		}//if (SearchConnectionPath(old, news))

		//将上次点击位置复原
		greens[old.one][old.tow].border();
	}//if (old.sign == 1 && !(old.one==news.one && old.tow==news.tow))

	old = news;	//将本次点击设为旧操作  （下次为新操作）
	old.sign = 1;	//将下次点击设为第二次点击

}

//搜索连接路径
bool SearchConnectionPath(ArrayIndex old, ArrayIndex news)
{
	//先将对象标记为以消除状态 便于搜索
	greens[old.one][old.tow].setsign(0);
	greens[news.one][news.tow].setsign(0);

	//新建队列
	path.CreateFIFO();
	path.INFIFO(news);	//入队
	node[news.one][news.tow].index.sign = 1;

	while (!path.JudgevoidFIFO())	//队列不为空
	{
		//出队
		ArrayIndex index = path.OUTFIFO().index;

		if (node[index.one][index.tow].FatherNode != NULL)
		{
			ArrayIndex n = node[index.one][index.tow].FatherNode->index;
			int one, tow;
			if (n.one == index.one)
			{
				one = n.one;
				tow = index.tow + index.tow - n.tow;
			}
			else
			{
				tow = n.tow;
				one = index.one + index.one - n.one;
			}
			if (infifo(index.one, index.tow, one, tow))
			{
				if (one == old.one && tow == old.tow)	//如果入队元素是目标元素
				{
					return true;
				}
			}
		}

		//如果连接成功 上
		if (infifo(index.one, index.tow, index.one - 1, index.tow))
		{
			if (index.one - 1 == old.one && index.tow == old.tow)	//如果入队元素是目标元素
			{
				return true;
			}
		}
		
		//左
		if (infifo(index.one, index.tow, index.one, index.tow - 1))
		{
			if (index.one == old.one && index.tow - 1 == old.tow)
			{
				return true;
			}
		}

		//下
		if (infifo(index.one, index.tow, index.one + 1, index.tow))
		{
			if (index.one + 1 == old.one && index.tow == old.tow)
			{
				return true;
			}
		}

		//右
		if (infifo(index.one, index.tow, index.one, index.tow + 1))
		{
			if (index.one == old.one && index.tow + 1 == old.tow)
			{
				return true;
			}
		}
	}

	//不可连接 重新标记为未消除状态
	greens[old.one][old.tow].setsign(1);
	greens[news.one][news.tow].setsign(1);
	return false;
}

//连接两个Node
bool infifo(int fatherone, int fathertow, int sonone, int sontow)
{
	//入队元素在有效范围内
	if (sonone >= 0 && sontow >= 0 && sonone < HClee + 2 && sontow < WClee + 2)
	{
		//入队元素还处于未被搜过状态 并且该位置蔬菜对象已消除
		if (node[sonone][sontow].index.sign == 0 && greens[sonone][sontow].getsign() == 0)
		{
			//如果该父节点的父节点指针不为空 即（至少已连接两个节点了）
			if (node[fatherone][fathertow].FatherNode != NULL)
			{
				ArrayIndex n = node[fatherone][fathertow].FatherNode->index;
				//if该父节点的父节点 与 该节点不再同一线上 即拐弯了
				if (n.one != sonone && n.tow != sontow)
				{
					if (node[fatherone][fathertow].VarCount >= 2)//拐点不能大于2个
					{
						return false;
					}
					else node[sonone][sontow].VarCount = node[fatherone][fathertow].VarCount + 1;
				}
				// 否则就是不拐弯 继承上个节点的折点数
				else node[sonone][sontow].VarCount = node[fatherone][fathertow].VarCount;
			}

			path.INFIFO(node[sonone][sontow].index);
			//标记已搜索
			node[sonone][sontow].index.sign = 1;
			//建立联系
			node[sonone][sontow].FatherNode = &node[fatherone][fathertow];
		}
		return true;
	}
	return false;
}