#include <token.h>

#include <stdio.h>
#include <stdlib.h>


const char* KEYWORDS[KEYWORD_LAST] = {
    "PRINT",
    "JUMP",
};


static const char* t2str(TokenType type);


TokenStream* new_tokenstream() {
    TokenStream* stream = malloc(sizeof(TokenStream));

    *stream = (TokenStream){NULL, NULL, NULL};
    return stream;
}


int append_token(TokenStream* stream, Token* token) {
    if( stream == NULL || token == NULL ) // what are you doing...
        return 1;

    if( stream->token == NULL ) { // assume head?
        stream->token = token;
        stream->tail = stream;
        return 0;
    }


    TokenStream* tail = stream->tail;
    TokenStream* new = malloc(sizeof(TokenStream));
    if( new == NULL )
        return 1;

    new->token = token;
    new->next = NULL;
    
    tail->next = new;
    stream->tail = new;

    return 0;
}


Token* new_token(TokenType type, const char* lexeme, unsigned short line, unsigned short character) {
    Token* token = malloc(sizeof(Token));

    *token = (Token){type, lexeme, line, character};
    return token;
}


void print_tokens(TokenStream* stream) {
    while(stream) {
        print_token(stream->token);
        stream = stream->next;
    }
}


void print_token(Token* token) {
    if( token == NULL ) return;

    printf("Token(%s, %s, %d, %d)\n",
        t2str(token->type),
        token->lexeme,
        token->line,
        token->character);
}


static const char* t2str(TokenType type) {
    switch(type) {
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_KEYWORD: return "KEYWORD";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_STRING: return "STRING";
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_DIVIDE: return "DIVIDE";
        case TOKEN_STAR: return "STAR";
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_EQUALS: return "EQUALS";
        case TOKEN_PAREN_OPEN: return "PAREN_OPEN";
        case TOKEN_PAREN_CLOSE: return "PAREN_CLOSE";
        case TOKEN_SQUARE_OPEN: return "SQUARE_OPEN";
        case TOKEN_SQUARE_CLOSE: return "SQUARE_CLOSE";
        case TOKEN_CURLY_OPEN: return "CURLY_OPEN";
        case TOKEN_CURLY_CLOSE: return "CURLY_CLOSE";
        case TOKEN_COLON: return "COLON";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        default: return "UNDEFINED";
    }
}