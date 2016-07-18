CC=g++49
CFLAGS=-std=c++11 -static-libstdc++

all:	client server

client.o: client.cpp
	$(CC) $(CFLAGS) -c client.cpp -o client.o

server.o: server.cpp 
	$(CC) $(CFLAGS) -c server.cpp -o server.o

client: client.o			
	$(CC) $(CFLAGS) client.o -o client

server: server.o	
	$(CC) $(CFLAGS) server.o -o server
	
clean:
	rm -f *.o  *.zip deska ./src/*.o
pack:
	zip xkonde03.tar.gz *.cpp *.h Makefile *pdf

