#include <stdio.h>
#include "lexer.h"

void printToken(Token token)
{
    switch(token.type)
{
    case KEYWORD:
        printf("%-25s : %s\n", "[KEYWORD]", token.lexeme);
        break;

    case IDENTIFIER:
        printf("%-25s : %s\n", "[IDENTIFIER]", token.lexeme);
        break;

    case OPERATOR:
        printf("%-25s : %s\n", "[OPERATOR]", token.lexeme);
        break;

    case SPECIAL_CHARACTER:
        printf("%-25s : %s\n", "[SPECIAL CHARACTER]", token.lexeme);
        break;

    case LITERALS:
        printf("%-25s : %s\n", "[LITERAL]", token.lexeme);
        break;

    case PREPROCESSOR:
        printf("%-25s : %s\n", "[PREPROCESSOR]", token.lexeme);
        break;

    default:
        //printf("%-25s : %s\n", "[UNKNOWN]", token.lexeme);
        break;
}
}


int main()
{
    if(initializeLexer() == 0)
        return 0;

    Token token;

    while((token = getNextToken()).type != UNKNOWN) 
    {
        printToken(token);
    }

    return 0;
}