CC=cc
CFLAGS=-Wall -Wextra -g3 -DLOGLEVEL=$(LOGLEVEL) -Iinclude/ -fPIC
LIBS=-lX11 -ldl
OBJ=lib/drn_sll.o
SO=
DLSO=lib/libdrn_cb.so
TARGET=bin/drn
TSRC=src/drn.c
TESTS=tests/drn_sll_test
PREFIX?=/usr/local
LOGLEVEL?=1

all: $(TARGET)
.PHONY: all clean tags test libinstall install

# Target build
$(TARGET): $(SO) $(DLSO)
$(TARGET): $(TSRC) $(OBJ)
	$(CC) $(CFLAGS) $(TSRC) $(OBJ) $(SO) -o $@ $(LIBS)

lib/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

lib/lib%.so: lib/%.o
	$(CC) -shared $< -o $@

# Testing
test: LOGLEVEL=3
test: clean $(TESTS) $(TARGET)
	tests/runtests.sh

tests/%_test: tests/%_test.c $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

# Install
libinstall: LOGLEVEL=0
libinstall: $(DLSO)
	sed -i 's|lib/libdrn_cb.so|$(PREFIX)/lib/libdrn_cb.so|' src/drn.c	
	-install -d $(PREFIX)/lib
	-install -m 644 $^ $(PREFIX)/lib
	sed -i 's|$(PREFIX)/lib/libdrn_cb.so|lib/libdrn_cb.so|' src/drn.c

install: LOGLEVEL=0
install: clean all libinstall
	install -d $(PREFIX)/bin
	install -m 755 bin/drn $(PREFIX)/bin
	install -m 644 lib/drn_sll.o $(PREFIX)/lib

uninstall:
	rm -f $(PREFIX)/bin/drn
	rm -f $(PREFIX)/lib/{drn_sll.o,libdrn_cb.so}

# Utilities
clean:
	rm -f lib/* bin/*
	rm -f vgcore* tests/drn_ssl_test tests/log

tags:
	ctags -e src/*.c include/*.h
