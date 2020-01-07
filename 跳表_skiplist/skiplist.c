#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MAX_LEVEL 10

typedef struct Node {
    int key;
    int value;
    struct Node *next[];
} Node;

typedef struct SkipList {
    int level;
    Node *head;
} SkipList;

Node *newNode(int level, int key, int value) {
    Node *node = (Node *)malloc(sizeof(*node) + level * sizeof(Node *));
    node->key = key;
    node->value = value;
}

SkipList *initSkipList() {
    SkipList *sl = (SkipList *)malloc(sizeof(*sl));
    sl->level = 0;
    sl->head = newNode(MAX_LEVEL, 0, 0);
    for (int i = 0; i < MAX_LEVEL; i++) {
        sl->head->next[i] = NULL;
    }
    srand(time(NULL));
    return sl;
}

int randomLevel() {
    int k = 1;
    while (rand() % 2) {
        k++;
    }
    if (k > MAX_LEVEL) {
        k = MAX_LEVEL;
    }
    return k;
}

bool insert(SkipList *sl, int key, int value) {
    Node *update[MAX_LEVEL];
    Node *p, *q = NULL;
    p = sl->head;
    int k = sl->level;
    for (int i = k-1; i >= 0; i--) {
        while ((q = p->next[i]) && (q->key < key)) {
            p = q;
        }
        update[i] = p;
    }

    if (q && q->key == key) {
        return false;
    }

    k = randomLevel();
    if (k > sl->level) {
        for (int i = k-1; i >= sl->level; i--) {
            update[i] = sl->head;
        }
        sl->level = k;
    }

    q = newNode(k, key, value);
    for (int i = 0; i < k; i++) {
        q->next[i] = update[i]->next[i];
        update[i]->next[i] = q;
    }
    return true;
}

int find(SkipList *sl, int key) {
    Node *p, *q = NULL;
    int k = sl->level;
    p = sl->head;
    for (int i = k-1; i >= 0; i--) {
        while ((q = p->next[i]) && (q->key < key)) {
            p = q;
        }
    }
    if (q && q->key == key) {
        return q->value;
    }
    return 0;
}

bool delete(SkipList *sl, int key) {
    Node *update[MAX_LEVEL];
    Node *p, *q = NULL;
    int k = sl->level;
    p = sl->head;
    for (int i = k-1; i >= 0; i--) {
        while ((q = p->next[i]) && (q->key < key)) {
            p = q;
        }
        update[i] = p;
    }
    if (q && q->key == key) {
        for (int i = 0; i < sl->level; i++) {
            if (update[i]->next[i] == q) {
                update[i]->next[i] = q->next[i];
            }
        }
        free(q);
        for (int i = sl->level-1; i >= 0; i--) {
            if (sl->head->next[i] == NULL) {
                sl->level--;
            }
        }
        return true;
    } else {
        return false;
    }
}

void printSL(SkipList *sl) {
    Node *p;
    for (int i = sl->level-1; i >= 0; i--) {
        p = sl->head;
        printf("Level %d: ", i);
        while (p->next[i]) {
            p = p->next[i];
            printf("%d -> ", p->value);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    SkipList *sl = initSkipList();
    insert(sl, 1, 1);
    insert(sl, 4, 4);
    insert(sl, 3, 3);
    insert(sl, 2, 2);
    printSL(sl);
}