#include <token.h>

#include <stdio.h>
#include <stdlib.h>


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

    printf("Token(%d, %s, %d, %d)\n",
        token->type,
        token->lexeme,
        token->line,
        token->character);
}