#include <parser.h>
#include <typedef.h>

#include <stdlib.h>
#include <stdio.h>


typedef struct SYMBOL {
    const char* identifier;
    struct SYMBOL* next;
} SYMBOL;

typedef struct SYMBOL_TABLE {
    SYMBOL* head, tail;
} SYMBOL_TABLE;

static SYMBOL_TABLE* ST;


static int error = 0;

static void compound_add_child(CompoundNode* parent, AST_Node* node);
static BinaryNode* new_binary_node(enum BINARY_OP operator);
static VarNode* new_var_node(const char* identifier);
static ConstNode* new_const_node(const char* value);
static CompoundNode* new_compound_node();

static inline TokenType peek_token();


TokenStream* g_current;


AST* generate_ast(TokenStream* tokens) {
    AST* root = malloc(sizeof(AST));

    return root;
}


static void compound_add_child(CompoundNode* parent, AST_Node* child) {
    if( parent == NULL )
        return;

    parent->no_children++;
    parent->children = realloc(parent->children, (parent->no_children * sizeof(AST_Node*)));
    
    if( parent->children == NULL ) {
        printf("Failed to realloc parent->children with no_children: %d\n", parent->no_children);
        exit(1);
    }

    parent->children[parent->no_children-1] = child;
}


static BinaryNode* new_binary_node(enum BINARY_OP operator) {
    BinaryNode* node = malloc(sizeof(BinaryNode));
    node->operator = operator;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static VarNode* new_var_node(const char* identifier) {
    VarNode* node = malloc(sizeof(VarNode));
    node->identifier = identifier;
    return node;
}

static ConstNode* new_const_node(const char* value) {
    ConstNode* node = malloc(sizeof(ConstNode));
    node->value = value;
    return node;
}

static CompoundNode* new_compound_node() {
    CompoundNode* node = malloc(sizeof(CompoundNode));
    node->no_children = 0;
    node->children = NULL;
    return node;
}


static inline TokenType peek_token() {
    if( g_current == NULL ) return -1;
    return g_current->next->token->type;
}