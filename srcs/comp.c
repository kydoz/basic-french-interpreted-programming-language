#include "ast_parcours.h"
#include "ast_construction.h"
#include "temp.h" //includes the tree itself
#include "table_symbole.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *read_string(size_t *idx);
double evaluate(size_t *idx);
bool val_bool(size_t *idx, Ast A);
bool evaluate_bool(size_t *idx, Ast A);
Ast read_arbre(size_t *idx);
void evaluate_aff(size_t *idx);
void evaluate_lire(size_t *idx);
void evaluate_affich(size_t *idx);
void evaluate_si_alors_sinon(Ast A);
void evaluate_while(size_t *idx);
void evaluate_arbre(size_t *idx, Ast A);

char *read_string(size_t *idx)
{
    size_t len = 0;
    memcpy(&len, objs_temp_bin + *idx, sizeof(size_t));
    *idx += sizeof(size_t);
    char *s = (char *)malloc(sizeof(char) * (len + 1));
    memcpy(s, objs_temp_bin + *idx, sizeof(char) * len);
    *idx += sizeof(char) * len;
    s[len] = '\0';
    return s;
}

double evaluate(size_t *idx)
{

    Ast expr = read_arbre(idx);
    if (expr == NULL)
    {
        printf("Erreur expr NUL\n");
        return 0;
    }
    double x1;
    double x2;
    double v;
    switch (expr->nature)
    {
    case OPERATION:
        switch (expr->operateur)
        {
        case N_PLUS: {
            expr->gauche = read_arbre(idx);
            expr->droite = read_arbre(idx);
            double res = evaluation(expr->gauche) + evaluation(expr->droite);
            liberer_arbre(expr);
            return res;
        }
        case N_MUL: {
            expr->gauche = read_arbre(idx);
            expr->droite = read_arbre(idx);
            double res = evaluation(expr->gauche) * evaluation(expr->droite);
            liberer_arbre(expr);
            return res;
        }
        case N_MOINS: {
            expr->gauche = read_arbre(idx);
            expr->droite = read_arbre(idx);
            double res = evaluation(expr->gauche) - evaluation(expr->droite);
            liberer_arbre(expr);
            return res;
        }
        case N_DIV:
            x1 = evaluate(idx);
            x2 = evaluate(idx);
            if (x2 == 0)
            {
                printf("\n\nErreur de calcul, division par 0\n");
            }
            liberer_arbre(expr);
            return x1 / x2;
            break;
        case N_MOD:
            x1 = evaluate(idx);
            x2 = evaluate(idx);
            if (x2 == 0)
            {
                printf("\n\nErreur de calcul, division par 0\n");
            }
            // expression must have integral type
            liberer_arbre(expr);
            return (int)x1 % (int)x2;
            break;
        default:
            return 0;
            break;
        }
        break;
    case VALEUR:
        //printf("this is a value\n");
        return expr->valeur + expr->partie_fraction;
    case INSTRUCTION:
        switch (expr->operateur)
        {
        case N_IDF:
            //printf("this is a var %s\n", expr->chaine);
            // printf("The idf: %s\n", expr->chaine);
            if (estPresentTS(expr->chaine, &v, NULL, NULL, NULL, NULL, NULL) != 1)
            {
                printf("Erreur syntaxique variable %s n'a pas été affectée\n", expr->chaine);
            }
            else
            {
                liberer_arbre(expr);
                return v;
            }
            break;
        case N_LISTE_INDICE:
        {
            // printf("The idf: %s\n", expr->chaine);
            expr->gauche = read_arbre(idx);
            int liste_indice = evaluation(expr->gauche);
            if (estPresentTS(expr->chaine, &v, NULL, NULL, NULL, NULL, &liste_indice) != 1)
            {
                printf("Erreur, %d est une indice incorrecte pour liste %s\n", liste_indice, expr->chaine);
            }
            else
            {
                liberer_arbre(expr);
                return v;
            }
            break;
        }
        case N_LON_LISTE:
        {
            int t;
            expr->gauche = read_arbre(idx);
            if (expr->gauche == NULL)
            {
                printf("Erreur syntaxique lon manque un argument\n");
            }
            else
            {
                TS_val_taille_liste(expr->gauche->chaine, &t);
                if (t == -1)
                {
                    printf("Erreur syntaxique variable %s n'est pas une liste\n", expr->gauche->chaine);
                }
                liberer_arbre(expr);
                return (double)t;
            }
            break;
        }
        default:
            liberer_arbre(expr);
            return 0;
            break;
        }
    default:
        liberer_arbre(expr);
        return 0;
        break;
    }
    liberer_arbre(expr);
    printf("Erreur de calcul\n");
}

