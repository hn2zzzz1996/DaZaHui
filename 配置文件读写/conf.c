#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct entry_t {
    char *key;
    char *value;
} entry_t;

typedef struct conf_t {
    int capacity;
    int size;
    entry_t **entry;
} conf_t;

void errExit(const char *msg) {
    fprintf(stderr, msg);
    exit(1);
}

void *Malloc(int size) {
    void *mem = malloc(size);
    if(mem == NULL) {
        errExit("mem alloc error.");
    }
    return mem;
}

conf_t *new_conf() {
    conf_t *conf = Malloc(sizeof(*conf));
    conf->capacity = 8;
    conf->size = 0;
    conf->entry = Malloc(sizeof(*conf->entry) * conf->capacity);

    return conf;
}

// 去除前后的空格
char *trim_space(char *str) {
    int len = strlen(str);
    char *head = str, *tail = str + len - 1;
    while(*head == ' ' || *head == '\t') {
        head++;
    }
    if(head - str == len) {
        return NULL;
    }

    while(*tail == ' ' || *tail == '\t') {
        tail--;
    }
    *(tail+1) = '\0';

    return head;
}

int set_conf(conf_t *conf, char *key, char *value) {
    key = trim_space(key);
    value = trim_space(value);
    if(key && value) {
        printf("%s = %sx\n", key, value);
    } else {
        fprintf(stderr, "key or value is null\n");
    }

    entry_t *entry = Malloc(sizeof(*entry));
    entry->key = Malloc(strlen(key)+1);
    entry->value = Malloc(strlen(value)+1);
    memcpy(entry->key, key, strlen(key)+1);
    memcpy(entry->value, value, sizeof(value)+1);
    
    conf->entry[conf->size++] = entry;
}
char* get_conf(conf_t *conf, char *key) {
    int i;
    entry_t **entry = conf->entry;
    for(i = 0; i < conf->size; i++) {
        if(strcmp(entry[i]->key, key) == 0) {
            return entry[i]->value;
        }
    }
    return NULL;
}

typedef enum STATE {
    START,
    KEY,
    EQUAL,
    VALUE
} STATE;

conf_t* open_conf_file(const char *filename) {
    conf_t *conf = new_conf();
    FILE *fp = fopen(filename, "r");
    if(fp == NULL) {
        errExit("can't open file!");
    }

    #define BUFSIZE 1024
    char line[BUFSIZE];

    while(fgets(line, BUFSIZE, fp)) {
        char *key = NULL, *value = NULL;
        char *tmp;
        STATE state = START;

        for(tmp = line; *tmp; tmp++) {
            if(state == START) {
                key = tmp;
                state = KEY;
                continue;
            } else if(state == KEY) {
                if(*tmp == '=') {
                    *tmp = '\0';
                    state = EQUAL;
                } else if(*tmp == '#') {
                    *tmp = '\0';
                    break;
                }
            } else if(state == EQUAL) {
                if(*tmp == '#') break;
                value = tmp;
                state = VALUE;
            } else if(state == VALUE) {
                if(*tmp == '#') {
                    *tmp = '\0';
                    break;
                }
            }
        }

        if(key && value) {
            //printf("%s=%sx\n", key, value);
            set_conf(conf, key, value);
        }
    }

    return conf;
}

int main() {
    conf_t *conf = open_conf_file("test.conf");
    printf("get_conf: \"%s\"\n", get_conf(conf, "hsq"));
    printf("get_conf: \"%s\"\n", get_conf(conf, "a a"));
    printf("get_conf: \"%s\"\n", get_conf(conf, "aaaa"));
}