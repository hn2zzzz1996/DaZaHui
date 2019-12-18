#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct entry_t {
    char *key;
    char *value;
    struct entry_t *next;
} entry_t;

typedef struct conf_t {
    int size;
    entry_t *entry_head;
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
    conf->size = 0;
    conf->entry_head = NULL;

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

entry_t *find_entry(conf_t *conf, char *key) {
    entry_t *entry = conf->entry_head;
    while(entry) {
        if(strcmp(entry->key, key) == 0) {
            return entry;
        }
        entry = entry->next;
    }
    
    return NULL;
}

void new_entry(conf_t *conf, char *key, char *value) {
    entry_t *entry = Malloc(sizeof(*entry));
    entry->key = Malloc(strlen(key)+1);
    entry->value = Malloc(strlen(value)+1);
    memcpy(entry->key, key, strlen(key)+1);
    memcpy(entry->value, value, sizeof(value)+1);
    
    entry->next = conf->entry_head;
    conf->entry_head = entry;
}

int set_conf(conf_t *conf, char *key, char *value) {
    key = trim_space(key);
    value = trim_space(value);
    if(key && value) {
        printf("%s = %sx\n", key, value);
    } else {
        fprintf(stderr, "key or value is null\n");
    }

    entry_t *entry = find_entry(conf, key);
    if(entry == NULL) {
        new_entry(conf, key, value);
    } else {
        entry->value = value;
    }
}

char* get_conf(conf_t *conf, char *key) {
    entry_t *entry = find_entry(conf, key);
    if(entry != NULL) {
        return entry->value;
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

    fclose(fp);
    return conf;
}

void save_conf_file(conf_t *conf, const char *name) {
    FILE *fp = fopen(name, "w");
    entry_t *head = conf->entry_head;
    while(head) {
        fprintf(fp, "%s = %s\n", head->key, head->value);
        head = head->next;
    }
    fclose(fp);
}

int main() {
    conf_t *conf = open_conf_file("test.conf");
    //printf("get_conf: \"%s\"\n", get_conf(conf, "hsq"));
    printf("get_conf: \"%s\"\n", get_conf(conf, "a a"));
    //printf("get_conf: \"%s\"\n", get_conf(conf, "aaaa"));
    //printf("get_conf: \"%s\"\n", get_conf(conf, "bbbb"));
    //save_conf_file(conf, "abc.conf");
}