#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#define SUCCESS 0
#define SUBPROCESS_PID 0
#define INIT_SUBPROCESS_ERROR -1
#define WAIT_ERROR -1
#define EXECVP_ERROR 2

int main(int argc, char** argv) {
    
    pid_t fork_result = fork();
    if (fork_result == INIT_SUBPROCESS_ERROR) {
        perror("Couldn't init subprocess");
        return INIT_SUBPROCESS_ERROR;
    }

    if (fork_result == SUBPROCESS_PID) {
        // Тут переписал, чтобы мы могли подтягивать имя файла, как аргумент из командной строки.
        execvp("cat", argv);

        // Исправил тут. Если execvp завершится удачно, то не произойдет обратная передача управления.
        // Поэтому, если мы попали на данную строчку, значит execvp завешился с ошибкой и вернул управление.
        perror("Execvp error");
        return EXECVP_ERROR;
    }

    printf("This message was printed by the parent process\n");
    return SUCCESS;
}