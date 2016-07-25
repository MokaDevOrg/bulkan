all: main

clean:
	rm -f bulkan/gen/*

bulkan/gen/tokens.cpp:
	lex -o $@ bulkan/parser/tokens.l

bulkan/gen/parser.cpp:
	bison -d -o $@ bulkan/parser/parser.y

main: bulkan/gen/tokens.cpp bulkan/gen/parser.cpp
	g++ main.cpp -o bin/parser bulkan/*.cpp bulkan/**/*.cpp --std=c++11 -Wno-deprecated-register -Wno-reorder -Wno-unused-function

run: clean main
	./bin/parser < tests/factorial.bulkan
	gcc -o output/output output/output_body.c
