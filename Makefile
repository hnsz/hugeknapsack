CC=gcc-4.9
FLAGS=-Wall -pedantic --std=c99
CFLAGS=$(FLAGS) -c
OBJS=knapsack.o

main: ks.encrypt ks.decrypt

ks.encrypt: encrypt.c $(OBJS) -lssl
	$(CC) $(FLAGS) encrypt.c $(OBJS) -o ks.encrypt -lssl -lcrypto

ks.decrypt: decrypt.c $(OBJS) -lssl
	$(CC) $(FLAGS) decrypt.c $(OBJS) -o ks.decrypt -lssl -lcrypto


knapsack.o: knapsack.c knapsack.h 
	$(CC) $(CFLAGS) knapsack.c


clean:
	rm *.o
