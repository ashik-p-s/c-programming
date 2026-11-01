#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#define STACK_SIZE 100
#define RED     "\033[1;31m"
#define RESET   "\033[0m"


static const char* keywords[MAX_KEYWORDS] =
{
    "int", "float", "return", "if", "else", "while", "for", "do",
    "break", "continue", "char", "double", "void", "switch",
    "case", "default", "const", "static", "sizeof", "struct"
};

static const char* compoundOperators[MAX_OPERATORS] =
{
    "++", "--", "==", "!=", "<=", ">=",
    "&&", "||", "+=", "-=", "*=", "/="
};

static const char* operators = "+-*/%=!<>|&";
static const char* specialCharacters = ",;{}()[]";

FILE *fp;

int lineNumber = 1;
int bracketCount = 0;
int bracketLine = 0;

char bracketStack[STACK_SIZE];
int top = -1;

void validateBraces(char ch)
{
    /*Opening brackets*/
    if(ch == '{' || ch == '(')
    {
        top++;
        bracketStack[top] = ch;
    }

    /*Closing brackets*/
    else if (ch =='}' || ch == ')')
    {
        if(top == -1)
        {
        printf(RED "[ERROR]" RESET
            "  Line %-3d | Unmatched '%c'\n",
            lineNumber, ch);
        }
        else if((ch == '}' && bracketStack[top] != '{') || (ch == ')' && bracketStack[top] != '(') )
        {
            printf(RED "[ERROR]" RESET
                "  Line %-3d | Mismatched '%c'\n",
                lineNumber, ch);
        }
        else
        {
            top--;
        }
    }
}




//  SQUARE BRACKET VALIDATION

void validateBracket(char ch) //[]
{
    if(ch == '[')
    {
        bracketCount++;
        bracketLine = lineNumber;
    }
    else if(ch == ']')
    {
        if(bracketCount == 0)
        {
            printf(RED "[ERROR]" RESET
                "  Line %-3d | Unmatched ']'\n",
                lineNumber);
        }
        else if(lineNumber != bracketLine)
        {
            printf(RED "[ERROR]" RESET
                "  Line %-3d | '[' and ']' must be on the same line\n",
                lineNumber);

            bracketCount = 0;
        }
        else
        {
            bracketCount--;
        }
    }
}

void validateEndOfFile(void)
{
    while(top != -1)
    {
        printf(RED "[ERROR]" RESET
                 "  End of file | Unclosed '%c'\n",
                            bracketStack[top]);

        top--;
    }

    if(bracketCount > 0)
    {
        printf(RED "[ERROR]" RESET
            "  Line %-3d | '[' must be closed on the same line\n",
            bracketLine);

        bracketCount = 0;
    }
}
/* Initialize Lexer */

int initializeLexer(void)
{
    char filename[30];

    printf("Enter the source filename : ");
    scanf("%29s", filename);

    fp = fopen(filename, "r");

    if(fp == NULL)
    {
        printf(RED "[ERROR]" RESET
                "  Unable to open the source file.\n");
        return 0;
    }

    return 1;
}


/* Check Keyword */

int isKeyword(const char *str)
{
    int i = 0;

    while(i < MAX_KEYWORDS)
    {
        if(strcmp(keywords[i], str) == 0)
            return 1;

        i++;
    }

    return 0;
}


/* Check Identifier */

int isIdentifier(const char *str)
{
    int i = 0;

    if(!((str[i] >= 'a' && str[i] <= 'z') ||
         (str[i] >= 'A' && str[i] <= 'Z') ||
         str[i] == '_'))
    {
        return 0;
    }

    i++;

    while(str[i] != '\0')
    {
        if(!((str[i] >= 'a' && str[i] <= 'z') ||
             (str[i] >= 'A' && str[i] <= 'Z') ||
             (str[i] >= '0' && str[i] <= '9') ||
             str[i] == '_'))
        {
            return 0;
        }

        i++;
    }

    return 1;
}


/* Check Constant */

int isConstant(const char *str)
{
    int i = 0;

    /* Character constant */

    if(str[0] == '\'' && str[strlen(str) - 1] == '\'')
        return 1;

    /* String constant */

    if(str[0] == '"' && str[strlen(str) - 1] == '"')
        return 1;

    /* Integer or Float constant */

    if(str[0] >= '0' && str[0] <= '9')
    {
        while((str[i] >= '0' && str[i] <= '9') ||
              str[i] == '.')
        {
            i++;
        }

        if(str[i] == '\0')
            return 1;
    }

    return 0;
}


