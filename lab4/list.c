#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node* create_node(char* data) {
    Node* node = (Node*)malloc(sizeof(Node));
    if(node == NULL) {
        fprintf(stderr, "Couldn't allocate memory");
        return NULL;
    }  

    size_t len = strlen(data);
    node->data = (char*)malloc(len);

    if(node->data == NULL) {
        fprintf(stderr, "Couldn't allocate memory");
        free(node);
        return NULL;
    }

    strcpy(node->data, data);
    node->next = NULL;
    return node;
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

Node* insert(Node** root, char* data) {
    Node* node = create_node(data);
    if(node == NULL) return NULL;

    if(*root == NULL) {
        *root = node;
        return node;
    }

    add_node(*root, node);
    return *root;
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
