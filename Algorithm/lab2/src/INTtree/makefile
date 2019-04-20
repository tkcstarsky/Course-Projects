CC = g++
C11FLAG = -std=c++11

all: main a.out
	./main
	./run.sh
	rm main *.o 1.dot input.txt a.out

a.out:vis_tree.cpp
	$(CC) -O3 vis_tree.cpp $(C11FLAG)

main: main.o INTtree.o
	$(CC) -o main main.o INTtree.o
		
main.o: INTtree.h

INTtree.o: INTtree.h

.PHONY: clean

clean:
	rm main *.o 1.dot input.txt output.png a.out
