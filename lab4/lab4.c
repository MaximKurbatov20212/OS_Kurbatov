#include "list.h"
#define MAX_BUF 80 

char* scan_info() {
    char* data = malloc(MAX_BUF * sizeof(char));
    fgets(data, MAX_BUF, stdin);
    data[strlen(data) - 1] = 0;
    return data;
}

int main() {
    printf("Type info until . : \n");
    Node* list = NULL;
    char* data = scan_info();

    while(data[0] != '.') {
        list = insert(list, data);
        data = scan_info();
    }
    print_list(list);
    free_list(list);
    return 0;
}