#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR 1
#define SUCCESS 0

typedef struct List {
    char* data;
    struct List* next;
} Node;

void create_node(char* data, Node* node, int* err) {
    size_t len = strlen(data);
    node->data = (char*)malloc(len);

    if(node->data == NULL) {
        fprintf(stderr, "Couldn't allocate memory");
        free(node);
        *err = ERROR;
        return;
    }

    for(int i = 0; i < len; i++) node->data[i] = data[i];
    node->next = NULL;
}

Node* add_node(Node* root, Node* node) {
    if(root == NULL) return node;

    Node* list = root; 
    while(list->next != NULL) {
        list = list->next;    
    }
    list->next = node;
    return root;
}

Node* insert(Node* root, char* data, int* err) {
    Node* node = (Node*)malloc(sizeof(Node));
    if(node == NULL) {
        fprintf(stderr, "Couldn't allocate memory");
        *err = ERROR;
        return root;
    }  

    create_node(data, node, err);

    if(*err == ERROR) return root;

    return add_node(root, node);
}

void free_list(Node* list) {
    while(list != NULL) {
        Node* tmp = list;
        list = list->next;    
        free(tmp->data);
        free(tmp);
    }
}

void print_list(Node* list) {
    while(list != NULL) {
        printf("%s", list->data);
        list = list->next;    
    }
}
