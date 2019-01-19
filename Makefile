CC=cc
CFLAGS=-Wall -Wextra -g3 -DLOGLEVEL=3 -Iinclude/ -fPIC
LIBS=-lX11 -ldl
TESTS=tests/drn_sll_test

all: clean bin/drn
.PHONY: all clean tags test

# Target build

bin/drn: lib/libdrn_cb.so 
bin/drn: src/drn.c lib/drn_sll.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

lib/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

lib/lib%.so: lib/%.o
	$(CC) -shared $< -o $@

# Testing

test: clean $(TESTS) bin/drn
	tests/runtests.sh

tests/drn_sll_test: lib/drn_sll.o tests/drn_sll_test.c 
	$(CC) $(CFLAGS) $^ -o $@


# Utilities

clean:
	rm -f lib/* bin/*
	rm -f vgcore* tests/drn_ssl_test

tags:
	ctags -e src/*.c include/*.h test/*.c

