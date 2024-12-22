CC=gcc
FLAGS=-std=c11 -Werror -Wextra -Wall

all: stat_analiz

stat_analiz:
	${CC} ${FLAGS} StatAnaliz.c -lm
	rm -rf *.o

clean:
	rm -rf a.out 