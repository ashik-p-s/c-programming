#ifndef LEXER_H
#define LEXER_H

#define MAX_KEYWORDS 20
#define MAX_TOKEN_SIZE 100
#define MAX_OPERATORS 12

typedef enum{
    KEYWORD,
    OPERATOR,
    SPECIAL_CHARACTER,
    LITERALS,
    IDENTIFIER,
    PREPROCESSOR,
    UNKNOWN,
    ERROR
} TokenType;


typedef struct {
    char lexeme[MAX_TOKEN_SIZE];
    TokenType type;
} Token;

//void initializeLexer(const char* filename);
int initializeLexer(void);
Token getNextToken();
void categorizeToken(Token* token);
int isKeyword(const char* str);
int isOperator(char ch);
int isSpecialCharacter(char ch);
int isConstant(const char* str);
int isIdentifier(const char* str);
int iscompoundOperators(const char *str);
int isBinary(const char *str);
int isOctal(const char *str);
int isHexadecimal(const char *str);
void validateBracket(char ch);
void validateBraces(char ch);
void validateEndOfFile();

#endif
