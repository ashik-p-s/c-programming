#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "contact.h"
#include "file.h"
#include<ctype.h>



void saveAndExit(Addressbook *addressBook) 
{
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}


void initialize(Addressbook *addressBook) {
    
    // Load contacts from file during initialization (After files)
    loadContactsFromFile(addressBook);
}


void create_contact(Addressbook *book)
{
    int i;
    char str[100];
    int flag;
    do
    {
        i = 0;
        flag = 1;


        printf("Enter Name : ");
        scanf(" %[^\n]",book -> contacts[book -> contactcount].name);
        strcpy(str , book -> contacts[book -> contactcount].name);

        while(str[i] != '\0')
        {
            if( !(isalpha(str[i]) || isspace(str[i]) || str[i] == '.') )
            {
                printf("Invalid input , Enter proper Name :\n");
                flag = 0;
                break;
            }
            i++;
        }

    }while(flag == 0);


    do 
    {
        i=0;
        flag = 1;
        printf("Enter the phone number : ");
        scanf(" %[^\n]",book -> contacts[book -> contactcount].number);
        strcpy(str , book -> contacts[book -> contactcount].number);

        while(str[i] != '\0')
        {
            if(str[i] < '0' || str[i] > '9')
            {
                flag = 0;
                break;
            }
            i++;
        }
        for (int j = 0; j < book->contactcount; j++) 
        {
            if (strcmp(str, book->contacts[j].number) == 0) 
            {
                flag = 0;
                printf("Number already exists! Please enter a unique number : \n");
                break;
            }
        }

        if(i != 10)
        {
            flag = 0;
            printf("Invalid number,try again !\n ");
    
        }
    }while(flag == 0);


    do 
    {
        i=0;
        flag = 1;
        printf("Enter the Email Address : ");
        scanf(" %s",book -> contacts[book -> contactcount].email);
        strcpy(str , book -> contacts[book -> contactcount].email);

        int at_count = 0;

        while(str[i] != '\0')
        {
            if(str[i] == '@')
            {
                at_count++;
            }
            if(str[i] == '@' && str[i+1] == '.')
            {
                flag = 0;
                printf("Invalid mail id format, enter the correct mail aadress : \n");
                break; 
            }
            

            if(!(islower(str[i]) || isdigit(str[i]) || str[i] == '@' || str[i] == '-' || str[i] == '_' || str[i] == '.'))
            {
                flag = 0;
                printf("Invalid mail id format, enter the correct mail aadress : \n");
                break;
            }
            i++;
        }

        if (flag == 0) continue;


        for (int j = 0; j < book->contactcount; j++) 
        {
            if (strcmp(str, book->contacts[j].email) == 0) 
            {
                flag = 0;
                printf("Email already exists! Please enter a unique email.\n");
                break;
            }
        }
        
        if (flag == 0) continue;

        if(strcmp(&str[strlen(str) - 4],".com") != 0 || at_count != 1 || str[0] == '@' || str[strlen(str) - 1] == '@')
        {
            flag = 0;
            printf("Invalid mail id format, enter the correct mail aadress : \n");
        }
        
    }while(flag == 0);
    
    book->contactcount++;
}



