#ifndef PARSER_H
#define PARSER_H

#include <AST.h>
#include <token.h>
#include <typedef.h>

#include <stddef.h>


struct Symbol {
    const char* name;
    const char* type;
};


typedef struct _S_Table {
    size_t size;
    struct Symbol* symbols;
} SymbolTable;


AST* generate_ast(TokenStream* tokens);



#endif // PARSER_H