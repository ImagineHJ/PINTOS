cc = gcc
target = testlib
objects = main.o bitmap.o debug.o hash.o hex_dump.o list.o

$(target): $(objects)
	$(cc) -o $(target) $(objects)

main.o : main.c main.h
	$(cc) -c -o main.o main.c

bitmap.o : bitmap.c bitmap.h limits.h round.h hex_dump.h
	$(cc) -c -o bitmap.o bitmap.c

debug.o : debug.c debug.h
	$(cc) -c -o debug.o debug.c

hash.o : hash.c hash.h
	$(cc) -c -o hash.o hash.c

hex_dump.o : hex_dump.c hex_dump.h round.h
	$(cc) -c -o hex_dump.o hex_dump.c

list.o : list.c list.h
	$(cc) -c -o list.o list.c

clean :
	rm $(target) $(objects)

