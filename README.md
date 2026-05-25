# Compilador Front-End en C

Proyecto académico para la materia de Compiladores.  
Este proyecto implementa la fase de **Front-End** de un compilador utilizando el lenguaje C.

---

# Descripción

El objetivo del proyecto es desarrollar un compilador para un lenguaje de programación simple y fuertemente tipado.

Actualmente el proyecto implementa:

- Lectura de archivos fuente `.cg`
- Análisis léxico (Lexer / Scanner)
- Generación de tokens
- Manejo básico de errores léxicos
- Reconocimiento de:
  - palabras reservadas
  - identificadores
  - números
  - operadores
  - símbolos

---

# Tecnologías Utilizadas

- Lenguaje C
- GCC Compiler
- Estructuras de datos básicas
- Manejo manual de memoria dinámica

---

# Estructura del Proyecto

```text
compiler/
│
├── main.c
│
├── lexer/
│   ├── lexer.c
│   ├── lexer.h
│   └── tokens.h
│
└── tests/
    └── test.cg