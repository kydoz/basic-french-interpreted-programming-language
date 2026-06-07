#include <string.h>
#include <stdlib.h> 
#include <stdio.h> 


#include "lecture_caracteres.h"
#include "analyse_syntaxique.h"
#include "analyse_lexicale.h"


void write_string(char *s, FILE * fptr) {
    size_t len=strlen(s);
    fwrite(&len,sizeof(size_t), 1, fptr);
    for (size_t i = 0; i < len; i++)
    {
        fwrite(&s[i], sizeof(char), 1, fptr);
    }   
}


void write_arbre(Ast racine, FILE * fptr) {
    //only one of the lists is active, switch accordingly
    
    unsigned char flag = (racine != NULL);
    fwrite(&flag, sizeof(flag), 1, fptr);
    if (racine==NULL)
    {
        return;
    }
    fwrite(&racine->nature, sizeof(TypeAst), 1, fptr);
    fwrite(&racine->operateur, sizeof(TypeOperateur), 1, fptr);
    //the rest should be based on the TypeOperateur, check ast_construction
    
    switch (racine->nature)
    {
    case VALEUR:
        fwrite(&racine->valeur, sizeof(int), 1, fptr);
        fwrite(&racine->partie_fraction, sizeof(double), 1, fptr);
        break;
    case OPERATION:
        //fwrite(&racine->operateur, sizeof(TypeOperateur), 1, fptr);
        write_arbre(racine->gauche, fptr);
        write_arbre(racine->droite, fptr);
        break;

    case INSTRUCTION:
        switch (racine->operateur)
        {
        case N_SEPINST:
        case N_AFF:
        case N_TANQUE:
            write_arbre(racine->gauche, fptr);
            write_arbre(racine->droite, fptr);
            break;

        case N_IDF:
        case N_LISTE_INDICE: //?
            write_string(racine->chaine, fptr); //write string change
            write_arbre(racine->gauche, fptr);
            break;

        case N_LIRE:
        case N_AFFICHER:
        case N_LON_LISTE:
            write_arbre(racine->gauche, fptr);
            break;
        
        case N_IF:
            write_arbre(racine->gauche, fptr);
            write_arbre(racine->droite, fptr);
            write_arbre(racine->central, fptr);
            break;

        case N_STRING:
            //fwrite(racine->chaine, sizeof(char), 1, fptr); //write string change
            write_string(racine->chaine, fptr);
            break;
        case N_ENTIER_LISTE:
            break;
        case N_NOMBRE_A_VIRGULE_LISTE:
            break;
        case N_STRING_LISTE:
            break;
        
        default:
            break;
        }
        break;

    default:
        break;
    }    
}

bool erreur=false;
extern bool erreur_interpretation; //vient de ast_parcours

//analyse syntaxique
void analyser(char *fichier, bool to_compile) {
    demarrer (fichier) ; 
    if (fin_de_sequence())
    {
        printf("Fichier vide\n");
        arreter();
        return;
    }
    
    Ast*A=(Ast*)malloc(sizeof(Ast));
    //rec_eag(A);
    rec_seq_inst(A);
    if (! fin_de_sequence())
    {
        //afficher(lexeme_courant());
        printf("Erreur syntaxique\n");
        liberer_arbre(*A);
        liberer_TS();
        free(A);
        arreter();
        return;
    }
    if (erreur)
    {
        printf("Une erreur s'est produite\n");
        //afficher_arbre(*A);
        liberer_arbre(*A);
        liberer_TS();
        free(A);
        arreter();
        return;
    }


    interpreter(*A);
    
    if (erreur_interpretation)
    {
        printf("Une erreur s'est produite\n");
        liberer_arbre(*A);
        liberer_TS();
        free(A);
        arreter() ; // termine l'analyse lexicale
        return;
    }

    if (to_compile==true) {
        FILE* fptr;
    
        if ((fptr = fopen("objs/temp.bin", "wb")) == NULL) {
            printf("Error! opening file");

            exit(1);
        }


            write_arbre(*A, fptr);
            fclose(fptr);
            system("xxd -i objs/temp.bin > include/temp.h");
            system("make COMP=1");
    }
    liberer_arbre(*A);
    liberer_TS();
    free(A);
    arreter() ; // termine l'analyse lexicale
    return; 
}


