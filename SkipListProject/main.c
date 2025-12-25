#include <stdio.h>
#include <stdlib.h>
#include "skiplist.h"

int main() {
    srand((unsigned)time(NULL)); // 初始化随机种子
    
    SkipList *list = createSkipList();
    char command[10];
    int key, data_in;
    
    printf("=== Skip List Test Driver ===\n");
    printf("Commands: \n");
    printf("  i <key> <data>  : Insert (e.g., i 10 999)\n");
    printf("  s <key>         : Search\n");
    printf("  d <key>         : Delete\n");
    printf("  p               : Print Structure\n");
    printf("  q               : Quit\n");
    printf("=============================\n");

    while (1) {
        printf("\nCMD> ");
        scanf("%s", command);

        if (strcmp(command, "q") == 0) {
            break;
        } 
        else if (strcmp(command, "i") == 0) {
            scanf("%d %d", &key, &data_in);
            ElementType val = {data_in};
            insert(list, key, val);
            printf("Inserted key: %d, data: %d\n", key, data_in);
        } 
        else if (strcmp(command, "d") == 0) {
            scanf("%d", &key);
            if (deleteNode(list, key))
                printf("Deleted key: %d\n", key);
            else
                printf("Key %d not found.\n", key);
        } 
        else if (strcmp(command, "s") == 0) {
            scanf("%d", &key);
            Node *result = search(list, key);
            if (result)
                printf("Found! Key: %d, Data: %d\n", result->key, result->value.data);
            else
                printf("Key %d not found.\n", key);
        }
        else if (strcmp(command, "p") == 0) {
            printSkipList(list);
        }
    }

    freeSkipList(list);
    return 0;
}
