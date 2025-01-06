CFLAGS=-Wall -Wswitch-enum -Wextra -std=c11 -pedantic
LIBS= 

.PHONY: all

all: ebasm bmi

ebasm: ebasm.c bm.c
	$(CC) $(CFLAGS) -o ebasm ebasm.c $(LIBS)

bmi: bmi.c bm.c
	$(CC) $(CFLAGS) -o bmi bmi.c $(LIBS)

.PHONY: examples
examples: ./examples/fib.bm ./examples/sum.bm

./examples/fib.bm: ./examples/fib.ebasm
	./ebasm ./examples/fib.ebasm ./examples/fib.bm

./examples/sum.bm: ./examples/sum.ebasm
	./ebasm ./examples/sum.ebasm ./examples/sum.bm

