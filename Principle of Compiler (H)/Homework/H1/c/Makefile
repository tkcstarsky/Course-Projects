all: adder.i adder32.i adder64.i adder32.s adder64.s adder.o adder
	rm -f adder32.i adder64.i adder.s
adder.i: adder.c
	gcc -E adder.c -o adder.i
adder32.i: adder.c
	gcc -E adder.c -o adder32.i
adder64.i: adder.c
	gcc -E adder.c -o adder64.i
adder32.s: adder.i
	gcc -S -m32 adder32.i
adder64.s: adder.i
	gcc -S -m64 adder64.i
adder.o: adder.c
	gcc -c adder.c
adder: adder.o
	gcc adder.o -o adder
clean:
	rm -f adder.i adder.s adder.o adder adder32.s adder64.s