void rec_eag(Ast *A) {
    //afficher(lexeme_courant());
    switch (lexeme_courant().nature)
    {
    case STRING:
        *A=creer_string(lexeme_courant().chaine);
        avancer();
        break;
    case ENTIER_LISTE:
        *A=creer_entier_liste(lexeme_courant().chaine);
        avancer();
        break;
    
    case NOMBRE_A_VIRGULES_LISTE:
        *A=creer_nombre_a_virgule_liste(lexeme_courant().chaine);
        avancer();
        break;
    
    case STRING_LISTE:
        *A=creer_string_liste(lexeme_courant().chaine);
        avancer();
        break;
    
    default:
        rec_seq_terme(A);
        break;
    }
}

void rec_seq_terme(Ast *A) {
    //afficher(lexeme_courant());
    Ast A1;
    rec_terme(&A1); rec_suite_seq_terme(A1, A);
}

void rec_suite_seq_terme(Ast Ag, Ast* A) {
    Ast A1,Ad;
    Lexeme Lc = lexeme_courant();
    switch (Lc.nature)
    {
    case PLUS:
    case MOINS:  {
        TypeOperateur *Op = (TypeOperateur*)malloc(sizeof(TypeOperateur));
        rec_op1(Op); rec_terme(&Ad);
        if (Ag==NULL || Ad==NULL)
        {
            erreur=true;
        }
        
        A1=creer_operation(*Op, Ag, Ad);
        free(Op);
        rec_suite_seq_terme(A1, A);
        break;
    }
    default:
        *A=Ag;
        //afficher_arbre(*A);
        break;
    }
}

void rec_terme(Ast *A) {
    rec_seq_facteur(A);
}

void rec_seq_facteur(Ast *A) {
    //afficher(lexeme_courant());
    Ast A1;
    rec_facteur(&A1); rec_suite_seq_facteur(A1, A);
}

void rec_suite_seq_facteur(Ast Ag, Ast* A) {
    Ast A1,Ad;
    Lexeme Lc = lexeme_courant();
    switch (Lc.nature)
    {
    case MUL:
    case DIV:
    case MOD: {
        TypeOperateur *Op = (TypeOperateur*)malloc(sizeof(TypeOperateur));
        rec_op2(Op); rec_facteur(&Ad); 
        if (Ag==NULL || Ad==NULL)
        {
            erreur=true;
        }
        A1=creer_operation(*Op, Ag, Ad);
        free(Op);
        /*
        printf("A1: ");
        afficher_arbre(*A1);
        printf("\n");*/
        rec_suite_seq_facteur(A1, A);
        break;
    }
    default:
        *A=Ag;
        //afficher_arbre(*A);
        //printf("\n");
        break;
    }
}


void rec_facteur(Ast *A) {
    Lexeme Lc = lexeme_courant();
    switch (Lc.nature)
    {
    case ENTIER:
        *A = creer_valeur(Lc.valeur, 0);
        avancer();
        break;
    case NOMBRE_A_VIRGULES:
        *A = creer_valeur(0, Lc.partie_fraction);
        avancer();
        break;
    
    case IDF: {
        /*
        double v=0;
        if (estPresentTS(Lc.chaine, &v)!=1)
        {
            printf("Erreur syntaxique a ligne:%d colonne:%d    variable n'a pas été affectée\n", Lc.ligne, Lc.colonne);
        } else {
            *A = creer_valeur((int)v, v-(int)v);
        }
        */
        //evaluer dans evaluation (dans ast parcours)
        Ast A_INDICE;
        char *chaine_idf = strdup(lexeme_courant().chaine);
        //AG=creer_idf(Lc.chaine);
        avancer();
        Lc=lexeme_courant();
        if (Lc.nature==POINT) {
            avancer();
            rec_eag(&A_INDICE);
            *A=creer_idf(chaine_idf, A_INDICE);
            
        } else {
            *A=creer_idf(chaine_idf, NULL);
        }
        free(chaine_idf);
        break;
    }
    /*
    case LISTE_INDICE:
        *A = creer_liste_indice(Lc.chaine, Lc.valeur);
        avancer();
        break;*/
    case LON_LISTE: {
        Ast AG;
        avancer();        
        Lc=lexeme_courant();
        if (Lc.nature==PARO)
        {
            avancer();
        } else {
            printf("Erreur syntaxique a ligne:%d colonne:%d   manque paranthese ouvrante '(' \n", Lc.ligne, Lc.colonne);
            erreur=true;
            AG=NULL;
        }
        Lc=lexeme_courant();
        if (Lc.nature==IDF)
        {
            AG=creer_idf(Lc.chaine, NULL);
            avancer();
        } /*else if (Lc.nature==LISTE_INDICE) {
            AG=creer_liste_indice(Lc.chaine, Lc.valeur);
            avancer();
        } */else {
            printf("Erreur syntaxique a ligne:%d colonne:%d   pour l'instant LON est compatible avec seulement des IDF contenats des listes \n", Lc.ligne, Lc.colonne);
            erreur=true;
            AG=NULL;
        }
        if (AG==NULL)
        {
            erreur=true;
        }
        *A=creer_lon_liste(AG);
        Lc=lexeme_courant();
        if (Lc.nature==PARF)
        {
            avancer();
        } else {
            printf("Erreur syntaxique a ligne:%d colonne:%d   manque paranthese fermante ')' \n", Lc.ligne, Lc.colonne);
            erreur=true;
        }
        break;
    }
    case PARO:
        avancer(); rec_eag(A);
        Lc=lexeme_courant();
        if (Lc.nature==PARF)
        {
            avancer();
        } else {
            printf("Erreur syntaxique\n");
            erreur=true;
        }
        break;

    default:
        printf("Erreur syntaxique a ligne:%d colonne:%d, un Entier relatif ou réel excepté ou un ( \n", Lc.ligne, Lc.colonne);
        *A=NULL;
    }
}

