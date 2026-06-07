#ifndef __AST_PARCOURS__
#define __AST_PARCOURS__
#include <stdbool.h>
#include "type_ast.h"


void afficher_arbre(Ast expr) ;
// affiche l'arbre abstrait de l'expression arithmetique expr

double evaluation(Ast expr) ;
// calcule la valeur de l'expression arithmetique expr
// FONCTION A COMPLETER !

void interpreter(Ast A);
void interpreter_aff(Ast A);
void interpreter_lire(Ast A);
void interpreter_affich(Ast A);
void interpreter_si_alors_sinon(Ast A);
bool valeur_booleenne(Ast A);
bool evaluation_bool(Ast expr);
void interpreter_while(Ast A);
void liberer_arbre(Ast A);


#endif

