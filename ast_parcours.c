
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast_parcours.h"
#include "table_symbole.h"

bool erreur_interpretation=false;



//N_SEPINST, N_IDF, N_AFF, N_LIRE, N_AFFICHER
void aff_operateur(TypeOperateur op){
	switch (op) {
		case N_PLUS:
			printf("+") ;
			break;
		case N_MOINS:
			printf("-") ;
			break;
		case N_MUL:
			printf("*") ;
			break;
		case N_DIV:
			printf("/");
			break;
		case N_MOD:
			printf("mod");
			break;
		case N_SEPINST:
			printf("N_SEPINST");
			break;
		case N_IDF:
			printf("N_IDF");
			break;
		case N_AFF:
			printf("N_AFF");
			break;
		case N_LIRE:
			printf("N_LIRE");
			break;
		case N_AFFICHER:
			printf("N_AFFICHER");
			break;
		case N_EGAL:
			printf("=");
			break;
		case N_GEQ:
			printf(">=");
			break;
		case N_GTH:
			printf(">");
			break;
		case N_LTH:
			printf("<");
			break;
		case N_LEQ:
			printf("<=");
			break;
		case N_NEQ:
			printf("!=");
			break;
		default:
			break;
	} 
}

void afficher_arbre(Ast expr) {
	if (expr==NULL)
	{
		return;
	}
	
	switch (expr->nature) {
               case OPERATION:
                        printf("(");
                        afficher_arbre(expr->gauche);
                        aff_operateur(expr->operateur) ;
                        afficher_arbre(expr->droite);
                        printf(")");
			break ;
               case VALEUR:
			   		if (expr->partie_fraction==0)
					{
						printf("%d", expr->valeur);
					}
					else 
					{
                  	printf("%f", expr->valeur+expr->partie_fraction);
					}
			break ;
				case INSTRUCTION:
					printf("(");
                        afficher_arbre(expr->gauche);
                        aff_operateur(expr->operateur) ;
                        afficher_arbre(expr->droite);
                        printf(")");
			break ;
			default:
				break;
	}
}

double evaluation(Ast expr) {

      // A COMPLETER !
	  if (expr==NULL)
	  {
		printf("Erreur expr NUL\n");
		erreur_interpretation=true;
		return 0;
	  }
	  double x1;
	  double x2;
	  double v;
	  switch (expr->nature) {
				case OPERATION:
						switch (expr->operateur)
						{
								case N_PLUS:
									return evaluation(expr->gauche)+evaluation(expr->droite);
								case N_MUL:
									return evaluation(expr->gauche)*evaluation(expr->droite);
								case N_MOINS:
									return evaluation(expr->gauche)-evaluation(expr->droite);
								case N_DIV: 
									x1 = evaluation(expr->gauche);
									x2 = evaluation(expr->droite);
									if (x2==0)
									{
										printf("\n\nErreur de calcul, division par 0\n");
										erreur_interpretation=true;
									}
									return x1/x2;
									break;
								case N_MOD:
									x1 = evaluation(expr->gauche); 
								    x2 = evaluation(expr->droite);
									if (x2==0)
									{
										printf("\n\nErreur de calcul, division par 0\n");
										erreur_interpretation=true;
									}
									//expression must have integral type
									return (int)x1%(int)x2;
									break;
								default:
									return 0;
									break;

						}
				break ;
				case VALEUR:
						return expr->valeur+expr->partie_fraction;
				case INSTRUCTION:
					switch (expr->operateur)
					{
						case N_IDF:
							
							//printf("The idf: %s\n", expr->chaine);
							if (estPresentTS(expr->chaine, &v, NULL,NULL,NULL,NULL, NULL)!=1)
							{
								printf("Erreur syntaxique variable %s n'a pas été affectée\n", expr->chaine);
								erreur_interpretation=true;
							} else {
								return v;
							}
							break;
						case N_LISTE_INDICE: {
							//printf("The idf: %s\n", expr->chaine);
							int liste_indice = evaluation(expr->gauche);
							if (estPresentTS(expr->chaine, &v, NULL,NULL,NULL,NULL, &liste_indice)!=1)
							{
								printf("Erreur, %d est une indice incorrecte pour liste %s\n", liste_indice, expr->chaine);
								erreur_interpretation=true;
							} else {
								return v;
							}
							break;
						}
						case N_LON_LISTE: {
							int t;
							if (expr->gauche==NULL)
							{
								printf("Erreur syntaxique lon manque un argument\n");
								erreur_interpretation=true;
							} else {
								TS_val_taille_liste(expr->gauche->chaine, &t);
								if (t==-1)
								{
									printf("Erreur syntaxique variable %s n'est pas une liste\n", expr->gauche->chaine);
									erreur_interpretation=true;
								}
								return (double)t;
							}
							break;
						}
						default:
							return 0;
							break;
					}
				default:
					return 0;
					break;

	  }
	  printf("Erreur de calcul\n");
	  erreur_interpretation=true;
}
//free arbre
void liberer_arbre(Ast A) {
	if (A==NULL)
	{
		//rien a faire
		return;
	}
	//printf("freeing");
	if (A->chaine!=NULL) free(A->chaine);
	if (A->entier_liste!=NULL) free(A->entier_liste);
	if (A->nombre_a_virgule_liste!=NULL) free(A->nombre_a_virgule_liste);
	if (A->string_liste!=NULL) free(A->string_liste);
	
	liberer_arbre(A->gauche);
	liberer_arbre(A->central);
	liberer_arbre(A->droite);
	free(A);
}

