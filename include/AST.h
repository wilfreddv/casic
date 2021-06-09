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


enum NodeTypes {
    NODE_BINARY,
    NODE_VAR,
    NODE_CONST,
    NODE_COMPOUND,
};


struct BinaryNode {
    enum BINARY_OP operator;
    AST_Node* left;
    AST_Node* right;
};


struct VarNode {
    const char* name;
};


struct ConstNode {
    const char* value;
};


struct CompoundNode {
    int no_children;
    AST_Node** children;
};


struct AST {
    AST_Node* program;
};


struct AST_Node {
    enum NodeTypes type;

    union Node {
        BinaryNode binary_node;
        VarNode var_node;
        ConstNode const_node;
        CompoundNode compound_node;
    } node;
};


AST* new_ast();
AST_Node* new_ast_node(enum NodeTypes type, ...);
void compound_node_add_child(CompoundNode* parent, AST_Node* child);

void print_ast(AST* root);


#endif // AST_H