/* ------------------------------------------------------------------------
-- paquetage machine_lexemes
--
-- analyse lexicale d'une expression arithmetique
--
-- P.Habraken - 18/01/2010
--
-- 2 aout 2006 - PH :
-- remplacement record a champs variants par record "plat"
------------------------------------------------------------------------ */

   typedef enum {
	  ERREUR_CAR,   // caractere incorrect
	  CHIFFRE,			// chiffre          
	  SYMBOLE,			// symbole,
     CHAR,           //a-z, A-Z
      C_FIN_SEQUENCE,   // caractere de fin de sequence  
      C_SEP_AFF         //caractere de seperation des affectations (;)
   } Nature_Caractere ;

   typedef enum { 
      ENTIER,           // sequence de chiffres
      NOMBRE_A_VIRGULES, //sequence de chiffres contenant un "virgules" (rationnels, irrationnels)
      PLUS,             // +
      MOINS,            // -
      MUL,              // *
      DIV,              // /
      MOD,		// %
      PARO,             // (
      PARF,             // )
      OPCOMP,		// =, !=, <, <=, >, >=
      COMP_EQ,		// =
      COMP_NEQ,		// !=
      COMP_LTH,		// <
      COMP_LEQ,		// <=
      COMP_GTH,		// >
      COMP_GEQ,		// >=
      BOOL_ET,		// et
      BOOL_OU,		// ou
      BOOL_NON,		// non
      SI,            //si
      ALORS,         //alors
      SINON,         //
      FSI,           //fsi
      TANQUE,	   	//tant que
      FAIRE,		//faire
      FTANQUE,		//ftq
      OPBOOL,		// et, ou, non
      STRING,             //"..."
      IDF,              //variable pour affectation
      AFF,          //<-
      FIN_SEQUENCE,     // pseudo lexeme ajoute en fin de sequence
      SEPINST,           //;
      AFFICHER,
      AFFICHER_TS,
      LIRE,
      ENTIER_LISTE,     //[1,2,3,...]
      NOMBRE_A_VIRGULES_LISTE, //[1.1, 2.1, 3.1, ...]
      STRING_LISTE,  //["Hello", "world",...]
      POINT,
      LON_LISTE,
      ERREUR
    } Nature_Lexeme ;


   typedef struct { 
      Nature_Lexeme nature;    // nature du lexeme
      unsigned int ligne;           // numero de ligne
      unsigned int colonne;         // numero de colonne
      char chaine[256];           // chaine de caracteres
      int valeur;          // valeur d'un entier
      double partie_fraction; //valeur de la partie fraction
   } Lexeme ; 

   void afficher(Lexeme l);

   void demarrer(char *nom_fichier);
   // e.i. : indifferent
   // e.f. : la sequence de lexemes est lue dans le fichier designe par
   //        nom_fichier
   //        fin_de_sequence <=> lexeme_courant.nature = FIN_SEQUENCE
   //        (non fin_de_sequence) => lexeme courant est le premier
   //        lexeme de la sequence
   //        l'exception Erreur_Lexicale est levee en cas d'erreur

   void avancer();
   // pre-condition : la machine sequentielle est demarree
   // e.i. : on pose i = rang du lexeme_courant :
   //        (non fin_de_sequence)
   //        et (non lexeme_courant.nature = FIN_SEQUENCE)
   // e.f. : fin_de_sequence <=> lexeme_courant.nature = FIN_SEQUENCE
   //        (non fin_de_sequence) => lexeme_courant est le lexeme i+1
   //        l'exception Erreur_Lexicale est levee en cas d'erreur

   Lexeme lexeme_courant();
   // pre-condition : la machine sequentielle est demarree
   // lexeme_courant est :
   // . le pseudo lexeme FIN_SEQUENCE si fin_de_sequence
   // . le pseudo lexeme ERREUR si une erreur a ete detectee
   // . le lexeme de rang i sinon

   int fin_de_sequence();
   // pre-condition : la machine sequentielle est demarree
   // fin_de_sequence vaut :
   // . vrai si tous les lexemes de la sequence ont ete reconnus
   // . faux sinon

   void arreter();
   // e.i. : la machine sequentielle est demarree
   // e.f. : la machine sequentielle est arretee
