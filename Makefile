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
test: clean all
	@if [ "$(EXEC)" = $(EXEC_COMP) ]; then echo "Cannot execute target with executable $(EXEC)"; exit 1; fi
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
			./$(EXEC) $${test}
			sleep 2
			echo
		done
	done


verif_valgrind:
	@if [ "$(EXEC)" = $(EXEC_COMP) ]; then echo "Cannot execute target with executable $(EXEC)"; exit 1; fi
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
	@if [ "$(EXEC)" = $(EXEC_COMP) ]; then echo "Cannot execute target with executable $(EXEC)"; exit 1; fi
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
	@if [ "$(EXEC)" = $(EXEC_COMP) ]; then echo "Cannot execute target with executable $(EXEC)"; exit 1; fi
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


create_fuzz_in: #add test files that dont read (lire function) from user (execution won't hang)
	@loop() {
		if [ ! -d "$${1}" ]; then return; fi

		for file in $${1}/*; do

			if [ -d "$${file}" ] && [ "$$(basename $${file})" != "testes_robustesse" ]; then loop "$${file}"; fi

			if [ -f "$${file}" ] && [ ! -n "$$(cat $${file} | grep "lire")" ]; then cp $${file} fuzz_in/; fi

		done
	}
	@mkdir -p fuzz_in
	@loop Tests


fuzz: create_fuzz_in
	@if [ "$(EXEC)" = $(EXEC_COMP) ]; then echo "Cannot execute target with executable $(EXEC)"; exit 1; fi
	@if [ ! -n "$${AFL_PATH}" ]; then echo "AFL_PATH vide, ajoutez le chemin"; exit; fi
	@if [ -n "$(FUZZ_UBSAN)" ]; then export AFL_USE_UBSAN=1; else export AFL_USE_ASAN=1; fi
	@echo "ASAN $${AFL_USE_ASAN}"
	@echo "UBSAN $${AFL_USE_UBSAN}"
	export AFL_DONT_OPTIMIZE=1
	@echo "compile the pgm for AFL"
	@$(MAKE) create_fuzz_in debug CC=$${AFL_PATH}/afl-gcc-fast
	echo "run AFL !"
	export AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES=1 
	export AFL_SKIP_CPUFREQ=1 
	if [ -n "$(TIMEOUT)" ]; then
		$${AFL_PATH}/afl-fuzz -m none -i fuzz_in/ -o fuzz_out/ ./$(EXEC) @@ &
		sleep $${TIMEOUT} #timeout, peut etre supprime plus tard
		kill $$! #$! est le pid de dernier commande du dernier plan execute
	else
		$${AFL_PATH}/afl-fuzz -m none -i fuzz_in/ -o fuzz_out/ ./$(EXEC) @@
	fi


