#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "semantic.h"
#include "symbol_table.h"

/*
|--------------------------------------------------------------------------
| getExpressionType()
|--------------------------------------------------------------------------
| Devuelve el tipo de una expresión.
|--------------------------------------------------------------------------
*/

static const char* getExpressionType(ASTNode* node) {

    if (node == NULL)
        return "undefined";

    /*
    |--------------------------------------------------------------------------
    | NÚMEROS
    |--------------------------------------------------------------------------
    */

    if (strcmp(node->type, "NUMBER") == 0) {

    // SI TIENE PUNTO -> FLOAT
    if (strchr(node->value, '.') != NULL) {

        return "float";
    }

    return "int";
}

    /*
    |--------------------------------------------------------------------------
    | BOOLEANOS
    |--------------------------------------------------------------------------
    */

    if (strcmp(node->type, "BOOLEAN") == 0) {

        return "bool";
    }

    /*
    |--------------------------------------------------------------------------
    | IDENTIFICADORES
    |--------------------------------------------------------------------------
    */

    if (strcmp(node->type, "IDENTIFIER") == 0) {

        return getSymbolType(node->value);
    }

    /*
    |--------------------------------------------------------------------------
    | OPERADORES
    |--------------------------------------------------------------------------
    */

    if (strcmp(node->type, "OPERATOR") == 0) {

        return getExpressionType(node->left);
    }

    return "undefined";
}

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

        /*
        |--------------------------------------------------------------------------
        | VALIDAR TIPOS
        |--------------------------------------------------------------------------
        */

        const char* variableType = getSymbolType(varName);

        const char* expressionType =
            getExpressionType(root->right);

        if (strcmp(variableType, expressionType) != 0) {

            printf(
                "Semantic Error: Type mismatch in assignment to '%s'\n",
                varName
            );

            exit(1);
        }

        printf(
            "[SEMANTIC] Assignment to '%s' is valid\n",
            varName
        );
    }

        /*
        |--------------------------------------------------------------------------
        | CGOUT
        |--------------------------------------------------------------------------
        */

        if (strcmp(root->type, "CGOUT") == 0) {

            if (
                strcmp(root->left->type, "IDENTIFIER") == 0 &&
                !symbolExists(root->left->value)
            ) {

                printf(
                    "Semantic Error: Variable '%s' not declared\n",
                    root->left->value
                );

                exit(1);
            }
        }

    /*
    |--------------------------------------------------------------------------
    | RECORRER ÁRBOL
    |--------------------------------------------------------------------------
    */

    semanticCheck(root->left);

    semanticCheck(root->right);

    semanticCheck(root->next);

    semanticCheck(root->extra);
    
    semanticCheck(root->extra2);
}