#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer/lexer.h"
#include "parser/parser.h"
#include "semantic/semantic.h" 
#include "semantic/symbol_table.h" 

/*
|--------------------------------------------------------------------------
| readFile()
|--------------------------------------------------------------------------
| Lee el contenido completo de un archivo y lo guarda en memoria.
|--------------------------------------------------------------------------
*/

char* readFile(const char* filename) {

    FILE* file = fopen(filename, "r");

    // Verificar si el archivo existe
    if (!file) {

        printf("Error opening file\n");

        exit(1);
    }

    // Ir al final del archivo
    fseek(file, 0, SEEK_END);

    // Obtener tamaño
    long size = ftell(file);

    // Regresar al inicio
    rewind(file);

    // Reservar memoria
    char* buffer =
    (char*) calloc(size + 1, sizeof(char));

    // Leer archivo completo
    size_t bytesRead =
        fread(buffer, 1, size, file);

    buffer[bytesRead] = '\0';

    // Agregar fin de cadena
    buffer[size] = '\0';

    fclose(file);

    return buffer;
}

/*
|--------------------------------------------------------------------------
| hasExtension()
|--------------------------------------------------------------------------
| Verifica que el archivo tenga extensión .cg
|--------------------------------------------------------------------------
*/

int hasExtension(const char* filename) {

    // Buscar último punto del nombre
    const char* dot = strrchr(filename, '.');

    if (!dot)
        return 0;

    // Comparar extensión
    return strcmp(dot, ".cg") == 0;
}

float evaluateExpression(ASTNode* node) {

    if (node == NULL)
        return 0;

    /*
    |--------------------------------------------------------------------------
    | NUMBER
    |--------------------------------------------------------------------------
    */

    if (strcmp(node->type, "NUMBER") == 0) {

        return atof(node->value);
    }

    /*
    |--------------------------------------------------------------------------
    | IDENTIFIER
    |--------------------------------------------------------------------------
    */

    if (strcmp(node->type, "IDENTIFIER") == 0) {

        return getSymbolValue(node->value);
    }

    /*
    |--------------------------------------------------------------------------
    | OPERADORES
    |--------------------------------------------------------------------------
    */

    if (strcmp(node->type, "OPERATOR") == 0) {

        float left =
            evaluateExpression(node->left);

        float right =
            evaluateExpression(node->right);

        /*
        |--------------------------------------------------------------------------
        | +
        |--------------------------------------------------------------------------
        */

        if (strcmp(node->value, "+") == 0) {

            return left + right;
        }

        /*
        |--------------------------------------------------------------------------
        | -
        |--------------------------------------------------------------------------
        */

        if (strcmp(node->value, "-") == 0) {

            return left - right;
        }

        /*
        |--------------------------------------------------------------------------
        | *
        |--------------------------------------------------------------------------
        */

        if (strcmp(node->value, "*") == 0) {

            return left * right;
        }

        /*
        |--------------------------------------------------------------------------
        | /
        |--------------------------------------------------------------------------
        */

        if (strcmp(node->value, "/") == 0) {

            return left / right;
        }
    }

    return 0;
}

int evaluateComparison(ASTNode* node) {

    if (node == NULL)
        return 0;

    /*
    |--------------------------------------------------------------------------
    | COMPARISON
    |--------------------------------------------------------------------------
    */

    if (strcmp(node->type, "COMPARISON") == 0) {

        float left =
            evaluateExpression(node->left);

        float right =
            evaluateExpression(node->right);

        /*
        |--------------------------------------------------------------------------
        | <
        |--------------------------------------------------------------------------
        */

        if (strcmp(node->value, "<") == 0) {

            return left < right;
        }

        /*
        |--------------------------------------------------------------------------
        | >
        |--------------------------------------------------------------------------
        */

        if (strcmp(node->value, ">") == 0) {

            return left > right;
        }

        /*
        |--------------------------------------------------------------------------
        | ==
        |--------------------------------------------------------------------------
        */

        if (strcmp(node->value, "==") == 0) {

            return left == right;
        }

        /*
        |--------------------------------------------------------------------------
        | !=
        |--------------------------------------------------------------------------
        */

        if (strcmp(node->value, "!=") == 0) {

            return left != right;
        }

        /*
        |--------------------------------------------------------------------------
        | <=
        |--------------------------------------------------------------------------
        */

        if (strcmp(node->value, "<=") == 0) {

            return left <= right;
        }

        /*
        |--------------------------------------------------------------------------
        | >=
        |--------------------------------------------------------------------------
        */

        if (strcmp(node->value, ">=") == 0) {

            return left >= right;
        }
    }

    return 0;
}

