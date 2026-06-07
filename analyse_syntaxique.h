#include "ast_construction.h"
#include "ast_parcours.h"
#include <stdbool.h>
#include "table_symbole.h"
#include <ctype.h>




void analyser(char *, bool);
void rec_eag(Ast*);
void rec_seq_terme(Ast*);
void rec_suite_seq_terme(Ast, Ast*);
void rec_terme(Ast*);
void rec_seq_facteur(Ast*);
void rec_suite_seq_facteur(Ast, Ast*);
void rec_facteur(Ast*);
void rec_op1(TypeOperateur*);
void rec_op2(TypeOperateur*);
void rec_aff();
void rec_inst(Ast *);
void rec_bloque_si(Ast *A);
void rec_seq_inst(Ast *);
void rec_suite_seq_inst(Ast, Ast*);
void rec_affich();
void rec_lire();
void rec_condition(Ast*);
void rec_seq_if(Ast *A);
void rec_suite_seq_if(Ast *Aelse);
void rec_if(Ast *Acond, Ast *Athen);

//bool calc
void rec_logique_eag(Ast *A);
void rec_seq_logique_terme(Ast *A);
void rec_suite_logique_seq_terme(Ast Ag, Ast* A);
void rec_logique_terme(Ast *A);
void rec_logique_seq_facteur(Ast *A);
void rec_logique_suite_seq_facteur(Ast Ag, Ast* A);
void rec_logique_facteur(Ast *A);
void rec_ou(TypeOperateur *O);
void rec_et(TypeOperateur *O);