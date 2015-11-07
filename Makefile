CXXFLAGS=-Wall -Werror -ansi -pedantic

all:
	rm -rf bin
	mkdir bin
	g++ $(CXXFLAGS) src/main.cpp -o bin/rshell

rshell: main.cpp
	g++ $(CXXFLAGS) src/main.cpp -o bin/rshell
