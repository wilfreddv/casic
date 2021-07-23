#include <AST.h>
#include <lexer.h>
#include <parser.h>
#include <token.h>
#include <intermediate.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


static void use(const void* _, ...){}


int has_arg(int argc, char** argv, const char* arg) {
    for(int i=0; i<argc; i++) {
        if( strcmp(argv[i], arg) == 0 )
            return 1;
    }

    return 0;
}


int main(int argc, char** argv) {
    if( argc < 2 ) {
        printf("usage: %s INPUTFILE [-S]\n", argv[0]);
        return 1;
    }


    FILE *fp;
    if( (fp = fopen(argv[1], "r")) == NULL ) {
        printf("Couldn't open file: %s\n", argv[1]);
        return 1;
    }


    fseek(fp, 0, SEEK_END);
    size_t filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* file = malloc(sizeof(char) * filesize);
    if( fread(file, 1, filesize, fp) != filesize ) {
        printf("Something went wrong reading the file.\n");
        return 1;
    }


    TokenStream* tokens = tokenize(argv[1], file, filesize);
    if( tokens == NULL ) {
        fprintf(stderr, "'tokenize' failed.\n");
        return 1;
    }
    // print_tokens(tokens);

    AST* ast = generate_ast(tokens);
    if( ast == NULL ) {
        fprintf(stderr, "Parsing failed, aborting...\n");
        return 1;
    }
    // printf("\n-----\n\n");
    // print_ast(ast);


    // optimize?

    char* filename = generate_intermediate(ast);

    if( has_arg(argc, argv, "-S") ) {
        // Only generate assembly, don't assemble and link
        char command[64];
        sprintf(command, "mv %s prog.s", filename);
        system(command);
        return 0;
    }

    // assemble.
    char command[128];
    sprintf(command, "nasm -felf64 %s -o prog.o", filename);
    
    int ret = 0;
    if( (ret = system(command)) == 0 ) {
        // Link it if assembling succeeded
        sprintf(command, "ld libcasic.o prog.o --dynamic-linker=/lib/ld-linux.so.2 -o prog");
        ret = system(command);
    }

    unlink(filename);

    use(use);
    return ret != 0;
}