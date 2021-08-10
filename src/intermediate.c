#include <AST.h>
#include <token.h>
#include <intermediate.h>

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>


#define _WRITE(x) write(fd, x, strlen(x));


char* generate_intermediate(AST* ast) {
    /*
     * Generate IR bytecode
     */
    assert(ast);

    char fname[] = "/tmp/casicXXXXXX";
    int fd;
    
    if( (fd = mkstemp(fname)) == -1 ) {
        perror("mkstemp");
        return NULL;
    }


    ast->program->visit(ast->program, fd);
    

    close(fd);
    return strdup(fname);
}