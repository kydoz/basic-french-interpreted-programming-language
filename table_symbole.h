

#ifndef ___TS___
#define ___TS___
#include <stddef.h>
#define LIDF 20  // nombre maximum de caractéres pour un IDF 
#define NBMAXSYMB 100  // nombre maximum d'IDF 

typedef struct {
   char nom[LIDF] ; // nom d'un identificateur
   double valeur ;     // valeur de cet identificateur
   char* string;
   int *entier_liste;
   double *nombre_a_virgule_liste;
   char **string_liste;
   size_t taille_liste;
} Couple ;


extern void initTS() ; 
// intialise la TS en effectant 0 à NbSymb

extern void afficheTS() ;
// affiche le contenu complet de la TS 

extern int estPresentTS(char *idf, double *v, char *s, int *entier_liste, double * nombre_a_virgule_liste, char **string_liste, int*liste_indice) ;
// si un couple (idf, v') est present dans la table alors 
//      affecte v' à *v 
//      estPresentTS renvoie vrai
// sinon
//      estPresentTS renvoie faux


extern void insererTS(char *idf, double v, char *s, int *entier_liste, double * nombre_a_virgule_liste, char **string_liste, size_t taille_liste, int**liste_indice) ;
// si un couple (idf,v') est present dans la table alors 
//      remplace la valeur v' par v 
// sinon
//      ajoute le couple (idf,v) dans la TS


extern void TS_val_taille_liste(char *idf, int *t);
extern void liberer_TS();

#endif