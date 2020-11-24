CC = g++
CFLAGS = -Wall -Wextra -Werror -std=c++2a
LIBS = -lsfml-network -lsfml-system -lstdc++

all: server client

server.o: server.cpp
	$(CC) -c server.cpp

client.o: client.cpp
	$(CC) -c client.cpp

server: server.o
	$(CC) -o server.run server.o $(CFLAGS) $(LIBS)

client: client.o
	$(CC) -o client.run client.o $(CFLAGS) $(LIBS)


clean:
	rm server.o client.o
