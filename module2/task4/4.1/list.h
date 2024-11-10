#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    void* data;
    struct Node* prev;
    struct Node* next;
} Node, List;

typedef int (*compare)(void*, void*);

Node* create_node(void* new_data);

void* find_node(Node* head, void* data, compare comparator);

Node* insert_to_list(Node* head, Node** tail, void* new_data, compare comparator);

Node* delete_from_list(Node* head, void* data, compare comparator);

void print_list(Node* head, void (*print_node)(void*));

char* concat(char* s1, char* s2);