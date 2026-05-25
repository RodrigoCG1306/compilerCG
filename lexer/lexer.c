#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"

/*
|--------------------------------------------------------------------------
| VARIABLES GLOBALES DEL LEXER
|--------------------------------------------------------------------------
|
| src  -> código fuente completo
| pos  -> posición actual dentro del texto
| line -> línea actual para manejo de errores
|--------------------------------------------------------------------------
*/

static const char* src;

static int pos = 0;

static int line = 1;

/*
|--------------------------------------------------------------------------
| initLexer()
|--------------------------------------------------------------------------
| Guarda el código fuente y reinicia el lexer.
|--------------------------------------------------------------------------
*/

void initLexer(const char* source) {

    src = source;

    pos = 0;

    line = 1;
}

/*
|--------------------------------------------------------------------------
| peek()
|--------------------------------------------------------------------------
| Devuelve el carácter actual SIN avanzar.
|--------------------------------------------------------------------------
*/

static char peek() {

    return src[pos];
}

/*
|--------------------------------------------------------------------------
| advance()
|--------------------------------------------------------------------------
| Devuelve el carácter actual y avanza una posición.
|--------------------------------------------------------------------------
*/

static char advance() {

    return src[pos++];
}

/*
|--------------------------------------------------------------------------
| skipSpaces()
|--------------------------------------------------------------------------
| Ignora espacios, tabs y saltos de línea.
|--------------------------------------------------------------------------
*/

static void skipSpaces() {

    while (
        peek() == ' '  ||
        peek() == '\t' ||
        peek() == '\n'
    ) {

        // Contar líneas
        if (peek() == '\n') {
            line++;
        }

        advance();
    }
}

/*
|--------------------------------------------------------------------------
| makeToken()
|--------------------------------------------------------------------------
| Crea y devuelve un token.
|--------------------------------------------------------------------------
*/

static Token makeToken(TokenType type, const char* lexeme) {

    Token token;

    token.type = type;

    strcpy(token.lexeme, lexeme);

    token.line = line;

    return token;
}

/*
|--------------------------------------------------------------------------
| getNextToken()
|--------------------------------------------------------------------------
| Función principal del análisis léxico.
|
| Convierte texto fuente en tokens.
|--------------------------------------------------------------------------
*/

Token getNextToken() {

    // Ignorar espacios
    skipSpaces();

    char c = peek();

    /*
    |--------------------------------------------------------------------------
    | FIN DE ARCHIVO
    |--------------------------------------------------------------------------
    */

    if (c == '\0') {

        return makeToken(TOKEN_EOF, "EOF");
    }

    /*
    |--------------------------------------------------------------------------
    | IDENTIFICADORES Y PALABRAS RESERVADAS
    |--------------------------------------------------------------------------
    */

    if (isalpha(c)) {

        char buffer[100];

        int i = 0;

        // Leer palabra completa
        while (isalnum(peek())) {

            buffer[i++] = advance();
        }

        buffer[i] = '\0';

        /*
        |--------------------------------------------------------------------------
        | PALABRAS RESERVADAS
        |--------------------------------------------------------------------------
        */

        if (strcmp(buffer, "int") == 0)
            return makeToken(TOKEN_INT, buffer);

        if (strcmp(buffer, "float") == 0)
            return makeToken(TOKEN_FLOAT, buffer);

        if (strcmp(buffer, "bool") == 0)
            return makeToken(TOKEN_BOOL, buffer);

        if (strcmp(buffer, "if") == 0)
            return makeToken(TOKEN_IF, buffer);

        if (strcmp(buffer, "else") == 0)
            return makeToken(TOKEN_ELSE, buffer);

        if (strcmp(buffer, "while") == 0)
            return makeToken(TOKEN_WHILE, buffer);

        if (strcmp(buffer, "for") == 0)
            return makeToken(TOKEN_FOR, buffer);

        if (strcmp(buffer, "do") == 0)
            return makeToken(TOKEN_DO, buffer);

        if (strcmp(buffer, "true") == 0)
            return makeToken(TOKEN_TRUE, buffer);

        if (strcmp(buffer, "false") == 0)
            return makeToken(TOKEN_FALSE, buffer);

        /*
        |--------------------------------------------------------------------------
        | SI NO ES PALABRA RESERVADA
        |--------------------------------------------------------------------------
        | Entonces es un identificador.
        */

        return makeToken(TOKEN_IDENTIFIER, buffer);
    }

    /*
    |--------------------------------------------------------------------------
    | NÚMEROS
    |--------------------------------------------------------------------------
    */

    if (isdigit(c)) {

        char buffer[100];

        int i = 0;

        while (isdigit(peek())) {

            buffer[i++] = advance();
        }

        buffer[i] = '\0';

        return makeToken(TOKEN_NUMBER, buffer);
    }

    /*
    |--------------------------------------------------------------------------
    | OPERADORES Y SÍMBOLOS
    |--------------------------------------------------------------------------
    */

    advance();

    switch (c) {

        case '+':
            return makeToken(TOKEN_PLUS, "+");

        case '-':
            return makeToken(TOKEN_MINUS, "-");

        case '*':
            return makeToken(TOKEN_MULT, "*");

        case '/':
            return makeToken(TOKEN_DIV, "/");

        case '=':
            return makeToken(TOKEN_ASSIGN, "=");

        case ';':
            return makeToken(TOKEN_SEMICOLON, ";");

        case '(':
            return makeToken(TOKEN_LPAREN, "(");

        case ')':
            return makeToken(TOKEN_RPAREN, ")");

        case '{':
            return makeToken(TOKEN_LBRACE, "{");

        case '}':
            return makeToken(TOKEN_RBRACE, "}");
    }

    /*
    |--------------------------------------------------------------------------
    | ERROR LÉXICO
    |--------------------------------------------------------------------------
    */

    return makeToken(TOKEN_ERROR, "ERROR");
}