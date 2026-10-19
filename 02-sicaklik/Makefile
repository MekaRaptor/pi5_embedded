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

clean:
	rm -f sicaklik test_alarm

.PHONY: all test analiz clean