/* Check Compound Operator */

int iscompoundOperators(const char *str)
{
    int i = 0;

    while(i < MAX_OPERATORS)
    {
        if(strcmp(compoundOperators[i], str) == 0)
            return 1;

        i++;
    }

    return 0;
}


/* Check Special Character */

int isSpecialCharacter(char ch)
{
    int i = 0;

    while(specialCharacters[i] != '\0')
    {
        if(specialCharacters[i] == ch)
            return 1;

        i++;
    }

    return 0;
}


/* Check Operator */

int isOperator(char ch)
{
    int i = 0;

    while(operators[i] != '\0')
    {
        if(operators[i] == ch)
            return 1;

        i++;
    }

    return 0;
}


/* Binary Validation */

int isBinary(const char *str)
{
    int i = 2;

    if(str[0] != '0' ||
       (str[1] != 'b' && str[1] != 'B'))
    {
        return 0;
    }

    if(str[2] == '\0')
        return 0;

    while(str[i] != '\0')
    {
        if(str[i] != '1' && str[i] != '0')
        {
            return 0;
        }

        i++;
    }

    return 1;
}


/* Octal Validation */

int isOctal(const char *str)
{
    int i = 1;

    if(str[0] != '0')
    {
        return 0;
    }

    if(str[1] == '\0')
        return 0;

    while(str[i] != '\0')
    {
        if(!(str[i] >= '0' && str[i] <= '7'))
        {
            return 0;
        }

        i++;
    }

    return 1;
}

/* Hexadecimal Validation*/
int isHexadecimal(const char *str)
{
    int i = 2;

    if(str[0] != '0' ||
       (str[1] != 'X' && str[1] != 'x'))
    {
        return 0;
    }

    if(str[2] == '\0')
        return 0;

    while(str[i] != '\0')
    {
        if(!((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'f') || (str[i] >= 'A' && str[i] <= 'F')))
        {
            return 0;
        }

        i++;
    }

    return 1;
}


/* Categorize Token */

void categorizeToken(Token *token)
{
    if(isKeyword(token->lexeme))
    {
        token->type = KEYWORD;
    }
    else if(isIdentifier(token->lexeme))
    {
        token->type = IDENTIFIER;
    }
    else if(isConstant(token->lexeme))
    {
        token->type = LITERALS;
    }
    else
    {
        token->type = UNKNOWN;
    }
}


/* Get Next Token */