void list_contact(Addressbook *book)
{
    int i=0;

    if(book -> contactcount == 0)
    {
        printf("\n----------------------------------------------------\n");
        printf("  [!] Notice: Address book is empty. No contacts found.\n");
        printf("----------------------------------------------------\n");
        return;

    }

    // Top border line of the title frame
    printf("\n============================================================================\n");

    // Perfectly centered title
    printf("%-46s\n", "                            ADDRESS BOOK");

    // REMOVED \n: This line now sits right beneath the title closing the header box
    printf("============================================================================\n");

    // Column headers
    printf("%-6s %-20s %-15s %-30s\n", "S.NO", "NAME", "PHONE NUMBER", "EMAIL ADDRESS");

    // Bottom header border line
    printf("============================================================================\n");


    for(i=0;i<book -> contactcount;i++)
    {
        printf("%-6d %-20s %-15s %-35s\n",i+1 ,book -> contacts[i].name,
                                book -> contacts[i].number,
                                book -> contacts[i].email);
        
    }

    printf("\n============================================================================\n");

    printf("Total Contacts : %d\n",book -> contactcount);
    printf("============================================================================\n");

    return ;
}
    
    
void search_contact(Addressbook *book)
{
    int option;

    if(book -> contactcount == 0)
    {
        printf("No Contact available\n");
        return;
    }


        printf("How do you like to search : \n");
        printf("1. Name\n");
        printf("2. Phone number\n");
        printf("3. Email-id\n\n");
        printf("Enter your option : ");
        scanf("%d",&option);

        switch(option)
        {
            case 1 : 
                    {
                        int i=0;
                        int found = 0;
                        char str[20];
                        printf("Enter the name :");
                        scanf(" %[^\n]", str );

                        printf("\n============================================================================\n");
                        printf("%-6s %-20s %-15s %-30s\n", "S.NO", "NAME", "PHONE NUMBER", "EMAIL ADDRESS");
                        printf("============================================================================\n");

                        int count = 0;
                        for(i=0;i<book -> contactcount;i++)
                        {
                            if(strstr(book -> contacts[i].name , str ) != NULL)
                            {
                                found = 1;
                                printf("%-6d %-20s %-15s %-30s\n\n\n",count+1,book -> contacts[i].name,
                                book -> contacts[i].number,
                                book -> contacts[i].email);
                                count++;
                            }
                        }
                        if(found == 0)
                        {
                            printf("Name not found\n");
                        }
                        break;
                    }
            case 2: 
                    {
                        int i=0;
                        char str[20];
                        int found = 0;
                        printf("Enter the phone number : ");
                        scanf("%s",str);


                        printf("\n============================================================================\n");
                        printf("%-6s %-20s %-15s %-30s\n", "S.NO", "NAME", "PHONE NUMBER", "EMAIL ADDRESS");
                        printf("============================================================================\n");


                        for(i=0;i<book -> contactcount;i++)
                        {
                            if(strcmp(book -> contacts[i].number , str) == 0)// strcmp ==0 means string equal
                            {
                                found = 1;
                                printf("%-6d %-20s %-15s %-30s\n\n\n", i+1 ,book -> contacts[i].name,
                                book -> contacts[i].number,
                                book -> contacts[i].email);
                            }
                        }
                        if(found == 0)
                        {
                            printf("Number not found\n");
                        }
                        break;
                    }

            case 3 : 
                    {
                        int i=0;
                        char str[50];
                        int found = 0;
                        printf("Enter the email address : ");
                        scanf("%s",str);


                        printf("\n============================================================================\n");
                        printf("%-6s %-20s %-15s %-30s\n", "S.NO", "NAME", "PHONE NUMBER", "EMAIL ADDRESS");
                        printf("============================================================================\n");

                        for(i=0;i<book -> contactcount;i++)
                        {
                        if(strcmp(book -> contacts[i].email , str) == 0)
                            {
                                found =1;
                                printf("%-6d %-20s %-15s %-30s\n\n\n",i+1,book -> contacts[i].name,
                                book -> contacts[i].number,
                                book -> contacts[i].email);
                            }
                        }
                        if(found == 0)
                        {
                            printf("Email not found\n");
                        }
                        break;
                    }
            default :
                        printf("Invalid input\n");

        }
      
    } 
    
int find_index(Addressbook *book)
{
    int option;
    
    // to check if no contacts present means say that
    if(book -> contactcount == 0)
    {
        printf("No Contacts available\n");
        return -1;
    }

    printf("How do you like to search : \n");
    printf("1. Name\n");
    printf("2. Phone number\n");
    printf("3. Email-id\n");
    printf("Enter your option : ");
    scanf("%d",&option);

    switch(option)
        {
            case 1 : 
                {
                    char name[30];
                    int index[100];
                    int count = 0;
                    int i,choice;

                    printf("Enter the name :");
                    scanf(" %[^\n]",name);

                    printf("\n============================================================================\n");
                    printf("%-6s %-20s %-15s %-30s\n", "S.NO", "NAME", "PHONE NUMBER", "EMAIL ADDRESS");
                    printf("============================================================================\n");
                    for(i=0;i<book->contactcount;i++)
                    {
                        if(strstr(book->contacts[i].name,name)!=NULL)
                        {
                            printf("%-5d %-20s %-15s %-30s\n",
                                                            i + 1,
                                                            book->contacts[i].name,
                                                            book->contacts[i].number,
                                                            book->contacts[i].email);

                            index[count]=i;
                            count++;
                        }
                    }
                    if(count==0)
                    {
                        printf("Name not found\n");
                        return -1;
                    }

                    printf("Select Contact : ");
                    scanf("%d",&choice);

                    if(choice<1 || choice>count)
                    {
                        printf("Invalid Choice\n");
                        return -1;
                    }

                    return index[choice-1];
                }
            
            case 2 : 
                {
                    char new_phone[20];
                    int i;

                    printf("Enter Phone number : ");
                    scanf("%s",new_phone);

                    for(i=0;i<book->contactcount;i++)
                    {
                        if(strcmp(book->contacts[i].number,new_phone)==0)
                            return i;
                    }

                    printf("Phone Number not found\n");
                    return -1;
                }
            case 3:
                {
                    char email[50];
                    int i;

                    printf("Enter Email : ");
                    scanf("%s",email);

                    for(i=0;i<book->contactcount;i++)
                    {
                        if(strcmp(book->contacts[i].email,email)==0)
                            return i;
                    }

                    printf("Email not found\n");
                    return -1;
                }

            default:
                printf("Invalid Choice\n");
                return -1;
    }
}



