
all:test_lexeme test_syntaxe

analyse_lexicale.c: lex.l analyse_lexicale.h
	flex -o $@ $<

comp: comp.o ast_parcours.o ast_construction.o table_symbole.o
	gcc -g -Wall -Ofast -o $@ $^

comp.o: comp.c temp.h ast_construction.h ast_parcours.h table_symbole.h
	gcc -g -Wall -Ofast -c $<

test_lexeme: analyse_lexicale.o lecture_caracteres.o  test_lexeme.o
	gcc -g -Wall -Ofast -o test_lexeme analyse_lexicale.o  lecture_caracteres.o  test_lexeme.o

test_syntaxe: analyse_syntaxique.o analyse_lexicale.o lecture_caracteres.o test_syntaxe.o ast_construction.o ast_parcours.o table_symbole.o
	gcc -g -Wall -Ofast -o $@ $^

analyse_lexicale.o: analyse_lexicale.c analyse_lexicale.h lecture_caracteres.h
	gcc -g -Wall -Ofast -c analyse_lexicale.c

analyse_syntaxique.o: analyse_syntaxique.c analyse_lexicale.h lecture_caracteres.h ast_construction.h ast_parcours.h table_symbole.h
	gcc -g -Wall -Ofast -c $<

lecture_caracteres.o: lecture_caracteres.h lecture_caracteres.c
	gcc -g -Wall -Ofast -c lecture_caracteres.c

test_lexeme.o: analyse_lexicale.h
	gcc -g -Wall -Ofast -c test_lexeme.c

test_syntaxe.o: test_syntaxe.c analyse_syntaxique.h analyse_lexicale.h
	gcc -g -Wall -Ofast -c $<


# TP3
ast_construction.o: ast_construction.c type_ast.h
	gcc -g -Wall -Ofast -c ast_construction.c

ast_parcours.o: ast_parcours.c type_ast.h table_symbole.h
	gcc -g -Wall -Ofast -c ast_parcours.c

essai_ast.o: essai_ast.c  ast_construction.h  ast_parcours.h  type_ast.h
	gcc -g -Wall -Ofast -c essai_ast.c

table_symbole.o: table_symbole.c table_symbole.h
	gcc -g -Wall -Ofast -c $<

clean:
	rm -f  test_lexeme test_syntaxe analyse_lexicale.c *.o


