#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define ERROR 1
#define SUCCESS 0

int main() {
    char* cwd = getcwd(NULL, PATH_MAX);

    if(cwd == NULL) {
        perror("pwd");
        return ERROR;
    }

    printf("%s\n", cwd);
    free(cwd); /* free memory allocated by getwd() */
    return SUCCESS;
}


