#ifndef LIST
#define LIST

typedef struct List {
    char* data;
    struct List* next;
} Node;

Node* create_node(char* data);
Node* add_node(Node* root, Node* node);
Node* insert(Node** root, char* data);
void free_list(Node* list);
void print_list(Node* list);

#endif 
