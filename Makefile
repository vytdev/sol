# Sol.
#

CWARNS= \
  -Wshadow \
  -Wundef \
  -Wwrite-strings \
  -Wdisabled-optimization \
  -Wmissing-declarations \
  # -pedantic

MYCFLAGS= $(CWARNS) -std=c99
MYLDFLAGS=

CC= gcc
AR= ar rc
RANLIB= ranlib
RM= rm -rf
MKDIR= mkdir -p

CFLAGS= -Wall -Wextra $(MYCFLAGS) -MMD -MP
LDFLAGS= $(MYLDFLAGS)

## build file names

OUTD= build

LIB_C= compile.c lexer.c parser_expr.c vm.c
LIB_O= $(LIB_C:%.c=$(OUTD)/%.o)
LIB_D= $(LIB_O:.o=.d)
LIB_T= libsol.a

SOL_C= main.c util.c
SOL_O= $(SOL_C:%.c=$(OUTD)/%.o)
SOL_D= $(SOL_O:.o=.d)
SOL_T= sol

ALL_O= $(LIB_O) $(SOL_O)
ALL_D= $(LIB_D) $(SOL_D)
ALL_T= $(LIB_T) $(SOL_T)

.PHONY: all debug release ci test clean help echo

all: $(ALL_T)
debug release ci: all

ci:
	$(MAKE) test

debug:    CFLAGS+= -Og -g
release:  CFLAGS+= -O2 -DNDEBUG -march=native
ci:       CFLAGS+= -Werror

test: all
	@echo 'No tests yet.'

clean:
	$(RM) $(OUTD) $(ALL_T)

help:
	@echo 'Sol Makefile'
	@echo '  help        Show this help'
	@echo '  clean       Delete generated files'
	@echo '  echo        Print make vars'
	@echo '  test        Run tests'
	@echo 'Build targets'
	@echo '  all         Default (plain) build'
	@echo '  debug       For debugging'
	@echo '  release     Optimized build'
	@echo '  ci          For CI checking'

echo:
	@echo 'CC = $(CC)'
	@echo 'AR = $(AR)'
	@echo 'RANLIB = $(RANLIB)'
	@echo 'RM = $(RM)'
	@echo 'MKDIR = $(MKDIR)'
	@echo 'MYCFLAGS = $(MYCFLAGS)'
	@echo 'MYLDFLAGS = $(MYLDFLAGS)'

$(LIB_T): $(LIB_O)
	$(AR) $@ $^
	$(RANLIB) $@

$(SOL_T): $(SOL_O) $(LIB_T)
	$(CC) $(LDFLAGS) -o $@ $^

$(OUTD)/%.o: %.c | $(OUTD)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OUTD):
	$(MKDIR) $@

-include $(ALL_D)
