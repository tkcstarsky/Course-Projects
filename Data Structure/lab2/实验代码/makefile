CC = g++

main: main.o elevator.o
	$(CC) -o main main.o elevator.o

main.o: elevator.h

elevator.o: elevator.h

.PHONY: clean

clean:
	rm main *.o
