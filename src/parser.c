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

SymbolTable* g_symbol;


AST_Node* literal();
AST_Node* factor();
AST_Node* term();
AST_Node* expression();
AST_Node* condition();
AST_Node* statement();
AST_Node* program();


void error(const char* message, int line, int character, ...);

static int find_symbol(const char* name) {
    if( g_symbol == NULL ) return -1;

    for(int i=0; i<g_symbol->size; i++) {
        if( strcmp(g_symbol->symbols[i].name, name) == 0 )
            return i;
    }

    return -1;
}
static void add_symbol(const char* name, const char* type) {
    if( g_symbol == NULL ) return;

    if( g_symbol->size == 0 ) {
        g_symbol->size++;
        g_symbol->symbols = realloc(g_symbol->symbols, sizeof(struct Symbol)*g_symbol->size);
        g_symbol->symbols[0] = (struct Symbol){name, type};
        return;
    }

    for(int i=0; i<g_symbol->size; i++) {
        int idx;
        if( (idx = find_symbol(name)) > -1 ) {
            if( g_symbol->symbols[idx].type != type ) {
                error("Reassignment with different type for identifier '%s'. (%s to %s)",
                    g_previous->line, g_previous->character, name, g_symbol->symbols[idx].type, type);
            }
        }
        else {
            g_symbol->size++;
            g_symbol->symbols = realloc(g_symbol->symbols, sizeof(struct Symbol)*g_symbol->size);
            g_symbol->symbols[g_symbol->size-1] = (struct Symbol){name, type};
        }
    }
}


static int got_error = 0;

void error(const char* message, int line, int character, ...) {
    printf("%d:%d: ", line, character);
    
    va_list ap;
    va_start(ap, character);
    vprintf(message, ap);
    va_end(ap);
    printf("\n");

    got_error = 1;
}


static inline int is_current(TokenType type) {
    return g_current->type == type;
}

static inline int is_previous(TokenType type) {
    return g_previous->type == type;
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
    // else if( g_current->type == TOKEN_EOF ) {
    //     error("Ran out of tokens...", g_current->line, g_current->character);
    // }
}


int accept(TokenType type) {
    if( g_current->type == type ) {
        next_token();
        return 1;
    }
    return 0;
}


int expect(TokenType type) {
    if( accept(type) )
        return 1;

    error("Unexpected token. Expected `%s`", g_current->line, g_current->character, t2str(type));
    return 0;
}


int accept_keyword(const char* str) {
    if( strcmp(str, g_current->lexeme) == 0 ) {
        next_token();
        return 1;
    }
    return 0;
}


int expect_keyword(const char* str) {
    if( accept_keyword(str) )
        return 1;
    
    error("Unexpected token. Expected `%s`", g_current->line, g_current->character, str);
    return 0;
}


AST_Node* literal() {
    if( !accept(TOKEN_STRING) && !accept(TOKEN_NUMBER) ) {
        error("Expected literal", g_current->line, g_current->character);
        return NULL;
    }

    AST_Node* node = new_ast_node(NODE_CONST);
    node->node.const_node.value = g_previous->lexeme;
    return node;
}


AST_Node* factor() {
    if( accept(TOKEN_IDENTIFIER) ) {
        // TODO: could be function possible??
        if( find_symbol(g_previous->lexeme) == -1 )
            error("Reference to undeclared symbol '%s'", g_previous->line, g_previous->character, g_previous->lexeme);
        AST_Node* node = new_ast_node(NODE_VAR);
        node->node.var_node.name = g_previous->lexeme;
        return node;
    }
    else if( accept(TOKEN_PAREN_OPEN) ) {
        AST_Node* node = expression();
        if( !expect(TOKEN_PAREN_CLOSE) ) return NULL;
        return node;
    }
    else {
        return literal();
    }
}


AST_Node* term() {
    AST_Node* l_node = factor();

    if( is_current(TOKEN_STAR) || is_current(TOKEN_DIVIDE) ) {
        next_token();
        AST_Node* node = new_ast_node(NODE_BINARY);
        node->node.binary_node.operator = strcmp(g_previous->lexeme, "*") == 0 ? BIN_MULTIPLY : BIN_DIVIDE;
        AST_Node* r_node = factor();
        node->node.binary_node.left = l_node;
        node->node.binary_node.right = r_node;
        return node;
    }

    return l_node;
}


