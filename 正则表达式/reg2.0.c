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
#define RE_STAR         (new_re(STAR, '*', NULL, 0))
#define RE_POINT        (new_re(POINT, '.', NULL, 0))

RE *compile(char *reg) {
    char *p;
    RE *new_reg = (RE*)Malloc(sizeof(*new_reg) * (strlen(reg) + 1));
    int i = 0;
    for (p = reg; *p; p++) {
        if (*p == '*') {
            
        } else if (*p == '.') {

        } else {

        }
    }
}

void print_re(RE *reg) {

}

int main() {
    RE *reg = compile("a*");
}