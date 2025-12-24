# P7-Skip-Lists

## Project Assignment
Skip list is a data structure that supports both searching and insertion in $O(logN)$ expected time.

This project requires you to introduce the skip lists, and to implement insertion, deletion, and searching in skip lists.  A formal proof is expected to show that the expected time for the skip list operations is O(logN).  You must generate test cases of different sizes to illustrate the time bound.

Grading Policy:
Programming: Implement the functions (6 pts.).  Write a test of performance program (1 pts.).  All the codes must be sufficiently commented.

Testing: Generate the test cases and give the run time table (2 pts.).  Plot the run times vs. input sizes for illustration (2 pts.).  Write analysis and comments with the proofs (5 pts.).  The analysis must be based on your testing results.

Documentation: Chapter 1 (1 pt.), Chapter 2 (2 pts.), and finally a complete report (1 point for overall style of documentation).

---

## 任务以及分工

### 一、 C语言实现核心难点
1.  **内存结构**：`Node` 结构体中 `forward` 是 `Node**`（指向指针的指针），需要根据层数动态 `malloc`。
2.  **指针操作**：`Insert` 和 `Delete` 必须维护一个 `Node *update[MAX_LEVEL]` 数组，记录每一层的前驱节点，否则链表会断裂。
3.  **内存管理**：删除节点时，先 `free(node->forward)` 再 `free(node)`，防止内存泄漏。
4.  **随机化**：使用 `rand()` 前必须在 main 中调用一次 `srand(time(0))`。

---

### 二、 三人分工 (针对 Rubric 优化)

#### 成员 A：核心代码 (The Coder) —— 攻克 Item 6 (10分)
*   **职责**：编写 `skiplist.c/h`。
*   **实现重点**：
    1.  定义 `struct Node` 和 `struct SkipList`。
    2.  实现 `Search`, `Insert`, `Delete`。**注意：** C语言极易段错误，操作指针前必须检查非空。
    3.  **生死线**：**全注释**。Rubric 规定注释不好扣10分。每行关键逻辑、每个变量定义都要写注释（如：`// update[i] stores the node at level i to be updated`）。
    4.  代码风格要整洁，变量命名要有意义。

#### 成员 B：测试与理论 (The Analyst) —— 攻克 Item 4 & 5 (6分)
*   **职责**：编写 `test_generator`，执行测试，负责数学推导。
*   **实现重点**：
    1.  **数据生成器**：写一个程序生成测试指令（如 `1000 RANDOM_INS`），避免手动输入。
    2.  **正确性测试 (Item 4 重点)**：设计 Edge Cases（删除不存在的数、删头尾、插重复值），并**截图**证明逻辑正确。
    3.  **时空复杂度 (Item 5)**：
        *   Time: 画出 $N$ vs Time 的对数曲线。
        *   Space: **新增要求**。计算 C 语言 `malloc` 的总字节数，分析空间复杂度 $O(N)$。

#### 成员 C：文档与流程 (The Architect) —— 攻克 Item 1, 2, 3 (4分)
*   **职责**：撰写文档，画图，打包。
*   **实现重点**：
    1.  **Chapter 1**：重写简介，不要复制题目。
    2.  **Chapter 2**：**画流程图**（Flowchart）描述 Search 算法，不要只贴代码。画出 C 语言内存布局图。
    3.  **整合**：将 B 的截图和 A 的代码片段整合，解释测试目的。
    4.  **打包**：确保文件结构清晰（Source, Report, TestData），ZIP 打包。

---

### 三、 5天冲刺计划

*   **Day 1 (理论与框架)**：
    *   **A**: 写好 `.h` 接口和 `struct`。
    *   **B**: 推导时空复杂度公式。
    *   **C**: 写 Chapter 1，画 SkipList 结构图。
*   **Day 2 (核心编码)**：
    *   **A**: 实现 `Insert` & `Search`，**边写边注释**。
    *   **B**: 写好 Data Generator，准备测试数据。
    *   **C**: 绘制 Search 算法流程图 (Item 2 要求)。
*   **Day 3 (正确性攻坚)**：
    *   **A**: 实现 `Delete`。
    *   **B**: 跑 **Correctness Test**，验证逻辑并**截图** (Item 4 重点)。
    *   **C**: 撰写 Chapter 2 & 3，解释算法和测试目的。
*   **Day 4 (性能与分析)**：
    *   **B**: 跑大数据量 (10w-100w)，记录 Time & Space。
    *   **A**: 检查代码风格，补全所有遗漏的注释。
    *   **C**: 撰写 Chapter 4，整合图表，分析“为什么符合 O(logN)”。
*   **Day 5 (收尾)**：
    *   检查封面、目录。
    *   检查代码能否在标准 C 环境编译。
    *   打包提交。