void edit_contact(Addressbook *book)
{
    int choice;
    
    int actual_index;
    actual_index = find_index(book);
    if(actual_index == -1)
        return;
    
    
    printf("What do you want to edit : \n");
    printf("1. Name\n");
    printf("2. Phone number\n");
    printf("3. Email-id\n");
    printf("Enter your choice : ");
    scanf("%d",&choice);

    if(choice < 1 || choice > 3)
    {
        printf("Invalid choice\n");
        return;
    }
    switch(choice)
        {
            case 1 : 
                    {
                        char new_name[30];
                        
                        int i=0;

                        printf("Enter the new name : ");
                        scanf(" %[^\n]",new_name);
                        
                        i=0;
                        while(new_name[i] != '\0')
                        {
                            if( !(isalpha(new_name[i]) || isspace(new_name[i]) || new_name[i] == '.') )
                            {
                                printf("Invalid name\n");
                                return ;
                            }
                            i++;
                        }
                        strcpy(book -> contacts[actual_index].name , new_name);
                        printf("Contact updated successfully.\n");
                        break;    
                    }

            case 2 :
                    {
                        char new_number[50];
                        printf("Enter the new phone number : ");
                        scanf(" %[^\n]",new_number);

                        int i = 0;
                        while(new_number[i] != '\0')
                        {
                            if(new_number[i] < '0' || new_number[i] > '9')
                            {
                                printf("Invalid number\n");
                                return;
                            }
                            i++;
                        }
                        
                        if(i != 10)
                        {
                            printf("Invalid number\n");
                            return ;
                        }
                        strcpy(book -> contacts[actual_index].number , new_number);
                        printf("Contact updated successfully.\n");
                        break;

                    }




            case 3 :
                    {
                        int i=0;
                        char new_mail[50];
                        
                        printf("Enter the new email address : ");
                        scanf("%s",new_mail);
                        int flag = 1;
                        int at_count = 0;
                        
                        while(new_mail[i] != '\0')
                        {
                            if(new_mail[i] == '@')
                            {
                                at_count++;
                            }
                            if(new_mail[i] == '@' && new_mail[i+1] == '.')
                            {
                                flag = 0;
                                printf("Invalid mail id format, enter the correct mail aadress : \n");
                                break; 
                            }
                            
                            if(!(islower(new_mail[i]) || isdigit(new_mail[i]) || new_mail[i] == '@' || new_mail[i] == '-' || new_mail[i] == '_' || new_mail[i] == '.' ))
                            {
                                printf("Invalid mail id format\n");
                                return;
                            }
                            i++;
                        }
            
                        if(strstr(new_mail,".com") == NULL || at_count != 1 || new_mail[0] == '@' || new_mail[strlen(new_mail) - 1] == '@')
                        {
                            printf("Invalid mail id format\n");
                            return;
                        }

                        strcpy(book -> contacts[actual_index].email , new_mail);
                        printf("Contact updated successfully.\n");
                        break;

                    }
            default:
                    printf("Invalid choice\n");     
        }
}


void delete_contact(Addressbook *book)
/*Find the contact index.
Shift all contacts after it one position left.
Decrement contactcount.
Print success message.*/
{

    int actual_index;

    actual_index = find_index(book);
    
    if(actual_index == -1)
        return;

    int i;
    for(i = actual_index ; i<book->contactcount - 1 ; i++)
    {
        book->contacts[i] = book->contacts[i+1];
    }

    book->contactcount--;
    
    printf("Deleted successfully\n");

}