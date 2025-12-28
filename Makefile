# Sol.
#

CC=         gcc
AR=         ar rcs
RM=         rm -rf

STD=        gnu99
CFLAGS=     -std=$(STD) -Wall -Wextra -MMD -MP
LDFLAGS=

LIB-SRC=        \
	compile.c     \
	lexer.c       \
	parser_expr.c \
	vm.c
LIB-OBJ=$(LIB-SRC:.c=.o)
LIB-DEP=$(LIB-SRC:.c=.d)
LIB-TRG=libsol.a

EXC-SRC= \
	main.c \
	util.c
EXC-OBJ=$(EXC-SRC:.c=.o)
EXC-DEP=$(EXC-SRC:.c=.d)
EXC-TRG=sol

.PHONY: default build debug release clean distclean help

default: release
build: $(LIB-TRG) $(EXC-TRG)

debug:    build
debug:    CFLAGS+=  -g -D _DEBUG
release:  build
release:  CFLAGS+=  -O2

-include $(LIB-DEP) $(EXC-DEP)

$(LIB-TRG): $(LIB-OBJ)
	$(AR) $@ $^

$(EXC-TRG): $(EXC-OBJ) $(LIB-TRG)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(LIB-OBJ) $(EXC-OBJ) $(LIB-TRG) $(EXC-TRG)

distclean: clean
	$(RM) $(LIB-DEP) $(EXC-DEP)

help:
	@echo 'Sol Makefile'
	@echo '  help         Show this help'
	@echo '  clean        Delete built files'
	@echo '  distclean    Clean including *.d'
	@echo 'Build types'
	@echo '  build        Plain build'
	@echo '  default      Default build'
	@echo '  debug        Debug build'
	@echo '  release      Release build'
	@echo 'Build targets'
	@echo '  libsol.a     Sol static lib'
	@echo '  sol          Sol standalone rt'
