all: businessdb

CFLAGS = -Wall -Wpedantic -Werror -Wextra
CC =  clang

businessdb: businessdb.c
	$(CC) $(CFLAGS) businessdb.c -o ./businessdb
