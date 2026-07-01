# Langage de Programmation Interprété Académique

Un langage de programmation interprété sur lequel j'ai travaillé pendant mes études (avec un mode "compilé" expérimental, plus de détails [ici](https://github.com/kydoz/academic-interpreted-programming-language/blob/main/comp.md)). Actuellement je travaille principalement sur ce mode compilé susmentionné et sur la robustesse globale du programme.  

## Configuration

L'interpréteur peut être construit en exécutant le makefile fourni avec `make` (plus de détails concernant les autres commandes du makefile se trouvent [ici](https://github.com/kydoz/academic-interpreted-programming-language/blob/main/make.md)). De plus, un programme pour vérifier les lexèmes est également disponible, celui-ci peut être construit en exécutant make avec la variable `LEXEME` définie (par exemple `make LEXEME=1`).  

## Utilisation

Deux exécutables peuvent être construits : `test_lexeme` et `test_syntaxe`. Le premier sert à vérifier les lexèmes trouvés dans un programme donné, le second interprète ledit programme. En passant l'option `-c` à `test_syntaxe`, un programme compilé appelé `comp` est produit dans le dossier `bin`.  

## Syntaxe

Les détails concernant la syntaxe peuvent être trouvés dans [compte_rendu.pdf](https://github.com/kydoz/academic-interpreted-programming-language/blob/main/compte_rendu.pdf).  

## Exemples

### Suite de nombres premiers

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

#### Résultat

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

### Tri à bulles

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

#### Résultat

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

### Tri par Insertion

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

#### Résultat

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