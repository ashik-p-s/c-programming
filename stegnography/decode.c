#include<stdio.h>
#include<string.h>

#include "decode.h"
#include "types.h"
#include "common.h"

Status read_and_validate_decode_args(char *argv[],DecodeInfo *decInfo)
{
    if(strstr(argv[2],"bmp") == NULL)
    {
        return e_failure;
    }
    else
    {
        decInfo->steg_image_fname = argv[2];
    }

    if(argv[3] != NULL)
    {
        if(strstr(argv[3],"txt") != NULL)
        {
            decInfo->decode_txt_fname = argv[3];
        }
        else
        {
            decInfo->decode_txt_fname = "decoded.txt";
        }
    }
    else
    {
        decInfo->decode_txt_fname = "decoded.txt";
    }
    return e_success;
}

Status open_decode_files(DecodeInfo *decInfo)
{
    decInfo->fptr_steg_image = fopen(decInfo->steg_image_fname,"rb");

    if(decInfo->fptr_steg_image == NULL)
    {
        perror("fopen ");
        fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->steg_image_fname);
        return e_failure;
    }

    decInfo->fptr_decode = fopen(decInfo->decode_txt_fname , "wb"); 

    if(decInfo->fptr_decode == NULL)
    {
        perror("fopen ");
        fprintf(stderr, "ERROR: Unable to open file %s\n",decInfo->decode_txt_fname);
        return e_failure;
    }

    return e_success;
}


Status decode_byte_from_lsb(char *data,char *image_buffer)// image byte lsb to character bit
{
    *data = 0;
    int i = 0;
    for(i=0;i<8;i++)
    {
        *data |= (image_buffer[i] & 1) << (7-i);
    }
    return e_success;
}

Status decode_size_from_lsb(int *data,char *image_buffer)
{
    *data = 0;
    int i = 0;
    for(i=0 ; i < sizeof(int) * 8 ; i++)
    {
        *data |= (image_buffer[i] & 1) << ((sizeof(int) * 8 - 1)-i);
    }
    return e_success;
}

Status compare_files(char *file1, char *file2)
{
    FILE *fp1 = fopen(file1,"rb");
    FILE *fp2 = fopen(file2,"rb");

    if(fp1 == NULL || fp2 == NULL)
    {
        return e_failure;
    }

    int ch1,ch2;

    while(1)
    {
        ch1 = fgetc(fp1);
        ch2 = fgetc(fp2);

        if(ch1 != ch2)
        {
            fclose(fp1);
            fclose(fp2);
            return e_failure;
        }

        if(ch1 == EOF)
        {
            break;
        }
    }
        fclose(fp1);
        fclose(fp2);

        return e_success;

}


Status do_decoding(DecodeInfo *decInfo)
{
    if(open_decode_files(decInfo) != e_success)
    {
        return e_failure;
    }
        
    printf("INFO: Files opened successfully.\n");

    fseek(decInfo->fptr_steg_image,54,SEEK_CUR);

    if(decode_magic_string(decInfo) == e_failure)
    {
        printf("ERROR: Decoding failed.\n");
        return e_failure;
    }

    printf("INFO: Magic string decoded successfully.\n");

    int extn_size;
    decode_secret_file_extn_size(&extn_size, decInfo);    

        
    if(extn_size != 4)
    {
        printf("ERROR: Invalid file extension size.\n");
        return e_failure;
    }

    printf("INFO: Secret file extension size decoded successfully.\n");

    char file_extn[5];
    decode_secret_file_extn(file_extn,extn_size,decInfo);
    if(strcmp(file_extn,".txt") != 0)
        {
            printf("ERROR: Decoding failed.\n");
            return e_failure;
        }    
    printf("INFO: Secret file extension decoded successfully.\n"); 

    int file_size;
    decode_secret_file_size(&file_size,decInfo);
    printf("INFO: Secret file size decoded successfully.\n");

    if(decode_secret_file_data(file_size,decInfo) == e_failure)
        {
            printf("ERROR: Decoding failed.\n");
            return e_failure;
        }

    printf("INFO: Secret file data decoded successfully.\n");
    fflush(decInfo->fptr_decode);
        
    if(compare_files("secret.txt", decInfo->decode_txt_fname) == e_success)
        {
            printf("INFO: Decoded file verified successfully.\n");
            printf("INFO: Decoding completed successfully.\n");
            return e_success;
        }
    else
        {
            printf("ERROR: Decoded file verification failed.\n");
            return e_failure;
        }
    
    
    fclose(decInfo->fptr_steg_image);
    fclose(decInfo->fptr_decode);

}


Status decode_magic_string(DecodeInfo *decInfo)
{
    char buffer[8];
    char ch;
    for(int i=0;i<2;i++)
    {
        fread(buffer,sizeof(char),8,decInfo->fptr_steg_image);
        decode_byte_from_lsb(&ch,buffer);
        if(ch != MAGIC_STRING[i])
        {
            return e_failure;
        } 
    }
    return e_success; 
}


Status decode_secret_file_extn_size(int *size, DecodeInfo *decInfo)
{
    char buffer[32];

    fread(buffer,sizeof(char),32,decInfo->fptr_steg_image);
    decode_size_from_lsb(size,buffer);
    
    return e_success;
}

Status decode_secret_file_extn(char *file_extn,int extn_size, DecodeInfo *decInfo)
{
    char buffer[8];//(.txt)

    for(int i=0;i<extn_size;i++)
    {
        fread(buffer,sizeof(char),8,decInfo->fptr_steg_image);
        decode_byte_from_lsb(&file_extn[i],buffer);
    }
    file_extn[extn_size] = '\0';
    return e_success;
}

Status decode_secret_file_size(int *file_size, DecodeInfo *decInfo)
{
    char buffer[32];
    fread(buffer,sizeof(char),32,decInfo->fptr_steg_image);
    decode_size_from_lsb(file_size,buffer);
    return e_success;
}

Status decode_secret_file_data(int file_size,DecodeInfo *decInfo)
{
    char buffer[8];
    char ch;
    
    for(int i = 0;i< file_size;i++)
    {
        fread(buffer,sizeof(char),8,decInfo->fptr_steg_image);
        decode_byte_from_lsb(&ch,buffer);
        fwrite(&ch,sizeof(char),1,decInfo->fptr_decode);
    }

    return e_success;
}