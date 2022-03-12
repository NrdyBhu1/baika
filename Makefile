LIBS=vte-2.91
CC=gcc
CFLAGS=-std=c99 -O2 -Wall -ggdb `pkg-config --cflags $(LIBS)`
LDFLAGS=`pkg-config --libs --static $(LIBS)` -lm
PREFIX=/usr/local
DEST=/bin/

all: config.h baika baika.1.gz

baika: baika.o
	$(CC) -o baika baika.o $(LDFLAGS)

baika.o: config.h baika.c
	$(CC) -c $(CFLAGS) baika.c

config.h: config.in.h
	cp config.in.h config.h -v

baika.1.gz: baika.1
	gzip -c baika.1 > baika.1.gz

install:
	install baika $(PREFIX)$(DEST)
	install baika.1.gz /usr/share/man/man1/

uninstall:
	rm $(PREFIX)$(DEST)/baika
	rm /usr/share/man/man1/baika.1.gz 

clean:
	rm baika.o
	rm baika
	rm baika.1.gz
	rm config.h
