#ifndef PARSER_H
#define PARSER_H
#include<stdio.h>

#define MAX_KEYWORDS 20

typedef enum
{
    IDLE,
    WORD,
    SINGLE_LINE_COMMENT,
    MULTI_LINE_COMMENT,
    PREPROCESSOR,
    NUMERIC_CONSTANT,
    ASCII_CHARACTER,
    HEADER,
    REGULAR_EXPRESSION
}State;

typedef enum
{
    KEYWORD,
    IDENTIFIER,
    NUMBER,
    PREPROCESSOR_TOKEN,
    STRING_TOKEN,
    COMMENT_TOKEN
} TokenType;

void write_html_header(FILE *fp2);
void write_html_footer(FILE *fp2);
void parser(FILE *fp1 ,FILE *fp2, int show_line_num);
int iskeyword(char *buffer);
void fputc_escaped(int ch, FILE *fp);
void format_token(FILE *fp2, char *buffer, TokenType type);

#endif