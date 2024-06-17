# Copyright (2024) Net Privacy Pro; see README.md and LICENSE for more details

CC=gcc
CFLAGS=-Wall -Wextra -g
LIBS=-lssl -lcrypto

all: server client

server: server.o
	$(CC) -o server server.o $(LIBS)

client: client.o
	$(CC) -o client client.o $(LIBS)

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

client.o: client.c
	$(CC) $(CFLAGS) -c client.c

clean:
	rm -f *.o server client
