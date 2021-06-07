#include <lexer.h>

#include <stdlib.h>
#include <string.h>
#include <ctype.h>



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
        return EOF;
    
    char c = lexer.file[lexer.ifp];
    lexer.character++;

    lexer.current = c;
    return c;
}


static inline char peek() {
    return lexer.ifp+1 < lexer.filesize ? lexer.file[lexer.ifp+1] : -1;
}


static inline void to_upper(char* string, size_t length);
static inline int is_keyword(const char* string);

static Token* number();
static Token* string();
static Token* id_or_keyword();


TokenStream* tokenize(const char* filename, const char* file, size_t filesize) {
    lexer = (struct Lexer){.filename = filename, .file = file, .filesize = filesize, .ifp = -1, .line = 1, .character = 0, .current = 0};
    TokenStream* token_stream = new_tokenstream();

    char c;
    while( (c = next_char()) != EOF ) {

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
            case '>':
                {
                    Token* token;
                    if( peek() == '=' )
                        token = new_token(TOKEN_GREQ, ">=", lexer.line, lexer.character);
                    else
                        token = new_token(TOKEN_GREATER, ">", lexer.line, lexer.character);
                    append_token(token_stream, token);
                }
                break;
            case '<':
                {
                    Token* token;
                    if( peek() == '=' )
                        token = new_token(TOKEN_SMEQ, "<=", lexer.line, lexer.character);
                    else
                        token = new_token(TOKEN_SMALLER, "<", lexer.line, lexer.character);
                    append_token(token_stream, token);
                }
                break;
            case '!':
                {
                    Token* token;
                    if( peek() == '=' )
                        token = new_token(TOKEN_NEQ, "!=", lexer.line, lexer.character);
                    else
                        token = new_token(TOKEN_EXCLAMATION_MARK, "!", lexer.line, lexer.character);
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
            case ' ':
            case '\t':
            case '\r':
                break;
            case '\n':
                lexer.line++;
                lexer.character = 0;
                break;
            default:
                if( isdigit(c) )
                    append_token(token_stream, number());
                else
                    {
                        Token* t = id_or_keyword(); // Filter out REM
                        if(t) append_token(token_stream, t);
                    }
                break;
        } // switch
    } // while

    append_token(token_stream, new_token(TOKEN_EOF, "EOF", lexer.line, lexer.character));
    return token_stream;
}


static Token* number() {
    int size = 32;
    char* buffer = malloc(size * sizeof(char));
    int p = 0;
    size_t location = lexer.character;

    if( lexer.current == '0' && isdigit(peek()) ) {
        printf("Numbers cannot have leading 0's: %ld:%ld\n", lexer.line, location);
        exit(1);
    }


    buffer[p++] = lexer.current;
    while( isdigit(peek()) ) {
        if( p > size ) {
            buffer = realloc(buffer, size + 32);
            size += 32;
        }
        buffer[p++] = next_char();
    }
    
    buffer = realloc(buffer, p);
    buffer[p] = 0;

    Token* token = new_token(TOKEN_NUMBER, buffer, lexer.line, location);
    return token;
}


static Token* string() {
    int size = 64;
    char* buffer = malloc(size * sizeof(char));
    int p = 0;
    size_t location = lexer.character;

    buffer[p++] = lexer.current;
    char c = next_char();
    while( c != EOF && c != '\n' ) {
        if( p > size ) {
            buffer = realloc(buffer, size + 64);
            size += 64;
        }

        buffer[p++] = c;
        if( c == '"' ) // We just added this
            break;

        c = next_char();
    }

    if( c == EOF || c == '\n' ) {
        printf("Unexpected EOF or newline while reading string: %ld:%ld\n", lexer.line, lexer.character);
        exit(1);
    }

    buffer = realloc(buffer, p);
    buffer[p] = 0;


    Token* token = new_token(TOKEN_STRING, buffer, lexer.line, location);
    return token;
}


static Token* id_or_keyword() {
    int size = 64;
    char* buffer = malloc(size * sizeof(char));
    int p = 0;
    size_t location = lexer.character;

    buffer[p++] = lexer.current;
    while( isalnum(peek()) || peek() == '_' ) {
        if( p > size ) {
            buffer = realloc(buffer, size + 64);
            size += 64;
        }

        buffer[p++] = next_char();
    }

    buffer = realloc(buffer, p);
    buffer[p] = 0;


    to_upper(buffer, p-1);

    // Skip over lines with REM
    if(strcmp(buffer, "REM") == 0) {
        while(peek() != '\n') next_char();
        return NULL;
    }


    TokenType t = is_keyword(buffer) ? TOKEN_KEYWORD : TOKEN_IDENTIFIER;

    Token* token = new_token(t, buffer, lexer.line, location);
    return token;
}


static inline void to_upper(char* string, size_t length) {
    for( int i=0; i<=length; i++ ) {
        string[i] = toupper(string[i]);
    }
}


static inline int is_keyword(const char* string) {
    for( int i=0; i<KEYWORD_LAST; i++ ) {
        if( strcmp(string, KEYWORDS[i]) == 0 ) {
            return 1;
        }
    }

    return 0;
}