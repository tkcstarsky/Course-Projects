#include <stdio.h>
int main(){
    long a[1][1];
    int i;
    long b[1][1];
    printf("a:%08lx\n", a);
    printf("b:%08lx\n", b);
    printf("i:%08lx\n", &i);
    return 0;
}
