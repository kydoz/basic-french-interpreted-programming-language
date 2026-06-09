SHELL := /bin/bash

CC=gcc
CC_LEX=flex

SRCDIR=srcs
OBJDIR=objs
EXECDIR=bin
INCLUDEDIR=include

DEBUG_FLAGS=-g
OPTIMIZATION_FLAG=-Ofast
CPPFLAGS=-I$(INCLUDEDIR)


EXEC_SYNTAXE=$(EXECDIR)/test_syntaxe
EXEC_LEXEME=$(EXECDIR)/test_lexeme
EXEC_COMP=$(EXECDIR)/comp
EXECS=$(EXEC_LEXEME) $(EXEC_SYNTAXE) $(EXEC_COMP)
RES_LEX=$(SRCDIR)/analyse_lexicale.c

ifdef LEXEME
SRCS=$(filter-out $(SRCDIR)/comp.c $(SRCDIR)/test_syntaxe.c,$(wildcard $(SRCDIR)/*.c))
EXEC=$(EXEC_LEXEME)
else ifdef COMP
SRCS=$(filter-out $(SRCDIR)/test_lexeme.c $(SRCDIR)/test_syntaxe.c,$(wildcard $(SRCDIR)/*.c))
EXEC=$(EXEC_COMP)
else
SRCS=$(filter-out $(SRCDIR)/comp.c $(SRCDIR)/test_lexeme.c,$(wildcard $(SRCDIR)/*.c))
EXEC=$(EXEC_SYNTAXE)
endif


OBJS=$(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
HDRS=$(wildcard $(SRCDIR)/*.h)

ifdef UBSAN # valgrind continue a marcher avec ubsan
CFLAGS+= -fsanitize=undefined $(DEBUG_FLAGS)
LDLIBS:=-lubsan  $(LDLIBS)
endif

ifdef ASAN
CFLAGS+=-fsanitize=address $(DEBUG_FLAGS)
LDLIBS:=-lasan $(LDLIBS)
endif


.PHONY: clean all debug comp fuzz

all: $(RES_LEX) $(EXEC)

debug: clean
debug: CFLAGS += $(DEBUG_FLAGS)
debug: OPTIMIZATION_FLAG = -O1
debug: all

$(RES_LEX): $(SRCDIR)/lex.l $(INCLUDEDIR)/analyse_lexicale.h
	$(CC_LEX) -o $@ $<

$(EXEC): $(OBJS)
	$(CC) $(OPTIMIZATION_FLAG) $^ -o $@ -Wl,-Bdynamic $(LDLIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(HDRS)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

clean:
	-rm -f  $(EXECS) $(SRCDIR)/$(RES_LEX) $(OBJDIR)/*

.ONESHELL: #https://www.gnu.org/software/make/manual/html_node/One-Shell.html
verif_valgrind:
	@if [ ! -e valgrind_resultats ] || [ ! -d valgrind_resultats ]; then mkdir valgrind_resultats; fi
	@$(MAKE) debug
	@declare -a texts=("Tests Simples\n" "Algos\n" "Testes robustesse\n")
	@declare -a locations=("Tests" "Tests/algos" "Tests/testes_robustesse")

	@len=$$(expr $${#locations[*]} - 1)
	@for i in $$(seq 0 $${len}); do
		printf "$${texts[$${i}]}"
		for test in $${locations[$${i}]}/*.txt; do

			if [ -d $${test} ]; then continue; fi

			echo $${test}
			cat $${test}
			echo
			name="$$(basename $${test})"
			name="$${name%.*}"
			valgrind --leak-check=full \
				--show-leak-kinds=all \
				--track-origins=yes \
				--verbose \
				--log-file=./valgrind_resultats/"$${name}"_valgrind.txt \
				./$(EXEC) $${test}
			echo
		done
	done
	@$(MAKE) clean

verif_asan:
	@if [ ! -e asan_resultats ] || [ ! -d asan_resultats ]; then mkdir asan_resultats; fi
	@$(MAKE) debug ASAN=1
	@declare -a texts=("Tests Simples\n" "Algos\n" "Testes robustesse\n")
	@declare -a locations=("Tests" "Tests/algos" "Tests/testes_robustesse")

	@len=$$(expr $${#locations[*]} - 1)
	@for i in $$(seq 0 $${len}); do
		printf "$${texts[$${i}]}"
		for test in $${locations[$${i}]}/*.txt; do

			if [ -d $${test} ]; then continue; fi

			echo $${test}
			cat $${test}
			echo
			name="$$(basename $${test})"
			name="$${name%.*}"
			./$(EXEC) $${test} 2> ./asan_resultats/"$${name}"_asan.txt
			echo
		done
	done
	@$(MAKE) clean


verif_ubsan:
	@if [ ! -e ubsan_resultats ] || [ ! -d ubsan_resultats ]; then mkdir ubsan_resultats; fi
	@$(MAKE) debug UBSAN=1
	@declare -a texts=("Tests Simples\n" "Algos\n" "Testes robustesse\n")
	@declare -a locations=("Tests" "Tests/algos" "Tests/testes_robustesse")

	@len=$$(expr $${#locations[*]} - 1)
	@for i in $$(seq 0 $${len}); do
		printf "$${texts[$${i}]}"
		for test in $${locations[$${i}]}/*.txt; do

			if [ -d $${test} ]; then continue; fi

			echo $${test}
			cat $${test}
			echo
			name="$$(basename $${test})"
			name="$${name%.*}"
			./$(EXEC) $${test} 2> ./ubsan_resultats/"$${name}"_ubsan.txt
			echo
		done
	done
	@$(MAKE) clean





