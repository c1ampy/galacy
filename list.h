/** 
 * @file list.h
 * @brief 这份头文件声明了泛型链表类型以及对其进行一系列操作的函数。
 * @author 陆营
 * @date 2025-12-01
 * @version v1.0
 */

#include <stdlib.h>

#ifndef LIST_H
#define LIST_H

/**
* @brief 链表结点
*/
typedef struct Node {
	void *data; // 泛型
	struct Node *next, *prev; // 双向链表
} Node;

/**
* @brief 链表本身
*/
typedef struct List {
	Node *head, *tail; // 头结点与尾结点
} List;

/**
 * @brief 初始化链表，最初链表只有一个结点，其即为头结点又为尾结点，头结点总不储存数据而尾结点储存数据。
 * @param list 指向链表指针的指针，创建的链表的头尾结点储存于 *list 所指向的内存空间。
 */
void list_init(List **list);

/**
 * @brief 从链表尾部插入元素。
 */
void list_tail_insert(List *list, void *data);

#endif /* LIST_H */