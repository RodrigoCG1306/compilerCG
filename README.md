# CompilerCG — Front-End de Compilador en C

Proyecto académico de la materia de **Compiladores**, desarrollado completamente en **Lenguaje C**, que implementa las fases principales del **Front-End de un compilador** para un lenguaje fuertemente tipado.

---

# Información General

- **Alumno:** José Rodrigo Cázares Godoy
- **Profesor:** Eduardo Ruben Elias Hernandez Gonzalez
- **Materia:** Compiladores
- **Lenguaje Implementado:** CompilerCG
- **Extensión del Lenguaje:** `.cg`
- **Lenguaje de Desarrollo:** C
- **Paradigma del Parser:** Recursive Descent Parser

---

# Descripción del Proyecto

Este proyecto implementa el **Front-End** de un compilador capaz de realizar:

- Análisis Léxico
- Análisis Sintáctico
- Construcción de AST
- Análisis Semántico
- Validación de Tipos
- Manejo de Estructuras de Control
- Manejo de Anidamiento

El compilador procesa archivos con extensión personalizada `.cg`.

---

# Características Implementadas

## Análisis Léxico (Lexer)

El lexer es responsable de:

- Lectura de archivos `.cg`
- Generación de tokens
- Reconocimiento de:
  - Identificadores
  - Números
  - Palabras reservadas
  - Operadores
  - Delimitadores
- Manejo de errores léxicos

### Tokens soportados

```text
int
float
bool
if
else
while
do
for

+
-
*
/

=
;
(
)
{
}
```

---

# Análisis Sintáctico (Parser)

El parser fue implementado mediante:

```text
Recursive Descent Parsing
```

El parser valida la estructura del programa y construye un:

```text
AST (Abstract Syntax Tree)
```

---

# Estructuras de Control Soportadas

## If

```c
if (x) {

    x = x + 1;

}
```

---

## If - Else

```c
if (x) {

    x = 1;

}
else {

    x = 2;

}
```

---

## While

```c
while (x) {

    x = x - 1;

}
```

---

## Do - While

```c
do {

    x = x - 1;

}
while (x);
```

---

## For

```c
for (x = 5; x; x = x - 1) {

    x = x - 1;

}
```

---

# Expresiones Matemáticas

El compilador soporta:

```text
+
-
*
/
```

con precedencia correcta de operadores.

Ejemplo:

```c
x = 10 + 5 * 2;
```

---

# Análisis Semántico

El análisis semántico realiza:

- Validación de variables declaradas
- Prevención de redeclaración
- Validación de tipos
- Manejo de tabla de símbolos
- Validación de asignaciones

---

# Lenguaje Fuertemente Tipado

Ejemplo válido:

```c
int x;

x = 10;
```

Ejemplo inválido:

```c
int x;

x = true;
```

Resultado:

```text
Semantic Error: Type mismatch
```

---

# Manejo de Anidamiento

El compilador soporta múltiples niveles de anidamiento.

Ejemplo de 3 niveles:

```c
int x;

x = 5;

if (x) {

    while (x) {

        do {

            x = x - 1;

        }
        while (x);

    }
}
```

---

# Estructura del Proyecto

```text
compilerCG/
│
├── lexer/
│   ├── lexer.c
│   ├── lexer.h
│   └── tokens.h
│
├── parser/
│   ├── parser.c
│   ├── parser.h
│   ├── ast.c
│   └── ast.h
│
├── semantic/
│   ├── semantic.c
│   ├── semantic.h
│   ├── symbol_table.c
│   └── symbol_table.h
│
├── tests/
│   └── test.cg
│
├── main.c
│
└── README.md
```

---

# Gramática Básica (BNF)

```bnf
<program> ::= <statement_list>

<statement_list> ::= <statement>
                   | <statement> <statement_list>

<statement> ::= <declaration>
              | <assignment>
              | <if_statement>
              | <while_statement>
              | <do_while_statement>
              | <for_statement>

<declaration> ::= <type> IDENTIFIER ";"

<type> ::= "int"
         | "float"
         | "bool"

<assignment> ::= IDENTIFIER "=" <expression> ";"

<expression> ::= <term>
               | <expression> "+" <term>
               | <expression> "-" <term>

<term> ::= <factor>
         | <term> "*" <factor>
         | <term> "/" <factor>

<factor> ::= NUMBER
           | IDENTIFIER

<if_statement> ::= "if" "(" <expression> ")" <block>
                 | "if" "(" <expression> ")" <block>
                   "else" <block>

<while_statement> ::= "while" "(" <expression> ")" <block>

<do_while_statement> ::= "do" <block>
                         "while" "(" <expression> ")" ";"

<for_statement> ::= "for"
                    "("
                    <assignment>
                    <expression>
                    ";"
                    <assignment>
                    ")"
                    <block>

<block> ::= "{"
            <statement_list>
            "}"
```

---

# Compilación

## Requisitos

- GCC
- MinGW (Windows)

---

## Compilar

```bash
gcc main.c lexer/lexer.c parser/parser.c parser/ast.c semantic/semantic.c semantic/symbol_table.c -o compiler
```

---

# Ejecución

```bash
.\compiler.exe tests/test.cg
```

---

# Ejemplo de Salida

```text
Parsing completed successfully

[SEMANTIC] Declared variable 'x' of type 'int'

[SEMANTIC] Assignment to 'x' is valid

Semantic analysis completed successfully
```

---

# Ejemplos de Errores

## Error Sintáctico

```c
int x
```

Resultado:

```text
Syntax Error: Expected ';'
```

---

## Error Semántico

```c
y = 10;
```

Resultado:

```text
Semantic Error: Variable 'y' not declared
```

---

# Tecnologías Utilizadas

- Lenguaje C
- GCC
- MinGW
- Visual Studio Code

---

# Autor

José Rodrigo Cázares Godoy

Proyecto desarrollado para fines académicos en la materia de Compiladores.