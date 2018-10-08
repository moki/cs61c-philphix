CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g3 -Wno-pointer-arith

all: philphix.out

philphix.out: philphix.o lib/djb2/djb2.o lib/hash_table/hash-table.o lib/char_vector/char-vector.o
	$(CC) $(CFLAGS) -o $@ $^

philphix.o: philphix.c philphix.h

lib/djb2/djb2.o: lib/djb2/djb2.c lib/djb2/djb2.h
lib/hash_table/hash-table.o: lib/hash_table/hash-table.c lib/hash_table/hash-table.h
lib/char_vector/char-vector.o: lib/char_vector/char-vector.c lib/char_vector/char-vector.h

test: all test-case

test-case:
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes cat test.txt | ./philphix.out replace.txt > output.txt
	echo "Difference between test output and reference output"
	diff output.txt reference.txt


clean:
	$(RM) philphix.out *.o vgcore.* lib/*/*.o