#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct AvlTree {
    void* value;
    unsigned char height;
    struct AvlTree* left;
    struct AvlTree* right;
} AvlTree;

typedef int (*compare)(void*, void*);

char* concat(char* s1, char* s2);

AvlTree* node_constructor(void* init_value);

void node_destructor(AvlTree* p);

unsigned char height(AvlTree* p);

unsigned char balance_check(AvlTree* p);

void fix_height(AvlTree* p);

AvlTree* rightRotation(AvlTree* p);

AvlTree* leftRotation(AvlTree* p);

AvlTree* make_balance(AvlTree* p);

AvlTree* insert_node(AvlTree* p, void* new_value, compare comparator);

AvlTree* search_node(AvlTree* p, void* value, compare comparator);

AvlTree* remove_min(AvlTree* p);

AvlTree* delete_node(AvlTree* p, void* value, compare comparator);

void print_tree(AvlTree* p, int space, void (*print_node)(void*));

void printVerticalWithLinks(AvlTree* root, int level, int isLeft);
