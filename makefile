

CPPFLAGS := -I C:/SFML-2.5.1/include -MMD -MP -DSFML_STATIC -IC:/SFML-2.5.1/include/SFML/Graphics.hpp
CXXFLAGS := -std=c++17 -Wall -W -pedantic
LDFLAGS  := -L C:/SFML-2.5.1/lib
LDLIBS   := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lsfml-audio


all: build

gameClient.o: gameClient.cpp
	g++ -c gameClient.cpp -I C:/SFML-min/SFML-2.5.1/include -std=c++17

build: gameClient.o
	g++ gameClient.o -o game.exe -I C:/SFML-min/SFML-2.5.1/include -L C:/SFML-min/SFML-2.5.1/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network

clean:
	rm gameClient.o game.exe
