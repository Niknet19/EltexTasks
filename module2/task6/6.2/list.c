#include "list.h"

Node* create_node(void* new_data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = new_data;
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;
}

Node* insert_to_list(Node* head, void* new_data, compare comparator) {
    Node* new_node = create_node(new_data);
    if (head == NULL) {
        head = new_node;
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

    // Поиск места для вставки
    Node* tmp = head;
    while (tmp->next != NULL && comparator(tmp->next->data, new_data) <= 0) {
        tmp = tmp->next;
    }

    // Вставка в середину и в конец
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
    printf("HELLO\n");
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
