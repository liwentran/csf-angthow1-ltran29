CC = gcc
CFLAGS = -g -Wall

SRCS = parsort.c is_sorted.c gen_rand_data.c
OBJS = $(SRCS:%.c=%.o)
EXES = $(SRCS:%.c=%)

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $*.o

all : $(EXES)

parsort : parsort.o
	$(CC) -o $@ $@.o

is_sorted : is_sorted.o
	$(CC) -o $@ $@.o

gen_rand_data : gen_rand_data.o
	$(CC) -o $@ $@.o

solution.zip : parsort.c Makefile README.txt
	rm -f $@
	zip -9r $@ parsort.c Makefile README.txt

clean :
	rm -f *.o $(EXES)
