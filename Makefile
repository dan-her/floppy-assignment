output: message.o
	g++ message.o -o output -lm

message.o: message.c
	g++ -c message.c

clean:
	rm *.o output