Token getNextToken()
{
    Token token;
    int ch, i = 0;

    if((ch = fgetc(fp)) == EOF)
    {
        validateEndOfFile();
        token.type = UNKNOWN;
        return token;
    }


    /* Skip spaces */

    while(ch == ' ' || ch == '\n' || ch == '\t')
    {   
        if(ch == '\n')
        {
            if(bracketCount > 0)
            {
                printf(RED "[ERROR]" RESET
                        "  Line %-3d | '[' must be closed on the same line\n",
                        bracketLine);

                bracketCount = 0;
            }

            lineNumber++;
        }

        ch = fgetc(fp);

        if(ch == EOF)
        {

            validateEndOfFile();

            token.type = UNKNOWN;
            return token;
        }
    }


    /* Preprocessor */

    if(ch == '#')
    {
        while(ch != '\n' && ch != EOF)
        {
            token.lexeme[i] = ch;
            i++;

            ch = fgetc(fp);
        }

        if(ch == '\n')
        {
            lineNumber++;
        }

        token.lexeme[i] = '\0';
        token.type = PREPROCESSOR;

        return token;
    }


    /* Keyword / Identifier */

    else if((ch >= 'a' && ch <= 'z') ||
            (ch >= 'A' && ch <= 'Z') ||
            ch == '_')
    {
        i = 0;

        while((ch >= 'a' && ch <= 'z') ||
              (ch >= 'A' && ch <= 'Z') ||
              (ch >= '0' && ch <= '9') ||
              ch == '_')
        {
            token.lexeme[i] = ch;
            i++;

            ch = fgetc(fp);
        }

        if(ch != EOF)
            ungetc(ch, fp);

        token.lexeme[i] = '\0';

        categorizeToken(&token);

        return token;
    }


    /* Special Character */

    else if(isSpecialCharacter(ch))
    {
        token.lexeme[0] = ch;
        token.lexeme[1] = '\0';

        token.type = SPECIAL_CHARACTER;

        validateBracket(ch);
        validateBraces(ch);
        return token;
    }


    /* Character Constant */

    else if(ch == '\'')
    {
        i = 0;

        /* Store opening ' */

        token.lexeme[i] = ch;
        i++;

        /* Read character after opening ' */

        ch = fgetc(fp);

        if(ch == EOF)
        {
            printf(RED "[ERROR]" RESET
                        "  Line %-3d | Invalid character literal\n",
                        lineNumber);

            token.type = UNKNOWN;

            return token;
        }

        /* Empty character literal */

        if(ch == '\'')
        {
            printf(RED "[ERROR]" RESET
                    "  Line %-3d | Zero characters between quotes\n",
                    lineNumber);

            token.type = UNKNOWN;

            return token;
        }

        /* Escape sequence */

        if(ch == '\\')
        {
            token.lexeme[i] = ch;
            i++;

            ch = fgetc(fp);

            if(ch == EOF)
            {
                printf(RED "[ERROR]" RESET
                            "  Line %-3d | Incomplete escape sequence\n",
                            lineNumber);

                token.type = UNKNOWN;

                return token;
            }

            token.lexeme[i] = ch;
            i++;
        }

        /* Normal character */

        else
        {
            token.lexeme[i] = ch;
            i++;
        }

        /* Closing ' */

        ch = fgetc(fp);

        if(ch == '\'')
        {
            token.lexeme[i] = ch;
            i++;

            token.lexeme[i] = '\0';

            categorizeToken(&token);

            return token;
        }
        else
        {
            printf(RED "[ERROR]" RESET
                    "  Line %-3d | Invalid character literal\n",
                    lineNumber);


            token.type = UNKNOWN;

            return token;
        }
    }


    /* Integer / Float Constant */

    else if(ch >= '0' && ch <= '9')
    {
        i = 0;

        token.lexeme[i] = ch;
        i++;

        ch = fgetc(fp);


        /*hexadecimal*/

        if(token.lexeme[0] == '0' &&
           (ch == 'x' || ch == 'X'))
        {
            token.lexeme[i] = ch;
            i++;

            ch = fgetc(fp);

            while((ch >= 'a' && ch <= 'z') ||
                  (ch >= 'A' && ch <= 'Z') ||
                  ch == '_' ||
                  (ch >= '0' && ch <= '9'))
            {
                token.lexeme[i] = ch;
                i++;

                ch = fgetc(fp);
            }

            token.lexeme[i] = '\0';

            if(isHexadecimal(token.lexeme) == 0)
            {
                if(ch != EOF)
                    ungetc(ch, fp);

                printf(RED "[ERROR]" RESET
                            "  Line %-3d | Invalid hexadecimal number: '%s'\n",
                            lineNumber, token.lexeme);

                token.type = ERROR;

                return token;
            }
            else
            {
                if(ch != EOF)
                    ungetc(ch, fp);

                token.type = LITERALS;

                return token;
            }
        }

        /* Binary */

        if(token.lexeme[0] == '0' &&
           (ch == 'b' || ch == 'B'))
        {
            token.lexeme[i] = ch;
            i++;

            ch = fgetc(fp);

            while((ch >= 'a' && ch <= 'z') ||
                  (ch >= 'A' && ch <= 'Z') ||
                  ch == '_' ||
                  (ch >= '0' && ch <= '9'))
            {
                token.lexeme[i] = ch;
                i++;

                ch = fgetc(fp);
            }

            token.lexeme[i] = '\0';

            if(isBinary(token.lexeme) == 0)
            {
                if(ch != EOF)
                    ungetc(ch, fp);

                printf(RED "[ERROR]" RESET
                        "  Line %-3d | Invalid binary number: '%s'\n",
                        lineNumber, token.lexeme);

                token.type = ERROR;

                return token;
            }
            else
            {
                if(ch != EOF)
                    ungetc(ch, fp);

                token.type = LITERALS;

                return token;
            }
        }


        /* Octal */

        if(token.lexeme[0] == '0' &&
           ch >= '0' && ch <= '9')
        {
            token.lexeme[i] = ch;
            i++;

            ch = fgetc(fp);

            while(ch >= '0' && ch <= '9')
            {
                token.lexeme[i] = ch;
                i++;

                ch = fgetc(fp);
            }

            token.lexeme[i] = '\0';

            if(isOctal(token.lexeme) == 0)
            {
                if(ch != EOF)
                    ungetc(ch, fp);

                printf(RED "[ERROR]" RESET
                        "  Line %-3d | Invalid octal number: '%s'\n",
                        lineNumber, token.lexeme);

                token.type = ERROR;

                return token;
            }
            else
            {
                if(ch != EOF)
                    ungetc(ch, fp);

                token.type = LITERALS;

                return token;
            }
        }


        /* Continue collecting digits */

        while(ch >= '0' && ch <= '9')
        {
            token.lexeme[i] = ch;
            i++;

            ch = fgetc(fp);
        }

        /* Number followed by alphabet identifier case*/

        if((ch >= 'a' && ch <= 'z') ||
           (ch >= 'A' && ch <= 'Z') ||
           ch == '_')
        {
            token.lexeme[i] = ch;
            i++;

            ch = fgetc(fp);

            while((ch >= 'a' && ch <= 'z') ||
                  (ch >= 'A' && ch <= 'Z') ||
                  ch == '_' ||
                  (ch >= '0' && ch <= '9'))
            {
                token.lexeme[i] = ch;
                i++;

                ch = fgetc(fp);
            }

            token.lexeme[i] = '\0';

            if(ch != EOF)
                ungetc(ch, fp);

            printf(RED "[ERROR]" RESET
                    "  Line %-3d | Invalid identifier/decimal number: '%s'\n",
                    lineNumber, token.lexeme);

            token.type = ERROR;

            return token;
        }


        /* Float */

        if(ch == '.')
        {
            token.lexeme[i] = ch;
            i++;

            ch = fgetc(fp);

            while(ch >= '0' && ch <= '9')
            {
                token.lexeme[i] = ch;
                i++;

                ch = fgetc(fp);
            }


            /* Decimal followed by alphabet */

            if((ch >= 'a' && ch <= 'z') ||
               (ch >= 'A' && ch <= 'Z') ||
               ch == '_')
            {
                token.lexeme[i] = ch;
                i++;

                ch = fgetc(fp);

                while((ch >= 'a' && ch <= 'z') ||
                      (ch >= 'A' && ch <= 'Z') ||
                      ch == '_' ||
                      (ch >= '0' && ch <= '9'))
                {
                    token.lexeme[i] = ch;
                    i++;

                    ch = fgetc(fp);
                }

                token.lexeme[i] = '\0';

                if(ch != EOF)
                    ungetc(ch, fp);

                printf(RED "[ERROR]" RESET
                    "  Line %-3d | Decimal number should not contain alphabets: '%s'\n",
                    lineNumber, token.lexeme);

                token.type = ERROR;

                return token;
            }


            if(ch != EOF)
                ungetc(ch, fp);

            token.lexeme[i] = '\0';

            categorizeToken(&token);

            return token;
        }


        /* Integer */

        else
        {
            if(ch != EOF)
                ungetc(ch, fp);

            token.lexeme[i] = '\0';

            categorizeToken(&token);

            return token;
        }
    }


    /* String Constant */

    else if(ch == '"')
    {
        i = 0;

        token.lexeme[i] = ch;
        i++;

        ch = fgetc(fp);

        while(ch != '"' && ch != EOF)
        {
            token.lexeme[i] = ch;
            i++;

            ch = fgetc(fp);
        }

        if(ch == '"')
        {
            token.lexeme[i] = ch;
            i++;

            token.lexeme[i] = '\0';

            categorizeToken(&token);

            return token;
        }
        else
        {
            printf(RED "[ERROR]" RESET
                    "  Line %-3d | Unterminated string literal\n",
                    lineNumber);

            token.type = UNKNOWN;

            return token;
        }
    }


    /* Operator */

    else if(isOperator(ch))
    {
        i = 0;

        /* Store first operator */

        token.lexeme[i] = ch;
        i++;

        /* Read next character */

        ch = fgetc(fp);


        /* Second character is also an operator */

        if(ch != EOF && isOperator(ch))
        {
            token.lexeme[i] = ch;
            i++;

            token.lexeme[i] = '\0';


            /* Check compound operator */

            if(iscompoundOperators(token.lexeme))
            {
                token.type = OPERATOR;

                return token;
            }


            /* Not a valid compound operator */

            ungetc(ch, fp);

            token.lexeme[1] = '\0';
            token.type = OPERATOR;

            return token;
        }


        /* Second character is not an operator */

        if(ch != EOF)
            ungetc(ch, fp);

        token.lexeme[i] = '\0';
        token.type = OPERATOR;

        return token;
    }


    /* Unknown */

    token.type = UNKNOWN;

    return token;
}