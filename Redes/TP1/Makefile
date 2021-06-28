all:
	gcc -Wall -c location.c -lm
	gcc -Wall -c common.c -lm
	gcc -Wall client.c location.o common.o -o client -lm
	gcc -Wall server.c location.o common.o -o server -lm

clean:
	rm *.o client server