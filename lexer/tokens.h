#ifndef TOKENS_H
#define TOKENS_H

/*
|--------------------------------------------------------------------------
| ENUMERACIÓN DE TOKENS
|--------------------------------------------------------------------------
| Aquí definimos TODOS los tipos de tokens que reconoce el lenguaje.
|--------------------------------------------------------------------------
*/

typedef enum {

    // TIPOS DE DATOS
    TOKEN_INT,
    TOKEN_FLOAT,
    TOKEN_BOOL,

    // ESTRUCTURAS DE CONTROL
    TOKEN_IF,
    TOKEN_ELSE,
    TOKEN_WHILE,
    TOKEN_FOR,
    TOKEN_DO,

    // VALORES BOOLEANOS
    TOKEN_TRUE,
    TOKEN_FALSE,

    // IDENTIFICADORES
    // Ejemplo: x, contador, total
    TOKEN_IDENTIFIER,

    // NÚMEROS
    TOKEN_NUMBER,

    // OPERADORES
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULT,
    TOKEN_DIV,
    TOKEN_ASSIGN,

    // SÍMBOLOS
    TOKEN_SEMICOLON,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,

    // FIN DE ARCHIVO
    TOKEN_EOF,

    // TOKEN INVÁLIDO
    TOKEN_ERROR

} TokenType;

/*
|--------------------------------------------------------------------------
| ESTRUCTURA TOKEN
|--------------------------------------------------------------------------
| Cada token almacena:
|
| - type   -> tipo de token
| - lexeme -> texto original encontrado
| - line   -> línea donde apareció
|--------------------------------------------------------------------------
*/

typedef struct {

    TokenType type;

    char lexeme[100];

    int line;

} Token;

#endif