#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define MAX_SYMBOLS 100

/*
|--------------------------------------------------------------------------
| ESTRUCTURA SYMBOL
|--------------------------------------------------------------------------
| Guarda información de variables declaradas.
|--------------------------------------------------------------------------
*/

typedef struct {

    char name[50];

    char type[20];

    float numberValue;

} Symbol;

/*
|--------------------------------------------------------------------------
| FUNCIONES
|--------------------------------------------------------------------------
*/

void addSymbol(const char* name, const char* type);

int symbolExists(const char* name);

const char* getSymbolType(const char* name);

/*
|--------------------------------------------------------------------------
| VALORES
|--------------------------------------------------------------------------
*/

void setSymbolValue(
    const char* name,
    float value
);

float getSymbolValue(
    const char* name
);

#endif