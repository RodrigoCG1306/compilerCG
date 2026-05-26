#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
static ASTNode* parseAssignmentNoSemicolon();
static ASTNode* parseComparison();
static ASTNode* parseCgout();
static ASTNode* parseMain();
static ASTNode* parseExpression();
static ASTNode* parseFunction();
static ASTNode* parseReturn();

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

        case TOKEN_CGOUT:
            return "cgout";

        case TOKEN_STRING:
            return "string";

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

    // IDENTIFICADOR O LLAMADA A FUNCIÓN
    if (currentToken.type == TOKEN_IDENTIFIER) {

        char name[50];

        strcpy(name, currentToken.lexeme);

        advanceToken();

        /*
        |--------------------------------------------------------------------------
        | LLAMADA A FUNCIÓN
        |--------------------------------------------------------------------------
        */

        if (currentToken.type == TOKEN_LPAREN) {

            match(TOKEN_LPAREN);
            match(TOKEN_RPAREN);

            ASTNode* callNode =
                createNode(
                    "FUNCTION_CALL",
                    name
                );

            return callNode;
        }

        /*
        |--------------------------------------------------------------------------
        | VARIABLE
        |--------------------------------------------------------------------------
        */

        node = createNode(
            "IDENTIFIER",
            name
        );

        return node;
    }

    if (currentToken.type == TOKEN_STRING) {
        ASTNode* node =
            createNode("STRING", currentToken.lexeme);

        match(TOKEN_STRING);

        return node;
    }

    // BOOLEANOS
    if (
        currentToken.type == TOKEN_TRUE ||
        currentToken.type == TOKEN_FALSE
    ) {

        node = createNode(
            "BOOLEAN",
            currentToken.lexeme
        );

        advanceToken();

        return node;
    }

    /*
    |--------------------------------------------------------------------------
    | PARÉNTESIS
    |--------------------------------------------------------------------------
    */

    if (currentToken.type == TOKEN_LPAREN) {

        match(TOKEN_LPAREN);

        ASTNode* node =
            parseExpression();

        match(TOKEN_RPAREN);

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
| parseComparison()
|--------------------------------------------------------------------------
| Maneja:
| <
| >
| <=
| >=
| ==
| !=
|--------------------------------------------------------------------------
*/

static ASTNode* parseComparison() {

    ASTNode* left =
        parseExpression();

    if (

        currentToken.type == TOKEN_LT  ||
        currentToken.type == TOKEN_GT  ||
        currentToken.type == TOKEN_LTE ||
        currentToken.type == TOKEN_GTE ||
        currentToken.type == TOKEN_EQUAL ||
        currentToken.type == TOKEN_NOT_EQUAL
    ) {

        Token operatorToken =
            currentToken;

        advanceToken();

        ASTNode* right =
            parseExpression();

        ASTNode* operatorNode =
            createNode(
                "COMPARISON",
                operatorToken.lexeme
            );

        operatorNode->left = left;
        operatorNode->right = right;

        left = operatorNode;
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
|
| else {
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
    ifNode->left = parseComparison();

    // )
    match(TOKEN_RPAREN);

    // bloque IF
    ifNode->right = parseBlock();

    /*
    |--------------------------------------------------------------------------
    | ELSE OPCIONAL
    |--------------------------------------------------------------------------
    */

    if (currentToken.type == TOKEN_ELSE) {

        // else
        match(TOKEN_ELSE);

        // bloque ELSE
        ifNode->extra = parseBlock();
    }

    return ifNode;
}

/*
|--------------------------------------------------------------------------
| parseWhile()
|--------------------------------------------------------------------------
| while:
|
| while (expression) {
|     statements
| }
|--------------------------------------------------------------------------
*/

static ASTNode* parseWhile() {

    ASTNode* whileNode = createNode("WHILE", "while");

    // while
    match(TOKEN_WHILE);

    // (
    match(TOKEN_LPAREN);

    // condición
    whileNode->left = parseComparison();

    // )
    match(TOKEN_RPAREN);

    // bloque
    whileNode->right = parseBlock();

    return whileNode;
}

/*
|--------------------------------------------------------------------------
| parseDoWhile()
|--------------------------------------------------------------------------
| do {
|     statements
| }
| while (expression);
|--------------------------------------------------------------------------
*/

static ASTNode* parseDoWhile() {

    ASTNode* node =
        createNode("DO_WHILE", "do-while");

    /*
    |--------------------------------------------------------------------------
    | do
    |--------------------------------------------------------------------------
    */

    match(TOKEN_DO);

    /*
    |--------------------------------------------------------------------------
    | BLOQUE
    |--------------------------------------------------------------------------
    */

    node->right = parseBlock();

    /*
    |--------------------------------------------------------------------------
    | while
    |--------------------------------------------------------------------------
    */

    match(TOKEN_WHILE);

    /*
    |--------------------------------------------------------------------------
    | (
    |--------------------------------------------------------------------------
    */

    match(TOKEN_LPAREN);

    /*
    |--------------------------------------------------------------------------
    | CONDICIÓN
    |--------------------------------------------------------------------------
    */

    node->left = parseComparison();

    /*
    |--------------------------------------------------------------------------
    | )
    |--------------------------------------------------------------------------
    */

    match(TOKEN_RPAREN);

    /*
    |--------------------------------------------------------------------------
    | ;
    |--------------------------------------------------------------------------
    */

    match(TOKEN_SEMICOLON);

    return node;
}

/*
|--------------------------------------------------------------------------
| parseFor()
|--------------------------------------------------------------------------
| for (
|     assignment;
|     expression;
|     assignment
| ) {
|     statements
| }
|--------------------------------------------------------------------------
*/

static ASTNode* parseFor() {

    ASTNode* node = createNode("FOR", "for");

    // for
    match(TOKEN_FOR);

    // (
    match(TOKEN_LPAREN);

    /*
    |--------------------------------------------------------------------------
    | INIT
    |--------------------------------------------------------------------------
    */

    node->left =
        parseAssignmentNoSemicolon();

    match(TOKEN_SEMICOLON);

    /*
    |--------------------------------------------------------------------------
    | CONDITION
    |--------------------------------------------------------------------------
    */

    node->right = parseComparison();

    match(TOKEN_SEMICOLON);

    /*
    |--------------------------------------------------------------------------
    | UPDATE
    |--------------------------------------------------------------------------
    */

    node->extra =
        parseAssignmentNoSemicolon();

    // )
    match(TOKEN_RPAREN);

    /*
    |--------------------------------------------------------------------------
    | BLOCK
    |--------------------------------------------------------------------------
    */

    node->extra2 = parseBlock();

    return node;
}

/*
|--------------------------------------------------------------------------
| parseCgout()
|--------------------------------------------------------------------------
| cgout("texto");
|--------------------------------------------------------------------------
*/

static ASTNode* parseCgout() {

    ASTNode* node =
        createNode("CGOUT", "cgout");

    match(TOKEN_CGOUT);
    match(TOKEN_LPAREN);

    node->left = parseExpression();

    match(TOKEN_RPAREN);
    match(TOKEN_SEMICOLON);

    return node;
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
| parseAssignmentNoSemicolon()
|--------------------------------------------------------------------------
| assignment sin ;
|--------------------------------------------------------------------------
*/

static ASTNode* parseAssignmentNoSemicolon() {

    ASTNode* node =
        createNode("ASSIGNMENT", "=");

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

static ASTNode* parseFunctionCallStatement() {

    char name[50];

    strcpy(name, currentToken.lexeme);

    match(TOKEN_IDENTIFIER);

    match(TOKEN_LPAREN);
    match(TOKEN_RPAREN);

    match(TOKEN_SEMICOLON);

    ASTNode* node =
        createNode(
            "FUNCTION_CALL",
            name
        );

    return node;
}

static ASTNode* parseReturn() {

    match(TOKEN_RETURN);

    ASTNode* node =
        createNode(
            "RETURN",
            "return"
        );

    node->left =
        parseExpression();

    match(TOKEN_SEMICOLON);

    return node;
}

static ASTNode* parseStatement() {
    /*printf(
        "[DEBUG] parseStatement -> token: '%s' line: %d\n",
        currentToken.lexeme,
        currentToken.line
    );*/

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
    | WHILE
    |--------------------------------------------------------------------------
    */

    if (currentToken.type == TOKEN_WHILE) {

        return parseWhile();
    }

    /*
    |--------------------------------------------------------------------------
    | DO-WHILE
    |--------------------------------------------------------------------------
    */

    if (currentToken.type == TOKEN_DO) {

        return parseDoWhile();
    }

    /*
    |--------------------------------------------------------------------------
    | FOR
    |--------------------------------------------------------------------------
    */

    if (currentToken.type == TOKEN_FOR) {

        return parseFor();
    }
    
    /*
    |--------------------------------------------------------------------------
    | CGOUT
    |--------------------------------------------------------------------------
    */

    if (currentToken.type == TOKEN_CGOUT) {

        return parseCgout();
    }

    if (currentToken.type == TOKEN_RETURN) {

        return parseReturn();
    }

    /*
    |--------------------------------------------------------------------------
    | ASIGNACIONES
    |--------------------------------------------------------------------------
    */

    if (currentToken.type == TOKEN_IDENTIFIER) {

        char name[50];

        strcpy(name, currentToken.lexeme);

        match(TOKEN_IDENTIFIER);

        /*
        |--------------------------------------------------------------------------
        | FUNCIÓN
        |--------------------------------------------------------------------------
        */

        if (currentToken.type == TOKEN_LPAREN) {

            match(TOKEN_LPAREN);
            match(TOKEN_RPAREN);

            match(TOKEN_SEMICOLON);

            ASTNode* node =
                createNode(
                    "FUNCTION_CALL",
                    name
                );

            return node;
        }

        /*
        |--------------------------------------------------------------------------
        | ASIGNACIÓN
        |--------------------------------------------------------------------------
        */

        match(TOKEN_ASSIGN);

        ASTNode* node =
            createNode(
                "ASSIGNMENT",
                "="
            );

        node->left =
            createNode(
                "IDENTIFIER",
                name
            );

        node->right =
            parseExpression();

        match(TOKEN_SEMICOLON);

        return node;
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

    /*
    |--------------------------------------------------------------------------
    | EOF O TOKENS VACÍOS
    |--------------------------------------------------------------------------
    */

    if (
        currentToken.type == TOKEN_EOF
    ) {

        return NULL;
    }

    syntaxError("Invalid statement");

    return NULL;
}

static ASTNode* parseFunction() {

    /*
    |--------------------------------------------------------------------------
    | TIPO
    |--------------------------------------------------------------------------
    */

    char returnType[20];

    strcpy(
        returnType,
        currentToken.lexeme
    );

    advanceToken();

    /*
    |--------------------------------------------------------------------------
    | NOMBRE
    |--------------------------------------------------------------------------
    */

    char functionName[50];

    strcpy(
        functionName,
        currentToken.lexeme
    );

    match(TOKEN_IDENTIFIER);

    /*
    |--------------------------------------------------------------------------
    | ()
    |--------------------------------------------------------------------------
    */

    match(TOKEN_LPAREN);
    match(TOKEN_RPAREN);

    /*
    |--------------------------------------------------------------------------
    | FUNCIÓN
    |--------------------------------------------------------------------------
    */

    ASTNode* node =
        createNode(
            "FUNCTION",
            functionName
        );

    /*
    |--------------------------------------------------------------------------
    | RETURN TYPE
    |--------------------------------------------------------------------------
    */

    node->extra =
        createNode(
            "TYPE",
            returnType
        );

    /*
    |--------------------------------------------------------------------------
    | BODY
    |--------------------------------------------------------------------------
    */

    node->left =
        parseBlock();

    return node;
}

static ASTNode* parseMain() {

    /*
    |--------------------------------------------------------------------------
    | int
    |--------------------------------------------------------------------------
    */

    match(TOKEN_INT);

    /*
    |--------------------------------------------------------------------------
    | elias
    |--------------------------------------------------------------------------
    */

    if (
        strcmp(currentToken.lexeme, "elias") != 0
    ) {

        syntaxError(
            "Expected 'elias()'"
        );
    }

    match(TOKEN_IDENTIFIER);

    /*
    |--------------------------------------------------------------------------
    | ()
    |--------------------------------------------------------------------------
    */

    match(TOKEN_LPAREN);
    match(TOKEN_RPAREN);

    /*
    |--------------------------------------------------------------------------
    | BLOQUE
    |--------------------------------------------------------------------------
    */

    ASTNode* node =
        createNode("MAIN", "elias");

    node->left =
        parseBlock();

    return node;
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

    ASTNode* root =
        createNode("PROGRAM", "ROOT");

    ASTNode* current = NULL;

    while (
        currentToken.type != TOKEN_EOF
    ) {

        ASTNode* func =
            parseFunction();

        /*
        |--------------------------------------------------------------------------
        | PRIMERA FUNCIÓN
        |--------------------------------------------------------------------------
        */

        if (root->left == NULL) {

            root->left = func;

            current = func;
        }

        /*
        |--------------------------------------------------------------------------
        | ENCADENAR
        |--------------------------------------------------------------------------
        */

        else {

            current->next = func;

            current = func;
        }
    }

    return root;
}