void rec_op1(TypeOperateur *O) {
    Lexeme Lc = lexeme_courant();
    switch (Lc.nature)
    {
    case PLUS:
        *O=N_PLUS;
        avancer();
        break;
    case MOINS:
        *O=N_MOINS;
        avancer();
        break;
    
    default: 
        Lc=lexeme_courant();
        printf("Erreur syntaxique a ligne:%d colonne:%d,  une Opérateur +, -, PLUS, MOINS exceptée\n", Lc.ligne, Lc.colonne);
        erreur=true;
        break;
    }
}

void rec_op2(TypeOperateur *O) {
    Lexeme Lc = lexeme_courant();
    switch (Lc.nature)
    {
    case MUL:
        *O=N_MUL;
        avancer();
        break;
    case DIV:
        *O=N_DIV;
        avancer();
        break;

    case MOD:
        *O=N_MOD;
        avancer();
        break;
    
    default:
        Lc=lexeme_courant();
        printf("Erreur syntaxique a ligne:%d colonne:%d,  une Opérateur *, /, MUL, DIV exceptée\n", Lc.ligne, Lc.colonne);
        erreur=true;
        break;
    }
}

void rec_seq_inst(Ast *A) {
    Ast A1;
    rec_inst(&A1); rec_suite_seq_inst(A1, A);
}

void rec_suite_seq_inst(Ast A1, Ast *A) {
    Ast A2;
    Lexeme Lc = lexeme_courant();
    switch (Lc.nature)
    {
    case SEPINST:
        avancer();
        Lc=lexeme_courant();
        /*
        if (Lc.nature==FIN_SEQUENCE)
        {
            *A=A1;
            break;
        }*/
        rec_seq_inst(&A2);
        if (A1==NULL || A2==NULL)
        {
            erreur=true;
        }
        *A=creer_sepinst(A1, A2);
        break;
    
    default:
        /*
        Lexeme Lc=lexeme_courant();
        printf("Erreur syntaxique a ligne:%d colonne:%d  manque de separateur ';'\n", Lc.ligne, Lc.colonne);
        erreur=true;*/
        *A=A1;
        break;
    }
}

