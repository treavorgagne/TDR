
CC = g++
CFLAGS = -Wall -Wextra -Werror -std=c++1z
LIBS = -L C:/SFML-min/SFML-2.5.1/lib  -lsfml-system -lsfml-window -lsfml-graphics -lstdc++
INC = -I C:/SFML-min/SFML-2.5.1/include

# ifeq ($(OS), Windows_NT)
# 	LIBS += -I C:/SFML-min/SFML-2.5.1/include -L C:/SFML-min/SFML-2.5.1/lib
# else
# 	LIBS += -lsfml-system -lsfml-window -lsfml-graphics -lstdc++
# endif

all: build

gamelogic.o: gamelogic.cpp
	$(CC) -c gamelogic.cpp $(INC)

build: gamelogic.o
	$(CC) gamelogic.o -o gamelogic.exe $(LIBS) $(CFLAGS)

clean:
	rm gamelogic.o gamelogic.exe
