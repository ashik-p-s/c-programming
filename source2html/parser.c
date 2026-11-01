#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char* keywords[MAX_KEYWORDS] =
{
    "int", "float", "return", "if", "else", "while", "for", "do",
    "break", "continue", "char", "double", "void", "switch",
    "case", "default", "const", "static", "sizeof", "struct"
};

// Helper function to safely output characters to HTML
void fputc_escaped(int ch, FILE *fp)
{
    if (ch == '<')
    {
        fputs("&lt;", fp);
    }
    else if (ch == '>')
    {
        fputs("&gt;", fp);
    }
    else if (ch == '&')
    {
        fputs("&amp;", fp);
    }
    else
    {
        fputc(ch, fp);
    }
}

void write_html_header(FILE *fp2)
{
    fputs("<!DOCTYPE html>\n<html>\n<head>\n<style>\n", fp2);
    fputs("body { background-color: #000000; color: #FFFFFF; font-family: monospace; }\n", fp2);
    fputs(".line-num { color: #888888; padding-right: 15px; user-select: none; }\n", fp2);
    fputs("</style>\n</head>\n<body>\n<pre>\n", fp2);
}

void write_html_footer(FILE *fp2)
{
    fputs("</pre>\n</body>\n</html>\n", fp2);
}

int iskeyword(char *buffer)
{
    int j = 0;
    while(j < MAX_KEYWORDS)
    {
        if(strcmp(buffer, keywords[j]) == 0)
        {
            return 1;
        }
        j++;
    }
    return 0;
}

void format_token(FILE *fp2, char *buffer, TokenType type)
{
    // 1. Open the span tag for coloring
    switch (type)
    {
        case KEYWORD: 
            fprintf(fp2, "<span style=\"color:blue\">");
            break;
        case NUMBER:
            fprintf(fp2, "<span style=\"color:purple\">");
            break;
        case PREPROCESSOR_TOKEN:
            fprintf(fp2, "<span style=\"color:orange\">");
            break;
        case STRING_TOKEN: 
            fprintf(fp2, "<span style=\"color:green\">");
            break;
        case COMMENT_TOKEN:
            fprintf(fp2, "<span style=\"color:grey\">");
            break;
        case IDENTIFIER:
            break;
    }

    // 2. Print each character with HTML escaping
    for (int i = 0; buffer[i] != '\0'; i++)
    {
        fputc_escaped(buffer[i], fp2);
    }

    // 3. Close the span tag
    if (type != IDENTIFIER)
    {
        fprintf(fp2, "</span>");
    }
}

void parser(FILE *fp1, FILE *fp2, int show_line_num)
{
    int quote_char;
    State current_state = IDLE;

    int ch;
    int i = 0;
    char buffer[1024];

    int line_count = 1;

    // Print line 1 number if flag enabled
    if (show_line_num)
    {
        fprintf(fp2, "<span class=\"line-num\">%4d </span>", line_count);
    }

    while((ch = fgetc(fp1)) != EOF)
    {
        if(current_state == IDLE)
        {
            if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
            {
                current_state = WORD;
                buffer[i++] = ch;
            }
            else if(ch >= '0' && ch <= '9')
            {
                current_state = NUMERIC_CONSTANT;
                buffer[i++] = ch;
            }
            else if(ch == '#')
            {
                current_state = PREPROCESSOR;
                buffer[i++] = ch;
            }
            else if(ch == '/')
            {
                int next_ch = fgetc(fp1);
                if(next_ch == '/')
                {
                    current_state = SINGLE_LINE_COMMENT;
                    buffer[i++] = '/';
                    buffer[i++] = '/';
                }
                else if (next_ch == '*')
                {
                    current_state = MULTI_LINE_COMMENT;
                    buffer[i++] = '/';
                    buffer[i++] = '*';
                }
                else
                {
                    if(next_ch != EOF)
                    {
                        ungetc(next_ch, fp1);
                    }
                    fputc_escaped('/', fp2);
                }
            }
            else if(ch == '\'' || ch == '\"')
            {
                current_state = ASCII_CHARACTER;
                quote_char = ch;
                buffer[i++] = ch;
            }
            else
            {
                fputc_escaped(ch, fp2);

                // Print new line number when '\n' is encountered
                if (ch == '\n' && show_line_num)
                {
                    line_count++;
                    fprintf(fp2, "<span class=\"line-num\">%4d </span>", line_count);
                }
            }
        }
        else if(current_state == WORD)
        {
            if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_')
            {
                buffer[i++] = ch;
            }
            else
            {
                buffer[i] = '\0';
                if(iskeyword(buffer))
                {
                    format_token(fp2, buffer, KEYWORD); 
                }
                else
                {
                    format_token(fp2, buffer, IDENTIFIER);
                }
                i = 0;
                current_state = IDLE;
                ungetc(ch, fp1);
            }
        }
        else if(current_state == NUMERIC_CONSTANT)
        {
            if((ch >= '0' && ch <= '9') || ch == '.')
            {
                buffer[i++] = ch;
            }
            else
            {
                buffer[i] = '\0';
                format_token(fp2, buffer, NUMBER);
                i = 0;
                current_state = IDLE;
                ungetc(ch, fp1);
            }
        }
        else if(current_state == PREPROCESSOR)
        {
            if(ch == '\n' || ch == ' ' || ch == '<' || ch == '\"')
            {
                buffer[i] = '\0';
                format_token(fp2, buffer, PREPROCESSOR_TOKEN);
                i = 0;
                current_state = IDLE;
                ungetc(ch, fp1);
            }
            else
            {
                buffer[i++] = ch;
            }
        }
        else if(current_state == SINGLE_LINE_COMMENT)
        {
            if(ch != '\n')
            {
                buffer[i++] = ch;
            }
            else
            {
                buffer[i] = '\0';
                format_token(fp2, buffer, COMMENT_TOKEN);
                i = 0;
                current_state = IDLE;
                ungetc(ch, fp1);
            }
        }
        else if(current_state == MULTI_LINE_COMMENT)
        {
            buffer[i++] = ch;
            if(ch == '*' && i >= 2)
            {
                int next_ch = fgetc(fp1);
                if(next_ch == '/')
                {
                    buffer[i++] = '/';
                    buffer[i] = '\0';
                    format_token(fp2, buffer, COMMENT_TOKEN);
                    i = 0;
                    current_state = IDLE;
                }
                else if(next_ch != EOF)
                {
                    ungetc(next_ch, fp1);
                }
            }
        }
        else if(current_state == ASCII_CHARACTER)
        {
            buffer[i++] = ch;
            if(ch == quote_char && buffer[i-2] != '\\')
            {
                buffer[i] = '\0';
                format_token(fp2, buffer, STRING_TOKEN);
                i = 0;
                current_state = IDLE;
            }
        }
    }
}