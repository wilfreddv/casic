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
     * This generates ASM... Not intermediate...
     * Not unfixable, though.
     */
    assert(ast);

    char template[] = "/tmp/casicXXXXXX";
    int fd;
    
    if( (fd = mkstemp(template)) == -1 ) {
        perror("mkstemp");
        return NULL;
    }

    _WRITE(\
    "section .data\n"
    "    str: db \"Hello world!\", 0xa, 0x0\n"
    "section .text\n"
    "    global main\n"
    "    extern print\n"
    "main:\n"
    "    mov rdi, str\n"
    "    call print\n"
    "    ret\n"
    );

    
    close(fd);
    return strdup(template);
}