bool val_bool(size_t *idx, Ast A) {
    double valeurg = evaluate(idx);
	double valeurd = evaluate(idx);
    printf("%lf\n", valeurg);
    printf("%lf\n", valeurd);
	switch (A->operateur)
	{
	case N_EGAL:
		return valeurg==valeurd;
	case N_GTH:
		return valeurg>valeurd;
	case N_GEQ:
		return valeurg>=valeurd;
	case N_LEQ:
		return valeurg<=valeurd;
	case N_LTH:
		return valeurg<valeurd;
	case N_NEQ:
		return valeurg!=valeurd;
	default:
		printf("Operation de comparaison non connue\n");
		return 0;
	}
}

bool evaluate_bool(size_t *idx, Ast A)
{
    // afficher_arbre(expr);
    Ast expr;
    if (!A)
        expr = read_arbre(idx);
    else {
        printf("A isnt null it seems\n");
        expr = A;
    }
    if (expr == NULL)
    {
        printf("Erreur expr NUL\n");
        return 0;
    }
    switch (expr->nature)
    {
    case OPERATION:
        switch (expr->operateur)
        {
        case N_ET:
            expr->gauche = read_arbre(idx);
            expr->droite = read_arbre(idx);
            return evaluate_bool(idx,expr->gauche) && evaluate_bool(idx,expr->droite);
        case N_OU:
            expr->gauche = read_arbre(idx);
            expr->droite = read_arbre(idx);
            return evaluate_bool(idx,expr->gauche) || evaluate_bool(idx,expr->droite);
        case N_NON:
            expr->gauche = read_arbre(idx);
            return !evaluate_bool(idx,expr->gauche);
        case N_EGAL:
        case N_NEQ:
        case N_GTH:
        case N_GEQ:
        case N_LTH:
        case N_LEQ:
            printf("operation it seems\n");
            return val_bool(idx, expr);
        default:
            printf("Erreur, operation boolenne inconnue\n");
            return 0;
        }
    default:
        printf("Erreur, operation manquante\n");
        return 0;
    }
}

