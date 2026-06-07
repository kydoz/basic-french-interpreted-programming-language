#include "table_symbole.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Couple TS[NBMAXSYMB] ;  // la variable Table des Symboles
int NbSymb ; 		        // le nombre courant de couples dans la table

void initTS() {
    NbSymb=0;
}
// intialise la TS en effectant 0 à NbSymb

void afficheTS() {
    for (size_t i = 0; i < NbSymb; i++)
    {
        printf("entrée %ld:\n", i);
        printf("    Variable: %s\n", TS[i].nom);
        printf("    Valeur associée: %f\n", TS[i].valeur);
        printf("    Chaine associée: %s\n", TS[i].string);
        printf("    Liste taille: %lu\n",TS[i].taille_liste);  
        printf("    Liste associée: [");
        if (TS[i].entier_liste!=NULL)
        {
            for (size_t x = 0; x <TS[i].taille_liste; x++)
            {
                printf("%d", TS[i].entier_liste[x]);
                if (x!=TS[i].taille_liste-1)
                {
                    printf(", ");
                }
                
            }
            
        } else if (TS[i].nombre_a_virgule_liste!=NULL) {
            for (size_t x = 0; x <TS[i].taille_liste; x++)
            {
                printf("%lf", TS[i].nombre_a_virgule_liste[x]);
                if (x!=TS[i].taille_liste-1)
                {
                    printf(", ");
                }
                
            }
        } else if (TS[i].string_liste!=NULL) {
            for (size_t x = 0; x <TS[i].taille_liste; x++)
            {
                printf("%s", TS[i].string_liste[x]);
                if (x!=TS[i].taille_liste-1)
                {
                    printf(", ");
                }
                
            }
        }
        printf("]\n");
        

    }
    
}
// affiche le contenu complet de la TS 

int estPresentTS(char *idf, double *v, char *s, int *entier_liste, double * nombre_a_virgule_liste, char **string_liste, int*liste_indice) {
    for (size_t i = 0; i < NbSymb; i++)
    {
        if (strcmp(TS[i].nom, idf)==0)
        {
            if (s!=NULL)
                //s=strdup(TS[i].string);
                strcpy(s, TS[i].string);
            /*
            else if (entier_liste!=NULL)
                entier_liste=TS[i].entier_liste;
            else if (nombre_a_virgule_liste!=NULL)
                nombre_a_virgule_liste=TS[i].nombre_a_virgule_liste;
            else if (string_liste!=NULL)
                string_liste=TS[i].string_liste;*/
            else if (liste_indice!=NULL) {
                if (*liste_indice>=TS[i].taille_liste)
                {
                    return 0;
                }
                
                if (TS[i].entier_liste!=NULL)
                {
                    *v=TS[i].entier_liste[*liste_indice];
                } else if (TS[i].nombre_a_virgule_liste!=NULL) {
                    *v=TS[i].nombre_a_virgule_liste[*liste_indice];
                } /*else if (TS[i].string_liste!=NULL)
                {
                    strcpy(s, TS[i].string_liste[*liste_indice]);
                } */else { //on n'a pas implemente les strings, pourrait poser des problemes lorsequ'on appelle de l'evaluation
                    return 0;
                }
            }
            else 
                *v=TS[i].valeur;
            return 1;
        }
        
    }
    return 0;
}
// si un couple (idf, v') est present dans la table alors 
//      affecte v' à *v 
//      estPresentTS renvoie vrai
// sinon
//      estPresentTS renvoie faux

