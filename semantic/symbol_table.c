#include <stdio.h>
#include <string.h>

#include "symbol_table.h"

/*
|--------------------------------------------------------------------------
| TABLA DE SÍMBOLOS
|--------------------------------------------------------------------------
*/

static Symbol table[MAX_SYMBOLS];

static int count = 0;

/*
|--------------------------------------------------------------------------
| addSymbol()
|--------------------------------------------------------------------------
| Agrega variable a la tabla.
|--------------------------------------------------------------------------
*/

void addSymbol(const char* name, const char* type) {

    strcpy(table[count].name, name);

    strcpy(table[count].type, type);

    count++;
}

/*
|--------------------------------------------------------------------------
| symbolExists()
|--------------------------------------------------------------------------
| Verifica si variable existe.
|--------------------------------------------------------------------------
*/

int symbolExists(const char* name) {

    for (int i = 0; i < count; i++) {

        if (strcmp(table[i].name, name) == 0) {

            return 1;
        }
    }

    return 0;
}

/*
|--------------------------------------------------------------------------
| getSymbolType()
|--------------------------------------------------------------------------
| Devuelve tipo de variable.
|--------------------------------------------------------------------------
*/

const char* getSymbolType(const char* name) {

    for (int i = 0; i < count; i++) {

        if (strcmp(table[i].name, name) == 0) {

            return table[i].type;
        }
    }

    return "undefined";
}