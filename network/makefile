

all: build


server.o: server.cpp
	g++ -Wall -Wextra -Werror -o server.o -c server.cpp

client.o: client.cpp
	g++ -Wall -Wextra -Werror -o client.o -c client.cpp

build: server.o client.o
	g++ server.o -o server.run -lsfml-network -lsfml-system
	g++ client.o -o client.run -lsfml-network -lsfml-system

clean:
	rm server.o client.o
