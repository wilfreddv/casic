#ifndef TYPEDEF_H
#define TYPEDEF_H


typedef struct Token Token;
typedef struct TokenStream TokenStream;
typedef enum TokenType TokenType;
typedef enum KeywordType KeywordType;

typedef void AST_Node;
typedef struct BinaryNode BinaryNode;
typedef struct VarNode VarNode;
typedef struct ConstNode ConstNode;
typedef struct CompoundNode CompoundNode;
typedef struct AST* AST;


#endif // TYPEDEF_H