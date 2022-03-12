#ifndef LIST
#define LIST

// Здесь исправил название структуры и сделал указатель на следующий узел(раньше был указатель на list)
typedef struct Node {
    char* data;
    struct Node* next;
} Node;

Node* create_node(char* data);
Node* add_node(Node* head, struct Node* node);
Node* insert(Node** head, char* data);
void free_list(Node* head);
void print_list(Node* head);

#endif 
