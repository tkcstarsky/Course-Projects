./fib_gen > fib_gen.ll
cat fib_gen.ll
echo The return = 
lli fib_gen.ll
echo $?
