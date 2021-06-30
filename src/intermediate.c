#include <AST.h>
#include <intermediate.h>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>


int analyze_static(AST* ast) {
    //SymbolTable* st = ast->symbol_table;
    AST_Node* root = ast->program;

    for(int i=0; i<root->node.compound_node.no_children; i++) {
        ;
    }

    return 1;
}


char* generate_intermediate(AST* ast) {
    assert(ast);

    char template[] = "/tmp/casicXXXXXX";
    int fd;
    
    if( (fd = mkstemp(template)) == -1 ) {
        perror("mkstemp");
        return NULL;
    }

    char* prog = \
    "       global  main\n"
    "\n"
    "       section .text\n"
    "main:  mov     eax,1\n"
    "       mov     ebx,0\n"
    "       int     80h";

    write(fd, prog, strlen(prog));

    // for(int i=0; i<ast->symbol_table->size; i++) {
    //     ast->symbol_table->symbols[i];
    // }

    
    close(fd);
    return strdup(template);
}