void rec_inst(Ast *A) {
    Ast AG, AD;
    Ast A_INDICE;
    Lexeme Lc = lexeme_courant();
    switch (Lc.nature)
    {
    case IDF: {
        char *chaine_idf = strdup(lexeme_courant().chaine);
        //AG=creer_idf(Lc.chaine);
        avancer();
        
        Lc=lexeme_courant();
        if (Lc.nature==AFF)
        {
            avancer();
            AG=creer_idf(chaine_idf, NULL);
        
        } else if (Lc.nature==POINT) {
            avancer();
            rec_eag(&A_INDICE);
            AG=creer_idf(chaine_idf, A_INDICE);
            avancer();
            
        } 
        else {
            printf("Erreur syntaxique a ligne:%d colonne:%d    variable n'a pas été affectée (manque '<-')\n", Lc.ligne, Lc.colonne);
            erreur=true;
            AG=NULL;

        }

        rec_eag(&AD);
        //afficheTS();
        if (AG==NULL || AD==NULL)
        {
            erreur=true;
        }
        *A=creer_aff(AG, AD);
        free(chaine_idf);
        break;
    }
    case AFFICHER:
        //rec_affich();
        avancer();
        Lc=lexeme_courant();
         if (Lc.nature==PARO)
        {
            avancer();
        } else {
            printf("Erreur syntaxique a ligne:%d colonne:%d   manque paranthese ouvrante '(' \n", Lc.ligne, Lc.colonne);
            erreur=true;
        }

        rec_eag(&AG);
        if (AG==NULL)
        {
            erreur=true;
        }
        *A = creer_affich(AG);

        Lc=lexeme_courant();
        if (Lc.nature==PARF)
        {
            avancer();
        } else {
            printf("Erreur syntaxique a ligne:%d colonne:%d   manque paranthese fermante ')' \n", Lc.ligne, Lc.colonne);
            erreur=true;
        }
        break;

    case AFFICHER_TS:
        avancer();
        Lc=lexeme_courant();
        if (Lc.nature==PARO)
        {
            avancer();
        } else {
            printf("Erreur syntaxique a ligne:%d colonne:%d   manque paranthese ouvrante '(' \n", Lc.ligne, Lc.colonne);
            erreur=true;
        }
        Lc=lexeme_courant();
        *A=creer_affich_ts();
        Lc=lexeme_courant();
        if (Lc.nature==PARF)
        {
            avancer();
        } else {
            printf("Erreur syntaxique a ligne:%d colonne:%d   manque paranthese fermante ')' \n", Lc.ligne, Lc.colonne);
            erreur=true;
        }
        break;
    case LIRE:
        avancer();
        Lc=lexeme_courant();
        if (Lc.nature==PARO)
        {
            avancer();
        } else {
            printf("Erreur syntaxique a ligne:%d colonne:%d   manque paranthese ouvrante '(' \n", Lc.ligne, Lc.colonne);
            erreur=true;
        }
        Lc=lexeme_courant();
        if (Lc.nature==IDF)
        {
            AG=creer_idf(Lc.chaine, NULL);
            avancer();
        } /*else if (Lc.nature==LISTE_INDICE) {
            AG=creer_liste_indice(Lc.chaine, Lc.valeur);
            avancer();
        } */else {
            printf("Erreur syntaxique a ligne:%d colonne:%d   essayer d'affecter une non-variable \n", Lc.ligne, Lc.colonne);
            erreur=true;
            AG=NULL;
        }
        if (AG==NULL)
        {
            erreur=true;
        }
        *A=creer_lire(AG);
        Lc=lexeme_courant();
        if (Lc.nature==PARF)
        {
            avancer();
        } else {
            printf("Erreur syntaxique a ligne:%d colonne:%d   manque paranthese fermante ')' \n", Lc.ligne, Lc.colonne);
            erreur=true;
        }
        break;

    case LON_LISTE:
        avancer();        
        Lc=lexeme_courant();
        if (Lc.nature==PARO)
        {
            avancer();
        } else {
            printf("Erreur syntaxique a ligne:%d colonne:%d   manque paranthese ouvrante '(' \n", Lc.ligne, Lc.colonne);
            erreur=true;
        }
        Lc=lexeme_courant();
        if (Lc.nature==IDF)
        {
            AG=creer_idf(Lc.chaine, NULL);
            avancer();
        } /*else if (Lc.nature==LISTE_INDICE) {
            AG=creer_liste_indice(Lc.chaine, Lc.valeur);
            avancer();
        } */else {
            printf("Erreur syntaxique a ligne:%d colonne:%d   pour l'instant LON est compatible avec seulement des IDF contenats des listes \n", Lc.ligne, Lc.colonne);
            erreur=true;
            AG=NULL;
        }
        if (AG==NULL)
        {
            erreur=true;
        }
        *A=creer_lon_liste(AG);
        Lc=lexeme_courant();
        if (Lc.nature==PARF)
        {
            avancer();
        } else {
            printf("Erreur syntaxique a ligne:%d colonne:%d   manque paranthese fermante ')' \n", Lc.ligne, Lc.colonne);
            erreur=true;
        }
        break;
    
    case SI:
        rec_seq_if(A);
        //afficher(lexeme_courant());
        switch (lexeme_courant().nature)
        {
        case FSI:
            avancer();
            break;
    
        default:
            printf("Erreur, manque fsi\n");
            erreur=true;
            break;
        }
        break;
    
    case TANQUE:
        avancer();
        rec_logique_eag(&AG);
        if (lexeme_courant().nature==FAIRE)
        {
            avancer();
        } else {
            printf("Erreur faire manquant\n");
            erreur=true;
        }
        rec_seq_inst(&AD);
        if (lexeme_courant().nature==FTANQUE)
        {
            avancer();
        } else {
            printf("%s\n", lexeme_courant().chaine);
            printf("Erreur ftq manquant\n");
            erreur=true;
        }
        if (AG==NULL || AD==NULL)
        {
            erreur=true;
        }
        *A=creer_tanque(AG, AD);
        break;
    /*
    case FIN_SEQUENCE:
        
        break;*/
    default:
        Lc=lexeme_courant();
        //afficher(lexeme_courant());
        printf("Erreur syntaxique a ligne:%d colonne:%d   %s  instruction inconnue\n", Lc.ligne, Lc.colonne, Lc.chaine);
        erreur=true;
        *A=NULL;
        break;
    }
}

