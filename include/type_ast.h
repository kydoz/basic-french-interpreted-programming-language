
#ifndef __TYPE_AST__
#define __TYPE_AST__ 
#include <stdio.h>
typedef enum {OPERATION, VALEUR, INSTRUCTION} TypeAst ;
typedef enum {N_PLUS, N_MUL, N_MOINS, N_DIV, N_MOD, N_SEPINST, N_IDF, N_AFF, N_LIRE, N_AFFICHER, N_IF, N_EGAL, N_GTH, N_LEQ, N_LTH, N_GEQ, N_NEQ, N_ET, N_OU, N_NON, N_TANQUE, N_AFFICHER_TS, N_STRING, N_CHAR, N_ENTIER_LISTE, N_NOMBRE_A_VIRGULE_LISTE, N_STRING_LISTE, N_LISTE_INDICE, N_LON_LISTE} TypeOperateur ;

typedef struct noeud { 
	TypeAst nature ;
	TypeOperateur operateur ;
	struct noeud *gauche, *droite, *central ;
      	int valeur ;
		char * chaine;
		double partie_fraction;
		int *entier_liste;
      double *nombre_a_virgule_liste;
      char **string_liste;
	  size_t taille_liste;
} NoeudAst ;

typedef NoeudAst *Ast ;

#endif  
