# Academic Interpreted Programming Language

An Interpreted Programming Language I worked on during my undergraduate studies (with an experimental "compiled" mode, more on that [here](https://github.com/kydoz/academic-interpreted-programming-language/blob/main/comp.md)). Nowadays I primarily work on this afore mentioned compiled mode and the overall robustness of the program.  

## Setup

The interpreter can be built by running the provided makefile with `make` (more details concerning other makefile commands can be found [here](https://github.com/kydoz/academic-interpreted-programming-language/blob/main/make.md)). Furthermore, a program to verify lexemas is also available, this can be built by running make with the `LEXEME` variable defined ( for instance `make LEXEME=1`).

## Usage

Two executables can be built: `test_lexeme` and `test_syntaxe`. The former is used to verify lexemas found in a given program, the latter interprets said program. By passing the `-c` option to `test_syntaxe` a compiled program called `comp` is produced in the `bin` folder.  

## Syntax

Details concerning the syntax can be found (in french) in [compte_rendu.pdf](https://github.com/kydoz/academic-interpreted-programming-language/blob/main/compte_rendu.pdf).
