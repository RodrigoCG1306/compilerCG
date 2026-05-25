#ifndef AST_H
#define AST_H

/*
|--------------------------------------------------------------------------
| ESTRUCTURA DEL AST
|--------------------------------------------------------------------------
| AST = Abstract Syntax Tree
|
| Representa la estructura lógica del programa.
|--------------------------------------------------------------------------
*/

typedef struct ASTNode {

    char type[50];

    char value[50];

    struct ASTNode* left;
    struct ASTNode* right;

} ASTNode;

/*
|--------------------------------------------------------------------------
| createNode()
|--------------------------------------------------------------------------
| Crea un nodo del AST.
|--------------------------------------------------------------------------
*/

ASTNode* createNode(const char* type, const char* value);

#endif