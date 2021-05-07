#include <AST.h>
#include <lexer.h>
#include <parser.h>
#include <token.h>

#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv) {
    if( argc != 2 ) {
        printf("usage: %s INPUTFILE\n", argv[0]);
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
    print_tokens(tokens);
    generate_ast(tokens);
}