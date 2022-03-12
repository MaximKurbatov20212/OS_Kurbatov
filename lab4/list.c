#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

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

Node* add_node(Node* head, Node* node) {

    Node* copy_head = head; 
    while(copy_head->next != NULL) {
        copy_head = copy_head->next;    
    }
    copy_head->next = node;
    return node;
}

Node* insert(Node** head, char* data) {
    Node* node = create_node(data);
    if(node == NULL) return NULL;

    if(*head == NULL) {
        *head = node;
        return node;
    }

    add_node(*head, node);
    return node;
}

void free_list(Node* head) {
    while(head != NULL) {
        Node* current_node = head;
        head = head->next;    
        free(current_node->data);
        free(current_node);
    }
}

void print_list(Node* head) {
    while(head != NULL) {
        printf("%s", head->data);
        head = head->next;    
    }
}