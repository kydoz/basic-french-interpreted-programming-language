
/* Exemple d'utilisation de l'analyse lexicale : 
    Affichage d'une sequence de lexemes            */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "analyse_lexicale.h"
#include "analyse_syntaxique.h"

int main (int argc, char *argv[]) {

   switch (argc) {
        case 2:
            analyser(argv[1], false);
                    break ;
        case 3:
            if (strcmp(argv[1], "-c")==0 || strcmp(argv[1], "--compile")==0)
            {
                analyser(argv[2], true);
                break;
            }
        default:
                printf("Usage: %s <fichier>\n", argv[0]); 
                printf("nombre d'arguments incorrects !\n");
                exit(1) ;
   } ;
   return 0 ;
}
