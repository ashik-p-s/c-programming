#include<stdio.h>
#include "contact.h"
#include "file.h"

int main()
{
    int choice;
    Addressbook book;
    book.contactcount = 0;
    loadContactsFromFile(&book); 

    do 
    {
        printf("\n============================================================================\n");
        printf("%47s\n", "ADDRESS BOOK MENU");
        printf("============================================================================\n");
        printf("1. Create contact\n");
        printf("2. Search contact\n");
        printf("3. Edit contact\n");
        printf("4. Delete contact\n");
        printf("5. List all contacts\n");
    	printf("6. Save contacts\n");		
        printf("7. Exit\n");
        printf("\n============================================================================\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) 
        {
                choice = 0; // 2. Force choice to 0 so it guarantees it hits the default case!
        }

        switch(choice)
        {
            case 1 : 
                    create_contact(&book);
                    break;

            case 2 : 
                    search_contact(&book);
                    break;

            case 3 : 
                    edit_contact(&book);
                    break;

            case 4 : 
                    delete_contact(&book);
                    break;

            case 5 : 
                    list_contact(&book);
                    break;
            
            case 6 : 
                    printf("Saving...\n");
                    saveContactsToFile(&book);
                    break;

            case 7 :
                    break;

            default : 
                    printf("Invalid choice. Please try again.\n");
                    while(getchar() != '\n'); 
                    break;

        }
    }while(choice != 7);

    
    return 0;
}
