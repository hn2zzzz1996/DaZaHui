#include <bits/stdc++.h>
using namespace std;

#define MAX_LEVEL 10

typedef struct Node{
    int num;
    struct Node *next[];
} Node;

typedef struct {
    int level;
    Node *head;
} Skiplist;

Node *newNode(int level, int num) {
    Node *node = (Node *)malloc(sizeof(*node) + level * sizeof(Node*));
    node->num = num;
    return node;
}

Skiplist* skiplistCreate() {
    Skiplist *sl = (Skiplist *)malloc(sizeof(*sl));
    sl->level = 1;
    sl->head = newNode(MAX_LEVEL, 0);
    for (int i = 0; i < MAX_LEVEL; i++) {
    	sl->head->next[i] = NULL;
	}
    srand(time(NULL));
    return sl;
}

bool skiplistSearch(Skiplist* sl, int target) {
    Node *p, *q = NULL;
    p = sl->head;
    for (int i = sl->level-1; i >= 0; i--) {
        while ((q = p->next[i]) && (q->num <= target)) {
            if (q->num == target) {
                return true;
            }
            p = q;
        }
    }
    return false;
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

void skiplistAdd(Skiplist* sl, int num) {
    Node *update[MAX_LEVEL];
    Node *p, *q = NULL;
    p = sl->head;
    for (int i = sl->level-1; i >= 0; i--) {
        while ((q = p->next[i]) && (q->num < num)) {
            p = q;
        }
        update[i] = p;
    }
    
    int k = randomLevel();
    if (k > sl->level) {
        for (int i = sl->level; i < k; i++) {
            update[i] = sl->head;
        }
        sl->level = k;
    }
    q = newNode(k, num);
    
    for (int i = k-1; i >= 0; i--) {
        q->next[i] = update[i]->next[i];
        update[i]->next[i] = q;
    }
}

bool skiplistErase(Skiplist* sl, int num) {
    Node *update[MAX_LEVEL];
    Node *p, *q = NULL;
    p = sl->head;
    for (int i = sl->level-1; i >= 0; i--) {
        while ((q = p->next[i]) && (q->num < num)) {
            p = q;
        }
        update[i] = p;
    }

    if (q && q->num == num) {
        for (int i = sl->level-1; i >= 0; i--) {
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

void skiplistFree(Skiplist* sl) {
    Node *p, *q;
    p = sl->head;
    while((q = p->next[0])) {
        free(p);
        p = q;
    }
    free(sl);
}

int main() {
	Skiplist* obj = skiplistCreate();
	//bool param_1 = skiplistSearch(obj, target);
	
	
	skiplistAdd(obj, 1);
	skiplistAdd(obj, 2);
	skiplistAdd(obj, 3);
	if (skiplistSearch(obj, 1)) {
		printf("ok!\n");
	}
	
	//bool param_3 = skiplistErase(obj, num);
	
	skiplistFree(obj);
}

