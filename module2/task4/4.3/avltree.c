#include "avltree.h"

char* concat(char* s1, char* s2) {
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);

    char* result = malloc(len1 + len2 + 1);

    if (!result) {
        fprintf(stderr, "malloc() failed: insufficient memory!\n");
        return NULL;
    }

    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1);

    return result;
}

AvlTree* node_constructor(void* init_value) {
    AvlTree* p = (AvlTree*)malloc(sizeof(AvlTree));
    p->value = init_value;
    p->height = 1;
    p->left = NULL;
    p->right = NULL;
    return p;
}

void node_destructor(AvlTree* p) {
    if (p->left) {
        node_destructor(p->left);
    }
    if (p->right) {
        node_destructor(p->right);
    }
    free(p);
}
unsigned char height(AvlTree* p) {
    if (p == NULL) {
        return 0;
    } else {
        return p->height;
    }
}

unsigned char balance_check(AvlTree* p) { return height(p->right) - height(p->left); }

void fix_height(AvlTree* p) {
    unsigned char leftheight = height(p->left);
    unsigned char rightheight = height(p->right);
    if (rightheight > leftheight) {
        p->height = ++rightheight;
    } else {
        p->height = ++leftheight;
    }
}

AvlTree* rightRotation(AvlTree* p) {
    AvlTree* LeftNode = p->left;
    p->left = LeftNode->right;
    LeftNode->right = p;
    fix_height(p);
    fix_height(LeftNode);
    return LeftNode;
}

AvlTree* leftRotation(AvlTree* p) {
    AvlTree* RightNode = p->right;
    p->right = RightNode->left;
    RightNode->left = p;
    fix_height(p);
    fix_height(RightNode);
    return RightNode;
}

AvlTree* make_balance(AvlTree* p) {
    fix_height(p);
    unsigned char bFactor = balance_check(p);
    if (bFactor == 2) {
        if (balance_check(p->right) < 0) {
            p->right = rightRotation(p->right);
        }
        return leftRotation(p);
    } else {
        if (bFactor == -2) {
            if (balance_check(p->left) > 0) {
                p->left = leftRotation(p->left);
            }
            return rightRotation(p);
        }
    }
    return p;
}

AvlTree* insert_node(AvlTree* p, void* new_value, int (*compare)(void*, void*)) {
    if (p == NULL) return node_constructor(new_value);
    if (compare(new_value, p->value) == 0) {
        p->left = insert_node(p->left, new_value, compare);
    } else {
        p->right = insert_node(p->right, new_value, compare);
    }
    return make_balance(p);
}

AvlTree* search_node(AvlTree* p, void* value, int (*compare)(void*, void*)) {
    if (p == NULL || compare(p->value, value) == 0) {
        return p;
    }
    if (compare(value, p->value) < 0) {
        return search_node(p->left, value, compare);
    } else {
        return search_node(p->right, value, compare);
    }
}

AvlTree* remove_min(AvlTree* p)  // удаление узла с минимальным ключом из дерева p
{
    if (p->left == 0) return p->right;
    p->left = remove_min(p->left);
    return make_balance(p);
}

// Удаление узла
AvlTree* delete_node(AvlTree* p, void* value, int (*compare)(void*, void*)) {
    if (p == NULL) return NULL;  // Узел не найден
    if (compare(p->value, value) < 0) {
        p->left = delete_node(p->left, value, compare);
    } else if (compare(p->value, value) > 0) {
        p->right = delete_node(p->right, value, compare);
    } else {
        // Узел найден
        AvlTree* left = p->left;
        AvlTree* right = p->right;
        free(p->value);
        free(p);  // Освобождаем память

        if (right == NULL) return left;  // Если нет правого поддерева
        AvlTree* min = right;
        while (min->left != NULL) {  // Находим минимальный ключ в правом поддереве
            min = min->left;
        }
        min->right = remove_min(right);
        // min->right = delete_node(right, min->value, compare);  // Удаляем узел
        min->left = left;
        return make_balance(min);
    }
    return make_balance(p);  // Балансируем дерево после удаления
}

void print_tree(AvlTree* root, int space, void (*print_node)(void*)) {
    int COUNT = 5;  // Количество пробелов между уровнями
    if (root == NULL) return;

    // Увеличиваем расстояние между уровнями
    space += COUNT;

    // Печатаем правое поддерево
    print_tree(root->right, space, print_node);

    // Печатаем текущий узел после отступов
    printf("\n");
    for (int i = COUNT; i < space; i++) printf(" ");
    print_node(root->value);
    // printf("%d\n", root->value);

    // Печатаем левое поддерево
    print_tree(root->left, space, print_node);
}

void printVerticalWithLinks(AvlTree* root, int level, int isLeft) {
    if (root == NULL) return;

    // Печатаем правое поддерево (оно будет выше текущего узла)
    printVerticalWithLinks(root->right, level + 1, 0);

    // Печать отступов для текущего узла
    for (int i = 0; i < level; i++) printf("        ");

    // Печать связи с родительским узлом
    if (level > 0) {
        if (isLeft) {
            printf("└── ");
        } else {
            printf("┌── ");
        }
    }

    // Печать значения узла
    printf("%d\n", root->value);

    // Печатаем левое поддерево (оно будет ниже текущего узла)
    printVerticalWithLinks(root->left, level + 1, 1);
}

typedef struct Contact {
    char name[50];
    int age;
} Contact;

int contact_compare(Contact* c1, Contact* c2) { return strcmp(c1->name, c2->name); }
int int_compare(int i1, int i2) { return i1 > i2; }
int contact_compare_with_str(const char* str, Contact* c2) { return strcmp(str, c2->name); }

int main() {
    // Contact contact1, contact2, contact3;
    // strncpy(contact1.name, "aaaa", sizeof("aaaa"));
    // contact1.age = 20;
    // strncpy(contact2.name, "bbbb", sizeof("bbbb"));
    // contact2.age = 33;
    // strncpy(contact3.name, "cccc", sizeof("cccc"));
    // contact3.age = 44;
    AvlTree* head = NULL;
    head = insert_node(head, 1, int_compare);
    head = insert_node(head, 2, int_compare);
    head = insert_node(head, 3, int_compare);
    head = insert_node(head, 4, int_compare);
    head = insert_node(head, 5, int_compare);
    head = insert_node(head, 6, int_compare);
    head = insert_node(head, 7, int_compare);
    head = insert_node(head, 8, int_compare);
    head = insert_node(head, 9, int_compare);
    head = insert_node(head, 9, int_compare);
    head = insert_node(head, 9, int_compare);
    head = insert_node(head, 9, int_compare);
    head = insert_node(head, 11, int_compare);
    head = insert_node(head, 15, int_compare);
    head = insert_node(head, 20, int_compare);
    printVerticalWithLinks(head, 0, 0);
    head = delete_node(head, 8, int_compare);
    //  head = delete_node(head, 3, int_compare);
    //      AvlTree* node1 = search_node(head, 2, int_compare);
    //      AvlTree* node2 = search_node(head, 3, int_compare);
    //       printf("%d", ((Contact*)(node1->value))->age);
    //      printf("%d", node1->value);
    //      if (node2 == NULL) printf("\nNo such node!");
    //       printf("%d", ((Contact*)(node2->value))->age);
    printf("\n\n");
    printVerticalWithLinks(head, 0, 0);
}
