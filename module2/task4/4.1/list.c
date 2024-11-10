#include "list.h"

Node* create_node(void* new_data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = new_data;
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;
}

Node* insert_to_list(Node* head, Node** tail, void* new_data, compare comparator) {
    Node* new_node = create_node(new_data);
    if (head == NULL) {
        head = new_node;
        *tail = new_node;
        return head;
    }
    // Вставка в начало
    // if (head == NULL || head->data >= new_data) {
    if (comparator(head->data, new_data) >= 0) {
        new_node->next = head;
        if (head != NULL) {
            head->prev = new_node;
        }
        return new_node;
    }
    //вставка в конец
    if (comparator((*tail)->data, new_data) >= 0) {
        new_node->prev = tail;
        new_node->next = NULL;
        *tail = new_node;
    }

    // Поиск места для вставки
    Node* tmp = head;
    while (tmp->next != NULL && comparator(tmp->next->data, new_data) <= 0) {
        tmp = tmp->next;
    }

    // Вставка в середину
    new_node->next = tmp->next;
    if (tmp->next != NULL) {
        tmp->next->prev = new_node;
    }
    tmp->next = new_node;
    new_node->prev = tmp;

    return head;
}

Node* delete_from_list(Node* head, void* data_to_delete, compare comparator) {
    // Вставка в начало
    while (head != NULL && comparator(head->data, data_to_delete) == 0) {
        Node* tmp = head;
        if (head->next != NULL) {
            head = head->next;
            head->prev = NULL;
            free(tmp->data);
            free(tmp);
        } else {
            free(tmp->data);
            free(tmp);
            head = NULL;
        }
        // return head;
    }

    Node* node = head;
    while (node != NULL) {
        if (comparator(node->data, data_to_delete) == 0) {
            Node* tmp = node;
            node->prev->next = node->next;
            if (node->next != NULL) {
                node->next->prev = node->prev;
                node = node->next;
                free(tmp->data);
                free(tmp);
                continue;
            }
            free(tmp->data);
            free(tmp);
        }
        node = node->next;
    }
    // free(data_to_delete);
    return head;
}

void print_list(Node* head, void (*print_node)(void*)) {
    if (head == NULL) printf("List is empty!\n");
    Node* tmp = head;
    while (tmp != NULL) {
        // printf("%d  ", tmp->data);
        print_node(tmp->data);
        tmp = tmp->next;
    }
    printf("\n");
}

void* find_node(Node* head, void* data, compare comparator) {
    if (head == NULL) printf("List is empty!\n");
    Node* tmp = head;
    while (tmp != NULL) {
        if (comparator(tmp->data, data) == 0) {
            return tmp->data;
        }
        tmp = tmp->next;
    }
    return NULL;
}

// int person_compare_with_name(Person* p, char* str) { return strcmp(concat(p->name, p->surname), str); }

// int person_compare(Person* p1, Person* p2) { return strcmp(p1->name, p2->name); }

// void print_node(Person* p1) { printf("\n%s\n", p1->name); }

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

// int main() {
//     Person p1, p2, p3, p4, p5, p6;
//     strcpy(p1.name, "bbbb");
//     strcpy(p2.name, "aaaa");
//     strcpy(p3.name, "cccc");
//     strcpy(p4.name, "jjjj");
//     strcpy(p5.name, "dddd");
//     strcpy(p6.name, "aaaa");

//     strcpy(p1.surname, "bbbbb");
//     strcpy(p2.surname, "aaaaa");
//     strcpy(p3.surname, "ccccc");
//     strcpy(p4.surname, "jjjjj");
//     strcpy(p5.surname, "ddddd");
//     strcpy(p6.surname, "aaaaa");

//     p1.age = 33;
//     p2.age = 54;
//     p3.age = 23;
//     p4.age = 26;
//     p5.age = 17;
//     p6.age = 54;
//     Node* head = create_node(&p1);
//     head = insert_to_list(head, &p2, person_compare);
//     head = insert_to_list(head, &p3, person_compare);
//     head = insert_to_list(head, &p4, person_compare);
//     head = insert_to_list(head, &p5, person_compare);
//     head = insert_to_list(head, &p6, person_compare);
//     print_list(head, print_node);
//     Person* p_ptr = find_node(head, concat("cccc", "ccccc"), person_compare_with_name);
//     printf("%d", p_ptr->age);
//     return 0;
// }
