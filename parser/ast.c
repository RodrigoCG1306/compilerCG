#include <stdlib.h>
#include <string.h>

#include "ast.h"

/*
|--------------------------------------------------------------------------
| createNode()
|--------------------------------------------------------------------------
| Reserva memoria y crea un nodo del AST.
|--------------------------------------------------------------------------
*/

ASTNode* createNode(const char* type, const char* value) {

    ASTNode* node = (ASTNode*) malloc(sizeof(ASTNode));

    strcpy(node->type, type);

    strcpy(node->value, value);

    node->left = NULL;
    node->right = NULL;

    return node;
}