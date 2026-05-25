#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "semantic.h"
#include "symbol_table.h"

/*
|--------------------------------------------------------------------------
| semanticCheck()
|--------------------------------------------------------------------------
| Recorre AST y valida semántica.
|--------------------------------------------------------------------------
*/

void semanticCheck(ASTNode* root) {

    if (root == NULL)
        return;

    /*
    |--------------------------------------------------------------------------
    | DECLARACIONES
    |--------------------------------------------------------------------------
    */

    if (strcmp(root->type, "DECLARATION") == 0) {

        // OBTENER NOMBRE VARIABLE
        char* varName = root->left->value;

        // OBTENER TIPO
        char* varType = root->value;

        // VALIDAR REDECLARACIÓN
        if (symbolExists(varName)) {

            printf(
                "Semantic Error: Variable '%s' already declared\n",
                varName
            );

            exit(1);
        }

        // AGREGAR A TABLA
        addSymbol(varName, varType);

        printf(
            "[SEMANTIC] Declared variable '%s' of type '%s'\n",
            varName,
            varType
        );
    }

    /*
    |--------------------------------------------------------------------------
    | ASIGNACIONES
    |--------------------------------------------------------------------------
    */

    if (strcmp(root->type, "ASSIGNMENT") == 0) {

        char* varName = root->left->value;

        // VALIDAR EXISTENCIA
        if (!symbolExists(varName)) {

            printf(
                "Semantic Error: Variable '%s' not declared\n",
                varName
            );

            exit(1);
        }

        printf(
            "[SEMANTIC] Assignment to '%s' is valid\n",
            varName
        );
    }

    semanticCheck(root->left);

    semanticCheck(root->right);
}