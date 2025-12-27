# Sol.
#

CC=         gcc
AR=         ar rcs
RM=         rm -rf

STD=        gnu99
CFLAGS=     -std=$(STD) -Wall -Wextra
LDFLAGS=

LIB-SRC=        \
	lexer.c       \
	parser.c      \
	parser_expr.c \
	util.c
LIB-OBJ=$(LIB-SRC:.c=.o)
LIB-TRG=libsol.a

EXC-SRC= \
	main.c
EXC-OBJ=$(EXC-SRC:.c=.o)
EXC-TRG=sol

.PHONY: default build debug release clean help

default: release
build: $(LIB-TRG) $(EXC-TRG)

debug:    build
debug:    CFLAGS+=  -g -D _DEBUG
release:  build
release:  CFLAGS+=  -O2

$(LIB-TRG): $(LIB-OBJ)
	$(AR) $@ $^

$(EXC-TRG): $(EXC-OBJ) $(LIB-TRG)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(LIB-OBJ) $(LIB-TRG)
	$(RM) $(EXC-OBJ) $(EXC-TRG)

help:
	@echo 'Sol Makefile'
	@echo '  help         Show this help'
	@echo '  clean        Delete built files'
	@echo '  build        Plain build'
	@echo '  default      Default build'
	@echo '  debug        Debug build'
	@echo '  release      Release build'
