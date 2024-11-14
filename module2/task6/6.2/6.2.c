#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "list.h"

#define MAX_CONTACT_SIZE 100

typedef struct Contact {
    char firstname[50];
    char surname[50];
    char workplace[50];
    char phone_number[20];
    char email[50];
    char socials_link[100];
    char messenger_page_link[100];
} Contact;

typedef struct Node {
    void* data;
    struct Node* prev;
    struct Node* next;
} Node, List;

typedef struct ContactList {
    Node* head;
    size_t size;
} ContactList;

typedef int (*compare)(void*, void*);
typedef Node* (*create_node_func)(void*);
typedef Node* (*insert_to_list_func)(Node*, void*, compare);
typedef Node* (*delete_from_list_func)(Node*, void*, compare);
typedef void (*print_list_func)(Node*, void (*)(void*));
typedef void* (*find_node_func)(Node*, void*, compare);
typedef char* (*concat_func)(char*, char*);

create_node_func create_node;
insert_to_list_func insert_to_list;
delete_from_list_func delete_from_list;
print_list_func print_list;
find_node_func find_node;
concat_func concat;

int contact_compare(Contact* c1, Contact* c2) { return strcmp(c1->surname, c2->surname); }
int contact_compare_with_str(Contact* c1, char* str2) {
    char* str1 = concat(c1->firstname, c1->surname);
    int res = strcmp(str1, str2);
    free(str1);
    return res;
}
int contact_compare_fullname(Contact* c1, Contact* c2) {
    char* str1 = concat(c1->surname, c1->firstname);
    char* str2 = concat(c2->surname, c2->firstname);
    int res = strcmp(str1, str2);
    free(str1);
    free(str2);
    return res;
}

void delete_contact(ContactList* list, const char* firstname, const char* surname) {
    if (list == NULL || firstname == NULL || surname == NULL || list->size <= 0) {
        printf("Invalid arguments.\n");
        return;
    }
    if (list->head == NULL) {
        printf("Nothing to delete. Contact not found.\n");
    }
    Node* head = delete_from_list(list->head, concat(firstname, surname), (compare)contact_compare_with_str);
    list->head = head;
    list->size--;
}

void update_contact_list(ContactList* list, const char* firstname, const char* surname, const char* new_workplace,
                         const char* new_phone_number, const char* new_email, const char* new_socials_link,
                         const char* new_messenger_page_link) {
    if (list->head == NULL || firstname == NULL || surname == NULL) {
        printf("Invalid arguments.\n");
        return;
    }
    printf("HERE\n");
    Contact* contact = find_node(list->head, concat(firstname, surname), (compare)contact_compare_with_str);
    if (contact == NULL) {
        printf("Nothing to update. No such contact in list.\n");
    } else {
        if (new_workplace != NULL) {
            strncpy(contact->workplace, new_workplace, sizeof(contact->workplace));
        }
        if (new_phone_number != NULL) {
            strncpy(contact->phone_number, new_phone_number, sizeof(contact->phone_number));
        }
        if (new_email != NULL) {
            strncpy(contact->email, new_email, sizeof(contact->email));
        }
        if (new_socials_link != NULL) {
            strncpy(contact->socials_link, new_socials_link, sizeof(contact->socials_link));
        }
        if (new_messenger_page_link != NULL) {
            strncpy(contact->messenger_page_link, new_messenger_page_link, sizeof(contact->messenger_page_link));
        }
        printf("Contact updated successfully.\n");
    }
}

void add_contact(ContactList* list, const char* firstname, const char* surname, const char* workplace,
                 const char* phone_number, const char* email, const char* socials_link,
                 const char* messenger_page_link) {
    if (list->size > MAX_CONTACT_SIZE) {
        printf("Contact list is full.");
        return;
    }
    Contact* newContact = malloc(sizeof(Contact));
    strncpy(newContact->firstname, firstname, sizeof(newContact->firstname) - 1);
    strncpy(newContact->surname, surname, sizeof(newContact->surname) - 1);
    strncpy(newContact->workplace, workplace, sizeof(newContact->workplace) - 1);
    strncpy(newContact->phone_number, phone_number, sizeof(newContact->phone_number) - 1);
    strncpy(newContact->email, email, sizeof(newContact->email) - 1);
    strncpy(newContact->socials_link, socials_link, sizeof(newContact->socials_link) - 1);
    strncpy(newContact->messenger_page_link, messenger_page_link, sizeof(newContact->messenger_page_link) - 1);
    list->head = insert_to_list(list->head, newContact, (compare)contact_compare);
    list->size++;
}

void print_contact(Contact* contact) {
    if (contact == NULL) {
        printf("Contact is NULL.\n");
        return;
    }

    printf("First Name: %s\n", contact->firstname);
    printf("Surname: %s\n", contact->surname);
    printf("Workplace: %s\n", contact->workplace);
    printf("Phone Number: %s\n", contact->phone_number);
    printf("Email: %s\n", contact->email);
    printf("Social Link: %s\n", contact->socials_link);
    printf("Messenger Page Link: %s\n", contact->messenger_page_link);
    printf("\n\n");
}

void print_contact_list(ContactList* list) {
    printf("\nContact List Information:\n");
    print_list(list->head, (void (*)(void*))print_contact);
}

int main() {
    // Загружаем библиотеку и символы
    void* handle = dlopen("./liblist.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Failed to load library: %s\n", dlerror());
        return 1;
    }

    create_node = dlsym(handle, "create_node");
    insert_to_list = dlsym(handle, "insert_to_list");
    delete_from_list = dlsym(handle, "delete_from_list");
    print_list = dlsym(handle, "print_list");
    find_node = dlsym(handle, "find_node");
    concat = dlsym(handle, "concat");

    if (!create_node || !insert_to_list || !delete_from_list || !print_list || !find_node || !concat) {
        fprintf(stderr, "Failed to load functions: %s\n", dlerror());
        dlclose(handle);
        return 1;
    }

    // Основная программа
    ContactList list;
    list.head = NULL;
    list.size = 0;

    add_contact(&list, "Никита", "Иванов", "ОАО Газпром", "+79143248677", "example1@mail.ru", "www.social.com/link_id",
                "telegram@example12");
    add_contact(&list, "Игорь", "Петров", "ОАО Газпром", "+7312258677", "example2@mail.ru", "www.social.com/link_id2",
                "telegram@example13");
    add_contact(&list, "Максим", "Абазов", "НГТУ", "+7312253341", "example3@mail.ru", "www.social.com/link_id3",
                "telegram@example14");

    print_contact_list(&list);

    update_contact_list(&list, "Никита", "Иванов", "ООО Олимп", "+79143248455", NULL, NULL, NULL);

    print_contact_list(&list);

    delete_contact(&list, "Никита", "Иванов");
    delete_contact(&list, "Игорь", "Петров");
    delete_contact(&list, "Максим", "Абазов");

    print_contact_list(&list);

    dlclose(handle);
    return 0;
}
