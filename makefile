all: main.o parsing.o
	gcc -o out main.o parsing.o

main.o: main.c parsing.h
	gcc -c main.c

parsing.o: parsing.c
	gcc -c parsing.c

run:
	./out

clean:
	rm out
	rm *.o
