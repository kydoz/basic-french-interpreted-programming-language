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