AST_Node* expression() {
    AST_Node* l_node, * expr_node;
    
    l_node = term();

    if( accept(TOKEN_PLUS) || accept(TOKEN_MINUS) ) {
        expr_node = new_ast_node(NODE_BINARY);
        expr_node->node.binary_node.operator = strcmp(g_previous->lexeme, "+") == 0 ? BIN_PLUS : BIN_MINUS;
        expr_node->node.binary_node.left = l_node;
        expr_node->node.binary_node.right = expression();
        return expr_node;
    }

    return l_node;
}


AST_Node* condition() {
    AST_Node* node = new_ast_node(NODE_BINARY);
    node->node.binary_node.left = expression();

    // FIX THIS
    if(
        accept(TOKEN_EQUALS)
        || accept(TOKEN_GREATER)
        || accept(TOKEN_SMALLER)
        || accept(TOKEN_GREQ)
        || accept(TOKEN_SMEQ)
        || accept(TOKEN_NEQ)
    ) {
        enum BINARY_OP operator;
        switch( g_previous->type ) {
            case TOKEN_EQUALS:
                operator = BIN_EQUALS;
                break;
            case TOKEN_GREATER:
                operator = BIN_GREATER;
                break;
            case TOKEN_SMALLER:
                operator = BIN_SMALLER;
                break;
            case TOKEN_GREQ:
                operator = BIN_GREQ;
                break;
            case TOKEN_SMEQ:
                operator = BIN_SMEQ;
                break;
            case TOKEN_NEQ:
                operator = BIN_NEQ;
                break;
            default:
                operator = BIN_LAST;
        }
        node->node.binary_node.operator = operator;
        node->node.binary_node.right = expression();
    }
    else {
        error("Expected comparison operator", g_current->line, g_current->character);
    }

    return node;
}


AST_Node* statement() {
    AST_Node* node = NULL;
    
    if( accept(TOKEN_IDENTIFIER) ) {
        node = new_ast_node(NODE_BINARY);
        AST_Node* c_node = new_ast_node(NODE_VAR);
        c_node->node.var_node.name = g_previous->lexeme;
        node->node.binary_node.left = c_node;

        expect(TOKEN_ASSIGN);
        node->node.binary_node.operator = BIN_ASSIGN;

        if( (node->node.binary_node.right = expression()) != NULL )
            add_symbol(c_node->node.var_node.name, t2str(g_current->type));
    }
    else if( is_current(TOKEN_KEYWORD) ) {
        if( accept_keyword("IF") ) {
            node = new_ast_node(NODE_IF);
            node->node.if_node.condition = condition();

            node->node.if_node._if = new_ast_node(NODE_COMPOUND);
            while( !accept_keyword("ELSE") && !accept_keyword("ENDIF") ) {
                if( accept(TOKEN_EOF) ) return NULL;
                compound_node_add_child(&node->node.if_node._if->node.compound_node, statement());
            }
            
            if( strcmp(g_previous->lexeme, "ELSE") == 0 ) {
                node->node.if_node._else = new_ast_node(NODE_COMPOUND);
                while( !accept_keyword("ENDIF") ) {
                    if( accept(TOKEN_EOF) ) return NULL;
                    compound_node_add_child(&node->node.if_node._else->node.compound_node, statement());
                }
            }

            if( strcmp(g_previous->lexeme, "ENDIF") != 0 ) {
                error("Unexpected token. Expected 'ENDIF'", g_previous->line, g_previous->character);
            }
        }
        // TODO: Function and keyword handling should be better
        else if( accept_keyword("PRINT") ) {
            node = new_ast_node(NODE_CALL);
            node->node.call_node.instruction = g_previous->lexeme;
            compound_node_add_child(&node->node.call_node.args->node.compound_node, expression());
        }
        else {
            // Invalid use of keyword
            error("Keyword '%s' cannot be used here.", g_current->line, g_current->character, g_current->lexeme);
            next_token();
        }
    }
    else {
        error("Statement: syntax error (%s)", g_current->line, g_current->character, g_current->lexeme);
        next_token();
    }
    
    return node;
}


AST_Node* program() {
    AST_Node* p = new_ast_node(NODE_COMPOUND);

    while( !accept(TOKEN_EOF) ) {
        compound_node_add_child(&p->node.compound_node, statement());
    }

    return p;
}


AST* generate_ast(TokenStream* tokenstream) {
    g_tokens = tokenstream;
    g_current = g_tokens->token;
    g_symbol = malloc(sizeof(SymbolTable));
    g_symbol->size = 0;
    g_symbol->symbols = NULL;

    ast_ptr = new_ast();
    
    ast_ptr->program = program();

#ifdef DEBUG
    if( !got_error )
        printf("Succesfully parsed!\n");
#endif

    return ast_ptr;
}