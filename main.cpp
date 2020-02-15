#include<graphics.h>
#include<time.h>
#include<conio.h>
#include<mmsystem.h>			//���ͷ�ļ�  ��������
#pragma comment(lib,"winmm.lib")//�����
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
//#pragma comment( linker, "/subsystem:/ "windows/ " /entry:/ "mainCRTStartup/ " " )
#include"Greens.h"    
#include"MyStack.h"
#include"MyFIFO.h"

#define Height 495	//���ڴ�С
#define Width  864
#define HClee 6		//��Ϸ���ϸ�����
#define WClee 9

//ȫ�� ����
Node node[HClee + 2][WClee + 2];
FIFO path;
ArrayIndex old;//x,y�����ϴε�����ӵ�С�� �Լ�sign�ж��Ƿ��ǡ��ڶ��Ρ���� 
Greens greens[HClee + 2][WClee + 2];//�߲˶���

//����������
void Gamestart();	//��Ϸ��ʼ����
void GameInit();	//��ʼ��
void Lefttop(MOUSEMSG m);
void InitGreens(IMAGE image[]);	//��ʼ���߲˶��� ������߿��Լ��߲˵ı߿�
void nodeInit();
void ShowGreens(int sleep = 10);
void ShowBack();
void ShowMarginBorder(int sleep = 200);
ArrayIndex returnNode(int one, int tow, int sign);
void RandomGreens();
void GreensExchange(Greens *g1, Greens *g2);//�߲˽���
bool SearchConnectionPath(ArrayIndex index1, ArrayIndex index2);
bool infifo(int fatherone, int fathertow, int sonone, int sontow);

/*#pragma comment( linker, "/subsystem:\"windows\" /entry:\"maincrtstartup\"")*/ 
//������
int main()
{

	Gamestart();
	return 0;
}

//��Ϸ��ʼ����
void Gamestart()
{
	//��������
	mciSendString("open music/startback.mp3 alias mdgd", 0, 0, 0);
	mciSendString("play mdgd repeat", 0, 0, 0);

	initgraph(Width,Height);

	IMAGE startback[4];//����ͼ����
	loadimage(&startback[0], "imag/index0.jpg");//����ͼ
	loadimage(&startback[1], "imag/index1.jpg");
	loadimage(&startback[2], "imag/index2.jpg");
	loadimage(&startback[3], "imag/index1.jpg");

	int i = 0;
	MOUSEMSG m;
	
	while (true) 
	{
		while (MouseHit())//r����������Ϣ
		{	
			m = GetMouseMsg();
			//�����ʼ��ť
			if (m.uMsg== WM_LBUTTONUP &&m.x > 444 && m.x < 518 && m.y>394 && m.y < 464)
			{
				mciSendString("close mdgd", NULL, 0, NULL);//�ر�����
				GameInit();//��Ϸ���ݳ�ʼ��
			}
		}
		BeginBatchDraw();
		Sleep(100);
		putimage(0, 0, &startback[++i%4]);//������
		EndBatchDraw();
	}
}

//��Ϸ���ݳ�ʼ��   ��ȡͼƬ  ��������
void GameInit()
{
	ShowBack();	//������

	//��������
	mciSendString("open music/backmusic.mp3 alias bm", 0, 0, 0);
	mciSendString("play bm repeat", 0, 0, 0);

	IMAGE image[9];
	//���߲�ͼƬ�������
	loadimage(&image[0], "imag/0.jpg");
	loadimage(&image[1], "imag/1.jpg");
	loadimage(&image[2], "imag/2.jpg");
	loadimage(&image[3], "imag/3.jpg");
	loadimage(&image[4], "imag/4.jpg");
	loadimage(&image[5], "imag/5.jpg");
	loadimage(&image[6], "imag/6.jpg");
	loadimage(&image[7], "imag/7.jpg");
	loadimage(&image[8], "imag/8.jpg");

	//��ʼ���߲˶��� ������߿��Լ��߲˵ı߿�
	InitGreens(image);
	//��ʼ��node�ڵ�
	nodeInit();

	MOUSEMSG m;
	while (true)//��ѭ��
	{
		while (MouseHit())//����������Ϣ
		{
			m = GetMouseMsg();
			//���������� ���ҵ����Χ���߲�����
			if (m.uMsg == WM_LBUTTONUP)
			{
				Lefttop(m);
			}
		}
	}
}

