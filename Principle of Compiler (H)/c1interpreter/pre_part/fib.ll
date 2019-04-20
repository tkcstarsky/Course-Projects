define i32 @fib(i32 %n) {
entry:  ;if(n=0)?
  %res = alloca i32, align 4  ;allocate space for return number(ret)
  %tmp1 = icmp eq i32 %n, 0
  br i1 %tmp1, label %n0, label %n1br

n0: ;n=0                                
  store i32 0, i32* %res, align 4
  br label %result

n1br: ;if(n=1)?                                 
  %tmp2 = icmp eq i32 %n, 1
  br i1 %tmp2, label %n1, label %n2

n1: ;n=1                              
  store i32 1, i32* %res, align 4
  br label %result

n2: ;n>1                              
  %nn1 = sub nsw i32 %n, 1              ;nn1=n-1;
  %back1 = call i32 @fib(i32 %nn1)      ;back1=fib(nn1);
  %nn2 = sub nsw i32 %n, 2              ;nn2=n-2;
  %back2 = call i32 @fib(i32 %nn2)      ;back2=fib(nn2);
  %n2res = add nsw i32 %back1, %back2   ;n2res=back1+back2;
  store i32 %n2res, i32* %res, align 4 
  br label %result

result: ;return ret                                 
  %r = load i32, i32* %res, align 4
  ret i32 %r                            ;return res;
}

define i32 @main(){
entry:  ;init x,i
  %x = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 0, i32* %x, align 4
  store i32 1, i32* %i, align 4
  br label %loop

loop:  ;for loop (i<8)                            
  %ic = load i32, i32* %i, align 4  
  %count = icmp slt i32 %ic, 8              ;(count<8)?
  br i1 %count, label %fibadd, label %quit

fibadd: ;loop contect                          
  %icall = load i32, i32* %i, align 4   
  %fibres = call i32 @fib(i32 %icall)       ;fibres=fib(i);
  %xcur = load i32, i32* %x, align 4
  %xres = add nsw i32 %fibres, %xcur
  store i32 %xres, i32* %x, align 4         ;x+=fibres;
  br label %countadd

countadd: ;i++                              
  %ip = load i32, i32* %i, align 4
  %ipp = add nsw i32 %ip, 1
  store i32 %ipp, i32* %i, align 4
  br label %loop

quit:  ;return ret                                
  %res = load i32, i32* %x, align 4
  ret i32 %res
}
