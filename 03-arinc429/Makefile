CC = gcc
CFLAGS = -Wall -Wextra

test_arinc: test_arinc.c arinc429.c arinc429.h
	$(CC) $(CFLAGS) -o $@ test_arinc.c arinc429.c

test: test_arinc
	./test_arinc

analiz:
	cppcheck --enable=all --inline-suppr --suppress=missingIncludeSystem *.c

kapsam:
	rm -f *.o *.gcda *.gcno *.gcov
	$(CC) $(CFLAGS) -O0 --coverage -fcondition-coverage -c arinc429.c
	$(CC) $(CFLAGS) -O0 -c test_arinc.c
	$(CC) --coverage -o test_kapsam test_arinc.o arinc429.o
	./test_kapsam
	gcov -b --conditions arinc429.c

clean:
	rm -f test_arinc test_kapsam *.o *.gcda *.gcno *.gcov

.PHONY: test analiz kapsam clean