#ifndef AST_H
#define AST_H

#include <token.h>


typedef struct Node {
    Token* token;
    struct Node* left;
    struct Node* right;
} Node;


typedef Node* AST;


#endif // AST_H