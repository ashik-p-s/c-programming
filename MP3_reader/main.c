#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "mp3_tag_reader.h"

int choice;
int main(int argc , char *argv[])
{
        /*Command Line Arguments*/
        if(argc > 1)
        {
                if(strcmp(argv[1],"-h") == 0)
                {
                        help();
                        return 0;
                }
                else if(strcmp(argv[1],"-v")== 0 )
                {
                        view();
                        return 0;
                }
                else if(strcmp(argv[1],"-e") == 0)
                {
                        edit();
                        return 0;
                }
                else
                {
                        printf("Invalid argument\n");
                        return 0;
                }
        }

        /*Normal menu*/
    do 
    {
        printf("\n");
        printf("╔══════════════════════════════════════╗\n");
        printf("║          MP3 TAG READER              ║\n");
        printf("║            & EDITOR                  ║\n");
        printf("╠══════════════════════════════════════╣\n");
        printf("║                                      ║\n");
        printf("║   [1]  Help                          ║\n");
        printf("║   [2]  View MP3 Tags                 ║\n");
        printf("║   [3]  Edit MP3 Tags                 ║\n");
        printf("║   [4]  Exit                          ║\n");
        printf("║                                      ║\n");
        printf("╚══════════════════════════════════════╝\n");
       
        printf("\n  Enter your choice : ");

       


        if(scanf("%d", &choice) != 1)
        {
        printf("Invalid choice!!!!\n");

        while(getchar() != '\n');
        continue;
        }

        switch(choice)
        {
            case 1 :
                    help();
                    break;
            case 2 :
                    view();
                    break;
            case 3 :
                    edit();
                    break;
            case 4 : 
                    break;
            default :
                    printf("Invalid choice!!!!\nEnter valid choice : ");
        }
    }while(choice != 4);

    return 0;
}

    
    
