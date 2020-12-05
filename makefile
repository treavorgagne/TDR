
CC = g++
CFLAGS = -Wall -Wextra -std=c++1z
LIBS =
INC =
ifeq ($(OS), Windows_NT)
	LIBS +=-L C:/SFML-min/SFML-2.5.1/lib
	INC += -I C:/SFML-min/SFML-2.5.1/include
else
	LIBS += -pthread
endif

LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lsfml-audio -lstdc++

all: client server 

server: gameServer.o server.o playerinfo.o
	$(CC) -o gameServer.exe gameServer.o server.o playerinfo.o $(LIBS)

client: SoundPlayer.o gameClient.o client.o playerinfo.o 
	$(CC) -o gameClient.exe SoundPlayer.o gameClient.o client.o playerinfo.o $(LIBS)

gameClient.o: gameClient.cpp
	$(CC) -c gameClient.cpp $(CFLAGS) $(INC)

gameServer.o: gameServer.cpp
	$(CC) -c gameServer.cpp $(CFLAGS) $(INC)

server.o: network/server.cpp
	$(CC) -c network/server.cpp $(CFLAGS) $(INC)

client.o: network/client.cpp
	$(CC) -c network/client.cpp $(CFLAGS) $(INC)

playerinfo.o: network/playerinfo.cpp
	$(CC) -c network/playerinfo.cpp $(CFLAGS) $(INC)

SoundPlayer.o: audio/SoundPlayer.cpp
	$(CC) -c audio/SoundPlayer.cpp $(CFLAGS) $(INC)

clean:
	rm gameClient.o gameClient.exe client.o playerinfo.o SoundPlayer.o
