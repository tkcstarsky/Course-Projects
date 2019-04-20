CC = g++
LLVMCONFIG = `llvm-config --cxxflags --ldflags --system-libs --libs`
fib_gen: fib_gen.cpp
	$(CC) fib_gen.cpp $(LLVMCONFIG) -o fib_gen
	./display.sh
clean:
	rm fib_gen fib_gen.ll
