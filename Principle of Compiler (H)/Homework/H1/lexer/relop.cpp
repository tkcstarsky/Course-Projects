#include"relop.h"
#include <stdlib.h>
#include <fstream>
#include <sstream>
using namespace std;
ifstream in("test.txt");

RELOP* trans_diag::getrelop(int n)
{
    int state=0;
    int i=n;
    char c;
    RELOP *token=new(RELOP);
    while(1)
    {
        switch(state){
            case 0:c=nextchar();
            i++;
            if (c == '<') {
                token->relop0=c;
                state = 1; 
            }
            else if (c == '=') {
                token->relop0=c;
                state = 5; 
            }
            else if (c == '>') {
                token->relop0=c;
                state = 6;
            }
            else fail();
            break;

            case 1:c=nextchar();
            i++;
            if (c == '=') {
                token->relop1=c;
                state = 2; 
            }
            else if (c == '>') {
                token->relop1=c;
                state = 3; 
            }
            else state = 4;
            break;

            case 2:token->attribute=LE;
            token->start=i-1;
            token->end=i;
            return token;

            case 3:token->attribute=NE;
            token->start=i-1;
            token->end=i;
            return token;

            case 4:token->attribute=LT;
            i--;
            retract(c);
            token->start=i;
            token->end=i;
            return token;

            case 5:token->attribute=EQ;
            token->start=i;
            token->end=i;
            return token;

            case 6:c=nextchar();
            i++;
            if (c == '=') {
                token->relop1=c;
                state = 7; 
            }
            else state = 8;
            break;

            case 7:token->attribute=GE;
            token->start=i-1;
            token->end=i;
            return token;

            case 8:token->attribute=GT;
            i--;
            retract(c);
            token->start=i;
            token->end=i;
            return token;
        }

    }
}

char trans_diag::nextchar()
{
    char c;
    if(ret) {
        ret=0;
        return retword;
    }
    else {
        in >> c;
        return c;
    }
}

void trans_diag::retract(char retw)
{
    ret=1;
    retword=retw;
}

void trans_diag::fail()
{
    if(in.eof()) {
        complete=1;
        exit(0);
        }
}

void trans_diag::printrelop(RELOP *prelop)
{
    printf("relop:%c",prelop->relop0);
    if((prelop->relop1=='>')||(prelop->relop1=='='))
    printf("%c",prelop->relop1);
    printf(" position:from %d to %d\n",prelop->start,prelop->end);
}