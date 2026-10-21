# Lexical Analyzer for SFort95

A lexical analyzer in C++ for SFort95, a small subset of Fortran 95. It was the first programming assignment in NJIT's CS280 (Programming Language Concepts).

This is part 1 of a three-part course project:

1. This repo turns source text into tokens.
2. [Parser for SFort95](https://github.com/iaalcantara17/Parser-for-SFort95-Language) checks the token stream against the grammar.
3. [SFort95 Interpreter](https://github.com/iaalcantara17/SFort95-Interpreter) runs the program as it parses it.

## What it does

`getNextToken` reads the source one character at a time and decides what to build from the first character it sees:

- Letters start an identifier or a keyword. The keywords it knows are `program`, `end`, `if`, `else`, `integer`, `real`, `character`, `print`, and `len`.
- Digits start an integer, or a real number when a `.` is followed by more digits.
- A `"` or `'` starts a string, which must close on the same line.
- `+ - * / = < > , ( ) .` each become a one-character token.
- `!` starts a comment that runs to the end of the line.
- Anything else is an error, and the program prints the line number and the bad lexeme.

`main` takes one file name and optional flags. `-all` prints every token as it's found. `-id`, `-kw`, `-int`, `-real`, and `-str` print the identifiers, keywords, integers, reals, and strings seen in the file. It also reports a missing file name, a file it can't open, an unknown flag, more than one file name, and an empty file.

## Files

| File | What it is |
| --- | --- |
| `PA_1_I_Alcantara.cpp` | My lexer and the `main` driver |
| `lex.h` | The token list and the `LexItem` class, with a CS280 header |
| `PA 1 Test Cases/` | Test inputs, each with a `.correct` file of the expected output |

## How to build and run

You need a C++ compiler such as `g++`. The repo builds on its own.

```sh
g++ -std=c++17 -o lexer PA_1_I_Alcantara.cpp
./lexer "PA 1 Test Cases/integers" -all -int
```

## Status

This is the version I committed for the first assignment, and it doesn't pass every test case in the folder:

- Keyword matching is case-sensitive, so `PROGRAM` in uppercase is read as an identifier.
- Two-character operators such as `::`, `**`, `//`, and `==` aren't handled, so `:` is reported as an error.
- `-all` prints keyword and operator tokens as numbers instead of names.
- It doesn't print the line and token counts that the `.correct` files expect.
