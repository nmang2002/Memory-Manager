# Makefile for mem memory system
# Navya Mangipudi & Emma Tran 
# muh, CSE 374, sp21
# 06/1/23

CC = gcc
CARGS = -Wall -std=c11

all: bench

# basic build
bench: bench.o getmem.o freemem.o mem_utils.o
	$(CC) $(CARGS) -o bench $^

# object files
bench.o: bench.c mem.h
	$(CC) $(CARGS) -g -c bench.c

getmem.o: getmem.c mem_impl.h mem.h
	$(CC) $(CARGS) -g -c getmem.c

freemem.o: freemem.c mem_impl.h mem.h
	$(CC) $(CARGS) -g -c freemem.c

mem_utils.o: mem_utils.c mem_impl.h mem.h
	$(CC) $(CARGS) -g -c mem_utils.c 


## make debug version
debug: CARGS += -g -D DEBUG
debug: bench

noassert: CARGS += -D NDEBUG
noassert: bench


## Utility targetscd
test: debug
	./bench
	./bench 5
	./bench 50000 10
	./bench 2 10 100
	./bench 18 5 100 16
	./bench 3 100 100 500 2000 1
	./bench 4 50 50 500 2000 1
	./bench 1 50 50 500 2000 1


clean:
	rm -f *.o bench *~ 