void interpreter(Ast A) {
	
	if (A==NULL) 
	{
		printf("Arbre vide\n");
		erreur_interpretation=true;
	} else if (A->nature!=INSTRUCTION) {
		printf("ERREUR, pas une instruction");
		erreur_interpretation=true;
	} else {
		//afficher_arbre(A);
		switch (A->operateur)
		{
		case N_SEPINST:
			interpreter(A->gauche);
			interpreter(A->droite);
			break;
		case N_AFF:
			interpreter_aff(A);
			break;
		case N_LIRE:
			interpreter_lire(A);
			break;
		case N_AFFICHER:
			interpreter_affich(A);
			break;

		case N_IF:
			interpreter_si_alors_sinon(A);
			break;

		case N_TANQUE:
			interpreter_while(A);
			break;
		case N_AFFICHER_TS:
			if (!erreur_interpretation)
			{
				afficheTS();
			}
			break;

		default:
			printf("Erreur, type d'opération inconnue\n");
			erreur_interpretation=true;
			break;
		}
	}
}

void interpreter_aff(Ast A) {
	if (A==NULL)
	  {
		printf("Erreur expr NUL\n");
		return;
	  }

	//inserer valeur trouver dans arbre droite dans la table de symbole
	char *idf = A->gauche->chaine;
	if (A->droite->operateur==N_STRING)
	{
		char* s = A->droite->chaine;
		insererTS(idf, 0, s, NULL, NULL, NULL, 0, NULL);
	} else if (A->droite->operateur==N_ENTIER_LISTE) {
		int *entier_liste = A->droite->entier_liste;
		insererTS(idf,0,NULL,entier_liste,NULL,NULL, A->droite->taille_liste, NULL);
	} else if (A->droite->operateur==N_NOMBRE_A_VIRGULE_LISTE) {
		double *nombre_a_virgule_liste = A->droite->nombre_a_virgule_liste;
		insererTS(idf,0,NULL,NULL,nombre_a_virgule_liste,NULL, A->droite->taille_liste, NULL);
	} else if (A->droite->operateur==N_STRING_LISTE) {
		char **string_liste = A->droite->string_liste;
		insererTS(idf,0,NULL,NULL,NULL,string_liste, A->droite->taille_liste, NULL);
	} else if (A->gauche->operateur==N_LISTE_INDICE) {
		double v = evaluation(A->droite);
		int liste_indice = (int)evaluation(A->gauche->gauche);
		int *ptr_liste_indice = &liste_indice;
		insererTS(idf, v, NULL, NULL, NULL, NULL, 0, &ptr_liste_indice);
		if (ptr_liste_indice==NULL)
		{
			printf("Erreur, %d est une indice incorrecte pour liste %s\n", liste_indice, A->gauche->chaine);
			erreur_interpretation=true;
		}
		
	} 
	else {
		double v = evaluation(A->droite);
		insererTS(idf, v, NULL, NULL, NULL, NULL, 0, NULL);
	}
}

void interpreter_lire(Ast A) {
	if (A==NULL)
	  {
		printf("Erreur expr NUL\n");
		return;
	  }
	//char *idf = strdup(A->gauche->chaine);
	char *idf = A->gauche->chaine;
	double v;
	//peuvent avoir des boucles infinis ici si format d'entrer incorrecte (a cause de scanf)
	int res =scanf("%lf", &v);
	if (res<=0)
	{
		printf("Erreur, Lire échoué\n");
		erreur_interpretation=true;
	}
	

	
	insererTS(idf, v, NULL, NULL, NULL, NULL,0, NULL);
}

void interpreter_affich(Ast A) {
	if (A==NULL)
	  {
		printf("Erreur expr NUL\n");
		return;
	  }
	if (A->gauche->operateur==N_STRING)
	{
		printf("%s", A->gauche->chaine); 
	} else {
		double v;
		v = evaluation(A->gauche);
		if (!erreur_interpretation)
		{
			if (v-(int)v==0)
			{
				printf("%d\n", (int)v);
			} else {
				printf("%lf\n", v);
			}
		}
	}
}

void interpreter_si_alors_sinon(Ast A) {
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
}

//N_EGAL, N_SUP, N_GTH, N_LEQ, N_GEQ, N_NEQ
bool valeur_booleenne(Ast A) {
	double valeurg = evaluation(A->gauche);
	double valeurd = evaluation(A->droite);
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
		erreur_interpretation=true;
		return 0;
	}
}

bool evaluation_bool(Ast expr) {
	//afficher_arbre(expr);
	if (expr==NULL)
	  {
		printf("Erreur expr NUL\n");
		return 0;
	  }
	  switch (expr->nature) {
				case OPERATION:
						switch (expr->operateur)
						{
								case N_ET:
									return evaluation_bool(expr->gauche) && evaluation_bool(expr->droite);
								case N_OU:
									return evaluation_bool(expr->gauche) || evaluation_bool(expr->droite);
								case N_NON:
									return !evaluation_bool(expr->gauche);
								case N_EGAL:
								case N_NEQ:
								case N_GTH:
								case N_GEQ:
								case N_LTH:
								case N_LEQ:
									return valeur_booleenne(expr);
								default: 
									printf("Erreur, operation boolenne inconnue\n");
									erreur_interpretation=true;
									return 0;
						}
				default:
					printf("Erreur, operation manquante\n");
					erreur_interpretation=true;
					return 0;

	  }
}

void interpreter_while(Ast A) {
	while (evaluation_bool(A->gauche))
	{
		interpreter(A->droite);
	}
	
}