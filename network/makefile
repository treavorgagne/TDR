CC = g++
CFLAGS = -Wall -Wextra -Werror -std=c++2a
LIBS = -lsfml-network -lsfml-system -lstdc++

all: server client

server.o: server.cpp
	$(CC) -c server.cpp

client.o: client.cpp
	$(CC) -c client.cpp

packet_overloads.o: packet_overloads.cpp
	$(CC) -c packet_overloads.cpp

server: server.o
	$(CC) -o server.run server.o packet_overloads.o $(CFLAGS) $(LIBS)

client: client.o
	$(CC) -o client.run client.o packet_overloads.o $(CFLAGS) $(LIBS)


clean:
	rm server.o client.o
