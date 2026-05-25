#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

#include "../lexer/lexer.h"

/*
|--------------------------------------------------------------------------
| TOKEN ACTUAL
|--------------------------------------------------------------------------
*/

static Token currentToken;

/*
|--------------------------------------------------------------------------
| advanceToken()
|--------------------------------------------------------------------------
| Obtiene el siguiente token.
|--------------------------------------------------------------------------
*/

static void advanceToken() {

    currentToken = getNextToken();
}

/*
|--------------------------------------------------------------------------
| syntaxError()
|--------------------------------------------------------------------------
| Muestra errores sintácticos.
|--------------------------------------------------------------------------
*/

static void syntaxError(const char* message) {

    printf(
        "Syntax Error: %s at line %d\n",
        message,
        currentToken.line
    );

    exit(1);
}

/*
|--------------------------------------------------------------------------
| match()
|--------------------------------------------------------------------------
| Verifica que el token actual sea el esperado.
|--------------------------------------------------------------------------
*/

static void match(TokenType expected) {

    if (currentToken.type == expected) {

        advanceToken();
    }
    else {

        syntaxError("Unexpected token");
    }
}

/*
|--------------------------------------------------------------------------
| parseExpression()
|--------------------------------------------------------------------------
| Por ahora:
| expresión = número o identificador
|--------------------------------------------------------------------------
*/

static ASTNode* parseExpression() {

    ASTNode* node;

    if (currentToken.type == TOKEN_NUMBER) {

        node = createNode("NUMBER", currentToken.lexeme);

        advanceToken();

        return node;
    }

    if (currentToken.type == TOKEN_IDENTIFIER) {

        node = createNode("IDENTIFIER", currentToken.lexeme);

        advanceToken();

        return node;
    }

    syntaxError("Invalid expression");

    return NULL;
}

/*
|--------------------------------------------------------------------------
| parseAssignment()
|--------------------------------------------------------------------------
| assignment:
|
| IDENTIFIER = expression ;
|--------------------------------------------------------------------------
*/

static ASTNode* parseAssignment() {

    ASTNode* node = createNode("ASSIGNMENT", "=");

    // IDENTIFICADOR
    node->left = createNode(
        "IDENTIFIER",
        currentToken.lexeme
    );

    match(TOKEN_IDENTIFIER);

    // =
    match(TOKEN_ASSIGN);

    // EXPRESIÓN
    node->right = parseExpression();

    // ;
    match(TOKEN_SEMICOLON);

    return node;
}

/*
|--------------------------------------------------------------------------
| parseDeclaration()
|--------------------------------------------------------------------------
| declaration:
|
| int x;
| float y;
| bool flag;
|--------------------------------------------------------------------------
*/

static ASTNode* parseDeclaration() {

    ASTNode* node;

    // GUARDAR TIPO
    if (currentToken.type == TOKEN_INT) {

        node = createNode("DECLARATION", "int");
    }
    else if (currentToken.type == TOKEN_FLOAT) {

        node = createNode("DECLARATION", "float");
    }
    else {

        node = createNode("DECLARATION", "bool");
    }

    // AVANZAR TIPO
    advanceToken();

    // IDENTIFICADOR
    node->left = createNode(
        "IDENTIFIER",
        currentToken.lexeme
    );

    match(TOKEN_IDENTIFIER);

    // ;
    match(TOKEN_SEMICOLON);

    return node;
}

/*
|--------------------------------------------------------------------------
| parseStatement()
|--------------------------------------------------------------------------
| Decide qué tipo de instrucción analizar.
|--------------------------------------------------------------------------
*/

static ASTNode* parseStatement() {

    // DECLARACIONES
    if (
        currentToken.type == TOKEN_INT ||
        currentToken.type == TOKEN_FLOAT ||
        currentToken.type == TOKEN_BOOL
    ) {

        return parseDeclaration();
    }

    // ASIGNACIONES
    if (currentToken.type == TOKEN_IDENTIFIER) {

        return parseAssignment();
    }

    syntaxError("Invalid statement");

    return NULL;
}

/*
|--------------------------------------------------------------------------
| parseProgram()
|--------------------------------------------------------------------------
| Punto principal del parser.
|--------------------------------------------------------------------------
*/

ASTNode* parseProgram() {

    advanceToken();

    ASTNode* root = createNode("PROGRAM", "ROOT");

    while (currentToken.type != TOKEN_EOF) {
        root->left = parseStatement();
    }

    return root;
}