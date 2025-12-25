#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#define MAX_LEVEL 16    // skiplist的最大层数 
#define P 0.5           // 每两层之间以插入元素建立新索引的概率
						//只有在第n层建立了新索引，该概率才会作用到第n到n+1层之间 

// 自定义结构体
//为方便测试此处采用简单的结构体，由于跳表不直接操作结构体所以结构体大小不会影响效率 
typedef struct {
    int data; 
} ElementType;

typedef int KeyType;
//从节点多的到少的层高度依次增加，最底层为第0层 
typedef struct Node{
    KeyType key;	//用于排序的键值 
    ElementType value;		//节点中存储的数据 
    int level;		//该节点的高度 
    struct Node **forward;	//存放第 
}Node;

typedef struct SkipList {
    Node *header;	//虚拟头节点 
    int level;		//跳表的总高度 
    int size;		//跳表中的节点个数 
} SkipList;

// 函数声明
SkipList* createSkipList();
void freeSkipList(SkipList *list);
int insert(SkipList *list, KeyType key, ElementType value);
int deleteNode(SkipList *list, KeyType key);
Node* search(SkipList *list, KeyType key);
void printSkipList(SkipList *list);

#endif
