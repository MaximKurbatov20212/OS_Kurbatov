#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define SUCCESS 0
#define SOCKET_ERROR -1
#define CONNECT_ERROR -1
#define WRITE_ERROR -1
#define CLOSE_ERROR -1
#define ERROR 1
#define EOF_OR_FGETS_ERROR NULL

#define MAX_BUF 256

#define DEFAULT_PROTOCOL 0


// создаем сокет, для локального общения
int get_socket() {
    int file_descriptor = socket(AF_LOCAL, SOCK_STREAM, DEFAULT_PROTOCOL);
    if(file_descriptor == SOCKET_ERROR) {
        perror("couldn't create socket");
        return ERROR;
    }
    return file_descriptor;
}

// Считываем по 256 байтов и отправляем серверу, зная файловый дескриптор сокета
int write_in_socket(int file_descriptor) {
    char message[MAX_BUF];
    char* fgets_result = NULL;

    while((fgets_result = fgets(message, MAX_BUF, stdin)) != EOF_OR_FGETS_ERROR) {

        int write_result = write(file_descriptor, message, strlen(message));

        if (write_result == WRITE_ERROR) {
            perror("couldn't write");
            return ERROR;
        }
    }

    return SUCCESS;
}

int main() {
    struct sockaddr_un addr; // адрес сокета

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_LOCAL;


    int file_descriptor = get_socket();
    if(file_descriptor == ERROR) return ERROR;

    // Свяжем сокет с известным локальным адресом сервера
    strcpy(addr.sun_path, "server");

    // пытаемся установить соединение 
    int connect_result = connect(file_descriptor, (struct sockaddr*) &addr, sizeof(addr));

    if (connect_result == CONNECT_ERROR) {
        close(file_descriptor);
        fprintf(stderr, "Server unreachable\n");
        return ERROR;
    }

    int write_result = write_in_socket(file_descriptor);
    if (write_result == WRITE_ERROR) {
        close(file_descriptor);
        return ERROR;
    }

    int close_result = close(file_descriptor);           
    if (close_result == CLOSE_ERROR) {
        perror("couldn't close");
        return ERROR;
    }

    return SUCCESS;
}