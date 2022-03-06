#include <stdio.h>
#include <string.h>
#include "list.h"

char* scan_info() {
    char* data = NULL;
    size_t len = 0;

    ssize_t l = getline(&data, &len, stdin);
    if(l == -1) {
        fprintf(stderr, "Couldn't allocate memory");
        return NULL;   
    }
    return data;
}

int main() {
    Node* list = NULL;
    char* data = scan_info();

    if(data == NULL) {
        free_list(list);
        return 0;
    }

    while(data[0] != '.') {
        Node* ret_ptr = insert(&list, data);
        if(ret_ptr == NULL) break;
        data = scan_info();

        if(data == NULL) break;
    }

    print_list(list);
    free_list(list);
    return 0;
}
