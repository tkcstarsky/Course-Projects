#include"relop.h"
int main()
{
    trans_diag trans;
    RELOP *relops;
    int n=0;
    printf("Test begin:\n");
    printf("Please input the sentence:\n");
    while(!trans.complete){
        relops=trans.getrelop(n);
        n=relops->end;
        trans.printrelop(relops);
    }
}