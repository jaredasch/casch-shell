all: main.o parsing.o piping.o
	gcc -o out main.o parsing.o piping.o

main.o: main.c parsing.h parsing.h
	gcc -c main.c

parsing.o: parsing.c
	gcc -c parsing.c

piping.o: piping.c
	gcc -c piping.c

run:
	./out

test:
	./out < test_commands

clean:
	rm out
	rm *.o
