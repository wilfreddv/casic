#include <parser.h>
#include <typedef.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>


static Token* g_current = NULL;
static Token* g_previous = NULL;
TokenStream* g_tokens = NULL;


AST* ast_ptr;


void literal();
void factor();
void term();
void expression();
void condition();
void statement();
void program();

static void add_node() {}


static int got_error = 0;

void error(const char* message, ...) {
    printf("%d:%d [%s]: ", g_current->line, g_current->character,
                           t2str(g_current->type));
    
    va_list ap;
    va_start(ap, message);
    vprintf(message, ap);
    va_end(ap);
    printf("\n");

    got_error = 1;
}


static inline int is_current(TokenType type) {
    return g_current->type == type;
}


TokenType peek() {
    if( g_tokens->next != NULL && g_tokens->next->token != NULL ) {
        return g_tokens->next->token->type;
    }
    return TOKEN_LAST+1;
}


void next_token() {
    if( g_tokens->next != NULL ) {
        g_previous = g_current;
        g_tokens = g_tokens->next;
        g_current = g_tokens->token;
    }
    else if( g_current->type != TOKEN_EOF ) {
        error("Ran out of tokens...");
    }
}


int accept(TokenType type) {
    if( g_current->type == type ) {
        add_node();
        next_token();
        return 1;
    }
    return 0;
}


int expect(TokenType type) {
    if( accept(type) )
        return 1;

    error("Unexpected token. Expected `%s`", t2str(type));
    return 0;
}


int accept_keyword(const char* str) {
    if( strcmp(str, g_current->lexeme) == 0 ) {
        next_token();
        return 1;
    }
    return 0;
} 


void literal() {
    if( !accept(TOKEN_STRING) && !accept(TOKEN_NUMBER) )
        error("Expected literal");
}


void factor() {
    if( accept(TOKEN_IDENTIFIER) ) {
    }
    else if( accept(TOKEN_PAREN_OPEN) ) {
        expression();
        expect(TOKEN_PAREN_CLOSE);
    }
    else {
        literal();
    }
}


void term() {
    factor();

    if( is_current(TOKEN_STAR) || is_current(TOKEN_DIVIDE) ) {
        next_token();
        factor();
    }
}


void expression() {
    if( is_current(TOKEN_PLUS) || is_current(TOKEN_MINUS) )
        next_token();

    term();

    if( accept(TOKEN_PLUS) || accept(TOKEN_MINUS) ) {
        term();
    }
}


void condition() {
    expression();
    if(
        accept(TOKEN_EQUALS)
        || accept(TOKEN_GREATER)
        || accept(TOKEN_SMALLER)
        || accept(TOKEN_GREQ)
        || accept(TOKEN_SMEQ)
        || accept(TOKEN_NEQ)
    ) {
        expression();
    }
    else {
        error("Expected comparison operator");
    }
}


void statement() {
    if( accept(TOKEN_IDENTIFIER) ) {
        expect(TOKEN_ASSIGN);
        expression();
    }
    else if( is_current(TOKEN_KEYWORD) ) {
        if( accept_keyword("IF") ) {
            condition();
            statement();
            if( accept_keyword("ELSE") ) {
                statement();
            }
        }
        else if( accept_keyword("PRINT") ) {
            expression();
        }
    }
    else {
        error("Statement: syntax error (%s)", g_current->lexeme);
        next_token();
    }

}


void program() {
    while( !accept(TOKEN_EOF) ) {
        statement();
    }
}


AST* generate_ast(TokenStream* tokenstream) {
    g_tokens = tokenstream;
    g_current = g_tokens->token;

    ast_ptr = new_ast();
    
    program();

#ifdef DEBUG
    if( !got_error )
        printf("Succesfully parsed!\n");
#endif

    return ast_ptr;
}