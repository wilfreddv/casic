#include <AST.h>
#include <typedef.h>

#include <stdio.h>
#include <stdlib.h>


void binary_visit(AST_Node* self, int fd) {
    BinaryNode node = self->node.binary_node;
    
    switch(node.operator) {
        case BIN_ASSIGN:
            break;
        default:
            break;
    }
}

void compound_visit(AST_Node* self, int fd) {}
void var_visit(AST_Node* self, int fd) {}
void const_visit(AST_Node* self, int fd) {}
void if_visit(AST_Node* self, int fd) {}
void call_visit(AST_Node* self, int fd) {}


AST* new_ast() {
    AST* ast = malloc(sizeof(AST));
    if( ast == NULL ) return NULL;

    ast->symbol_table = malloc(sizeof(SymbolTable));
    if( ast->symbol_table == NULL ) return NULL;

    ast->symbol_table->size = 0;
    ast->symbol_table->symbols = NULL;


    return ast;
}


AST_Node* new_ast_node(enum NodeTypes type) {
    AST_Node* node = malloc(sizeof(AST_Node));
    if( node == NULL ) return NULL;

    node->type = type;

    switch(type) {
        case NODE_BINARY:
            node->visit = &binary_visit;
            node->node.binary_node = (BinaryNode){ .left=NULL, .right=NULL, .operator=BIN_LAST };
            break;
        case NODE_COMPOUND:
            node->visit = &compound_visit;
            node->node.compound_node = (CompoundNode){ .no_children=0, .children=NULL };
            break;
        case NODE_VAR:
            node->visit = &var_visit;
            node->node.var_node = (VarNode){ .name=NULL };
            break;
        case NODE_CONST:
            node->visit = &const_visit;
            node->node.const_node = (ConstNode){ .value=NULL };
            break;
        case NODE_IF:
            node->visit = &if_visit;
            node->node.if_node = (IfNode){ .condition=NULL, ._if=NULL, ._else=NULL };
            break;
        case NODE_CALL:
            node->visit = &call_visit;
            node->node.call_node = (CallNode){ .instruction=NULL, .args=new_ast_node(NODE_COMPOUND) };
            break;
        default:
            return NULL;
    }

    return node;
}


void compound_node_add_child(CompoundNode* parent, AST_Node* child) {
    if( parent == NULL || child == NULL ) return;

    parent->no_children++;

    parent->children = realloc(parent->children, sizeof(AST_Node*) * parent->no_children);
    parent->children[parent->no_children-1] = child;
}



static inline const char* operator2string(enum BINARY_OP op) {
    switch(op) {
        case BIN_PLUS:      return "PLUS";
        case BIN_MINUS:     return "MINUS";
        case BIN_DIVIDE:    return "DIVIDE";
        case BIN_MULTIPLY:  return "MULTIPLY";
        case BIN_ASSIGN:    return "ASSIGN";
        case BIN_EQUALS:    return "EQUALS";
        case BIN_GREATER:   return "GREATER";
        case BIN_SMALLER:   return "SMALLER";
        case BIN_GREQ:      return "GREATER_EQUAL";
        case BIN_SMEQ:      return "SMALLER_EQUAL";
        case BIN_NEQ:       return "NOT_EQUAL";
        default: return "";
    }
}

static inline void printtabs(unsigned n) { for(int i=0; i<n; i++) printf("  "); }

static void print_node(AST_Node* node, int level) {
    if( node == NULL ) return;
    printtabs(level++);

    switch(node->type) {
        case NODE_BINARY:
            printf("BinaryNode\n");
            printtabs(level);
            printf("operator: %s\n", operator2string(node->node.binary_node.operator));
            printtabs(level);
            printf("left:\n");
            print_node(node->node.binary_node.left, level);
            printtabs(level);
            printf("right:\n");
            print_node(node->node.binary_node.right, level);
            break;
        case NODE_VAR:
            printf("VarNode\n");
            printtabs(level);
            printf("name: %s\n", node->node.var_node.name);
            printtabs(level);
            printf("type: \n");
            break;
        case NODE_CONST:
            printf("ConstNode\n");
            printtabs(level);
            printf("value: %s\n", node->node.const_node.value);
            break;
        case NODE_COMPOUND:
            printf("CompoundNode\n");
            for(int i=0; i<node->node.compound_node.no_children; i++)
                print_node(node->node.compound_node.children[i], level);
            break;
        case NODE_IF:
            printf("IfNode\n");
            printtabs(level);
            printf("condition:\n");
            print_node(node->node.if_node.condition, level);
            printtabs(level);
            printf("if:\n");
            print_node(node->node.if_node._if, level);
            if( node->node.if_node._else != NULL ) {
                printtabs(level);
                printf("else:\n");
                print_node(node->node.if_node._else, level);
            }
            break;
        case NODE_CALL:
            printf("CallNode\n");
            printtabs(level);
            printf("Instruction: %s\n", node->node.call_node.instruction);
            for(int i=0; i<node->node.call_node.args->node.compound_node.no_children; i++)
                print_node(node->node.call_node.args->node.compound_node.children[i], level);
            break;
        default:
            break;
    }
}


void print_ast(AST* root) {
    if( root == NULL ) return;

    SymbolTable* st = root->symbol_table;
    printf("Symbol Table (%ld):\n", st->size);
    for(int i=0; i<st->size; i++) {
        printf("Name: %s\tType: %s\n", st->symbols[i].name, t2str(st->symbols[i].type));
    }

    CompoundNode* program = &root->program->node.compound_node;

    for( int i=0; i < program->no_children; i++ ) {
        print_node(program->children[i], 0);
    }
}