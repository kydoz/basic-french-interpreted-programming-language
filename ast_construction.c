#include <stdlib.h>
#include <stdio.h> 
#include <string.h>

#include "type_ast.h"

Ast creer_operation(TypeOperateur opr , Ast opde_gauche , Ast opde_droit) 
{
      if (opde_gauche == NULL && opde_droit == NULL) {
                  //printf("ERREUR_EXPRESSION_DROIT_ARBRE_NULL\n");
                  //printf("ERREUR_EXPRESSION_GAUCHE_ARBRE_NULL\n");
      
      
      }
      Ast expr ;
      //calloc initialise la mémoire allouée a 0, utiliser pour éviter des erreurs 
      //"Uninitialised value was created by a heap allocation" de valgrind 
      expr = (Ast) calloc (1, sizeof(NoeudAst)); 
      expr->chaine=NULL;
      expr->nature = OPERATION;
      expr->operateur = opr;
         expr->gauche = opde_gauche;
         expr->droite = opde_droit;
         expr->central=NULL;
      return expr ;
}

Ast creer_valeur(int val, double partie_fraction) { 
      Ast expr ;
      expr = (Ast) calloc (1, sizeof(NoeudAst));
      //expr = (Ast) malloc(sizeof(NoeudAst));
      expr->chaine=NULL;
      expr->nature = VALEUR;
      expr->valeur = val;
      expr->partie_fraction=partie_fraction;
      expr->gauche=NULL;
      expr->droite=NULL;
      expr->central=NULL;
      return expr ;
}

Ast creer_sepinst(Ast AG, Ast AD) {
      if (AG == NULL || AD == NULL) {
            if (AG==NULL)
            {
                  //printf("ERREUR_EXPRESSION_DROIT_ARBRE_NULL\n");
            } if(AD==NULL) {
                  //printf("ERREUR_EXPRESSION_GAUCHE_ARBRE_NULL\n");
            }
	 
      } 
      Ast sepinst ;
      sepinst = (Ast) calloc (1, sizeof(NoeudAst));
      sepinst->chaine=NULL;
      sepinst->nature = INSTRUCTION;
      sepinst->operateur = N_SEPINST;
         sepinst->gauche = AG;
         sepinst->droite = AD;
         sepinst->central=NULL;
      ;
      return sepinst ;

}

Ast creer_idf(char *chaine, Ast A_INDICE) {
      Ast idf;
      idf=(Ast) calloc (1, sizeof(NoeudAst));
      idf->nature = INSTRUCTION;
      if (A_INDICE==NULL)
      {
            idf->operateur=N_IDF;
      } else {
            idf->operateur=N_LISTE_INDICE;
      }
      idf->chaine=strdup(chaine);
      idf->gauche=A_INDICE;
      idf->droite=NULL;
      idf->central=NULL;
      return idf;
}

Ast creer_aff(Ast AG, Ast AD) {
      if (AG == NULL || AD == NULL) {
            if (AG==NULL)
            {
                  //printf("ERREUR_EXPRESSION_DROIT_ARBRE_NULL\n");
            } if(AD==NULL) {
                  //printf("ERREUR_EXPRESSION_GAUCHE_ARBRE_NULL\n");
            }
	 
      } 
      Ast aff ;
      aff = (Ast) calloc (1, sizeof(NoeudAst));
      aff->chaine=NULL;
      aff->nature = INSTRUCTION;
      aff->operateur = N_AFF;
         aff->gauche = AG;
         aff->droite = AD;
         aff->central=NULL;
      return aff ;
}

Ast creer_lire(Ast AG) {
      if (AG == NULL) {
            //printf("ERREUR_EXPRESSION_DROIT_ARBRE_NULL\n");
	      
      } 
      Ast lire ;
      lire = (Ast) calloc (1, sizeof(NoeudAst));
      lire->chaine=NULL;
      lire->nature = INSTRUCTION;
      lire->operateur = N_LIRE;
         lire->gauche = AG;
         lire->droite=NULL;
         lire->central=NULL;
      return lire ;
}

Ast creer_affich(Ast AG) {
      if (AG == NULL) {
            //printf("ERREUR_EXPRESSION_DROIT_ARBRE_NULL\n");
	      
      }
      Ast affich ;
      affich = (Ast) calloc (1, sizeof(NoeudAst));
      affich->chaine=NULL;
      affich->nature = INSTRUCTION;
      affich->operateur = N_AFFICHER;
         affich->gauche = AG;
         affich->droite=NULL;
         affich->central=NULL;
      return affich ;
}

Ast creer_if(Ast AG, Ast AC, Ast AD) {
      if (AG==NULL)
      {
            //printf("ERREUR manque arbre condition\n");
            
      }
      Ast si ;
      si = (Ast) calloc (1, sizeof(NoeudAst));
      si->nature = INSTRUCTION;
      si->operateur = N_IF;
      si->chaine=NULL;
         si->gauche = AG;
         si->droite = AD;
         si->central=AC;
      
      return si ;
}

Ast creer_tanque(Ast Acond, Ast Abody) {
      if (Acond==NULL)
      {
            //printf("ERREUR manque arbre condition\n");
            
      }
      Ast tanque;
      tanque = (Ast) calloc (1, sizeof(NoeudAst));
      tanque->nature=INSTRUCTION;
      tanque->chaine=NULL;
      tanque->operateur=N_TANQUE;
         tanque->gauche = Acond;
         tanque->droite = Abody;
         tanque->central=NULL;
      return tanque ;
}

