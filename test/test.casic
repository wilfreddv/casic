#ifndef TOKEN_H
#define TOKEN_H


typedef enum TokenType {
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD,

    TOKEN_NUMBER,
    TOKEN_STRING,

    TOKEN_PLUS,     // +
    TOKEN_MINUS,    // -
    TOKEN_DIVIDE,   // /
    TOKEN_STAR,     // *
    TOKEN_ASSIGN,   // =
    TOKEN_EQUALS,   // ==

    TOKEN_PAREN_OPEN,   // (
    TOKEN_PAREN_CLOSE,  // )
    TOKEN_SQUARE_OPEN,  // [
    TOKEN_SQUARE_CLOSE, // ]
    TOKEN_CURLY_OPEN,   // {
    TOKEN_CURLY_CLOSE,  // }

    TOKEN_COLON,        // :
    TOKEN_SEMICOLON,    // ;
} TokenType;


typedef struct Token {
    TokenType type;
    const char* lexeme;
    unsigned short line;
    unsigned short character;
} Token;


typedef struct TokenStream {
    Token* token;
    struct TokenStream* next;
    struct TokenStream* tail;
} TokenStream;


TokenStream* new_tokenstream();
int append_token(TokenStream* stream, Token* token);
Token* new_token(TokenType type, const char* lexeme, unsigned short line, unsigned short character);

void print_tokens(TokenStream* stream);
void print_token(Token* token);


#endif // TOKEN_H