//bloque_if <- seq_if finsi
//seq_if <- if suite_seq_if 
//if <- si cond alors seq_inst
//suite_seq_if <- epsilon
//suite_seq_if <- sinon seq_if
//suite_seq_if <- sinon seq_inst

void rec_bloque_si(Ast *A) {
    rec_seq_if(A);
    //afficher(lexeme_courant());
    switch (lexeme_courant().nature)
    {
        case FSI:
            avancer();
            break;
    
        default:
            printf("Erreur syntaxique a ligne:%d colonne:%d  manque fsi\n", lexeme_courant().ligne, lexeme_courant().colonne);
            erreur=true;
            break;
    }
}

void rec_seq_if(Ast *A) {
    //afficher(lexeme_courant());
    Ast Acond, Athen, Aelse;
    rec_if(&Acond, &Athen); rec_suite_seq_if(&Aelse);
    if (Acond==NULL)
        {
            erreur=true;
        }
    *A=creer_if(Acond, Athen, Aelse);
}

void rec_suite_seq_if(Ast *Aelse) {
    Lexeme Lc = lexeme_courant();
    //afficher(lexeme_courant());
    switch (Lc.nature)
    {
    case SINON:
        avancer();
        switch (lexeme_courant().nature) {
            case SI:
                rec_seq_if(Aelse);
                break;
            default:
                rec_seq_inst(Aelse);
                break;
        /*
        if (Lc.nature==FIN_SEQUENCE)
        {
            *A=A1;
            break;
        }*/
        }
        break;
    
    default:
        //epsilon
        *Aelse=NULL;
        break;
    }
}

void rec_if(Ast *Acond, Ast *Athen) {
    switch (lexeme_courant().nature) {
        case SI:
            avancer();
            rec_logique_eag(Acond);
            if (lexeme_courant().nature==ALORS)
            {
                avancer();
            } else {
                printf("Erreur syntaxique a ligne:%d colonne:%d  manque alors\n", lexeme_courant().ligne, lexeme_courant().colonne);
                erreur=true;
            }
            rec_seq_inst(Athen);
            break;
        default:
            printf("Erreur syntaxique a ligne:%d colonne:%d  manque si\n", lexeme_courant().ligne, lexeme_courant().colonne);
            erreur=true;
            break;
    }
}

void rec_condition(Ast *A) {
    
    Ast AG, AD;
    TypeOperateur *Op = (TypeOperateur*)malloc(sizeof(TypeOperateur));;
    rec_eag(&AG);


    //OPCOMP
    //N_IF, N_EGAL, N_SUP, N_GTH, N_LEQ, N_GEQ, N_NEQ
    switch (lexeme_courant().nature)
    {
    case COMP_EQ:
        avancer();
        *Op=N_EGAL;
        break;
    case COMP_GEQ:
        //afficher(lexeme_courant());
        *Op=N_GEQ;
        avancer();
        break;
    case COMP_GTH:
        *Op=N_GTH;
        avancer();
        break;
    case COMP_LEQ:
        *Op=N_LEQ;
        avancer();
        break;
    case COMP_LTH:
        *Op=N_LTH;
        avancer();
        break;
    case COMP_NEQ:
        *Op=N_NEQ;
        avancer();
        break;
    
    default:
        printf("%s\n", lexeme_courant().chaine);
        printf("Erreur syntaxique a ligne:%d colonne:%d  manque operateur de comparaison\n", lexeme_courant().ligne, lexeme_courant().colonne);
        erreur=true;
        break;
    }
    rec_eag(&AD);
    if (AG==NULL || AD==NULL)
        {
            erreur=true;
        }
    *A=creer_operation(*Op,AG,AD);
    free(Op);
}


