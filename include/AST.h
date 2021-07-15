#ifndef AST_H
#define AST_H

#include <token.h>
#include <typedef.h>

#include <stddef.h>


struct Symbol {
    const char* name;
    TokenType type;
};


typedef struct _S_Table {
    size_t size;
    struct Symbol* symbols;
} SymbolTable;


enum BINARY_OP {
    BIN_PLUS,
    BIN_MINUS,
    BIN_DIVIDE,
    BIN_MULTIPLY,
    BIN_ASSIGN,
    BIN_EQUALS,

    BIN_GREATER,
    BIN_SMALLER,
    BIN_GREQ,
    BIN_SMEQ,
    BIN_NEQ,

    BIN_LAST,
};


enum NodeTypes {
    NODE_BINARY,
    NODE_VAR,
    NODE_CONST,
    NODE_COMPOUND,
    NODE_IF,
    NODE_CALL,
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


struct IfNode  {
    AST_Node* condition;
    AST_Node* _if;
    AST_Node* _else;
};


struct CallNode {
    const char* instruction;
    AST_Node* args;
};


struct CompoundNode {
    int no_children;
    AST_Node** children;
};


struct AST {
    SymbolTable* symbol_table;
    AST_Node* program;
};


struct AST_Node {
    enum NodeTypes type;
    void (*visit)(AST_Node* self);

    union Node {
        BinaryNode binary_node;
        VarNode var_node;
        ConstNode const_node;
        CompoundNode compound_node;
        IfNode if_node;
        CallNode call_node;
    } node;
};


AST* new_ast();
AST_Node* new_ast_node(enum NodeTypes type);
void compound_node_add_child(CompoundNode* parent, AST_Node* child);

void print_ast(AST* root);


#endif // AST_H