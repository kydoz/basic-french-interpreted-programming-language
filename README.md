# Academic Interpreted Programming Language

An Interpreted Programming Language I worked on during my undergraduate studies (with an experimental "compiled" mode, more on that [here](https://github.com/kydoz/academic-interpreted-programming-language/blob/main/comp.md)). Nowadays I primarily work on the afore mentioned compiled mode and the overall robustness of the program.  

## Setup

The interpreter can be built by running the provided makefile with `make` (more details concerning other makefile commands can be found [here](https://github.com/kydoz/academic-interpreted-programming-language/blob/main/make.md)). Furthermore, a program to verify lexemas is also available, this can be built by running make with the `LEXEME` variable defined ( for instance `make LEXEME=1`).

## Usage

Two executables can be built: `test_lexeme` and `test_syntaxe`. The former is used to verify lexemas found in a given program, the latter interprets said program. By passing the `-c` option to `test_syntaxe` a compiled program called `comp` is produced in the `bin` folder.  

## Syntax

Details concerning the syntax can be found (in french) in [compte_rendu.pdf](https://github.com/kydoz/academic-interpreted-programming-language/blob/main/compte_rendu.pdf).

## Examples

### Sequence of prime numbers

```
x<-0;
afficher("Nombres primes jusqu'a: ");
lire(x);
i<-2;
tant que i <= x faire
    si i < 2 alors
        Premier<-0
    sinon
        Premier<-1
    fsi;
    j<-2;
    tant que j * j <= i et Premier = 1 faire
        r<-i;
        tant que r >= j faire
            r<-r - j
        ftq;
        si r = 0 alors
            Premier<-0
        fsi;
        j<-j+1
    ftq;
    si Premier = 1 alors
        afficher(i)
    fsi;
    i<-i+1
ftq
```

#### Result

```
➜ ./bin/test_syntaxe Tests/algos/suite_premiers.txt
Nombres primes jusqu'a: 100
2
3
5
7
11
13
17
19
23
29
31
37
41
43
47
53
59
61
67
71
73
79
83
89
97
```

### Bubble sort

```
l<-[11,3,2,7,6,5,4,3,2,1];
n<-lon(l);
i<-0;
tant que i<n faire
    j<-0;
        tant que j<(n-i-1) faire
                si l.j>l.(j+1) alors
                        tmp<-l.j;
                        l.j<-l.(j+1);
                        l.(j+1)<-tmp
                fsi;
                j<-j+1
        ftq;
        i<-i+1
ftq;
afficherTS()
```

#### Result

```
➜ ./bin/test_syntaxe Tests/algos/bubble_sort.txt 
entrée 0:
    Variable: l
    Valeur associée: 0.000000
    Chaine associée: (null)
    Liste taille: 10
    Liste associée: [1, 2, 2, 3, 3, 4, 5, 6, 7, 11]
entrée 1:
    Variable: n
    Valeur associée: 10.000000
    Chaine associée: (null)
    Liste taille: 0
    Liste associée: []
entrée 2:
    Variable: i
    Valeur associée: 10.000000
    Chaine associée: (null)
    Liste taille: 0
    Liste associée: []
entrée 3:
    Variable: j
    Valeur associée: 0.000000
    Chaine associée: (null)
    Liste taille: 0
    Liste associée: []
entrée 4:
    Variable: tmp
    Valeur associée: 2.000000
    Chaine associée: (null)
    Liste taille: 0
    Liste associée: []
```

### Insertion sort

```
l<-[16,3,20,17,3,5,4,9,2,1,0,0];
n<-lon(l);
i<-1;
tant que i < n faire
    cle<-l.i;
    j<-i-1;
    tant que j >= 0 et l.j > cle faire
        l.(j+1)<-l.j;
        j<-j-1
    ftq;
    l.(j+1)<-cle;
    i<-i+1
ftq;
afficherTS()
```

#### Result

```
➜ ./bin/test_syntaxe Tests/algos/insertion.txt 
entrée 0:
    Variable: l
    Valeur associée: 0.000000
    Chaine associée: (null)
    Liste taille: 12
    Liste associée: [0, 0, 1, 2, 3, 3, 4, 5, 9, 16, 17, 20]
entrée 1:
    Variable: n
    Valeur associée: 12.000000
    Chaine associée: (null)
    Liste taille: 0
    Liste associée: []
entrée 2:
    Variable: i
    Valeur associée: 12.000000
    Chaine associée: (null)
    Liste taille: 0
    Liste associée: []
entrée 3:
    Variable: cle
    Valeur associée: 0.000000
    Chaine associée: (null)
    Liste taille: 0
    Liste associée: []
entrée 4:
    Variable: j
    Valeur associée: 0.000000
    Chaine associée: (null)
    Liste taille: 0
    Liste associée: []
```