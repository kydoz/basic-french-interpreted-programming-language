# The (experimental) compiled mode  

During the implementation of this project, I've had an interesting idea circling in the back of my head: how one would make this interpreted language compiled with relative minimal effort. There was this simple solution that I settled on: if there was a way to export the Abstract Syntax Tree (**AST**) generated while interpreting a given program, it could be possible to embed this tree into a C program which would traverse it, which could therefore be compiled. This document details the current state of this solution.  


### Extracting the AST  

The first step of this envisionned compilation process is the extraction of the AST, in this case into a header file. This was enabled by the option of the CLI tool "xxd", which has an option "-i", given a binary file, to output said file as a C header file (more on xxd for example [here](https://manpages.ubuntu.com/manpages/focal/man1/xxd.1.html) or by reading it's man page).  

In the program, this extraction is done as follows:  
First of all, we check if the `-c` or `--compile` argument is defined, if so, the to_compile argument of the analyse function is set to true. If this is the case in the afore mentioned function, the AST produced after interpretation is written to a temporary binary file, which is then converted to a header file using xxd as described above.


### Compilation step  

After extraction discussed above, the program can finally be compiled. The program to compile is "comp.c" which traverses the AST included in "temp.h". The compilation is done by calling make with the COMP variable defined (more on the makefile [here](https://github.com/kydoz/academic-interpreted-programming-language/blob/main/make.md)). The resulting binary is called "comp" and can be found in the bin folder.