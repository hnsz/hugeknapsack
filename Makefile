CC=gcc-4.9
FLAGS=-Wall -pedantic --std=c99
CFLAGS=$(FLAGS) -c
OBJS=knapsack.o

a.out: main.c $(OBJS) -lssl
	$(CC) $(FLAGS) main.c $(OBJS) -lssl -lcrypto

knapsack.o: knapsack.c knapsack.h 
	$(CC) $(CFLAGS) knapsack.c


clean:
	rm *.o
