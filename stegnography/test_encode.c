#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc ,char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;
    //uint img_size;


    OperationType ret = check_operation_type(argv);

    if(ret == e_encode)
    {
        if(argc < 4)
        {
            return -1;
        }

        Status status = read_and_validate_encode_args(argv, &encInfo);
        if(status == e_success)
        {
            printf("INFO: Arguments validated successfully.\n");
            do_encoding(&encInfo);
        }
        else
        {
            printf("ERROR: Argument validation failed.\n");
            return -1; 
        }
    }
    else if(ret == e_decode)
    {
        if(argc < 3)
        {
            return -1;
        }

        Status status  = read_and_validate_decode_args(argv,&decInfo);
        if(status == e_success)
        {
            printf("INFO: Arguments validated successfully.\n");
            do_decoding(&decInfo);
        }
        else
        {
            printf("ERROR: Argument validation failed.\n");
            return -1; 
        }
    }
    else
    {
        printf("ERROR: Unsupported operation.\n");
        return -1;
    }
    
    return 0;
}
    /* Fill with sample filenames
    encInfo.src_image_fname = "beautiful.bmp";
    encInfo.secret_fname = "secret.txt";
    encInfo.stego_image_fname = "stego_img.bmp";

    // Test open_files
    if (open_files(&encInfo) == e_failure)
    {
    	printf("ERROR: %s function failed\n", "open_files" );
    	return 1;
    }
    else
    {
    	printf("SUCCESS: %s function completed\n", "open_files" );
    }

    // Test get_image_size_for_bmp
    img_size = get_image_size_for_bmp(encInfo.fptr_src_image);
    printf("INFO: Image size = %u\n", img_size);

    return 0;
}*/
