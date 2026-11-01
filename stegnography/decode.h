#ifndef DECODE_H
#define DECODE_H

#include<stdio.h>
#include "types.h"

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUFF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    char *steg_image_fname;
    char *decode_txt_fname;
    
    FILE *fptr_steg_image;
    FILE *fptr_decode;// file pointer for decoded o/p file
} DecodeInfo;

/*check operation type */
OperationType check_operation_type(char *argv[]);

/* read and validate Decode args froma argv */
Status read_and_validate_decode_args(char *argv[] , DecodeInfo *decInfo);

/*Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/*Status open_files*/
Status open_decode_files(DecodeInfo *decInfo);

Status decode_byte_from_lsb(char *data,char *image_buffer);

Status decode_size_from_lsb(int *data, char *image_buffer);

Status decode_magic_string(DecodeInfo *decInfo);

Status decode_secret_file_extn_size(int *size, DecodeInfo *decInfo);

Status decode_secret_file_extn(char *file_extn,int extn_size, DecodeInfo *decInfo);

Status decode_secret_file_size(int *file_size, DecodeInfo *decInfo);

Status decode_secret_file_data(int file_size,DecodeInfo *decInfo);

Status compare_files(char *file1, char *file2);

#endif





