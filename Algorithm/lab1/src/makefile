CC = g++

main: main.o Pointpair.o
	$(CC) -o main main.o Pointpair.o

main.o: Pointpair.h

Pointpair.o: Pointpair.h

.PHONY: clean

clean:
	rm main *.o
