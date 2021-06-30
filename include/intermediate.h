#ifndef INTERMEDIATE_H
#define INTERMEDIATE_H

#include <AST.h>

#include <stdio.h>


int analyze_static(AST* ast);
char* generate_intermediate(AST* ast);


#endif // INTERMEDIATE_H