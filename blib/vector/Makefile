run: main
	./vector_test

main: clean vector.o
	gcc -o vector_test vector_test.c vector.o

vector.o:
	gcc -c vector.c

clean:
	rm -f *.o
	rm -f vector_test
