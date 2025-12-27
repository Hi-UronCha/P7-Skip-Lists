#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <windows.h>
#include "skiplist.h"

// 宏定义测试规模
#define TEST_CORRECTNESS_SIZE 100
#define NUM_TEST_SCALES 9

// --- 辅助函数 ---

// 获取当前时间（秒），用于计算耗时
double get_time() {
    return (double)clock() / CLOCKS_PER_SEC;
}

// 估算跳表占用的内存 (Byte)
long long estimate_memory(SkipList *list) {
    long long total_bytes = sizeof(SkipList); // 跳表结构体本身
    Node *curr = list->header;
    while (curr != NULL) {
        // 节点结构体大小
        total_bytes += sizeof(Node);
        // 柔性数组/指针数组 forward 的大小: (level + 1) * 指针大小
        total_bytes += (curr->level + 1) * sizeof(Node*);
        curr = curr->forward[0];
    }
    return total_bytes;
}

// --- 第一部分：正确性测试 (Unit Test) ---
void test_correctness() {
    printf("========== 1. 开始正确性与边界测试 ==========\n");
    SkipList *list = createSkipList();

    // 1. 典型案例：正常插入
    printf("[Test] 插入 10 个随机节点...\n");
    int keys[] = {10, 50, 30, 20, 40, 90, 60, 80, 70, 100};
    for (int i = 0; i < 10; i++) {
        ElementType val = {keys[i] * 10};
        insert(list, keys[i], val);
    }
    assert(list->size == 10);
    printf("   -> 插入 size 检查通过。\n");

    // 2. 顺序性检查：第0层应当是有序链表
    printf("[Test] 检查第 0 层是否有序...\n");
    Node *curr = list->header->forward[0];
    while (curr && curr->forward[0]) {
        assert(curr->key < curr->forward[0]->key);
        curr = curr->forward[0];
    }
    printf("   -> 顺序性检查通过。\n");

    // 3. 边界案例：重复插入 (Update)
    printf("[Test] 重复插入 key=50，检查 value 是否更新...\n");
    ElementType newVal = {9999};
    insert(list, 50, newVal); // 更新
    assert(list->size == 10); // size 不应该增加
    Node *res = search(list, 50);
    assert(res != NULL && res->value.data == 9999);
    printf("   -> 重复插入更新机制通过。\n");

    // 4. 极端案例：查找不存在的 Key
    printf("[Test] 查找不存在的 Key (key=101, key=-1)...\n");
    assert(search(list, 101) == NULL);
    assert(search(list, -1) == NULL);
    printf("   -> 查找不存在 Key 通过。\n");

    // 5. 典型案例：删除存在的 Key
    printf("[Test] 删除存在的 Key (key=30)...\n");
    int delRes = deleteNode(list, 30);
    assert(delRes == 1);
    assert(search(list, 30) == NULL);
    assert(list->size == 9);
    printf("   -> 删除存在的 Key 通过。\n");

    // 6. 极端案例：删除不存在的 Key
    printf("[Test] 删除不存在的 Key (key=999)...\n");
    delRes = deleteNode(list, 999);
    assert(delRes == 0);
    assert(list->size == 9);
    printf("   -> 删除不存在 Key 通过。\n");

    // 7. 极端案例：全部删除直到为空
    printf("[Test] 删除剩余所有节点...\n");
    for (int i = 0; i < 10; i++) {
        if (keys[i] == 30) continue; // 已删除
        deleteNode(list, keys[i]);
    }
    assert(list->size == 0);
    assert(list->header->forward[0] == NULL);
    printf("   -> 清空测试通过。\n");

    freeSkipList(list);
    printf("========== 正确性测试全部通过 ==========\n\n");
}