/*
logique_eag → logique_seq_terme
logique_seq_terme → logique_terme logique_suite_seq_terme
logique_suite_seq_terme → logique_op1 logique_terme logique_suite_seq_terme
logique_suite_seq_terme → ε
logique_terme → logique_seq_facteur
logique_seq_facteur → logique_facteur logique_suite_seq_facteur
logique_suite_seq_facteur → logique_op2 logique_facteur logique_suite_seq_facteur
logique_suite_seq_facteur → ε
logique_facteur → cond
logique_facteur → PARO logique_eag PARF
logique_facteur → BOOL_NON logique_eag
op1 → BOOL_OU
op2 → BOOL_ET
*/


void rec_logique_eag(Ast *A) {
    rec_seq_logique_terme(A);
}

void rec_seq_logique_terme(Ast *A) {
    //afficher(lexeme_courant());
    Ast A1;
    rec_logique_terme(&A1); rec_suite_logique_seq_terme(A1, A);
}

void rec_suite_logique_seq_terme(Ast Ag, Ast* A) {
    Ast A1,Ad;
    Lexeme Lc = lexeme_courant();
    switch (Lc.nature)
    {
    case BOOL_OU: {
        TypeOperateur *Op = (TypeOperateur*)malloc(sizeof(TypeOperateur));
        rec_ou(Op); rec_logique_terme(&Ad);
        if (Ag==NULL || Ad==NULL)
        {
            erreur=true;
        }
        A1=creer_operation(*Op, Ag, Ad);
        free(Op);
        rec_suite_logique_seq_terme(A1, A);
        break;
    }
    default:
        *A=Ag;
        //afficher_arbre(*A);
        break;
    }
}

void rec_logique_terme(Ast *A) {
    rec_logique_seq_facteur(A);
}

void rec_logique_seq_facteur(Ast *A) {
    //afficher(lexeme_courant());
    Ast A1;
    rec_logique_facteur(&A1); rec_logique_suite_seq_facteur(A1, A);
}

void rec_logique_suite_seq_facteur(Ast Ag, Ast* A) {
    Ast A1,Ad;
    Lexeme Lc = lexeme_courant();
    switch (Lc.nature)
    {
    case BOOL_ET: {
        TypeOperateur *Op = (TypeOperateur*)malloc(sizeof(TypeOperateur));
        rec_et(Op); rec_logique_facteur(&Ad); 
        if (Ag==NULL || Ad==NULL)
        {
            erreur=true;
        }
        A1=creer_operation(*Op, Ag, Ad);
        free(Op);
        /*
        printf("A1: ");
        afficher_arbre(*A1);
        printf("\n");
        */
        rec_logique_suite_seq_facteur(A1, A);
        break;
    }
    default:
        *A=Ag;
        //afficher_arbre(*A);
        //printf("\n");
        break;
    }
}


void rec_logique_facteur(Ast *A) {
    Lexeme Lc = lexeme_courant();
    switch (Lc.nature) {
    case PARO:
        avancer(); rec_logique_eag(A);
        Lc=lexeme_courant();
        if (Lc.nature==PARF)
        {
            avancer();
        } else {
            printf("Erreur syntaxique a ligne:%d colonne:%d  manque ')'\n", lexeme_courant().ligne, lexeme_courant().colonne);
            erreur=true;
        }
        break;
    case BOOL_NON: {
        Ast AG;
        avancer(); rec_logique_eag(&AG);
        if (AG==NULL)
        {
            erreur=true;
        }
        *A=creer_operation(N_NON, AG, NULL);
        break;
    }
    default:
        rec_condition(A);
    }
}

void rec_ou(TypeOperateur *O) {
    Lexeme Lc = lexeme_courant();
    switch (Lc.nature)
    {
    case BOOL_OU:
        *O=N_OU;
        avancer();
        break;
    
    default:
        Lc=lexeme_courant();
        printf("Erreur syntaxique a ligne:%d colonne:%d,  une Opérateur 'ou' exceptée\n", Lc.ligne, Lc.colonne);
        erreur=true;
        break;
    }
}

void rec_et(TypeOperateur *O) {
    Lexeme Lc = lexeme_courant();
    switch (Lc.nature)
    {
    case BOOL_ET:
        *O=N_ET;
        avancer();
        break;
    
    default:
        Lc=lexeme_courant();
        printf("Erreur syntaxique a ligne:%d colonne:%d,  une Opérateur 'et' exceptée\n", Lc.ligne, Lc.colonne);
        erreur=true;
        break;
    }
}


