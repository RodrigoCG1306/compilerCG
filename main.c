#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer/lexer.h"
#include "parser/parser.h"
#include "semantic/semantic.h"

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
    char* buffer = (char*) malloc(size + 1);

    // Leer archivo completo
    fread(buffer, 1, size, file);

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

    /*
    |--------------------------------------------------------------------------
    | LIBERAR MEMORIA
    |--------------------------------------------------------------------------
    */

    free(source);

    return 0;
}