#include <AST.h>
#include <typedef.h>

#include <stdio.h>
#include <stdlib.h>


AST* new_ast() {
    AST* ast = malloc(sizeof(AST));
    if( ast == NULL ) return NULL;
    ast->program = new_ast_node(NODE_COMPOUND);
    if( ast->program == NULL ) return NULL;
    
    return ast;
}


AST_Node* new_ast_node(enum NodeTypes type, ...) {
    AST_Node* node = malloc(sizeof(AST_Node));
    if( node == NULL ) return NULL;

    node->type = type;

    switch(type) {
        case NODE_BINARY: break;
        case NODE_COMPOUND:
            node->node.compound_node = (CompoundNode){ .no_children=0, .children=NULL };
            break;
        case NODE_VAR: break;
        case NODE_CONST: break;
        default:
            return NULL;
    }

    return node;
}


void compound_node_add_child(CompoundNode* parent, AST_Node* child) {
    if( parent == NULL || child == NULL ) return;

    parent->no_children++;

    parent->children = realloc(parent->children, sizeof(AST_Node*) * parent->no_children);
    parent->children[parent->no_children] = child;
}


void print_ast(AST* root) {
    printf("`print_ast` not yet implemented...");
}