//��ʼ���߲˶��� ������߿���߲��Լ��߲˵ı߿�
void InitGreens(IMAGE image[])
{
	int imageH = image[0].getheight();
	int imageW = image[0].getwidth();

	//���߲˳�ʼ��
	greens[0][0] = Greens(0, 0, NULL, 0);
	greens[0][0].setsign(0);

	//�ȸ���һ�и�ֵ
	for (int i = 1; i < WClee + 2; i++)
	{
		greens[0][i] = Greens(greens[0][i - 1].getx() + imageW + 2,	//��ȡ��߶����x+ͼƬ���
							  greens[0][i - 1].gety(),				//��ȡ��߶����y
							  NULL,NULL);
		greens[0][i].setsign(0);
	}

	//���յ�һ�и������и�ֵ
	for (int i = 0; i < WClee + 2; i++)//��
	{
		for (int j = 1; j < HClee + 2; j++)//��
		{
			greens[j][i] = Greens(greens[j - 1][i].getx(),				//��ȡ�ϱ߶����x
								  greens[j - 1][i].gety() + imageH + 2,	//��ȡ�ϱ߶���ĸ�+ͼƬ�߶�
								  NULL, NULL);
			greens[j][i].setsign(0);
		}
	}

	//�ӵڶ��еڶ��п�ʼ �� �����ڶ��е����ڶ��� ���������ͼƬ�ͱ��
	int k = 0;
	for (int i = 1; i < WClee + 1; i++)//��
	{
		for (int j = 1; j < HClee + 1; j++)//��
		{
			greens[j][i].setimageAndNumber(image[k % 9], k % 9);
			greens[j][i].setsign(1);
			k++;
		}
	}

	//��������߲˶����ͼƬ�ͱ��
	RandomGreens();

	//����߿�
	ShowMarginBorder();
	//���߲�
	ShowGreens();	
}

//��������߲�ͼƬ
void RandomGreens()
{
	srand(unsigned(time(0)));
	for (int i = 1; i < WClee + 1; i++)//��
	{
		for (int j = 1; j < HClee + 1; j++)//��
		{
			GreensExchange(&greens[j][i], &greens[(rand() % 6) + 1][(rand() % 9) + 1]);
		}
	}
}

//�߲˽���
void GreensExchange(Greens *g1,Greens *g2)
{
	//��ָ������ת��Ϊ ����ʵ��
	Greens greens1 = *g1;
	Greens greens2 = *g2;

	//��ʼ����
	Greens Temp = *g1;
	greens1.setimageAndNumber(greens2.getimage(), greens2.getnumber());
	greens2.setimageAndNumber(Temp.getimage(), Temp.getnumber());

	//�����������ʱ����ʵ�� ���¸�ֵ��ָ���ʾ�� ����
	*g1 = greens1;
	*g2 = greens2;
}

//������
void ShowBack()
{
	IMAGE backgame;
	loadimage(&backgame, "imag/bakcgame.jpg");
	putimage(0, 0, &backgame);
}

//���߲�
void ShowGreens(int sleep)
{
	for (int i = 1; i < WClee + 1; i++)//��
	{
		for (int j = 1; j < HClee + 1; j++)//��
		{
			if (greens[j][i].getsign() == 1)
			{
				//���߲�
				greens[j][i].putGreens();
				//���ڿ�
				greens[j][i].border();
				Sleep(sleep);
			}
		}
	}
}

//����߿�
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

//�ڵ�node ��ʼ��
void nodeInit()
{
	for (int i = 0; i < WClee + 2; i++)//��
	{
		for (int j = 0; j < HClee + 2; j++)//��
		{
			node[j][i].index = returnNode(j, i ,0);
			node[j][i].FatherNode = NULL;
			node[j][i].SonNode = NULL;
			node[j][i].VarCount = 0;
		}
	}
}

//���±�ת��ΪArrayIndex����ά�±����ͣ�
ArrayIndex returnNode(int one,int tow,int sign)
{
	ArrayIndex index;
	index.one = one;
	index.tow = tow;
	index.sign = 0;
	return index;
}

