#include <stdio.h>

typedef struct Contact
{
    char *firstname;
    char *surname;
    char *workplace;
    char *phone_number;
    char *email;
    char *socials_link;
    char *messenger_page_link;
} Contact;

typedef struct MessengerProfile
{
    char *link;
} MessengerProfile;

int main()
{
    printf("hello");
    return 0;
}