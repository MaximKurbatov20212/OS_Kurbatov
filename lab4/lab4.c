#include <stdio.h>
#include <string.h>
#include "list.h"
#define EXIT_SYMBOL '.'
#define ERROR_LENGHT -1

char* scan_info() {
    char* data = NULL;
    size_t len = 0;

    ssize_t string_len = getline(&data, &len, stdin);

    // Назвал переменную нормально и задефаинил номер ошибки как ERROR_LENGHT
    if(string_len == ERROR_LENGHT) {
        fprintf(stderr, "Couldn't allocate memory");
        return NULL;
    }
    return data;
}

char is_stopstring(char* data) {
    return data[0] == EXIT_SYMBOL;
}

int main() {
    Node* head_of_list = NULL;
    char* data = NULL;

    // переписал цикл, раньше делал первое считвание до цикла
    // Убрал data[0] != '.' и добавил функцию для определения этот + задефаинил сам стопсимвол
    while((data = scan_info()) != NULL && !is_stopstring(data)) {

        // Убрал переменную ret_ptr, тк не было понятно, какой она несет смысл
        // Возвращаю указатель на новый узел, если NULL - то ошибка аллокации
        Node* new_node = insert(&head_of_list, data);
        if(new_node == NULL) break;
    }

    print_list(head_of_list);
    free_list(head_of_list);
    return 0;
}

