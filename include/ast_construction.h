
#ifndef __AST_CONSTRUCTION__
#define __AST_CONSTRUCTION__

#include "type_ast.h"

Ast creer_operation(TypeOperateur opr , Ast opde_gauche , Ast opde_droit) ;
// renvoie un arbre abstrait de nature OPERATEUR,  d'operateur opr 
// et ayant opde_gauche et opde_droit pour fils gauche et droit

Ast creer_valeur(int val, double partie_fraction) ; 
// renvoie un arbre abstrait "feuille", de nature OPERATION  
// et de valeur val

Ast creer_sepinst(Ast AG, Ast AD);

Ast creer_idf(char *chaine, Ast A_INDICE);

Ast creer_aff(Ast AG, Ast AD);

Ast creer_lire(Ast AG);

Ast creer_affich(Ast AG);

Ast creer_if(Ast AG, Ast AC, Ast AD);

Ast creer_tanque(Ast Acond, Ast Abody);

Ast creer_affich_ts();

Ast creer_string(char *s);

Ast creer_entier_liste(char s[]);

Ast creer_nombre_a_virgule_liste(char s[]);

Ast creer_string_liste(char s[]);

Ast creer_liste_indice(char *s, int i);

Ast creer_lon_liste(Ast AG);

#endif
