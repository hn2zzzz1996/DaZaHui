#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int matchthere(char *regexp, char *text);
int matchstar(char c, char *regexp, char *text);

int match(char *regexp, char *text) {
    if (regexp[0] == '^') {
        return matchthere(regexp + 1, text);
    }
    do {
        // 可能会匹配空串
        if (matchthere(regexp, text)) {
            return 1;
        }
    } while (*text++ != '\0');
}

int matchthere(char *regexp, char *text) {
    if (regexp[0] == '\0')
        return 1;
    else if (regexp[0] == '$' && regexp[1] == '\0')
        return *text == '\0';
    else if (regexp[1] == '*')
        return matchstar(regexp[0], regexp+2, text);
    else if(*text != '\0' && (regexp[0] == '.' || regexp[0] == *text))
        return matchthere(regexp+1, text+1);
    return 0;
}

int matchstar(char c, char *regexp, char *text) {
    do {
        // 对于*，匹配0次或多次
        if (matchthere(regexp, text))
            return 1;
    } while (*text != '\0' && (*text++ == c || c == '.'));
    return 0;
}

int main() {
    if(match("aa*", "aaa")) {
        printf("yes!\n");
    }
}