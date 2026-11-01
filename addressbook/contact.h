#ifndef CONTACT_H
#define CONTACT_H


typedef struct Contact
{
    char name[50];
    char number[15];
    char email[50];
}Contact;

typedef struct Addressbook
{
    Contact contacts[100];
    int contactcount;

}Addressbook;

void create_contact(Addressbook *);
void search_contact(Addressbook *);
void edit_contact(Addressbook *);
void delete_contact(Addressbook *);
void list_contact(Addressbook *);
#endif