#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define DEFAULT_PROTOCOL 0

#define LISTEN_ERROR -1
#define CLOSE_ERROR -1
#define ACCEPT_ERROR -1
#define READ_ERROR -1
#define SOCKET_ERROR -1
#define BIND_ERROR -1

#define ERROR 1

#define MAX_LEN 1
#define SUCCESS 0
#define MAX_BUF 256 

#define END_OF_FILE 0

int to_upper_case(int file_descriptor) {
    int read_result;
    char message[MAX_BUF];

    while ((read_result = read(file_descriptor, message, MAX_BUF)) != END_OF_FILE) {

        if (read_result == READ_ERROR) {
            perror("couldn't read");
            return ERROR;
        }

        for (int i = 0; i < read_result; i++) {
            message[i] = toupper(message[i]);   
            printf("%c", message[i]);
        }
    }

    return SUCCESS;
}

int main() {
    struct sockaddr_un addr;
    
    // удаляем старые имена, если они сохранились с предыдущих шагов 
    unlink("server");

    memset(&addr, 0, sizeof(addr));

    // получаем фаловый дескриптор сокета
    int server_fd = socket(AF_LOCAL, SOCK_STREAM, DEFAULT_PROTOCOL);

    if (server_fd == SOCKET_ERROR) {
        perror("couldn't get socket");
        return ERROR;
    }

    // Свяжем сокет с известным локальным адресом
    addr.sun_family = AF_LOCAL;
    strcpy(addr.sun_path, "server");

    // назначим локальное имя сокету сервера, чтобы клиенты знали, куда посылать пакеты
    int bind_result = bind(server_fd, (const struct sockaddr *) &addr, sizeof(addr));
    if (bind_result == BIND_ERROR) {
        perror("couldn't bind");
        return ERROR;
    }

    // сделаем этот сокет слушащим не более 1 сообщения в очереди
    int listen_result = listen(server_fd, MAX_LEN);
    if (listen_result == LISTEN_ERROR) {
        perror("couldn't listen");
        return ERROR;
    }

    // получаем запрос от клиента, игнорируя информацию о клиенте
    int client_fd = accept(server_fd, NULL, NULL);
    if (client_fd == ACCEPT_ERROR) {
        perror("couldn't accept");
        return ERROR;
    }

    // теперь мы знаем файловый дескриптор клиента, а больше клиентов у нас не будет,
    // значит можно закрыть сокет сервера
    int close_result = close(server_fd);
    if (close_result == CLOSE_ERROR) {
        perror("couldn't close");
        return ERROR;
    }

    // получаем сообщения и переводим их в верхний регистр
    int read_result = to_upper_case(client_fd);
    if (read_result == ERROR) {
        close(client_fd); 
        return ERROR;
    }
    
    // закрываем сокет клиента
    close_result = close(client_fd); 
    if (close_result == CLOSE_ERROR) {
        perror("couldn'tclose");
        return ERROR;
    }

    // отвязываем сокет от локального адреса
    unlink("server");
    return SUCCESS;
}
