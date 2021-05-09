#ifndef AST_H
#define AST_H

#include <token.h>
#include <typedef.h>


enum BINARY_OP {
    PLUS,
    MINUS,
    DIVIDE,
    MULTIPLY,
    ASSIGN,
    EQUALS,
};


struct BinaryNode {
    enum BINARY_OP operator;
    AST_Node* left;
    AST_Node* right;
};


struct VarNode {
    const char* identifier;
};


struct ConstNode {
    const char* value;
};


struct CompoundNode {
    int no_children;
    AST_Node** children;
};


#endif // AST_H