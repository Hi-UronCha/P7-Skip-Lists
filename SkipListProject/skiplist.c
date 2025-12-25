#include "skiplist.h"

//创建节点 
Node* createNode(KeyType key, ElementType value, int level) {
    Node *node = (Node *)malloc(sizeof(Node));//给节点申请内存 
   //赋值 
    node->key = key;
    node->value = value;
    node->level = level;
    //
    node->forward = (Node **)calloc(level + 1, sizeof(Node*));
    return node;
}

//初始化跳表 
SkipList* createSkipList() {
	//申请存储空间赋初值 
    SkipList *list = (SkipList *)malloc(sizeof(SkipList));
    list->level = 0;
    list->size = 0;
    //建立虚拟头节点确保该节点在最顶层(键值最小，level最高) 
    ElementType dummy = {0}; 
    list->header = createNode(INT_MIN, dummy, MAX_LEVEL);
    return list;
}

//算法核心：利用随机算法维护跳表的结构
//在理想跳表中第h层节点数量为n/2^h，即该点作为第h层索引的概率为0.5^h 
int randomLevel() {
    int lvl = 0;
    //增加层次的条件：层次小于最大层次+随机数小于既定概率（更新的概率为P 
    while ((float)rand() / RAND_MAX < P && lvl < MAX_LEVEL - 1) {
        lvl++;
    }
    return lvl;
}

//释放内存 
void freeSkipList(SkipList *list) {
    Node *current = list->header;
    //逐层释放 
    while (current != NULL) { 
    	//记录下一层的指针 
        Node *next = current->forward[0];
        free(current->forward);
        free(current);
        current = next;//更新循环变量 
    }
    free(list);
}

//实现查找 
Node* search(SkipList *list, KeyType key) {
    Node *x = list->header;
    //外层循环 ：从顶层到底层 
    for (int i = list->level; i >= 0; i--) {
    	//内层循环在层内查找往下层走的节点 
        while (x->forward[i] != NULL && x->forward[i]->key < key) {
            x = x->forward[i];
        }
    }
    //x为理论上的目标节点，若键值不等则表示不存在 
    x = x->forward[0];
    if (x != NULL && x->key == key) return x;
    return NULL;
}

//实现插入 
int insert(SkipList *list, KeyType key, ElementType value) {
    Node *update[MAX_LEVEL];
    Node *x = list->header;
    for (int i = list->level; i >= 0; i--) {
        while (x->forward[i] != NULL && x->forward[i]->key < key) {
            x = x->forward[i];
        }
        update[i] = x;
    }
    x = x->forward[0];
    if (x != NULL && x->key == key) {
        x->value = value; // Update value
        return 0;
    }
    int newLevel = randomLevel();
    if (newLevel > list->level) {
        for (int i = list->level + 1; i <= newLevel; i++) {
            update[i] = list->header;
        }
        list->level = newLevel;
    }
    Node *newNode = createNode(key, value, newLevel);
    for (int i = 0; i <= newLevel; i++) {
        newNode->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = newNode;
    }
    list->size++;
    return 1;
}

//实现删除（用返回值表示是否存在该键值） 
int deleteNode(SkipList *list, KeyType key) {
    Node *update[MAX_LEVEL];	//存放前驱节点 
    Node *x = list->header;
    //找到该节点并记录过程中经过的其他节点 
    for (int i = list->level; i >= 0; i--) {
        while (x->forward[i] != NULL && x->forward[i]->key < key) {
            x = x->forward[i];
        }
        update[i] = x;
    }
    x = x->forward[0];	 //理论上的目标节点 
    //如果确实找到了： 
    if (x != NULL && x->key == key) {
    	//逐层删除 
        for (int i = 0; i <= list->level; i++) {
        	//如果前驱节点没有指向x，则表示已经删完 
            if (update[i]->forward[i] != x) break;
            //否则向上继续删除 
            update[i]->forward[i] = x->forward[i];
        }
        //释放内存 
        free(x->forward);
        free(x);
        //如果顶层被删除则更新高度 
        while (list->level > 0 && list->header->forward[list->level] == NULL) {
            list->level--;
        }
        //成功删除节点数减小 
        list->size--;
        return 1;
    }
    return 0;
}

//输出辅助函数 
void printSkipList(SkipList *list) {
    for (int i = list->level; i >= 0; i--) {
        Node *x = list->header->forward[i];
        printf("Level %d: ", i);
        while (x != NULL) {
            printf("[%d] -> ", x->key);
            x = x->forward[i];
        }
        printf("NULL\n");
    }
}
