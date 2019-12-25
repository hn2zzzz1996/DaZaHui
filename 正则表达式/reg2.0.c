#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum RETYPE {
    CHAR,
    STR,
    STAR,
    POINT,
} RETYPE;

typedef struct RE {
    RETYPE type;
    int ch;
    char *cc1;
    int ncc1;
    struct RE *next;
} RE;

void *Malloc(int size) {
    void *mem = malloc(size);
    if (mem == NULL) {
        fprintf(stderr, "Mem alloc error!");
        exit(1);
    }
    return mem;
}

RE *new_re(RETYPE type, int ch, char *cc1, int ncc1) {  
    RE *reg = (RE *)Malloc(sizeof *reg);
    reg->type = type;
    reg->ch = ch;
    if (type == STR) {
        int size = sizeof(char) * (strlen(cc1) + 1);
        reg->cc1 = (char *)Malloc(size);
        memcpy(reg->cc1, cc1, size);
    } else {
        reg->cc1 = NULL;
    }
    reg->ncc1 = ncc1;
}

#define RE_CHAR(ch)     (new_re(CHAR, (ch), NULL, 0))
#define RE_STAR()         (new_re(STAR, '*', NULL, 0))
#define RE_POINT()        (new_re(POINT, '.', NULL, 0))
#define RE_STR(cc)        (new_re(STR, 's', cc, 0))

RE *reverse_list(RE *head) {
    RE *reverse = NULL, *tmp;
    while(head) {
        tmp = head->next;
        head->next = reverse;
        reverse = head;
        head = tmp;
    }
    return reverse;
}

RE *compile(char *reg) {
    char *p;
    int i = 0;
    RE *head = NULL;
    int cnt[257];
    for (p = reg; *p; p++) {
        RE *node;
        if (*p == '*') {
            node = RE_STAR();
        } else if (*p == '.') {
            node = RE_POINT();
        } else if (*p == '[') {
            memset(cnt, 0, sizeof(cnt));
            p++;
            while(*p != ']') {
                if (*p == '-') {
                    for (i = *(p-1); i <= *(p+1); i++) {
                        cnt[i] = 1;
                    }
                    p++;
                } else {
                    cnt[*p] = 1;
                }
                p++;
            }
            
            char cc[256];
            int k = 0;
            for (i = 0; i < 257; i++) {
                if (cnt[i]) {
                    cc[k++] = i;
                }
            }
            cc[k] = '\0';
            node = RE_STR(cc);
        } else {
            node = RE_CHAR(*p);
        }
        node->next = head;
        head = node;
    }
    head = reverse_list(head);
    return head;
}

void print_re(RE *reg) {
    RE *node = reg;
    while (node) {
        switch (node->type)
        {
            case CHAR:
                printf("%c", node->ch);
                break;
            case STR:
                printf("[%s]", node->cc1);
                break;
            case STAR:
                printf("*");
                break;
            case POINT:
                printf(".");
                break;
        }
        node = node->next;
    }
}

int main() {
    //RE *reg = compile("a*.*bcd");
    RE *reg = compile("[ad]");
    print_re(reg);
}