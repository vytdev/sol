# Sol.
#

CC=         gcc
RM=         rm -rf

STD=        gnu99
CFLAGS=     -std=$(STD) -Wall -Wextra
LDFLAGS=

SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
TRG=sol

.PHONY: default build debug release clean help

default: release
build: $(TRG)

debug:    build
debug:    CFLAGS+=    -g -D _DEBUG

release:  build
release:  CFLAGS+=  -O2

$(TRG): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJ) $(TRG)

help:
	@echo 'Sol Makefile'
	@echo '  help         Show this help'
	@echo '  clean        Delete built files'
	@echo '  build        Plain build'
	@echo '  default      Default build'
	@echo '  debug        Debug build'
	@echo '  release      Release build'
