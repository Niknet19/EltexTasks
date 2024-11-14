#include <string.h>

#include "avltree.h"

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

typedef struct ContactList {
    AvlTree* root;
    // Contact contacts[MAX_CONTACT_SIZE];
    size_t size;
} ContactList;

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
    if (list->root == NULL) {
        printf("Nothing to delete. Contact not found.\n");
    }
    AvlTree* root = delete_node(list->root, concat(firstname, surname), contact_compare_with_str);
    list->root = root;
    list->size--;
}

void update_contact_list(ContactList* list, const char* firstname, const char* surname, const char* new_workplace,
                         const char* new_phone_number, const char* new_email, const char* new_socials_link,
                         const char* new_messenger_page_link) {
    if (list->root == NULL || firstname == NULL || surname == NULL) {
        printf("Invalid arguments.\n");
        return;
    }

    Contact* contact = (search_node(list->root, concat(firstname, surname), contact_compare_with_str))->value;
    if (contact == NULL) {
        printf("Nothing to update. No such contact in list.\n");
    } else {
        if (new_workplace != NULL) {
            strncpy(contact->workplace, new_workplace, sizeof(contact->workplace));
            // list->contacts[i].workplace[sizeof(list->contacts[i].workplace) - 1] = '\0';
        }
        if (new_phone_number != NULL) {
            strncpy(contact->phone_number, new_phone_number, sizeof(contact->phone_number));
            // list->contacts[i].phone_number[sizeof(list->contacts[i].phone_number) - 1] = '\0';
        }
        if (new_email != NULL) {
            strncpy(contact->email, new_email, sizeof(contact->email));
            // list->contacts[i].email[sizeof(list->contacts[i].email) - 1] = '\0';
        }
        if (new_socials_link != NULL) {
            strncpy(contact->socials_link, new_socials_link, sizeof(contact->socials_link));
            // list->contacts[i].socials_link[sizeof(list->contacts[i].socials_link) - 1] = '\0';
        }
        if (new_messenger_page_link != NULL) {
            strncpy(contact->messenger_page_link, new_messenger_page_link, sizeof(contact->messenger_page_link));
            // list->contacts[i].messenger_page_link[sizeof(list->contacts[i].messenger_page_link) - 1] = '\0';
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
    list->root = insert_node(list->root, newContact, contact_compare);
    list->size++;
    // list->contacts[list->size++] = newContact;
}

void print_contact(Contact* contact) {
    if (contact == NULL) {
        printf("Contact is NULL.\n");
        return;
    }

    // printf("First Name: %s\n", contact->firstname);
    printf("Surname: %s\n", contact->surname);
    // printf("Workplace: %s\n", contact->workplace);
    // printf("Phone Number: %s\n", contact->phone_number);
    // printf("Email: %s\n", contact->email);
    // printf("Social Link: %s\n", contact->socials_link);
    // printf("Messenger Page Link: %s\n", contact->messenger_page_link);
    // printf("\n\n");
}

void print_contact_list(ContactList* list) {
    printf("\nContact List Information:\n");
    // for (size_t i = 0; i < list->size; i++) {
    //  print_contact(&(list->contacts[i]));
    //}

    print_tree(list->root, 0, print_contact);
}

int main() {
    ContactList list;
    list.root = NULL;
    list.size = 0;
    add_contact(&list, "Никита", "Иванов", "ОАО Газпром", "+79143248677", "example1@mail.ru", "www.social.com/link_id",
                "telegram@example12");
    add_contact(&list, "Игорь", "Петров", "ОАО Газпром", "+7312258677", "example2@mail.ru", "www.social.com/link_id2",
                "telegram@example13");
    add_contact(&list, "Максим", "Абазов", "НГТУ", "+7312253341", "example3@mail.ru", "www.social.com/link_id3",
                "telegram@example14");
    add_contact(&list, "Максим", "ЗЗЗЗЗЗ", "НГТУ", "+7312253341", "example3@mail.ru", "www.social.com/link_id3",
                "telegram@example14");
    print_contact_list(&list);

    // update_contact_list(&list, "Никита", "Иванов", "ООО Олимп", "+79143248455", NULL, NULL, NULL);
    // print_contact_list(&list);
    delete_contact(&list, "Никита", "Иванов");
    // delete_contact(&list, "Игорь", "Петров");
    // delete_contact(&list, "Максим", "Абазов");
    print_contact_list(&list);
    // printf("%ld\n", list.size);
    return 0;
}