Ast read_arbre(size_t *idx)
{
    if (objs_temp_bin[(*idx)++] == 0)
        return NULL;

    // int valeur;
    // memcpy(&valeur, bin + *idx, sizeof(valeur));
    //*idx += sizeof(valeur);

    Ast node;
    TypeAst nature;
    TypeOperateur operateur;
    memcpy(&nature, objs_temp_bin + *idx, sizeof(TypeAst));
    *idx += sizeof(TypeAst);
    memcpy(&operateur, objs_temp_bin + *idx, sizeof(TypeOperateur));
    *idx += sizeof(TypeOperateur);

    switch (nature)
    {
    case VALEUR:
    {
        int valeur = 0;
        memcpy(&valeur, objs_temp_bin + *idx, sizeof(int));
        *idx += sizeof(int);
        double partie_fraction = 0;
        memcpy(&partie_fraction, objs_temp_bin + *idx, sizeof(double));
        *idx += sizeof(double);
        node = creer_valeur(valeur, partie_fraction);
        break;
    }
    case OPERATION:
    {
        // Ast G = read_arbre(idx);
        // Ast D = read_arbre(idx);
        node = creer_operation(operateur, NULL, NULL);
        break;
    }
    case INSTRUCTION:
        switch (operateur)
        {
        case N_SEPINST:
        {
            // Ast G = read_arbre(idx);
            // Ast D = read_arbre(idx);
            node = creer_sepinst(NULL, NULL);
            break;
        }
        case N_AFF:
        {
            // Ast G = read_arbre(idx);
            // Ast D = read_arbre(idx);
            node = creer_aff(NULL, NULL);
            break;
        }
        case N_TANQUE:
        {
            // Ast G = read_arbre(idx);
            // Ast D = read_arbre(idx);
            node = creer_tanque(NULL, NULL);
            break;
        }
        case N_IDF:
        {
            char *s = read_string(idx); // modify the function and also write string length to bin
            Ast indice = read_arbre(idx);
            node = creer_idf(s, indice);
            free(s);
            break;
        }
            /*
            need to figure out
            case N_LISTE_INDICE:
                char * s=read_string(idx); //modify the function and also write string length to bin
                Ast indice=read_arbre(idx);
                node=creer_liste_indice()
                break;
            */

        case N_LIRE:
        {
            // Ast G = read_arbre(idx);
            node = creer_lire(NULL);
            break;
        }
        case N_AFFICHER:
        {
            // Ast G = read_arbre(idx);
            node = creer_affich(NULL);
            break;
        }
        case N_LON_LISTE:
        {
            Ast G = read_arbre(idx);
            node = creer_lon_liste(G);
            break;
        }
        case N_IF:
        {
            // Ast G = read_arbre(idx);
            // Ast D = read_arbre(idx);
            // Ast C = read_arbre(idx);
            node = creer_if(NULL, NULL, NULL);
            break;
        }
        case N_STRING:
        {
            char *s = read_string(idx);
            node = creer_string(s);
            free(s);
            break;
        }
        case N_AFFICHER_TS:
            node = creer_affich_ts();
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
    default:
        break;
    }
    return node;
}

void evaluate_aff(size_t *idx)
{
    Ast AG = read_arbre(idx);
    Ast AD = read_arbre(idx);
    // inserer valeur trouver dans arbre droite dans la table de symbole
    char *idf = AG->chaine;
    if (AD->operateur == N_STRING)
    {
        char *s = AD->chaine;
        insererTS(idf, 0, s, NULL, NULL, NULL, 0, NULL);
    }
    else if (AD->operateur == N_ENTIER_LISTE)
    {
        int *entier_liste = AD->entier_liste;
        insererTS(idf, 0, NULL, entier_liste, NULL, NULL, AD->taille_liste, NULL);
    }
    else if (AD->operateur == N_NOMBRE_A_VIRGULE_LISTE)
    {
        double *nombre_a_virgule_liste = AD->nombre_a_virgule_liste;
        insererTS(idf, 0, NULL, NULL, nombre_a_virgule_liste, NULL, AD->taille_liste, NULL);
    }
    else if (AD->operateur == N_STRING_LISTE)
    {
        char **string_liste = AD->string_liste;
        insererTS(idf, 0, NULL, NULL, NULL, string_liste, AD->taille_liste, NULL);
    }
    else if (AG->operateur == N_LISTE_INDICE)
    {
        int liste_indice = (int)evaluate(idx); // gauche
        double v = evaluate(idx);              // droite
        int *ptr_liste_indice = &liste_indice;
        insererTS(idf, v, NULL, NULL, NULL, NULL, 0, &ptr_liste_indice);
        if (ptr_liste_indice == NULL)
        {
            printf("Erreur, %d est une indice incorrecte pour liste %s\n", liste_indice, AG->chaine);
            // erreur_interpretation=true;
        }
    }
    else
    {
        double v = evaluation(AD);
        insererTS(idf, v, NULL, NULL, NULL, NULL, 0, NULL);
    }
    liberer_arbre(AG);
    liberer_arbre(AD);
}

void evaluate_lire(size_t *idx)
{
    // char *idf = strdup(A->gauche->chaine);

    Ast AG = read_arbre(idx);
    char *idf = AG->chaine;
    double v;
    // peuvent avoir des boucles infinis ici si format d'entrer incorrecte (a cause de scanf)
    int res = scanf("%lf", &v);
    if (res <= 0)
    {
        printf("Erreur, Lire échoué\n");
    }

    insererTS(idf, v, NULL, NULL, NULL, NULL, 0, NULL);
    liberer_arbre(AG);
}

void evaluate_affich(size_t *idx)
{
    if (false)
    {
        printf("%s", "String printing not implemented");
    }
    else
    {
        double v;
        v = evaluate(idx);
        if (v - (int)v == 0)
        {
            printf("%d\n", (int)v);
        }
        else
        {
            printf("%lf\n", v);
        }
    }
}
/*
void evaluate_si_alors_sinon(size_t *idx)
{
    
    Ast AG = read_arbre(idx);
    bool cond = evaluate_bool(idx, AG); // gauche
    if (cond)
    {
        printf("doing center\n");
        read_arbre(idx); // droite
        evaluate_arbre(idx, NULL); // centrale
    }
    else
    {
        printf("doing whatever\n");
        evaluate_arbre(idx, NULL); // droite
        read_arbre(idx); // centrale
    }
}*/

void evaluate_while(size_t *idx)
{
    Ast AG = read_arbre(idx);
    while (evaluate_bool(idx, AG)) // gauche
    {
        evaluate_arbre(idx, NULL); // droite
    }
}

Ast read_arbre_rec(size_t *idx) {
     if (objs_temp_bin[(*idx)++] == 0)
        return NULL;

    // int valeur;
    // memcpy(&valeur, bin + *idx, sizeof(valeur));
    //*idx += sizeof(valeur);

    Ast node;
    TypeAst nature;
    TypeOperateur operateur;
    memcpy(&nature, objs_temp_bin + *idx, sizeof(TypeAst));
    *idx += sizeof(TypeAst);
    memcpy(&operateur, objs_temp_bin + *idx, sizeof(TypeOperateur));
    *idx += sizeof(TypeOperateur);

    switch (nature)
    {
    case VALEUR:
    {
        int valeur = 0;
        memcpy(&valeur, objs_temp_bin + *idx, sizeof(int));
        *idx += sizeof(int);
        double partie_fraction = 0;
        memcpy(&partie_fraction, objs_temp_bin + *idx, sizeof(double));
        *idx += sizeof(double);
        node = creer_valeur(valeur, partie_fraction);
        break;
    }
    case OPERATION:
    {
        Ast G = read_arbre_rec(idx);
        Ast D = read_arbre_rec(idx);
        node = creer_operation(operateur, G, D);
        break;
    }
    case INSTRUCTION:
        switch (operateur)
        {
        case N_SEPINST:
        {
            Ast G = read_arbre_rec(idx);
            Ast D = read_arbre_rec(idx);
            node = creer_sepinst(G, D);
            break;
        }
        case N_AFF:
        {
            Ast G = read_arbre_rec(idx);
            Ast D = read_arbre_rec(idx);
            node = creer_aff(G, D);
            break;
        }
        case N_TANQUE:
        {
            Ast G = read_arbre_rec(idx);
            Ast D = read_arbre_rec(idx);
            node = creer_tanque(G, D);
            break;
        }
        case N_IDF:
        {
            char *s = read_string(idx); // modify the function and also write string length to bin
            Ast indice = read_arbre_rec(idx);
            node = creer_idf(s, indice);
            free(s);
            break;
        }
            /*
            need to figure out
            case N_LISTE_INDICE:
                char * s=read_string(idx); //modify the function and also write string length to bin
                Ast indice=read_arbre(idx);
                node=creer_liste_indice()
                break;
            */

        case N_LIRE:
        {
            Ast G = read_arbre_rec(idx);
            node = creer_lire(G);
            break;
        }
        case N_AFFICHER:
        {
            Ast G = read_arbre_rec(idx);
            node = creer_affich(G);
            break;
        }
        case N_LON_LISTE:
        {
            Ast G = read_arbre_rec(idx);
            node = creer_lon_liste(G);
            break;
        }
        case N_IF:
        {
            Ast G = read_arbre_rec(idx);
            Ast D = read_arbre_rec(idx);
            Ast C = read_arbre_rec(idx);
            node = creer_if(G, C, D);
            break;
        }
        case N_STRING:
        {
            char *s = read_string(idx);
            node = creer_string(s);
            free(s);
            break;
        }
        case N_AFFICHER_TS:
            node = creer_affich_ts();
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
    default:
        break;
    }
    return node;
}

/*
void evaluate_si_alors_sinon(Ast A) {
	bool cond = evaluation_bool(A->gauche);
	if (cond)
	{
		interpreter(A->central);
	} else {
		if (A->droite!=NULL) //sinon optionnel
		{
			interpreter(A->droite);
		}
	}
    
}*/

void evaluate_arbre(size_t *idx, Ast Arb)
{

    // int valeur;
    // memcpy(&valeur, bin + *idx, sizeof(valeur));
    //*idx += sizeof(valeur);
    Ast A;
    if (!Arb)
        A = read_arbre(idx);
    else {
        printf("Arb isnt null it seems in eval_arbre\n");
        A = Arb;
    }
    if (A==NULL)
        printf("Arbre vide\n");
    else if (A->nature!=INSTRUCTION)
        printf("Pas une instruction %d\n", A->nature);
    else {

        switch (A->operateur)
        {
        case N_SEPINST:
            evaluate_arbre(idx, NULL); // gauche
            evaluate_arbre(idx, NULL); // droite
            break;
        case N_AFF:
            evaluate_aff(idx);
            break;
        case N_LIRE:
            evaluate_lire(idx);
            break;
        case N_AFFICHER:
            evaluate_affich(idx);
            break;

        case N_IF: {
            Ast G = read_arbre_rec(idx);
            Ast D = read_arbre_rec(idx);
            Ast C = read_arbre_rec(idx);
            Ast node = creer_if(G, C, D);
            interpreter_si_alors_sinon(node);
            liberer_arbre(node);
            break;
        }

        case N_TANQUE: {
            Ast G = read_arbre_rec(idx);
            Ast D = read_arbre_rec(idx);
            Ast node = creer_tanque(G, D);
            interpreter_while(node);
            liberer_arbre(node);
            break;
        }
        case N_AFFICHER_TS:
            afficheTS();
            break;

        default:
            fprintf(stderr, "Erreur, type d'opération inconnue\n");
            break;
        }
    }
    liberer_arbre(A);
}

int main()
{
    size_t index = 0;
    evaluate_arbre(&index, NULL);
    liberer_TS();
    return 0;
}
