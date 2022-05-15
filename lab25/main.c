#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

#define NUMBER_OF_DESCRIPTORS 2

#define PIPE_ERROR -1
#define FORK_ERROR -1
#define READ_ERROR -1
#define WRITE_ERROR -1
#define CLOSE_ERROR -1
#define ERROR -1
#define SUCCESS 0

#define END_OF_FILE 0

#define FIRST_DESCRIPTOR 0
#define SECOND_DECRIPTIOR 1

#define CHILD_PID 0

#define MAX_BUF 256

void write_upper_message(char message[], int message_size) {
    for (int i = 0; i < message_size; i++) {
        message[i] = toupper(message[i]);
        printf("%c", message[i]);
    }
}

int close_both_file_descriptors(int file_descriptors[]) {
    int close_result = close(file_descriptors[FIRST_DESCRIPTOR]);
    if(close_result == CLOSE_ERROR) {
        perror("couldn't close");
        return ERROR;
    }

    close_result = close(file_descriptors[SECOND_DECRIPTIOR]);
    if(close_result == CLOSE_ERROR) {
        perror("couldn't close");
        return ERROR;
    }
    return SUCCESS;
}

int source_process(int file_descriptor[NUMBER_OF_DESCRIPTORS]) {
    pid_t fork_result = fork();

    if (fork_result == FORK_ERROR) {
        perror("couldn't fork");
        return FORK_ERROR;
    }

    // Родительский процесс пойдет создавать receiver_process
    if (fork_result != CHILD_PID) {
        return fork_result;
    }

    // Закрываем лишний конец трубы
    int close_result = close(file_descriptor[FIRST_DESCRIPTOR]);
    
    if (close_result == EOF){
        perror("couldn't close file descriptor");
        return CLOSE_ERROR;
    }

    char message[MAX_BUF];
    char* fgets_result = NULL;

    // Считываем по 256 символов их отправяем их приемнику, пока не конец файла
    while((fgets_result = fgets(message, MAX_BUF, stdin)) != NULL) {

        int write_result = write(file_descriptor[SECOND_DECRIPTIOR], message, strlen(message));
    
        if (write_result == WRITE_ERROR) {
            perror("couldn't write to pipe");
            return ERROR;
        }
    }

    // Закрываем оставшийся конец программного канала
    close_result = close(file_descriptor[SECOND_DECRIPTIOR]); 
    if(close_result == CLOSE_ERROR) {
        perror("couldn't close");
        return ERROR;
    }

    return fork_result;
}

int receiver_process(int file_descriptor[NUMBER_OF_DESCRIPTORS]) {
    pid_t fork_result = fork();

    if (fork_result == FORK_ERROR) {
        perror("fork error");
        return FORK_ERROR;
    }

    // Родительский процесс закрывает оба конца канала, тк не учавствует в общении
    if (fork_result != CHILD_PID){
        int error = close_both_file_descriptors(file_descriptor);
        if (error == ERROR) return ERROR;
        return SUCCESS;
    }

    // Закрываем лишний конец программного канала 
    int close_result = close(file_descriptor[SECOND_DECRIPTIOR]);
    if(close_result == CLOSE_ERROR) {
        perror("couldn't close");
        return ERROR;
    }

    char message[MAX_BUF];
    int read_result = END_OF_FILE;

    // Считываем по 256 символов 
    do {
        int read_result = read(file_descriptor[FIRST_DESCRIPTOR], message, MAX_BUF);

        if (read_result == READ_ERROR) {
            perror("couldn't read from pipe");
            return ERROR;
        }

        write_upper_message(message, read_result);

    } while(read_result == END_OF_FILE); 

    // Закрываем оставшийся конец программного канала
    close_result = close(file_descriptor[FIRST_DESCRIPTOR]); 
    if(close_result == CLOSE_ERROR) {
        perror("couldn't close");
        return ERROR;
    }

    return SUCCESS;
}

int main() {
    int file_descriptors[NUMBER_OF_DESCRIPTORS];

    // создаем программный канал, который заполнит наш массив фаловыми дескрипторами
    pid_t pipe_result = pipe(file_descriptors);

    if(pipe_result == PIPE_ERROR) {
        perror("pipe error");
        return PIPE_ERROR;
    }

    // Source процесс - исток, он считывает данные из стандартного потока ввода
    // Процесс закроет один из концов программного канала(который для записи данных в канал)
    // Далее через оставшийся файловый дескриптор передаст текст с верхним и нижним регистром
    pid_t pid = source_process(file_descriptors);

    if (pid == ERROR) return ERROR;

    if (pid != CHILD_PID) {
        // Reciever процесс - приемник, он принимает данные из source_process через программный канал
        // Процесс закроет один из концов программного канала(который для чтения данных из канала)
        int receiver_result = receiver_process(file_descriptors);
        if (receiver_result == ERROR) return ERROR;
    }

    // Родительский процесс не должен завершаться, чтобы мы могли что-то вводить в bash-e
    wait(NULL);
    return SUCCESS;
}