void executeAST(ASTNode* node) {

    if (node == NULL)
        return;

            /*
            |--------------------------------------------------------------------------
            | ASSIGNMENT
            |--------------------------------------------------------------------------
            */

            if (strcmp(node->type, "ASSIGNMENT") == 0) {

                char* varName =
                    node->left->value;

                float value =
                    evaluateExpression(node->right);

                setSymbolValue(varName, value);
            }

            /*
            |--------------------------------------------------------------------------
            | WHILE
            |--------------------------------------------------------------------------
            */

            if (strcmp(node->type, "WHILE") == 0) {

                while (
                    evaluateComparison(node->left)
                ) {

                    executeAST(node->right);
                }
            }

            /*
            |--------------------------------------------------------------------------
            | FOR
            |--------------------------------------------------------------------------
            */

            if (strcmp(node->type, "FOR") == 0) {

                /*
                |--------------------------------------------------------------------------
                | INIT
                |--------------------------------------------------------------------------
                */

                executeAST(node->left);

                /*
                |--------------------------------------------------------------------------
                | LOOP
                |--------------------------------------------------------------------------
                */

                while (
                    evaluateComparison(node->right)
                ) {

                    /*
                    |--------------------------------------------------------------------------
                    | BLOCK
                    |--------------------------------------------------------------------------
                    */

                    executeAST(node->extra2);

                    /*
                    |--------------------------------------------------------------------------
                    | UPDATE
                    |--------------------------------------------------------------------------
                    */

                    executeAST(node->extra);
                }
                return;
            }

    if (strcmp(node->type, "CGOUT") == 0) {

    /*
    |--------------------------------------------------------------------------
    | STRING
    |--------------------------------------------------------------------------
    */

    if (strcmp(node->left->type, "STRING") == 0) {

        printf("%s\n", node->left->value);
    }

    /*
    |--------------------------------------------------------------------------
    | EXPRESIONES
    |--------------------------------------------------------------------------
    */

    else {

        printf("%g\n",
            evaluateExpression(node->left));
    }
}

    executeAST(node->left);
    executeAST(node->right);
    executeAST(node->extra);
    executeAST(node->extra2);
    executeAST(node->next);
}

/*
|--------------------------------------------------------------------------
| main()
|--------------------------------------------------------------------------
| Punto de entrada del compilador.
|--------------------------------------------------------------------------
*/

int main(int argc, char* argv[]) {

    /*
    |--------------------------------------------------------------------------
    | VALIDAR ARGUMENTOS
    |--------------------------------------------------------------------------
    */

    if (argc < 2) {

        printf("Usage: ./compiler archivo.cg\n");

        return 1;
    }

    /*
    |--------------------------------------------------------------------------
    | VALIDAR EXTENSIÓN
    |--------------------------------------------------------------------------
    */

    if (!hasExtension(argv[1])) {

        printf("Invalid file extension\n");

        return 1;
    }

    /*
    |--------------------------------------------------------------------------
    | LEER ARCHIVO FUENTE
    |--------------------------------------------------------------------------
    */

    char* source = readFile(argv[1]);

    /*
    |--------------------------------------------------------------------------
    | INICIAR ANALIZADOR LÉXICO
    |--------------------------------------------------------------------------
    */

    initLexer(source);

    ASTNode* root = parseProgram();

    printf("Parsing completed successfully\n");

    semanticCheck(root);

    printf("Semantic analysis completed successfully\n");
    executeAST(root);

    /*
    |--------------------------------------------------------------------------
    | LIBERAR MEMORIA
    |--------------------------------------------------------------------------
    */

    free(source);

    return 0;
}