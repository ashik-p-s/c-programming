
#include "file.h"

void saveContactsToFile(Addressbook *Book)
{
    FILE *fp;
    fp = fopen("Contacts.txt","w");
    if(fp == NULL)
    {
        printf("Error : Could not open file to save contacts.\n");
        return;
    }
    fprintf(fp , "%d\n",Book -> contactcount);// to write total contactcount at top
    
    for(int i = 0;i < Book->contactcount;i++)
    {
        fprintf(fp , "%s,%s,%s\n" , Book->contacts[i].name,
                                    Book->contacts[i].number,
                                    Book->contacts[i].email);
    }

    fclose(fp);

}


void loadContactsFromFile(Addressbook *Book)
{
    FILE *fp;
    fp = fopen("Contacts.txt","r");
    if(fp == NULL)
    {
        Book -> contactcount = 0;
        return;
    }

    if(fscanf(fp , "%d\n",&Book -> contactcount) != 1)// to write total contactcount at top
    {
        Book -> contactcount = 0;
        fclose(fp);
        return;
    }

    for(int i = 0;i < Book->contactcount;i++)
    {
        fscanf(fp , "%[^,],%[^,],%[^\n]\n" , Book->contacts[i].name,
                                    Book->contacts[i].number,
                                    Book->contacts[i].email); 
    }
    fclose(fp);
    
}