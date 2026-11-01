#include<stdio.h>
#include<string.h>

#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */

Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for(int i=0;i<8;i++)
    {
        image_buffer[i] = (image_buffer[i] & ~1) | ( (data >> (7-i) & 1));
    }
    return e_success;
}

Status encode_size_to_lsb(int data, char *image_buffer)
{
    for(int i = 0; i < sizeof(int) * 8; i++)
    {
        image_buffer[i] = (image_buffer[i] & ~1) | ((data >> (sizeof(int) * 8 - 1 - i)) & 1);
    }

    return e_success;

}

Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}


uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    

    // Return image capacity
    return width * height * 3;
}

/* Check operation type */
OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1],"-e") == 0)
    {
        return e_encode;
    }
    else if(strcmp(argv[1],"-d") == 0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }
}

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    if(strstr(argv[2] ,"bmp") == NULL)
    {
        return e_failure;
    }
    else
    {
        encInfo -> src_image_fname = argv[2];
    }

    if(strstr(argv[3] ,"txt") == NULL)
    {
        return e_failure;
    }
    else
    {
        encInfo->secret_fname = argv[3];//Store the whole secret filename secret.txt in secret_fname.
        for(int i = 0; i < MAX_FILE_SUFFIX; i++)
        {
            encInfo->extn_secret_file[i] = argv[3][strlen(argv[3]) - 4 + i];// extracting .txt
        }
    }
    
    if(argv[4] != NULL)
    {
        if(strstr(argv[4] , "bmp") != NULL)
        {
            encInfo->stego_image_fname = argv[4];
        }
        else
        {
            encInfo->stego_image_fname = "steged_img.bmp";
        }
    }
    else
    {
        encInfo->stego_image_fname = "steged_img.bmp";
    }    
    return e_success;

}



Status check_capacity(EncodeInfo *encInfo)
{

    fseek(encInfo->fptr_secret,0,SEEK_END);

    encInfo->size_secret_file = ftell(encInfo->fptr_secret) ;
    
    rewind(encInfo->fptr_secret);

    // get image capacity
    uint image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);

    rewind(encInfo->fptr_src_image);
    // calculate required image bytes
    uint required_capacity = (2 + MAX_FILE_SUFFIX + 2*sizeof(int) + encInfo->size_secret_file) * 8;

    if(image_capacity >= required_capacity)
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}



/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo)
{
    if(open_files(encInfo) == e_success)
    {
        printf("INFO: Files opened successfully.\n");
    
        if(check_capacity(encInfo) == e_success)
        {
            printf("INFO: Image has sufficient capacity.\n");

            copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image);
            printf("INFO: BMP header copied successfully.\n");

            encode_magic_string(MAGIC_STRING,encInfo);
            printf("INFO: Magic string encoded successfully.\n");

            encode_secret_file_extn_size(MAX_FILE_SUFFIX,encInfo);
            printf("INFO: Secret file extension size encoded successfully.\n");

            encode_secret_file_extn(encInfo->extn_secret_file,encInfo);
            printf("INFO: Secret file extension encoded successfully.\n");

            encode_secret_file_size(encInfo->size_secret_file,encInfo);
            printf("INFO: Secret file size encoded successfully.\n");

            encode_secret_file_data(encInfo);
            printf("INFO: Secret file data encoded successfully.\n");
            
            copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image);
            printf("INFO: Remaining image data copied successfully.\n");
        }
        else
        {
            printf("ERROR: Insufficient image capacity.\n");
            return e_failure;
        }
    }
    else
    {
        return e_failure;
    }

    printf("INFO: Encoding completed successfully.\n");
    printf("INFO: Stego image created: %s\n", encInfo->stego_image_fname);
    return e_success;
}

Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char buffer[54];
    fread(buffer, sizeof(char), 54, fptr_src_image);
    fwrite(buffer,sizeof(char),54,fptr_dest_image);
    return e_success;
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    char buffer[8];
    for(int i=0;i<2;i++)
    {
        fread(buffer,sizeof(char),8,encInfo->fptr_src_image);
        encode_byte_to_lsb(magic_string[i],buffer);
        fwrite(buffer,sizeof(char),8,encInfo->fptr_stego_image);
    }
    return e_success;
}


Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)
{
    char buffer[32];

    fread(buffer,sizeof(char),32,encInfo->fptr_src_image);
    encode_size_to_lsb(size,buffer);
    fwrite(buffer,sizeof(char),32,encInfo->fptr_stego_image);
    
    return e_success;
}


Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    char buffer[8];//(.txt)

    for(int i=0;i<4;i++)
    {
        fread(buffer,sizeof(char),8,encInfo->fptr_src_image);
        encode_byte_to_lsb(file_extn[i],buffer);
        fwrite(buffer,sizeof(char),8,encInfo->fptr_stego_image);
    }
    return e_success;
}



Status encode_secret_file_size(int file_size, EncodeInfo *encInfo)
{
    char buffer[32];
    fread(buffer,sizeof(char),32,encInfo->fptr_src_image);
    encode_size_to_lsb(file_size,buffer);
    fwrite(buffer,sizeof(char),32,encInfo->fptr_stego_image);
    return e_success;
}


Status encode_secret_file_data(EncodeInfo *encInfo)
{
    char buffer[8];
    char ch;
    
    for(int i = 0;i<encInfo->size_secret_file;i++)
    {
        fread(&ch,sizeof(char),1,encInfo->fptr_secret);
        fread(buffer,sizeof(char),8,encInfo->fptr_src_image);
        encode_byte_to_lsb(ch,buffer);
        fwrite(buffer,sizeof(char),8,encInfo->fptr_stego_image);
    }
    return e_success;
}

Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char buffer[1024];
    size_t bytes_read;

    while((bytes_read = fread(buffer, sizeof(char), 1024, fptr_src)) > 0)
    {
        fwrite(buffer, sizeof(char), bytes_read, fptr_dest);
    }

    return e_success;
}

