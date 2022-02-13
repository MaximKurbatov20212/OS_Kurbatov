#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct List {
    char* data;
    struct List* next;
} Node;

Node* insert(Node* root, char* data) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;

    if(root == NULL) return node;

    Node* list = root; 
    while(list->next != NULL) {
        list = list->next;    
    }
    list->next = node;
    return root;
}

void free_list(Node* list) {
    while(list != NULL) {
        Node* tmp = list;
        list = list->next;    
        free(tmp);
    }
}

void print_list(Node* list) {
    while(list != NULL) {
        printf("%s ", list->data);
        list = list->next;    
    }
}