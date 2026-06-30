# Langage de Programmation Interprété Académique

Un langage de programmation interprété sur lequel j'ai travaillé pendant mes études (avec un mode "compilé" expérimental, plus de détails [ici](https://github.com/kydoz/academic-interpreted-programming-language/blob/main/comp.md)). Actuellement je travaille principalement sur ce mode compilé susmentionné et sur la robustesse globale du programme.  

## Configuration

L'interpréteur peut être construit en exécutant le makefile fourni avec `make` (plus de détails concernant les autres commandes du makefile se trouvent [ici](https://github.com/kydoz/academic-interpreted-programming-language/blob/main/make.md)). De plus, un programme pour vérifier les lexèmes est également disponible, celui-ci peut être construit en exécutant make avec la variable `LEXEME` définie (par exemple `make LEXEME=1`).  

## Utilisation

Deux exécutables peuvent être construits : `test_lexeme` et `test_syntaxe`. Le premier sert à vérifier les lexèmes trouvés dans un programme donné, le second interprète ledit programme. En passant l'option `-c` à `test_syntaxe`, un programme compilé appelé `comp` est produit dans le dossier `bin`.  

## Syntaxe

Les détails concernant la syntaxe peuvent être trouvés dans [compte_rendu.pdf](https://github.com/kydoz/academic-interpreted-programming-language/blob/main/compte_rendu.pdf).  