CC=clang
CFLAGS=-c -Wall -O3
LDFLAGS=
SOURCES=bsdfan.c common.c parser.c mystring.c system.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=bsdfan

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o bsdfan
install:
	cp ./bsdfan /usr/local/sbin/ && cp ./bsdfan.conf /usr/local/etc/ && cp ./bsdfan.1.gz /usr/local/man/man1/
deinstall:
	rm -f /usr/local/sbin/bsdfan && rm -f /usr/local/etc/bsdfan.conf && rm -f /usr/local/man/man1/bsdfan.1.gz
