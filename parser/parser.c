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
| PROTOTIPOS
|--------------------------------------------------------------------------
*/

static ASTNode* parseStatement();

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
| tokenToString()
|--------------------------------------------------------------------------
| Convierte un TokenType en texto legible.
|--------------------------------------------------------------------------
*/

static const char* tokenToString(TokenType type) {

    switch (type) {

        case TOKEN_INT:
            return "int";

        case TOKEN_FLOAT:
            return "float";

        case TOKEN_BOOL:
            return "bool";

        case TOKEN_IDENTIFIER:
            return "identifier";

        case TOKEN_NUMBER:
            return "number";

        case TOKEN_ASSIGN:
            return "=";

        case TOKEN_SEMICOLON:
            return ";";

        case TOKEN_PLUS:
            return "+";

        case TOKEN_MINUS:
            return "-";

        case TOKEN_MULT:
            return "*";

        case TOKEN_DIV:
            return "/";

        case TOKEN_LPAREN:
            return "(";

        case TOKEN_RPAREN:
            return ")";

        case TOKEN_LBRACE:
            return "{";

        case TOKEN_RBRACE:
            return "}";

        case TOKEN_IF:
            return "if";

        case TOKEN_ELSE:
            return "else";

        case TOKEN_WHILE:
            return "while";

        case TOKEN_FOR:
            return "for";

        case TOKEN_DO:
            return "do";

        case TOKEN_EOF:
            return "EOF";

        default:
            return "unknown";
    }
}

/*
|--------------------------------------------------------------------------
| match()
|--------------------------------------------------------------------------
| Verifica que el token actual sea el esperado.
|
| Parámetro:
| expected -> tipo de token que el parser espera encontrar.
|
| Funcionamiento:
|
| 1. Compara el token actual con el esperado.
| 2. Si coinciden:
|       - consume el token
|       - avanza al siguiente
|
| 3. Si NO coinciden:
|       - muestra error sintáctico descriptivo
|       - termina la ejecución
|
| Ejemplo:
|
| Código:
|     x = 10;
|
| El parser espera:
|     IDENTIFIER -> '=' -> NUMBER -> ';'
|
| Si falta ';':
|
|     x = 10
|
| Resultado:
|     Syntax Error: Expected ';' but got 'EOF'
|--------------------------------------------------------------------------
*/

static void match(TokenType expected) {

    // TOKEN CORRECTO
    if (currentToken.type == expected) {

        advanceToken();
    }

    // ERROR SINTÁCTICO
    else {

        printf(
            "Syntax Error: Expected '%s' but got '%s' at line %d\n",
            tokenToString(expected),
            currentToken.lexeme,
            currentToken.line
        );

        exit(1);
    }
}

/*
|--------------------------------------------------------------------------
| parseFactor()
|--------------------------------------------------------------------------
| factor:
|
| NUMBER
| IDENTIFIER
|--------------------------------------------------------------------------
*/

static ASTNode* parseFactor() {

    ASTNode* node;

    // NÚMERO
    if (currentToken.type == TOKEN_NUMBER) {

        node = createNode(
            "NUMBER",
            currentToken.lexeme
        );

        advanceToken();

        return node;
    }

    // IDENTIFICADOR
    if (currentToken.type == TOKEN_IDENTIFIER) {

        node = createNode(
            "IDENTIFIER",
            currentToken.lexeme
        );

        advanceToken();

        return node;
    }

    syntaxError("Invalid factor");

    return NULL;
}

/*
|--------------------------------------------------------------------------
| parseTerm()
|--------------------------------------------------------------------------
| term:
|
| factor
| factor * factor
| factor / factor
|--------------------------------------------------------------------------
*/

static ASTNode* parseTerm() {

    ASTNode* left = parseFactor();

    while (
        currentToken.type == TOKEN_MULT ||
        currentToken.type == TOKEN_DIV
    ) {

        ASTNode* opNode;

        // *
        if (currentToken.type == TOKEN_MULT) {

            opNode = createNode("OPERATOR", "*");
        }

        // /
        else {

            opNode = createNode("OPERATOR", "/");
        }

        advanceToken();

        opNode->left = left;

        opNode->right = parseFactor();

        left = opNode;
    }

    return left;
}