void insererTS(char *idf, double v, char *s, int *entier_liste, double * nombre_a_virgule_liste, char **string_liste, size_t taille_liste, int**liste_indice) {
    for (size_t i = 0; i < NbSymb; i++)
    {
        if (strcmp(TS[i].nom, idf)==0)
        {
            if (s!=NULL) {

                TS[i].string=strdup(s);
                TS[i].valeur=0;
                TS[i].entier_liste=NULL;
                TS[i].nombre_a_virgule_liste=NULL;
                TS[i].string_liste=NULL;
                TS[i].taille_liste=0;

                
            } if (liste_indice==NULL) {
                TS[i].valeur=v;
                TS[i].string=NULL;
            } else {
                if (**liste_indice>=TS[i].taille_liste)
                {
                    //indice trop grand
                    *liste_indice=NULL;
                    return;
                }
            }
            if (entier_liste!=NULL) {

                TS[i].valeur=0;
                TS[i].string=NULL;
                TS[i].entier_liste=calloc(taille_liste, sizeof(int));
                //TS[i].entier_liste=entier_liste;
                memcpy(TS[i].entier_liste, entier_liste, taille_liste*sizeof(int));
                TS[i].nombre_a_virgule_liste=NULL;
                TS[i].string_liste=NULL;
                TS[i].taille_liste=taille_liste;
            } else if (TS[i].entier_liste!=NULL) {
                TS[i].entier_liste[**liste_indice]=(int)v;
            }

            if (nombre_a_virgule_liste!=NULL) {
                TS[i].valeur=0;
                TS[i].string=NULL;
                TS[i].entier_liste=NULL;
                TS[i].nombre_a_virgule_liste=calloc(taille_liste, sizeof(double));
                memcpy(TS[i].nombre_a_virgule_liste, nombre_a_virgule_liste, taille_liste*sizeof(double));
                TS[i].string_liste=NULL;
                TS[i].taille_liste=taille_liste;
            } else if (TS[i].nombre_a_virgule_liste!=NULL) {
                TS[i].nombre_a_virgule_liste[**liste_indice]=v;
            } 
            if (string_liste!=NULL) {
                TS[i].valeur=0;
                TS[i].string=NULL;
                TS[i].entier_liste=NULL;
                TS[i].nombre_a_virgule_liste=NULL;
                TS[i].string_liste=calloc(taille_liste, sizeof(char*)*256);
                memcpy(TS[i].string_liste, string_liste, taille_liste*sizeof(char*)*256);
                TS[i].taille_liste=taille_liste;
            } else if (TS[i].string_liste!=NULL) {
                strcpy(TS[i].string_liste[**liste_indice], s);
            }
            return;
        }
    }
    strcpy(TS[NbSymb].nom, idf);
    if (s!=NULL) {

                TS[NbSymb].string=strdup(s);
                TS[NbSymb].valeur=0;
                TS[NbSymb].entier_liste=NULL;
                TS[NbSymb].nombre_a_virgule_liste=NULL;
                TS[NbSymb].string_liste=NULL;
                TS[NbSymb].taille_liste=0;

                
            } else if (entier_liste!=NULL) {
                TS[NbSymb].valeur=0;
                TS[NbSymb].string=NULL;
                TS[NbSymb].entier_liste=calloc(taille_liste, sizeof(int));
                //TS[NbSymb].entier_liste=entier_liste;
                memcpy(TS[NbSymb].entier_liste, entier_liste, taille_liste*sizeof(int));
                TS[NbSymb].nombre_a_virgule_liste=NULL;
                TS[NbSymb].string_liste=NULL;
                TS[NbSymb].taille_liste=taille_liste;
            } else if (nombre_a_virgule_liste!=NULL) {
                TS[NbSymb].valeur=0;
                TS[NbSymb].string=NULL;
                TS[NbSymb].entier_liste=NULL;
                TS[NbSymb].nombre_a_virgule_liste=calloc(taille_liste, sizeof(double));
                memcpy(TS[NbSymb].nombre_a_virgule_liste, nombre_a_virgule_liste, taille_liste*sizeof(double));
                TS[NbSymb].string_liste=NULL;
                TS[NbSymb].taille_liste=taille_liste;
            } else if (string_liste!=NULL) {
                TS[NbSymb].valeur=0;
                TS[NbSymb].string=NULL;
                TS[NbSymb].entier_liste=NULL;
                TS[NbSymb].nombre_a_virgule_liste=NULL;
                TS[NbSymb].string_liste=calloc(taille_liste, sizeof(char*)*256);
                memcpy(TS[NbSymb].string_liste, string_liste, taille_liste*sizeof(char*)*256);
                TS[NbSymb].taille_liste=taille_liste;
            }
            else {
                TS[NbSymb].valeur=v;
                TS[NbSymb].string=NULL;
            }
    NbSymb++;
    
}
// si un couple (idf,v') est present dans la table alors 
//      remplace la valeur v' par v 
// sinon
//      ajoute le couple (idf,v) dans la TS


void TS_val_taille_liste(char *idf, int *t) {
    for (size_t i = 0; i < NbSymb; i++)
    {
        if (strcmp(TS[i].nom, idf)==0)
        {
            *t=TS[i].taille_liste;
            return;
        }
    }
    *t=-1;
}

void liberer_TS() {
    for (size_t i = 0; i < NbSymb; i++) {
        if (TS[i].entier_liste!=NULL) free(TS[i].entier_liste);
        if (TS[i].nombre_a_virgule_liste!=NULL) free(TS[i].nombre_a_virgule_liste);
        if (TS[i].string_liste!=NULL) {
            for (size_t x = 0; x < TS[i].taille_liste; x++) {
                free(TS[i].string_liste[x]);
            }
            free(TS[i].string_liste);
        }
    }
}

