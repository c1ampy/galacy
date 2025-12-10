#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <Windows.h>
#include <graphics.h>
#include <stdbool.h>
#include "list.h"
#include "object.h"

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 800
#define FPS 60
#define ENEMY_NUM 10
#define BULLET_NUM 50
#define BULLET_TIME 0.5
#define ENEMY_TIME 0.1

#define PLAYER_WIDTH 50
#define PLAYER_HEIGHT 50
#define ENEMY_WIDTH 40
#define ENEMY_HEIGHT 40
#define BULLET_WIDTH 10
#define BULLET_HEIGHT 20

int existEnemyNum = 0;
int score = 0;

int startTime;

Object *player;
List *enemy_list = NULL, *bullet_list = NULL;
int player_speed, enemy_speed, bullet_speed; // 单位：像素每帧
// 上面的各类 speed 参数不设置为常量，因为后续可能推出动态难度系统。

bool object_collide(const Object *obj1, const Object *obj2);
void playerMove();
void initEnemyPlane();
void initBullet();
void enemyPlaneMove();
void bulletMove();
void enemyCheck();

void playerMove()
{
    
    if (GetAsyncKeyState('W') & 0x8000 || GetAsyncKeyState(VK_UP) & 0x8000)
    {
        if(player->y > 0)
        {
            player->y -= player_speed;
        }
    }
    if (GetAsyncKeyState('S') & 0x8000 || GetAsyncKeyState(VK_DOWN) & 0x8000)
    {
        if(player->y < SCREEN_HEIGHT - PLAYER_HEIGHT)
        {
            player->y += player_speed;
        }
    }
    if (GetAsyncKeyState('A') & 0x8000 || GetAsyncKeyState(VK_LEFT) & 0x8000)
    {
        if(player->x > 0)
        {
            player->x -= player_speed;
        }
    }
    if (GetAsyncKeyState('D') & 0x8000 || GetAsyncKeyState(VK_RIGHT) & 0x8000)
    {
        if(player->x < SCREEN_WIDTH - PLAYER_WIDTH)
        {
            player->x += player_speed;
        }
    }
}

void initEnemyPlane() // 生成敌机
{

    int endTime = time(NULL);                        // 获取当前时间
    double deltaTime = difftime(endTime, startTime); // 计算两次释放敌机的时间差
    if (deltaTime >= ENEMY_TIME)
    {                                  // 判断时间差够不够大，这个时间差可以随难度变化
        if (existEnemyNum < ENEMY_NUM) // 判断飞机数量，屏幕上飞机不能过多
        {
            Object *new_enemy = (Object *)malloc(sizeof(Object));
            if (!new_enemy)
            {
                fprintf(stderr, "内存分配失败。\n");
                exit(EXIT_FAILURE);
            }
            int x = rand() % (SCREEN_WIDTH - ENEMY_WIDTH); // 确保出现在画幅内
            int y = -ENEMY_HEIGHT;                                             // 确保纵向上飞机从画幅外掉落
            new_enemy->x = x;
            new_enemy->y = y;
            new_enemy->type = ENEMY;
            list_append(enemy_list, new_enemy);
            existEnemyNum++; // 敌机数量++
        }
        startTime = endTime; // 生成后将初始时间更新成最近的一次生成敌机时间
    }
}

void initBullet() // 生成子弹
{

    int endTime = time(NULL);                        // 获取当前时间
    double deltaTime = difftime(endTime, startTime); // 计算两次释放敌机的时间差
    if (deltaTime >= BULLET_TIME)
    { // 判断时间差够不够大，这个时间差可以随难度变化
        Object *new_bullet = (Object *)malloc(sizeof(Object));
        if (!new_bullet)
        {
            fprintf(stderr, "内存分配失败。\n");
            exit(EXIT_FAILURE);
        }
        int x = player->x + PLAYER_WIDTH / 2 - BULLET_WIDTH/2;
        int y = player->y - BULLET_HEIGHT; // 确保出现在玩家飞机的上方正中
        new_bullet->x = x;
        new_bullet->y = y;
        new_bullet->type = BULLET;
        list_append(bullet_list, new_bullet);
        startTime = endTime; // 生成后将初始时间更新成最近的一次生成子弹时间
    }
}

void enemyPlaneMove() // 敌机移动，当敌机到达屏幕最底下时销毁
{
    for (Node *enemy_node = enemy_list->head->next; enemy_node;)
    {
        Node *next_enemy_node = enemy_node->next;
        Object *enemy = (Object *)enemy_node->data;
        enemy->y += enemy_speed;
        if (enemy->y + ENEMY_HEIGHT > SCREEN_HEIGHT) // 判断是否到达屏幕底部
        {
            list_random_erase(enemy_list, enemy_node); // 删除该敌机
            //--hp 或者 游戏结束
        }
        enemy_node = next_enemy_node; // 节点前移
    }
}

void bulletMove() // 子弹移动，当子弹到达屏幕上部时销毁
{
    for (Node *bullet_node = bullet_list->head->next; bullet_node;)
    {
        Node *next_bullet_node = bullet_node->next;
        Object *bullet = (Object *)bullet_node->data;
        bullet->y -= bullet_speed;
        if (bullet->y < 0) // 判断是否到达屏幕顶部
        {
            list_random_erase(bullet_list, bullet_node); // 删除该子弹
        }
        bullet_node = next_bullet_node; // 节点前移
    }
}

void enemyCheck() // 对于所有敌机，判断其与子弹、玩家是否碰撞。
{
    for (Node *enemy_node = enemy_list->head->next; enemy_node;)
    {
        Node *next_enemy_node = enemy_node->next; // 同上
        Object *enemy = (Object *)enemy_node->data;

        for (Node *bullet_node = bullet_list->head->next; bullet_node;)
        {
            Node *next_bullet_node = bullet_node->next; // 同上
            Object *bullet = (Object *)bullet_node->data;

            if (object_collide(enemy, bullet))
            {
                list_random_erase(enemy_list, enemy_node);
                list_random_erase(bullet_list, bullet_node);
                score++; // 得分+1
            }

            bullet_node = next_bullet_node;
        }

        if (object_collide(enemy, player))
        {
            // --hp 或结束游戏等
        }

        enemy_node = next_enemy_node;
    }
}

int main()
{

    return 0;
}