// --- 第二部分：时空复杂度测试 (Benchmark) ---
void test_performance() {
    printf("========== 2. 开始时空复杂度测试 ==========\n");
    // 定义测试规模 N
    int scales[NUM_TEST_SCALES] = {1000, 2000, 5000, 10000, 20000, 50000, 100000, 200000, 500000};

    // 打印表头
    printf("%-10s | %-15s | %-15s | %-15s | %-15s\n", 
           "N (Size)", "Insert Time(s)", "Search Time(s)", "Memory(MB)", "Avg Level");
    printf("--------------------------------------------------------------------------------\n");

    for (int k = 0; k < NUM_TEST_SCALES; k++) {
        int N = scales[k];
        SkipList *list = createSkipList();

        // 准备随机数据
        // 使用 malloc 防止栈溢出
        int *data = (int*)malloc(N * sizeof(int));
        for(int i=0; i<N; i++) {
            // 保证 key 范围比较大，减少碰撞，或者故意允许碰撞测试稳定性
            data[i] = rand() | (rand() << 15); 
        }

        // --- 1. 测试 Insert 时间 ---
        double start = get_time();
        for (int i = 0; i < N; i++) {
            ElementType val = {i};
            insert(list, data[i], val);
        }
        double end = get_time();
        double insert_time = end - start;

        // --- 2. 测试 Search 时间 (查找 N 次) ---
        // 为了公平，查找一半存在的，一半随机的
        start = get_time();
        for (int i = 0; i < N; i++) {
            int key;
            if (i % 2 == 0) key = data[i]; // 存在的
            else key = rand();             // 随机的
            search(list, key);
        }
        end = get_time();
        double search_time = end - start;

        // --- 3. 计算空间占用 ---
        long long bytes = estimate_memory(list);
        double mb = (double)bytes / (1024 * 1024);

        // --- 4. 计算平均层高 ---
        // 理论值应该是 ~ 1/(1-p) = 2 (当P=0.5时)
        long long total_levels = 0;
        Node *curr = list->header->forward[0];
        while(curr) {
            total_levels += curr->level;
            curr = curr->forward[0];
        }
        double avg_level = (double)total_levels / N;

        // 输出结果
        printf("%-10d | %-15.4f | %-15.4f | %-15.4f | %-15.2f\n", 
               N, insert_time, search_time, mb, avg_level);

        free(data);
        freeSkipList(list);
    }
    printf("--------------------------------------------------------------------------------\n");
    printf("说明: Search Time 是执行 N 次查找的总时间。\n");
}

void run_benchmark() {
    // 配置区域
    int start_n = 50000;      // 起始大小
    int end_n   = 2000000;    // 结束大小
    int step    = 50000;      // 步长
    int repeat  = 3;          // 每个N跑3次取平均，消除抖动

    printf("N,InsertTime,SearchTime\n"); // CSV 表头

    for (int n = start_n; n <= end_n; n += step) {
        double total_insert_time = 0;
        double total_search_time = 0;

        for (int r = 0; r < repeat; r++) {
            SkipList *list = createSkipList();
            
            // 生成随机数据 (预先生成以排除 rand() 对计时的影响)
            int *keys = (int*)malloc(n * sizeof(int));
            for (int i = 0; i < n; i++) keys[i] = rand() | (rand() << 15);

            // 1. 计时插入
            double t1 = get_time();
            for (int i = 0; i < n; i++) {
                ElementType val = {i};
                insert(list, keys[i], val);
            }
            double t2 = get_time();
            total_insert_time += (t2 - t1);

            // 2. 计时查找
            t1 = get_time();
            for (int i = 0; i < n; i++) {
                // 查找刚才插入的 key
                search(list, keys[i]); 
            }
            t2 = get_time();
            total_search_time += (t2 - t1);

            // 清理
            free(keys);
            freeSkipList(list);
        }

        // 输出平均时间
        printf("%d,%.6f,%.6f\n", 
               n, 
               total_insert_time / repeat, 
               total_search_time / repeat);
        
        // 刷新缓冲区，防止程序崩溃看不到数据
        fflush(stdout); 
    }
}

int main() {

    SetConsoleOutputCP(65001); // 设置控制台输出为 UTF-8 编码
    // 必须初始化随机种子，否则跳表退化为链表
    srand((unsigned)time(NULL));

    test_correctness();
    test_performance();
    run_benchmark();

    return 0;
}