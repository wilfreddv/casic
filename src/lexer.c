#include <lexer.h>

#include <stdlib.h>
#include <string.h>


#define IS_NUMERIC(x) ((x) < 58 && (x) > 47)


struct Lexer {
    const char* filename;
    const char* file;
    size_t filesize;
    size_t ifp;
    size_t line, character;
    char current;
};

static struct Lexer lexer;

static char next_char() {
    lexer.ifp++;
    if( lexer.ifp >= lexer.filesize )
        return 0;
    
    char c = lexer.file[lexer.ifp];
    lexer.character++;
    if( c == '\n' ) {
        lexer.line++;
        lexer.character = 0;
    }

    lexer.current = c;
    return c;
}


static inline char peek() {
    return lexer.ifp+1 < lexer.filesize ? lexer.file[lexer.ifp+1] : -1;
}


static Token* number();
static Token* string();
static Token* id_or_keyword();


TokenStream* tokenize(const char* filename, const char* file, size_t filesize) {
    lexer = (struct Lexer){.filename = filename, .file = file, .filesize = filesize, .ifp = -1, .line = 1, .character = 0, .current = 0};
    TokenStream* token_stream = new_tokenstream();

    char c;
    while( (c = next_char()) ) {

        switch(c) {
            case '+':
                append_token(token_stream, new_token(TOKEN_PLUS, "+", lexer.line, lexer.character));
                break;
            case '-':
                append_token(token_stream, new_token(TOKEN_MINUS, "-", lexer.line, lexer.character));
                break;
            case '/':
                append_token(token_stream, new_token(TOKEN_DIVIDE, "/", lexer.line, lexer.character));
                break;
            case '*':
                append_token(token_stream, new_token(TOKEN_STAR, "*", lexer.line, lexer.character));
                break;
            case '=':
                {
                    Token* token;
                    if( peek() == '=' )
                        token = new_token(TOKEN_EQUALS, "==", lexer.line, lexer.character);
                    else
                        token = new_token(TOKEN_ASSIGN, "=", lexer.line, lexer.character);
                    append_token(token_stream, token);
                }
                break;
            case '(':
                append_token(token_stream, new_token(TOKEN_PAREN_OPEN, "(", lexer.line, lexer.character));
                break;
            case ')':
                append_token(token_stream, new_token(TOKEN_PAREN_CLOSE, ")", lexer.line, lexer.character));
                break;
            case '[':
                append_token(token_stream, new_token(TOKEN_SQUARE_OPEN, "[", lexer.line, lexer.character));
                break;
            case ']':
                append_token(token_stream, new_token(TOKEN_SQUARE_CLOSE, "]", lexer.line, lexer.character));
                break;
            case '{':
                append_token(token_stream, new_token(TOKEN_CURLY_OPEN, "{", lexer.line, lexer.character));
                break;
            case '}':
                append_token(token_stream, new_token(TOKEN_CURLY_CLOSE, "}", lexer.line, lexer.character));
                break;
            case ':':
                append_token(token_stream, new_token(TOKEN_COLON, ":", lexer.line, lexer.character));
                break;
            case ';':
                append_token(token_stream, new_token(TOKEN_SEMICOLON, ";", lexer.line, lexer.character));
                break;
            case '"':
                append_token(token_stream, string());
                break;
            default:
                if( IS_NUMERIC(c) )
                    append_token(token_stream, number());
                else
                    append_token(token_stream, id_or_keyword());
                break;
        }
    }

    return token_stream;
}


static Token* number() {
    int size = 32;
    char* buffer = malloc(size * sizeof(char));
    int p = 0;

    buffer[p++] = lexer.current;
    while( IS_NUMERIC(peek()) ) {
        if( p > size ) {
            buffer = realloc(buffer, size + 32);
            size += 32;
        }
        buffer[p++] = next_char();
    }
    
    buffer = realloc(buffer, p);
    buffer[p] = 0;

    Token* token = new_token(TOKEN_NUMBER, buffer, lexer.line, lexer.character);
    return token;
}


static Token* string() { return NULL; }
static Token* id_or_keyword() { return NULL; }