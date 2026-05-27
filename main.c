#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer/lexer.h"
#include "parser/parser.h"
#include "semantic/semantic.h" 
#include "semantic/symbol_table.h" 

ASTNode* programRoot = NULL;
float returnValue = 0;
int hasReturn = 0;
ASTNode* findFunction(const char* name);
void executeAST(ASTNode* node);

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
    | FUNCTION CALL
    |--------------------------------------------------------------------------
    */

    if (
        strcmp(node->type, "FUNCTION_CALL") == 0
    ) {


        ASTNode* function =
            findFunction(node->value);

        /*
        |--------------------------------------------------------------------------
        | PARÁMETROS Y ARGUMENTOS
        |--------------------------------------------------------------------------
        */

        ASTNode* param =
            function->extra2;

        ASTNode* arg =
            node->left;

        while (
            param != NULL &&
            arg != NULL
        ) {

            /*
            |--------------------------------------------------------------------------
            | EVALUAR ARGUMENTO
            |--------------------------------------------------------------------------
            */

            float value =
                evaluateExpression(arg);

            /*
            |--------------------------------------------------------------------------
            | CREAR VARIABLE SI NO EXISTE
            |--------------------------------------------------------------------------
            */

            if (!symbolExists(param->value)) {

                addSymbol(
                    param->value,
                    param->extra->value
                );
            }

            /*
            |--------------------------------------------------------------------------
            | ASIGNAR VALOR
            |--------------------------------------------------------------------------
            */

            setSymbolValue(
                param->value,
                value
            );

            param = param->next;

            arg = arg->next;
        }

        if (function == NULL) {

            printf(
                "Runtime Error: Function '%s' not found\n",
                node->value
            );

            exit(1);
        }

        /*
        |--------------------------------------------------------------------------
        | RESETEAR RETURN
        |--------------------------------------------------------------------------
        */
        returnValue = 0;

        hasReturn = 0;

        /*
        |--------------------------------------------------------------------------
        | EJECUTAR FUNCIÓN
        |--------------------------------------------------------------------------
        */

        ASTNode* current =
            function->left;

        while (current != NULL) {

            executeAST(current);

            if (hasReturn)
                break;

            current = current->next;
        }

        return returnValue;
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

ASTNode* findFunction(const char* name) {

    ASTNode* current =
        programRoot->left;

    while (current != NULL) {

        if (
            strcmp(current->type, "FUNCTION") == 0 &&
            strcmp(current->value, name) == 0
        ) {

            return current;
        }

        current = current->next;
    }

    return NULL;
}

void executeAST(ASTNode* node) {

    if (node == NULL)
        return;

             /*
            |--------------------------------------------------------------------------
            | RETURN
            |--------------------------------------------------------------------------
            */

            if (strcmp(node->type, "RETURN") == 0) {

                returnValue =
                    evaluateExpression(node->left);
;

                hasReturn = 1;

                return;
            }


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

            /*
            |--------------------------------------------------------------------------
            | BLOCK
            |--------------------------------------------------------------------------
            */
            if (strcmp(node->type, "BLOCK") == 0) {

                ASTNode* current =
                    node->left;

                while (current != NULL) {

                    executeAST(current);

                    if (hasReturn)
                        return;

                    current = current->next;
                }

                return;
            }

            /*
            |--------------------------------------------------------------------------
            | FUNCTION CALL
            |--------------------------------------------------------------------------
            */

            if (
                strcmp(node->type, "FUNCTION_CALL") == 0
            ) {

                ASTNode* function =
                    findFunction(node->value);

                if (function == NULL) {

                    printf(
                        "Runtime Error: Function '%s' not found\n",
                        node->value
                    );

                    exit(1);
                }

                executeAST(function->left);

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
    return;
}

    executeAST(node->left);
    executeAST(node->right);
    executeAST(node->extra);
    executeAST(node->extra2);
}

ASTNode* findMainFunction(ASTNode* root) {

    ASTNode* current = root->left;

    while (current != NULL) {

        if (
            strcmp(current->type, "FUNCTION") == 0 &&
            strcmp(current->value, "elias") == 0
        ) {

            return current;
        }

        current = current->next;
    }

    return NULL;
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
    programRoot = root;
    

    printf("Parsing completed successfully\n");

    semanticCheck(root);

    printf("Semantic analysis completed successfully\n");

    /*
    |--------------------------------------------------------------------------
    | BUSCAR elias()
    |--------------------------------------------------------------------------
    */

    ASTNode* mainFunction =
        findMainFunction(root);

    if (mainFunction == NULL) {

        printf(
            "Runtime Error: No elias() function found\n"
        );

        return 1;
    }

    /*
    |--------------------------------------------------------------------------
    | EJECUTAR BODY DE elias()
    |--------------------------------------------------------------------------
    */

    executeAST(mainFunction->left);

    /*
    |--------------------------------------------------------------------------
    | LIBERAR MEMORIA
    |--------------------------------------------------------------------------
    */

    free(source);

    return 0;
}