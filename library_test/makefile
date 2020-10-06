

all: build


test.o: test.cpp
	g++ -Wall -Wextra -Werror -o test.o -c test.cpp


build: test.o
	g++ test.o -o test.run -lsfml-network


clean:
	rm test.o
