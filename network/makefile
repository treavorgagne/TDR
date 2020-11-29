CC = g++
CFLAGS = -Wall -Wextra -Werror -std=c++1z
LIBS =
INC =

ifeq ($(OS), Windows_NT)
	LIBS +=-L C:/SFML-min/SFML-2.5.1/lib
	INC += -I C:/SFML-min/SFML-2.5.1/include
endif

LIBS += -lsfml-system -lsfml-network -lstdc++

all: server client

server.o: server.cpp
	$(CC) -c server.cpp server_test.cpp $(INC)

client.o: client.cpp
	$(CC) -c client.cpp client_test.cpp $(INC)

playerinfo.o: playerinfo.cpp
	$(CC) -c playerinfo.cpp $(INC)

server: server.o playerinfo.o server_test.o
	$(CC) -o server_test.exe server.o playerinfo.o server_test.o $(CFLAGS) $(LIBS)

client: client.o playerinfo.o client_test.o
	$(CC) -o client_test.exe client.o playerinfo.o client_test.o $(CFLAGS) $(LIBS)

clean:
	rm server.o client.o
