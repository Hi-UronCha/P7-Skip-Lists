# Project: Skip List Implementation

**Course:** Advanced Data Structure  
**University:** Zhejiang University  
**Group No.:** 13  
**Group Members:**  

* Zhao Menglei
* Pu Yuancan
* Zhou Haowen

## 1. Project Overview

We provide a complete implementation of a **Skip List** data structure in C. This project includes:

*   **Core Library (`skiplist.c/h`):** Implements the Skip List data structure supporting $O(\log N)$ expected time complexity for Search, Insertion, and Deletion.
*   **Interactive CLI (`main.c`):** A command-line interface to manually manipulate the Skip List (Insert, Delete, Search, Print).
*   **Test Suite (`test.c`):** A dedicated driver for Correctness Verification (Unit Tests) and Performance Benchmarking (Time/Space Complexity analysis).

## 2. File Structure

`README.md` this file.

`report.pdf` the final report for this project (Analysis, Complexity Proofs, and Test Results).

The `source code` directory contains:

*   `skiplist.h`: Header file defining the `Node` and `SkipList` structures and function prototypes.
*   `skiplist.c`: Core implementation of the Skip List algorithms (Search, Insert, Delete, Random Level Generation).
*   `main.c`: The main entry point for the **Interactive Mode**. It parses user commands to modify the list.
*   `test.c`: The entry point for **Testing Mode**. It runs automated correctness checks and generates performance data.

## 3. How to Compile & Run

Since `main.c` and `test.c` both contain a `main()` function, they must be compiled separately depending on which mode you want to run.

### Mode 1: Interactive CLI (Default)

Use this mode to manually interact with the Skip List.

**Linux / macOS / Windows (MinGW):**

```bash
cd "source code"
gcc main.c skiplist.c -o skiplist_cli
./skiplist_cli   # On Windows: skiplist_cli.exe
```


### Mode 2: Automated Testing & Benchmark

Use this mode to run the correctness assertions and generate performance data (as seen in the report).



**Linux / macOS / Windows (MinGW):**

```bash
cd "source code"
gcc test.c skiplist.c -o skiplist_test
./skiplist_test  # On Windows: skiplist_test.exe
```

**Note**: The benchmark section in `test.c` may take a few seconds to complete as it processes large datasets (up to $N=500,000$).

**Note:** the test suite are randomly generated, and the performance data may vary slightly due to randomness. You can change the test case size by modifying the `N` macro in `test.c`.

## 4. Input/Output Format (Interactive Mode)

When running the **Interactive CLI** (`main.c`), the program accepts the following commands:

*   **Insert:** `i <key> <value>`
    *   Example: `i 10 999` (Inserts key 10 with value 999)
*   **Search:** `s <key>`
    *   Example: `s 10` (Searches for key 10)
*   **Delete:** `d <key>`
    *   Example: `d 10` (Deletes key 10)
*   **Print:** `p`
    *   Visualizes the current structure of the Skip List level by level.
*   **Quit:** `q`
    *   Exits the program.

**Example Session:**

```text
CMD > i 5 100
Inserted key: 5, data: 100
CMD > s 5
Found! Key: 5, Data: 100
CMD > d 5
Deleted key: 5
CMD > q
```
