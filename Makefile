CC=cc
CFLAGS=-Wall -Wextra -Wfatal-errors -g3 -DLOGLEVEL=$(LOGLEVEL) -Iinclude/ -fPIC
LIBS=-lX11 -ldl
OBJ=lib/drn.o lib/drn_strings.o lib/drn_x11.o lib/drn_signal.o lib/drn_dlsym.o
SO=
DLSO=lib/libdrn_cb.so
TARGET=bin/drn
TSRC=src/main.c
TESTS=
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
	install -m 755 $(TARGET) $(PREFIX)/bin

uninstall:
	rm -f $(PREFIX)/$(TARGET)
	rm -f $(addprefix $(PREFIX)/, $(DLSO))

# Utilities
clean:
	rm -f $(TARGET) $(OBJ) $(DLSO) $(SO)
	rm -f vgcore* tests/drn_ssl_test tests/log

tags:
	etags src/*.c include/*.h

hello:
	echo $(LIBS)/$(DLSO)
