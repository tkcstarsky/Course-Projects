#include<iostream>
#include<string>

#define LE 1
#define NE 2
#define LT 3
#define EQ 4
#define GE 5
#define GT 6

class RELOP{
    public:
    char relop0,relop1;
    int attribute;
    int start,end;
};
class trans_diag{
    private:
    int ret=0;
    char retword;

    public:
    int complete=0;
    RELOP* getrelop(int n);
    char nextchar();
    void printrelop(RELOP *prelop);
    void fail();
    void retract(char retw);
};