/*
|--------------------------------------------------------------------------
| parseExpression()
|--------------------------------------------------------------------------
| expression:
|
| term
| term + term
| term - term
|--------------------------------------------------------------------------
*/

static ASTNode* parseExpression() {

    ASTNode* left = parseTerm();

    while (
        currentToken.type == TOKEN_PLUS ||
        currentToken.type == TOKEN_MINUS
    ) {

        ASTNode* opNode;

        // +
        if (currentToken.type == TOKEN_PLUS) {

            opNode = createNode("OPERATOR", "+");
        }

        // -
        else {

            opNode = createNode("OPERATOR", "-");
        }

        advanceToken();

        opNode->left = left;

        opNode->right = parseTerm();

        left = opNode;
    }

    return left;
}

/*
|--------------------------------------------------------------------------
| parseBlock()
|--------------------------------------------------------------------------
| block:
|
| {
|    statements
| }
|--------------------------------------------------------------------------
*/

static ASTNode* parseBlock() {

    match(TOKEN_LBRACE);

    ASTNode* block = createNode("BLOCK", "BLOCK");

    ASTNode* current = NULL;

    while (1) {

        // FIN DEL BLOQUE
        if (currentToken.type == TOKEN_RBRACE) {
            break;
        }

        // ERROR: EOF antes de cerrar bloque
        if (currentToken.type == TOKEN_EOF) {

            syntaxError("Expected '}'");
        }

        ASTNode* stmt = parseStatement();

        if (block->left == NULL) {

            block->left = stmt;

            current = stmt;
        }
        else {

            current->next = stmt;

            current = stmt;
        }
    }

    match(TOKEN_RBRACE);

    return block;
}

/*
|--------------------------------------------------------------------------
| parseIf()
|--------------------------------------------------------------------------
| if:
|
| if (expression) {
|     statements
| }
|--------------------------------------------------------------------------
*/

static ASTNode* parseIf() {

    ASTNode* ifNode = createNode("IF", "if");

    // if
    match(TOKEN_IF);

    // (
    match(TOKEN_LPAREN);

    // condición
    ifNode->left = parseExpression();

    // )
    match(TOKEN_RPAREN);

    // bloque
    ifNode->right = parseBlock();

    return ifNode;
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
    printf(
        "[DEBUG] parseStatement -> token: '%s' line: %d\n",
        currentToken.lexeme,
        currentToken.line
    );

    /*
    |--------------------------------------------------------------------------
    | DECLARACIONES
    |--------------------------------------------------------------------------
    */

    if (
        currentToken.type == TOKEN_INT ||
        currentToken.type == TOKEN_FLOAT ||
        currentToken.type == TOKEN_BOOL
    ) {

        return parseDeclaration();
    }

    /*
    |--------------------------------------------------------------------------
    | IF
    |--------------------------------------------------------------------------
    */

    if (currentToken.type == TOKEN_IF) {
        return parseIf();
    }

    /*
    |--------------------------------------------------------------------------
    | ASIGNACIONES
    |--------------------------------------------------------------------------
    */

    if (currentToken.type == TOKEN_IDENTIFIER) {

        return parseAssignment();
    }

    /*
    |--------------------------------------------------------------------------
    | ERRORES ESPECÍFICOS
    |--------------------------------------------------------------------------
    */

    if (currentToken.type == TOKEN_SEMICOLON) {

        syntaxError("Unexpected ';'");
    }

    if (currentToken.type == TOKEN_ASSIGN) {

        syntaxError("Assignment without variable");
    }

    if (currentToken.type == TOKEN_NUMBER) {

        syntaxError("Statement cannot start with number");
    }

    syntaxError("Unknown statement");

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

    ASTNode* current = NULL;

    while (currentToken.type != TOKEN_EOF) {

        ASTNode* stmt = parseStatement();

        // PRIMER NODO
        if (root->left == NULL) {

            root->left = stmt;

            current = stmt;
        }

        // ENCADENAR STATEMENTS
        else {

            current->next = stmt;

            current = stmt;
        }
    }

    return root;
}