CC=cc
LOGLEVEL?=2
CFLAGS=-Wall -Wextra -g3 -DLOGLEVEL=$(LOGLEVEL) -Iinclude/ -fPIC
LIBS=-lX11 -ldl
TESTS=tests/drn_sll_test
PREFIX?=/usr/local

all: bin/drn
.PHONY: all clean tags test install

# Target build

bin/drn: lib/libdrn_cb.so 
bin/drn: src/drn.c lib/drn_sll.o
	$(CC) $(CFLAGS) src/drn.c lib/drn_sll.o -o $@ $(LIBS)

lib/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

lib/lib%.so: lib/%.o
	$(CC) -shared $< -o $@

# Testing

test: LOGLEVEL=3
test: clean $(TESTS) bin/drn
	tests/runtests.sh

tests/drn_sll_test: lib/drn_sll.o tests/drn_sll_test.c 
	$(CC) $(CFLAGS) $^ -o $@

# Install

install: LOGLEVEL=1
install: all
	sed -i 's|lib/libdrn_cb.so|/usr/local/lib/libdrn_cb.so|' src/drn.c
	rm bin/drn
	make bin/drn
	install -d $(PREFIX)/bin
	install -m 755 bin/drn $(PREFIX)/bin
	install -d $(PREFIX)/lib
	install -m 644 lib/drn_sll.o $(PREFIX)/lib
	install -m 644 lib/libdrn_cb.so $(PREFIX)/lib
	sed -i 's|/usr/local/lib/libdrn_cb.so|lib/libdrn_cb.so|' src/drn.c

uninstall:
	rm -f $(PREFIX)/bin/drn
	rm -f $(PREFIX)/lib/{drn_sll.o,libdrn_cb.so}
	rm -f $(PREFIX)/etc/drn/drn.service

# Systemd unit file install

systemd: etc/drn.service
	install -d $(HOME)/.config/systemd/user
	install -m 644 etc/drn.service $(HOME)/.config/systemd/user
	systemctl --user enable drn.service
	systemctl --user start drn.service

unsystemd:
	systemctl --user stop drn.service
	systemctl --user disable drn.service
	rm -f $(HOME)/.config/systemd/user/drn.service

# Utilities

clean:
	rm -f lib/* bin/*
	rm -f vgcore* tests/drn_ssl_test tests/log

tags:
	ctags -e src/*.c include/*.h

