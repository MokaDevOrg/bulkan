all: main

clean:
	rm -f parser.cpp parser.hpp tokens.cpp

tokens.cpp:
	lex -o tokens.cpp tokens.l

parser.cpp:
	bison -d -o parser.cpp parser.y

main: tokens.cpp parser.cpp
	g++ main.cpp -o parser parser.cpp tokens.cpp Context.cpp CodeGeneration/*.cpp --std=c++11 -Wno-deprecated-register -Wno-reorder -Wno-unused-function

run: clean main
	./parser < ../design/demo.bulkan
