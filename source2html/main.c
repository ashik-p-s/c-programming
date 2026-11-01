#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s <source.c> <output.html> [-n]\n", argv[0]);
        return 1;
    }

    int show_line_num = 0;
    if (argc >= 4 && strcmp(argv[3], "-n") == 0)
    {
        show_line_num = 1;
    }

    FILE *fp1 = fopen(argv[1], "r");
    if (fp1 == NULL)
    {
        perror("Error opening source file");
        return 1;
    }

    FILE *fp2 = fopen(argv[2], "w");
    if (fp2 == NULL)
    {
        perror("Error opening output file");
        fclose(fp1);
        return 1;
    }

    write_html_header(fp2);
    parser(fp1, fp2, show_line_num);
    write_html_footer(fp2);

    fclose(fp2);
    fclose(fp1);

    printf("Conversion complete.\n");
    return 0;
}