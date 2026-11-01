#include<stdio.h>
#include<string.h>

void help()
{
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════╗\n");
    printf("                MP3 TAG READER & EDITOR\n");
    printf("════════════════════════════════════════════════════════════\n");

    printf("\nUSAGE:\n");
    printf("  -h                     Display help information\n");
    printf("  -V                     View MP3 tag information\n");
    printf("  -e                     Edit MP3 tags\n");

    printf("\nEDIT OPTIONS:\n");
    printf("  -t                     Modify Title tag\n");
    printf("  -T                     Modify Track tag\n");
    printf("  -a                     Modify Artist tag\n");
    printf("  -A                     Modify Album tag\n");
    printf("  -y                     Modify Year tag\n");
    printf("  -c                     Modify Comment tag\n");
    printf("  -g                     Modify Genre tag\n");


    printf("\n════════════════════════════════════════════════════════════\n");
}
int lit_to_big(int size)
{
    char *ptr = (char *)&size;

    char temp = ptr[0];
    ptr[0] = ptr[3];
    ptr[3] = temp;

    temp = ptr[1];
    ptr[1] = ptr[2];
    ptr[2] = temp;

    return size;

}

void view()
{
    FILE *fp;
    fp = fopen("backup.mp3.mpeg","rb");
    if(fp == NULL)
    {
        perror("Error opening File!!!\n");
        return;
    }
    char tag1[4];
    fread(tag1,sizeof(char),3,fp);
    tag1[3] = '\0';
    if(strcmp(tag1,"ID3") != 0)
    {
        printf("Error!!Wrong file!!\n");
    }
    fseek(fp,7,SEEK_CUR);


    char tag2[5];

    printf("\n");
    printf("╔══════════════════════════════════════════════════════════╗\n");
    printf("║                    MP3 TAG INFORMATION                   ║\n");
    printf("╠══════════════════════════════════════════════════════════╣\n");
    printf("║                                                          ║\n");

    for(int i=0;i<6;i++)
    {
        fread(tag2,sizeof(char),4,fp);
        tag2[4] = '\0';
       

        char label[100];
        if(strcmp(tag2,"TPE1") == 0)
        {
            strcpy(label,"ARTIST");
        }
        else if(strcmp(tag2,"TIT2") == 0)
        {
            strcpy(label,"TITLE");
        }
        else if(strcmp(tag2,"TALB") == 0)
        {
            strcpy(label,"ALBUM");
        }
        else if(strcmp(tag2,"TYER") == 0)
        {
            strcpy(label,"YEAR");
        }
        else if(strcmp(tag2,"TCON") == 0)
        {
            strcpy(label,"CONTENT TYPE");
        }
        else if(strcmp(tag2,"TCOM") == 0)
        {
            strcpy(label,"COMPOSER");
        }
        else
        {
            strcpy(label,"UNKNOWN");
        }

        int size;
        fread(&size,sizeof(char),4,fp);
        size = lit_to_big(size);
        
        fseek(fp, 2, SEEK_CUR);

        char content[size+1];
        fread(content,sizeof(char),size,fp);

        content[size] = '\0';

        printf("║   %-12s : %-39s ║\n", label, content + 1);
    }
    printf("║                                                          ║\n");
    printf("╚══════════════════════════════════════════════════════════╝\n");

}
void edit()
{
   /* int choice;
    printf("\nWhich tag do u want to edit ?\n");
    printf("1.Title\n2.Artist\n3.Album\n4.Year\n5.Content type\n6.Composer\n");
    printf("Enter the choice : ");
    scanf("%d",&choice);
    */


    char tag[15];
    char frame_ID[50];
    char new_content[50];
    printf("\nEnter the tag : \n");
    scanf("%s",tag);
    printf("Enter the edit content : \n");
    scanf(" %[^\n]",new_content);

    if(strcmp(tag,"-a") == 0)
    {
        strcpy(frame_ID , "TPE1");
    }
    if(strcmp(tag,"-t") == 0)
    {
        strcpy(frame_ID , "TIT2");
    }
    if(strcmp(tag,"-A") == 0)
    {
        strcpy(frame_ID , "TALB");
    }
    if(strcmp(tag,"-y") == 0)
    {
        strcpy(frame_ID , "TYER");
    }
    if(strcmp(tag,"-g") == 0)
    {
        strcpy(frame_ID , "TCON");
    }
    if(strcmp(tag,"-c") == 0)
    {
        strcpy(frame_ID , "TCOM");
    }
        if(strcmp(tag,"-T") == 0)
    {
        strcpy(frame_ID , "TRCK");
    }


    FILE *fp1;
    fp1 = fopen("backup.mp3.mpeg","rb");
    if(fp1 == NULL)
    {
        perror("Error opening File!!!\n");
        return;
    }

    FILE *fp2;
    fp2 = fopen("duplicate.mp3.mpeg","wb");

    char buffer[1024];

    char tag2[5];

    char header[10];

    fread(header, sizeof(char), 10, fp1);
    fwrite(header, sizeof(char), 10, fp2);


    for(int i=0;i<6;i++)
    {
        fread(tag2,sizeof(char),4,fp1);
        tag2[4] = '\0';

        int size;
        fread(&size,sizeof(char),4,fp1);
        int original_size = size;
        size = lit_to_big(size);

        char flags[2];
        fread(flags,sizeof(char),2,fp1);

        char content[size+1];
        fread(content,sizeof(char),size,fp1);
        content[size] = '\0';
        
        if(strcmp(tag2, frame_ID) == 0)
        {
            size = strlen(new_content) + 1;
            size = lit_to_big(size);

            fwrite(tag2, sizeof(char), 4, fp2);
            fwrite(&size, sizeof(char), 4, fp2);
            fwrite(flags, sizeof(char), 2, fp2);
            fwrite(content, sizeof(char), 1, fp2);
            fwrite(new_content, sizeof(char), strlen(new_content), fp2);

        }
        else
        {
            fwrite(tag2, sizeof(char), 4, fp2);
            fwrite(&original_size, sizeof(char), 4, fp2);
            fwrite(flags, sizeof(char), 2, fp2);
            fwrite(content, sizeof(char),size, fp2);
        }
    }

    int n;

    while((n = fread(buffer, sizeof(char), 1024, fp1)) > 0)
    {
        fwrite(buffer, sizeof(char), n, fp2);
    }
    fclose(fp1);
    fclose(fp2);
    remove("backup.mp3.mpeg");
    rename("duplicate.mp3.mpeg", "backup.mp3.mpeg");
    
    printf("\nTag edited successfully!\n");
}
