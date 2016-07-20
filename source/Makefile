all: main

clean:
	rm -f parser.cpp parser.hpp tokens.cpp

tokens.cpp:
	lex -o tokens.cpp tokens.l

parser.cpp:
	bison -d -o parser.cpp parser.y

main: tokens.cpp parser.cpp
	g++ -o parser main.cpp parser.cpp tokens.cpp --std=c++11 -Wno-deprecated-register -Wno-reorder -Wno-unused-function

run: clean main
	./parser < ../design/demo.bulkan