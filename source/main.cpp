//仿照b站上的视频，现在还不能发射子弹，也没有敌机

#include <conio.h>
#include <graphics.h>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <Windows.h>
#pragma	comment(lib,"Winmm.lib")

#define SCREEN_WIDTH 400
#define SCREEN_HEIGH 800

#define PLANE_SIZE 50

#define ENEMY_NUM 8

#define ENEMY_SPEED 1.0

#define  BULLET_NUM 10

typedef struct pos
{
	int x;
	int y;

}POS;

typedef struct plane
{
	POS planePos;
	POS planeBullets[BULLET_NUM];
	int bulletLen;
	int bulletSpeed;
}PLANE;

PLANE myPlane;
PLANE enemyPlanes[ENEMY_NUM];
int enemyPlaneLen;
static time_t startTime, endTime;
IMAGE img[3];
int score = 0;

void initGame();
void drawGame();
void updateGame();



void initGame()
{
	initgraph(SCREEN_WIDTH, SCREEN_HEIGH);
	score = 0;

	srand((unsigned)time(NULL));//随机数种子

	myPlane.bulletLen = 0;
	myPlane.bulletSpeed = 3;
	myPlane.planePos = { SCREEN_WIDTH / 2 - PLANE_SIZE / 2,SCREEN_HEIGH - PLANE_SIZE };

	enemyPlaneLen = 0;

	startTime = time(NULL);

}




void drawGame()
{
	BeginBatchDraw();   //开始批量贴图
	//背景
	putimage(0, 0, &img[0]);
	putimage(myPlane.planePos.x - PLANE_SIZE / 2, myPlane.planePos.y - PLANE_SIZE / 2, &img[2], SRCAND);

	for (int i = 0; i < enemyPlaneLen; i++) {
		putimage(enemyPlanes[i].planePos.x - PLANE_SIZE / 2, enemyPlanes[i].planePos.y - PLANE_SIZE / 2, &img[1]);

		for (int i = 0; i < myPlane.bulletLen; i++)
		{
			solidcircle(myPlane.planeBullets[i].x, myPlane.planeBullets[i].y, PLANE_SIZE / 4);
		}

		RECT rect = { 0,PLANE_SIZE,SCREEN_WIDTH,SCREEN_HEIGH };
		setbkmode(TRANSPARENT);           // 文字背景透明
		settextcolor(WHITE);              // 设置文字颜色为白色
		char str[64] = { 0 };
		snprintf(str, sizeof(str), "分数: %d", score);
		drawtext(str, &rect, DT_TOP | DT_CENTER);
	}

	EndBatchDraw();//结束批量贴图
}


void updateGame()
{   //更改位置
	if (GetAsyncKeyState('W') & 0x8000)
	{
		myPlane.planePos.y -= 2;
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{
		myPlane.planePos.y += 2;
	}
	if (GetAsyncKeyState('A') & 0x8000)
	{
		myPlane.planePos.x -= 2;
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		myPlane.planePos.x += 2;
	}

	//发射子弹，这里我没做出来，好像120行写错了

	if (_kbhit())
	{
		if (_getch() == ' ')
		{
			if (enemyPlaneLen < BULLET_NUM) {
				PlaySound("img/1.wav", NULL, SND_FILENAME | SND_ASYNC | SND_NOWAIT);
				myPlane.planeBullets[enemyPlaneLen] = myPlane.planePos;
				myPlane.bulletLen++;
			}
		}
	}
}

int main() {


	loadimage(&img[0], "img/背景.png", SCREEN_WIDTH, SCREEN_HEIGH);
	loadimage(&img[1], "img/敌机.png", PLANE_SIZE, PLANE_SIZE);
	loadimage(&img[2], "img/飞机.png", PLANE_SIZE, PLANE_SIZE);

	initGame();
	while (1)
	{
		drawGame();
		updateGame();
		Sleep(16);
	}


	getchar();
	getchar();
	return 0;
}