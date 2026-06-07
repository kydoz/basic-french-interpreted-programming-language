
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
		// demarre l'analyse lexicale sur le fichier transmis en argument
        /*
        double *res_frac=(double*)malloc(sizeof(double)); 
            if (res_frac == NULL)
            {
                    fprintf(stderr, "Erreur allocation memoire\n");
                    return 1;
            }*/
            analyser(argv[1], false);
            /*
                if (*res_frac-(int)*res_frac==0) //regarder si on a un int
                    printf("Le résultat: %d\n", (int)*res_frac);    
                else {
                    printf("Le résultat: %f \n", *res_frac);
                }
                    free(res_frac);*/
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
