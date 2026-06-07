
  /* Inspiré par Spécification de l'analyse lexicale de la V1 de la calculette
         pour l'outil flex */

%{

  /* types et variables utlisés par l'analyseur */

#include <stdio.h>
#include <stdlib.h>
#include "analyse_lexicale.h"
#include <string.h>


   unsigned int numcol=1 ; // numero de colonne courant

   Lexeme LC ;  // lexeme_courant


   /* fonction d'initialisation du lexème courant */
   void initLC () {
		strcpy(LC.chaine, yytext);
		LC.ligne=yylineno;
		LC.colonne=numcol;
		numcol=numcol+yyleng;
	      } ;


    /* ajouter dans ast_construction.c  */
    /*
    size_t longeur_liste(char s[]) {
      size_t i = 0;
      size_t num_virgule = 0;
      while (i < strlen(s))
      {
        if(s[i]==',') num_virgule++;
        i++;
      }
      return ++num_virgule;
    }
    */


  
%}

%option yylineno
%option noyywrap
%option nounput
%option noinput
 /* ajouter option nounput, noinput pour eviter des warnings (on les utilise pas) */
 /* expressions régulières définissant les catégories lexicales */
 /*pour le regle STRING: https://stackoverflow.com/questions/2039795/regular-expression-for-a-string-literal-in-flex-lex*/

    

CHIFFRE [0-9]
AFF  "<-"
SEPINST ";"
CHAR [a-zA-Z]
WHITESPACE [ \t\r\n]*


ENTIER    {CHIFFRE}+
NOMBRE_A_VIRGULES    {ENTIER}[.]{ENTIER} 
OPERATEUR     [-+*/%]
PARO	  "("
PARF	  ")"
POINT   "."

