#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

#define SUCCESS 0
#define SUBPROCESS_PID 0
#define INIT_SUBPROCESS_ERROR -1
#define WAIT_ERROR -1

int main() {

    // Создаем дочерний процесс
    pid_t fork_result = fork();
    if (fork_result == INIT_SUBPROCESS_ERROR) {
        perror("Couldn't init subprocess");
        return INIT_SUBPROCESS_ERROR;
    }

    // Условие на PID, что это дочерний процесс
    if (fork_result == SUBPROCESS_PID) {
        char *parametrs[] = {"cat", "large.txt", NULL};
        // вызов: cat large.txt. Те мы подменяем текущую программу lab9.exe на cat.exe.
        execvp(parametrs[0], parametrs);
        return SUCCESS;
    }

    printf("This message was printed by the parent process\n");
    return SUCCESS;
}

