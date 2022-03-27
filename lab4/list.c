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

    // Я так понял, мы никогда в си не можем безопасно копировать строки, тк
    // даже символ конца строки можно затереть и мы не узнаем, что строка закончилась.
    // А еще я случайно наткнулся на такое странное поведение:
    // 
    // char* a = (char*)malloc(1 * sizeof(char));
    // a[0] = '0';
    // a[1] = '1';
    // a[2] = '2';
    // a[3] = '3';
    // a[4] = '4';
    // a[5] = '5';
    // printf("%s", a);
    // Вывод: 012345, хотя памяти выделено недостаточно. Так что мы можем даже не понять, что 
    // большая строка копируется в меньшую.

    // Убрал strcpy, размер data никак не больше node->data
    for(int i = 0; i < len; i++) {
        node->data[i] = data[i];
    }

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