LIRE "lire"
AFFICHER "afficher"
AFFICHER_TS "afficherTS"
SI "si"
ALORS "alors"
SINON "sinon"
FSI "fsi"
TANQUE "tant que"
FAIRE "faire"
FTANQUE "ftq"
LON_LISTE   "lon"
COMP_OPERATEUR "!="|"<="|">="|[<>=]
BOOL_OPERATEUR "et"|"ou"|"non"
STRING ["]([^"\\]|\\.)*["]
IDF   {CHAR}+
ENTIER_LISTE  "["{WHITESPACE}({ENTIER}{WHITESPACE}","{WHITESPACE})*{ENTIER}{WHITESPACE}"]"
NOMBRE_A_VIRGULES_LISTE   "["{WHITESPACE}({NOMBRE_A_VIRGULES}{WHITESPACE}","{WHITESPACE})*{NOMBRE_A_VIRGULES}{WHITESPACE}"]"
STRING_LISTE  "["{WHITESPACE}({STRING}{WHITESPACE}","{WHITESPACE})*{STRING}{WHITESPACE}"]"
%%

  /* actions effectuées par l'analyseur lors de la reconnaissance d'un lexème */

 /* Pour les séparateurs on met juste à jour la valeur de numcol */
[ ]    {numcol = numcol + 1;}
[\t]   {numcol = numcol + 4;}
[\n]   {numcol = 1;}


 /* lexeme de type ENTIER */
{ENTIER}       {
		initLC() ;
		LC.nature = ENTIER; 
		LC.valeur=atoi(yytext);
		LC.partie_fraction=0;
		return 0 ;
		}
 /* lexeme de type NOMBRE_A_VIRGULES */ /*METTRE UNE ESPACE AVANT LES COMMENTAIRES (marche pas sans)*/
{NOMBRE_A_VIRGULES}   {
    initLC();
    LC.nature=NOMBRE_A_VIRGULES;
    LC.valeur=0;
    LC.partie_fraction=atof(yytext);
    return 0;
    }

{STRING} {
    initLC();
    LC.nature=STRING;
    /*suprmier le premier et dernier "*/
    LC.chaine[strlen(LC.chaine)-1]='\0';
    memmove(LC.chaine, LC.chaine+1, strlen(LC.chaine));
    return 0;
}

 /* lexeme de type AFF */
{AFF} {
    initLC();
    LC.nature=AFF;
    return 0;
}

 /*lexeme de type SEPINST*/
{SEPINST} {
    initLC();
    LC.nature=SEPINST;
    return 0;
}

{LIRE} {
    initLC();
    LC.nature=LIRE;
    return 0;
}

{AFFICHER} {
    initLC();
    LC.nature=AFFICHER;
    return 0;
}

{AFFICHER_TS} {
    initLC();
    LC.nature=AFFICHER_TS;
    return 0;
}

{TANQUE} {
    initLC();
    LC.nature=TANQUE;
    return 0;
}

{FAIRE} {
    initLC();
    LC.nature=FAIRE;
    return 0;
}

{FTANQUE} {
    initLC();
    LC.nature=FTANQUE;
    return 0;
}

{LON_LISTE} {
    initLC() ;
		LC.nature = LON_LISTE; 
		return 0 ;
}

{COMP_OPERATEUR} {
    initLC();
      if (strcmp(yytext, "!=")==0)
          LC.nature=COMP_NEQ;
      else if (strcmp(yytext, "<=")==0)
          LC.nature=COMP_LEQ;
      else if (strcmp(yytext, ">=")==0)
          LC.nature=COMP_GEQ;
      else {
          switch(yytext[0]) {
            case '<':
              LC.nature=COMP_LTH;
              break;
            case '>':
              LC.nature=COMP_GTH;
              break;
            case '=':
              LC.nature=COMP_EQ;
              break;
          }
      }
    return 0;
}

{BOOL_OPERATEUR} {
  initLC();
  if (strcmp(yytext, "et")==0)
          LC.nature=BOOL_ET;
      else if (strcmp(yytext, "ou")==0)
          LC.nature=BOOL_OU;
      else 
          LC.nature=BOOL_NON;
  return 0;
}

{SI}  {
    initLC();
    LC.nature=SI;
    return 0;
}

{ALORS}  {
    initLC();
    LC.nature=ALORS;
    return 0;
}

{SINON}  {
    initLC();
    LC.nature=SINON;
    return 0;
}

{FSI}  {
    initLC();
    LC.nature=FSI;
    return 0;
}

 /* lexeme de type IDF */
{IDF}  {
    initLC();
    LC.nature=IDF;
    return 0;
}

 /* lexeme de type OPERATEUR */
{OPERATEUR} 	{
		initLC() ;
		switch (yytext[0]) {
			case '+' : LC.nature = PLUS; break ;
			case '-' : LC.nature = MOINS; break ;
			case '*' : LC.nature = MUL; break ;
			case '/' : LC.nature = DIV; break ;
			case '%':  LC.nature = MOD; break;
		} ;
		return 0 ;
}

{ENTIER_LISTE} {
    initLC();
    LC.nature=ENTIER_LISTE;
    /*suprmier le premier et dernier "*/
    LC.chaine[strlen(LC.chaine)-1]='\0';
    memmove(LC.chaine, LC.chaine+1, strlen(LC.chaine));

    /* ajouter dans partie analyse syntaxique (ast_construction) pour efficaciter
    printf("Longeur de la liste: %lu\n", longeur_liste(LC.chaine));
    LC.entier_liste=calloc(longeur_liste(LC.chaine), sizeof(int));
    char *copie = strdup(LC.chaine);
    LC.entier_liste[0]=atoi(strtok(copie, ","));
    printf("Élément %d de la liste: %d\n", 0, LC.entier_liste[0]);
    for(size_t i=1; i < longeur_liste(LC.chaine); i++) {
      LC.entier_liste[i] = atoi(strtok(NULL, ","));
      printf("Élément %lu de la liste: %d\n", i, LC.entier_liste[i]);
    }
    free(copie);*/
    return 0;
}

{NOMBRE_A_VIRGULES_LISTE} {
    initLC();
    LC.nature=NOMBRE_A_VIRGULES_LISTE;
    /*suprmier le premier et dernier "*/
    LC.chaine[strlen(LC.chaine)-1]='\0';
    memmove(LC.chaine, LC.chaine+1, strlen(LC.chaine));


    /* ajouter dans partie analyse syntaxique (ast_construction) pour efficaciter
    printf("Longeur de la liste: %lu\n", longeur_liste(LC.chaine));
    LC.nombre_a_virgule_liste=calloc(longeur_liste(LC.chaine), sizeof(double));
    char *copie = strdup(LC.chaine);
    LC.nombre_a_virgule_liste[0]=atof(strtok(copie, ","));
    printf("Élément %d de la liste: %lf\n", 0, LC.nombre_a_virgule_liste[0]);
    for(size_t i=1; i < longeur_liste(LC.chaine); i++) {
      LC.nombre_a_virgule_liste[i] = atof(strtok(NULL, ","));
      printf("Élément %lu de la liste: %lf\n", i, LC.nombre_a_virgule_liste[i]);
    }
    free(copie);
    printf("Élément %d de la liste: %lf\n", 0, LC.nombre_a_virgule_liste[0]);
    */
    return 0;
}

{STRING_LISTE} {
    initLC();
    LC.nature=STRING_LISTE;
    /*suprmier le premier et dernier "*/
    LC.chaine[strlen(LC.chaine)-1]='\0';
    memmove(LC.chaine, LC.chaine+1, strlen(LC.chaine));
    //printf("Before: %s\n", LC.chaine);
    //remove_spaces(LC.chaine);
    //printf("After: %s\n", LC.chaine);

    /* ajouter dans partie analyse syntaxique (ast_construction) pour efficaciter
    printf("Longeur de la liste: %lu\n", longeur_liste(LC.chaine));
    LC.string_liste=calloc(longeur_liste(LC.chaine), sizeof(char*)*256);
    char *copie = strdup(LC.chaine);
    LC.string_liste[0]=strdup(strtok(copie, ","));
    LC.string_liste[0][strlen(LC.string_liste[0])-1]='\0';
    memmove(LC.string_liste[0], LC.string_liste[0]+1, strlen(LC.string_liste[0]));
    printf("Élément %d de la liste: %s\n", 0, LC.string_liste[0]);
    for(size_t i=1; i < longeur_liste(LC.chaine); i++) {
      LC.string_liste[i]=strdup(strtok(NULL, ","));
      printf("%s\n", LC.string_liste[i]);
      LC.string_liste[i][strlen(LC.string_liste[i])-1]='\0';
      memmove(LC.string_liste[i], LC.string_liste[i]+2, strlen(LC.string_liste[i]));
      printf("Élément %lu de la liste: %s\n", i, LC.string_liste[i]);
    }
    free(copie);
    */


    return 0;
}




 /* lexeme de type PARO */
{PARO} 	{
		initLC() ;
		LC.nature = PARO; 
		return 0 ;
		}

 /* lexeme de type PARF */
{PARF} 	{
		initLC() ;
		LC.nature = PARF; 
		return 0 ;
		}

{POINT} {
  initLC() ;
		LC.nature = POINT; 
		return 0 ;
}

 /* lexeme de type FIN_DE_SEQUENCE  */
<<EOF>>     {
		initLC() ;	
		LC.nature=FIN_SEQUENCE ; 
		return 0 ;
            }
        


 /* Tous les autres cas  sont des erreurs lexicales  ...  */

.               {
		initLC() ;	
		LC.nature=ERREUR ;
		return 0 ;
	        }

%%

	/* fonctions auxilliaires fournies/utilisées par l'analyseur */

// renvoie la chaine de caracteres correspondant a la nature du lexeme
char *Nature_vers_Chaine (Nature_Lexeme nature) {
        switch (nature) {
                case ENTIER: return "ENTIER" ;
		case NOMBRE_A_VIRGULES: return "NOMBRE_A_VIRGULES";
                case PLUS: return "PLUS" ;
                case MOINS: return "MOINS" ;
                case MUL: return "MUL" ;
                case DIV: return "DIV" ;
		case MOD: return "MOD";
                case PARO: return "PARO" ;
                case PARF: return "PARF" ;
                case FIN_SEQUENCE: return "FIN_SEQUENCE" ;
		case IDF: return "IDF";
		case AFFICHER: return "AFFICHER";
		case LIRE: return "LIRE";
		case AFF: return "AFF";
    case SI: return "SI";
    case ALORS: return "ALORS";
    case SINON: return "SINON";
    case FSI: return "FSI";
    case TANQUE: return "TANQUE";
    case FAIRE: return "FAIRE";
    case FTANQUE: return "FTANQUE";
    case COMP_EQ: case COMP_GEQ: case COMP_GTH: case COMP_LEQ: case COMP_LTH: case COMP_NEQ: return "OPCOMP";
    case BOOL_ET: case BOOL_NON: case BOOL_OU: return "BOOL_COMP";
		case SEPINST: return "SEPINST";
    case STRING:  return "STRING";
    case AFFICHER_TS: return "AFFICHER_TS";
    case ENTIER_LISTE: return "ENTIER_LISTE";
    case NOMBRE_A_VIRGULES_LISTE: return "NOMBRE_A_VIRGULES_LISTE";
    case STRING_LISTE: return "STRING_LISTE";
    case POINT: return "POINT";
    case LON_LISTE: return "LON_LISTE";
                default: return "ERREUR" ;
        } ;
   }

// affichage d'un lexème 
void afficher(Lexeme l) {

      switch (l.nature) {
         case FIN_SEQUENCE:
            break;
         default:
            printf("ligne: %d, ", l.ligne);
            printf("colonne: %d, ", l.colonne);
            printf("[") ;
            printf("nature = %s", Nature_vers_Chaine(l.nature)) ;
            printf(", chaine = %s, ", l.chaine) ;
            switch(l.nature) {
                 case ENTIER:
                      printf("valeur = %d", l.valeur); break;
                 case NOMBRE_A_VIRGULES:
                      printf("valeur = %lf", l.partie_fraction); break;
                 default:
                      break;
            } ;
            printf("]\n") ;
        } ;
   }


// lecture du lexeme suivant
void avancer() {
  yylex() ;  // fonction générée automatiquement !
} 

// valeur du lexeme courant
Lexeme lexeme_courant() {
  return LC ;
}

// fin de sequence de lexemes
int fin_de_sequence() {
      return lexeme_courant().nature == FIN_SEQUENCE;
}


// ouverture du fichier d'entrée (s'il existe)
void demarrer(char *nom_fichier) {
  if (strlen(nom_fichier) != 0) {
	yyin = fopen(nom_fichier, "r") ;
	if (!yyin) {
          printf("erreur ouverture fichier %s\n", nom_fichier) ;
	   exit(1) ;
       } 
  } ;
  avancer() ; // lecture du 1er lexeme
}

// fermeture du fichier d'entrée (s'il existe)
void arreter() {
	if (yyin != stdin) fclose(yyin) ;
	yylex_destroy(); //libere input buffer, https://stackoverflow.com/questions/43671389/minimal-bison-flex-generated-code-has-memory-leak
}

