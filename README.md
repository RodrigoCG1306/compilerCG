<div align="center">

# ⚙️ CompilerCG
### Custom Compiler made in C

<img src="https://img.shields.io/badge/language-C-blue.svg">
<img src="https://img.shields.io/badge/status-Academic%20Project-success">
<img src="https://img.shields.io/badge/parser-Recursive%20Descent-orange">
<img src="https://img.shields.io/badge/platform-Windows%20%7C%20Linux-lightgrey">

---

### 🚀 Academic compiler developed from scratch in C

Lexer • Parser • AST • Semantic Analysis • Symbol Table • Type Checking

</div>

---

# 📌 Description

**CompilerCG** is a custom academic compiler developed in **C** for a programming language using the `.cg` extension.

The project was built to understand and implement the core phases of compiler construction:

- Lexical Analysis
- Syntax Analysis
- AST Construction
- Semantic Analysis
- Symbol Tables
- Type Validation
- Control Structure Parsing

---

# ✨ Features

## 🔎 Lexical Analysis
The lexer identifies and tokenizes:

- Keywords
- Variables
- Operators
- Literals
- Symbols
- Control structures

---

## 🌳 Recursive Descent Parser

Manual parser implementation using Recursive Descent Parsing.

Supported structures:

- Variable declarations
- Arithmetic expressions
- Conditions
- Nested blocks
- Loops
- Assignments

---

## 🧠 Semantic Analysis

The semantic analyzer validates:

- Variable declarations
- Type compatibility
- Scope management
- Semantic correctness

---

## 🏗️ AST (Abstract Syntax Tree)

The compiler builds an internal AST representation for program analysis and validation.

---

# 🔁 Supported Control Structures

```c
if
if-else
while
do-while
for
```

✅ Supports up to **3 nested levels**

---

# 📂 Project Structure

```bash
compilerCG/
│
├── lexer/
│   ├── lexer.c
│   ├── lexer.h
│
├── parser/
│   ├── parser.c
│   ├── parser.h
│
├── semantic/
│   ├── semantic.c
│   ├── semantic.h
│
├── tests/
│
├── main.c
│
└── README.md
```

---

# 🛠️ Technologies Used

| Technology | Purpose |
|---|---|
| C | Main language |
| GCC | Compilation |
| Recursive Descent | Syntax parsing |
| Dynamic Structures | AST & Symbol Table |

---

# 📜 Example `.cg` Program

```cg
Int x;
Int y;

x = 10;
y = 20;

if(y > x)
{
    echo('y is greater');
}
else
{
    echo('x is greater');
}
```

---

# ⚡ Compilation

## 🐧 Linux / MacOS

```bash
gcc main.c lexer/*.c parser/*.c semantic/*.c -o compilercg
```

---

## 🪟 Windows (MinGW)

```bash
gcc main.c lexer/*.c parser/*.c semantic/*.c -o compilercg.exe
```

---

# ▶️ Execution

## Linux / MacOS

```bash
./compilercg file.cg
```

## Windows

```bash
compilercg.exe file.cg
```

---

# 🧪 Example Output

```bash
Lexical analysis completed successfully
Parsing completed successfully
Semantic analysis completed successfully
Compilation finished
```

---

# 🚨 Error Detection

CompilerCG detects:

- ❌ Syntax errors
- ❌ Undeclared variables
- ❌ Invalid assignments
- ❌ Type mismatches
- ❌ Invalid expressions
- ❌ Scope errors

Example:

```bash
Syntax error in line 5:
expected ')' but got ERROR
```

---

# 🧬 Grammar Example

```bnf
program     -> declaration_list

declaration -> type identifier ';'

statement   -> if_statement
             | while_statement
             | assignment
             | block

expression  -> term (( '+' | '-' ) term)*

term        -> factor (( '*' | '/' ) factor)*
```

---

# 🎯 Learning Objectives

This project was developed to practice and understand:

- Compiler Design
- Parsing Techniques
- Syntax Trees
- Semantic Validation
- Formal Grammars
- Symbol Tables
- Language Processing

---

# 📸 Suggested Future Improvements

- [ ] Code generation
- [ ] Intermediate representation
- [ ] Optimization phase
- [ ] Custom virtual machine
- [ ] Better error recovery
- [ ] GUI interface
- [ ] Syntax highlighting

---

# 👨‍💻 Author

## José Rodrigo Cázares Godoy

🎓 Computer Engineering — CUCosta

---

## Professor

**Eduardo Ruben Elias Hernandez Gonzalez**

---

# 📊 Project Status

```diff
+ Lexer
+ Parser
+ AST
+ Semantic Analysis
+ Symbol Table
+ Type Checking
+ Control Structures
- Optimization
- Code Generation
```

---

# 📄 License

This project was developed for **educational purposes only**.

---

<div align="center">

## ⭐ If you like this project, consider giving it a star ⭐

</div>