//�������
void Lefttop(MOUSEMSG m)
{
	int imageH = greens[1][1].getimage().getheight();
	int imageW = greens[1][1].getimage().getwidth();

	//��ȡÿ���߲˶���Ŀ��
	int greensW = greens[0][1].getx() - greens[0][0].getx();
	int greensH = greens[1][0].gety() - greens[0][0].gety();
	int TowW = m.x / greensW;		//������������� ת��Ϊ���ӵ��±�
	int FirstW = m.y / greensH;		//FirstW��һά TowW����ά
	
	//�����Χ��Ч
	if (TowW<1 || FirstW<1 || TowW>WClee + 1 || FirstW>HClee + 1 || greens[FirstW][TowW].getsign()==0)
	{
		if (old.sign)//������ε��Ϊ�ڶ��ε��
		{
			//���ϴε������Ϊ��ɫ
			greens[old.one][old.tow].border();
			old.sign = 0;//���´ε������Ϊ��һ�ε��
		}
		return;
	}

	ArrayIndex news;	//���浱ǰ�����λ�õĸ��ӵ��±�
	news.one = FirstW;
	news.tow = TowW;

	//�����ε��λ�õ���
	greens[news.one][news.tow].border(RED);

	//������ε��Ϊ�ڶ��ε��  
	if (old.sign == 1)
	{
		//����Ĳ���ͬһ����ͼƬ
		if (greens[old.one][old.tow].getnumber() != greens[FirstW][TowW].getnumber())
		{
			greens[old.one][old.tow].border();
			greens[news.one][news.tow].border();
			old.sign = 0;
			return;
		}
		else if (old.one == news.one && old.tow == news.tow)
		{ 
			//�������ͬһ��ͼƬ
			old.sign = 0;
			greens[news.one][news.tow].border();
			return;
		}
		//�㷨���� ����·��
		if (SearchConnectionPath(old, news))
		{
			//�ҵ���
			ArrayIndex index1 = node[old.one][old.tow].index;
			ArrayIndex index2 = node[old.one][old.tow].FatherNode->index;

			//ֱ��������ȫ
			//fillroundrect()
			setfillcolor(GREEN);
			while (1)
			{
				//ֱ���������ڵ���������
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
				//���ر���ĸ�Ԫ��
				index1 = index2;
				index2 = node[index2.one][index2.tow].FatherNode->index;
			}

			BeginBatchDraw();	//��ʼ������ͼ

			path.EmptyFIFO();	//��ն���
			nodeInit();			//����������¼���
			ShowBack();			//������
			ShowMarginBorder(0);//����߿�  0�ӳ�
			ShowGreens(0);		//���߲�  0�ӳ�

			EndBatchDraw();//����������ͼ

			old.sign = 0;		
			return;
		}
		else
		{
			//û�ҵ�
			old.sign = 0;	//���´ε����Ϊ��һ�ε��
			greens[old.one][old.tow].border();
			greens[news.one][news.tow].border();
			nodeInit();
			return;
		}//if (SearchConnectionPath(old, news))

		//���ϴε��λ�ø�ԭ
		greens[old.one][old.tow].border();
	}//if (old.sign == 1 && !(old.one==news.one && old.tow==news.tow))

	old = news;	//�����ε����Ϊ�ɲ���  ���´�Ϊ�²�����
	old.sign = 1;	//���´ε����Ϊ�ڶ��ε��

}

//��������·��
bool SearchConnectionPath(ArrayIndex old, ArrayIndex news)
{
	//�Ƚ�������Ϊ������״̬ ��������
	greens[old.one][old.tow].setsign(0);
	greens[news.one][news.tow].setsign(0);

	//�½�����
	path.CreateFIFO();
	path.INFIFO(news);	//���
	node[news.one][news.tow].index.sign = 1;

	while (!path.JudgevoidFIFO())	//���в�Ϊ��
	{
		//����
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
				if (one == old.one && tow == old.tow)	//������Ԫ����Ŀ��Ԫ��
				{
					return true;
				}
			}
		}

		//������ӳɹ� ��
		if (infifo(index.one, index.tow, index.one - 1, index.tow))
		{
			if (index.one - 1 == old.one && index.tow == old.tow)	//������Ԫ����Ŀ��Ԫ��
			{
				return true;
			}
		}
		
		//��
		if (infifo(index.one, index.tow, index.one, index.tow - 1))
		{
			if (index.one == old.one && index.tow - 1 == old.tow)
			{
				return true;
			}
		}

		//��
		if (infifo(index.one, index.tow, index.one + 1, index.tow))
		{
			if (index.one + 1 == old.one && index.tow == old.tow)
			{
				return true;
			}
		}

		//��
		if (infifo(index.one, index.tow, index.one, index.tow + 1))
		{
			if (index.one == old.one && index.tow + 1 == old.tow)
			{
				return true;
			}
		}
	}

	//�������� ���±��Ϊδ����״̬
	greens[old.one][old.tow].setsign(1);
	greens[news.one][news.tow].setsign(1);
	return false;
}

//��������Node
bool infifo(int fatherone, int fathertow, int sonone, int sontow)
{
	//���Ԫ������Ч��Χ��
	if (sonone >= 0 && sontow >= 0 && sonone < HClee + 2 && sontow < WClee + 2)
	{
		//���Ԫ�ػ�����δ���ѹ�״̬ ���Ҹ�λ���߲˶���������
		if (node[sonone][sontow].index.sign == 0 && greens[sonone][sontow].getsign() == 0)
		{
			//����ø��ڵ�ĸ��ڵ�ָ�벻Ϊ�� �������������������ڵ��ˣ�
			if (node[fatherone][fathertow].FatherNode != NULL)
			{
				ArrayIndex n = node[fatherone][fathertow].FatherNode->index;
				//if�ø��ڵ�ĸ��ڵ� �� �ýڵ㲻��ͬһ���� ��������
				if (n.one != sonone && n.tow != sontow)
				{
					if (node[fatherone][fathertow].VarCount >= 2)//�յ㲻�ܴ���2��
					{
						return false;
					}
					else node[sonone][sontow].VarCount = node[fatherone][fathertow].VarCount + 1;
				}
				// ������ǲ����� �̳��ϸ��ڵ���۵���
				else node[sonone][sontow].VarCount = node[fatherone][fathertow].VarCount;
			}

			path.INFIFO(node[sonone][sontow].index);
			//���������
			node[sonone][sontow].index.sign = 1;
			//������ϵ
			node[sonone][sontow].FatherNode = &node[fatherone][fathertow];
		}
		return true;
	}
	return false;
}