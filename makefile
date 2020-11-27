
CC = g++
CFLAGS = -Wall -Wextra -std=c++1z
LIBS =
ifeq ($(OS), Windows_NT)
	LIBS += -I C:/SFML-2.5.1/include -MMD -MP -DSFML_STATIC -IC:/SFML-2.5.1/include/SFML/Graphics.hpp
	LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lsfml-audio
else
	LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lsfml-audio
endif



all: client

gameClient.o: gameClient.cpp
	$(CC) -c gameClient.cpp $(CFLAGS) $(LIBS)

client: gameClient.o client.o playerinfo.o
	$(CC) -o gameClient.exe gameClient.o client.o playerinfo.o $(LIBS)


clean:
	rm gameClient.o game.exe

server.o: network/server.cpp
	$(CC) -c network/server.cpp $(LIBS)


client.o: network/client.cpp
	$(CC) -c network/client.cpp $(LIBS)


playerinfo.o: network/playerinfo.cpp
	$(CC) -c network/playerinfo.cpp $(LIBS)


clean:
	rm gameClient.o
