#ifndef PARSER_H
#define PARSER_H

#include <AST.h>
#include <token.h>
#include <typedef.h>

#include <stddef.h>


AST generate_ast(TokenStream* tokens);


#endif // PARSER_H