Ast creer_affich_ts() {
      Ast ts;
      ts=(Ast) calloc (1, sizeof(NoeudAst));
      ts->nature = INSTRUCTION;
      ts->operateur=N_AFFICHER_TS;
      ts->chaine=NULL;
      ts->gauche=NULL;
      ts->droite=NULL;
      ts->central=NULL;
      return ts;
}

Ast creer_string(char *s) {
      Ast string;
      string=(Ast) calloc (1,sizeof(NoeudAst)); //
      string->nature = VALEUR;
      string->operateur=N_STRING;
      string->chaine = strdup(s);
      string->gauche=NULL;
      string->droite=NULL;
      string->central=NULL;
      return string;
}

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

//N_ENTIER_LISTE, N_NOMBRE_A_VIRGULE_LISTE, N_STRING_LISTE

Ast creer_entier_liste(char s[]) {
    Ast liste = (Ast) calloc (1,sizeof(NoeudAst)) ;

      liste->nature = VALEUR;
      liste->operateur=N_ENTIER_LISTE;
      liste->chaine = strdup(s);
      liste->gauche=NULL;
      liste->droite=NULL;
      liste->central=NULL;


    //printf("Longeur de la liste: %lu\n", longeur_liste(s));
    liste->taille_liste=longeur_liste(s);
    liste->entier_liste=calloc(liste->taille_liste, sizeof(int));
    char *copie = strdup(s);
    liste->entier_liste[0]=atoi(strtok(copie, ","));
    //printf("Élément %d de la liste: %d\n", 0, liste->entier_liste[0]);
    for(size_t i=1; i < longeur_liste(s); i++) {
      liste->entier_liste[i] = atoi(strtok(NULL, ","));
      //printf("Élément %lu de la liste: %d\n", i, liste->entier_liste[i]);
    }
    free(copie);
    return liste;
}

Ast creer_nombre_a_virgule_liste(char s[]) {
    Ast liste = (Ast) calloc (1,sizeof(NoeudAst)) ;

      liste->nature = VALEUR;
      liste->operateur=N_NOMBRE_A_VIRGULE_LISTE;
      liste->chaine = strdup(s);
      liste->gauche=NULL;
      liste->droite=NULL;
      liste->central=NULL;


    //printf("Longeur de la liste: %lu\n", longeur_liste(s));
    liste->taille_liste=longeur_liste(s);
    liste->nombre_a_virgule_liste=calloc(liste->taille_liste, sizeof(double));
    char *copie = strdup(s);
    liste->nombre_a_virgule_liste[0]=atof(strtok(copie, ","));
    //printf("Élément %d de la liste: %lf\n", 0, liste->nombre_a_virgule_liste[0]);
    for(size_t i=1; i < longeur_liste(s); i++) {
      liste->nombre_a_virgule_liste[i] = atof(strtok(NULL, ","));
      //printf("Élément %lu de la liste: %lf\n", i, liste->nombre_a_virgule_liste[i]);
    }
    free(copie);
    //printf("Élément %d de la liste: %lf\n", 0, liste->nombre_a_virgule_liste[0]);
    return liste;
}

Ast creer_string_liste(char s[]) {
      Ast liste = (Ast) calloc (1,sizeof(NoeudAst)) ;

      liste->nature = VALEUR;
      liste->operateur=N_STRING_LISTE;
      liste->chaine = strdup(s);
      liste->gauche=NULL;
      liste->droite=NULL;
      liste->central=NULL;

      //printf("Longeur de la liste: %lu\n", longeur_liste(s));
    liste->taille_liste=longeur_liste(s);
    liste->string_liste=calloc(liste->taille_liste, sizeof(char*)*256);
    char *copie = strdup(s);
    liste->string_liste[0]=strdup(strtok(copie, ","));
    liste->string_liste[0][strlen(liste->string_liste[0])-1]='\0';
    memmove(liste->string_liste[0], liste->string_liste[0]+1, strlen(liste->string_liste[0]));
    //printf("Élément %d de la liste: %s\n", 0, liste->string_liste[0]);
    for(size_t i=1; i < longeur_liste(s); i++) {
      liste->string_liste[i]=strdup(strtok(NULL, ","));
      //printf("%s\n", liste->string_liste[i]);
      liste->string_liste[i][strlen(liste->string_liste[i])-1]='\0';
      memmove(liste->string_liste[i], liste->string_liste[i]+2, strlen(liste->string_liste[i]));
      //printf("Élément %lu de la liste: %s\n", i, liste->string_liste[i]);
    }
    free(copie);
    return liste;

}

Ast creer_liste_indice(char *s, int i) {
      Ast liste_indice = (Ast) calloc (1,sizeof(NoeudAst)) ;

      liste_indice->nature = VALEUR;
      liste_indice->operateur=N_LISTE_INDICE;
      liste_indice->chaine = strdup(s);
      liste_indice->valeur=i;
      liste_indice->gauche=NULL;
      liste_indice->droite=NULL;
      liste_indice->central=NULL;

      return liste_indice;
}



Ast creer_lon_liste(Ast AG) {
      if (AG == NULL) {
            //printf("ERREUR_EXPRESSION_DROIT_ARBRE_NULL\n");
	      
      } 
      Ast lon ;
      lon = (Ast) calloc (1, sizeof(NoeudAst));
      lon->chaine=NULL;
      lon->nature = INSTRUCTION;
      lon->operateur = N_LON_LISTE;
         lon->gauche = AG;
         lon->droite=NULL;
         lon->central=NULL;
      return lon ;
}