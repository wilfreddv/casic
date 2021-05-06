#ifndef LEXER_H
#define LEXER_H

#include <token.h>
#include <typedef.h>

#include <stdio.h>


TokenStream* tokenize(const char* filename, const char* file, size_t filesize);


#endif // LEXER_H