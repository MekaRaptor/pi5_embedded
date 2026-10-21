CC = gcc
CFLAGS = -Wall -Wextra

all: sicaklik test_alarm

sicaklik: sicaklik.c alarm.c alarm.h
	$(CC) $(CFLAGS) -o $@ sicaklik.c alarm.c

test_alarm: test_alarm.c alarm.c alarm.h
	$(CC) $(CFLAGS) -o $@ test_alarm.c alarm.c

test: test_alarm
	./test_alarm

analiz:
	cppcheck --enable=all --suppress=missingIncludeSystem *.c

kapsam:
	rm -f *.o *.gcda *.gcno *.gcov
	$(CC) $(CFLAGS) -O0 --coverage -fcondition-coverage -c alarm.c
	$(CC) $(CFLAGS) -O0 -c test_alarm.c
	$(CC) --coverage -o test_kapsam test_alarm.o alarm.o
	./test_kapsam
	gcov -b --conditions alarm.c

clean:
	rm -f sicaklik test_alarm test_kapsam *.o *.gcda *.gcno *.gcov

.PHONY: all test analiz kapsam clean