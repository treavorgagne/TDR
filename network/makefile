CC = g++
CFLAGS = -Wall -Wextra -Werror -std=c++1z
LIBS =
ifeq ($(OS), Windows_NT)
	LIBS += -I C:/SFML-min/SFML-2.5.1/include -L C:/SFML-min/SFML-2.5.1/lib
else
	LIBS += -lsfml-network -lsfml-system -lstdc++
endif

all: server client

server.o: server.cpp
	$(CC) -c server.cpp

client.o: client.cpp
	$(CC) -c client.cpp

playerinfo.o: playerinfo.cpp
	$(CC) -c playerinfo.cpp

server: server.o playerinfo.o
	$(CC) -o server.run server.o playerinfo.o $(CFLAGS) $(LIBS)

client: client.o playerinfo.o
	$(CC) -o client.run client.o playerinfo.o $(CFLAGS) $(LIBS)


clean:
	rm server.o client.o
