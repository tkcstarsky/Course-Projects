#include <iostream>
#include "Pointpair.h"
#include <stdlib.h>
#include <fstream>
#include <sstream>
#define numsize 5000
#define rand1 2334
#define rand2 8932
using namespace std;

int main(int argc,char *argv[])
{
    int maxnumber=10000;
    int i,m;
    double x,y;
    char k;
    Pointpair PP(maxnumber);
    srand((unsigned)time(NULL));
    ifstream in(argv[1]);
    //printf("%d",in.eof());
    while(!in.eof())
    {
       
        in>>x>>k>>y>>k;
        //printf("x=%lf,y=%lf\n",x,y);
        PP.Addpoint(x,y);
    }
    /*cout<<"Please input the points' total number:"<<endl;
    cin>>maxnumber;
    cout<<"Choose the mode:"<<endl<<"1.random number"<<endl<<"2.input by yourself"<<endl;
    cin>>m;
   
    if(m==1)
    {
        for(i=0;i<maxnumber;i++)
        {
            x=rand()%numsize+(double)(rand())/rand1;
            y=rand()%numsize+(double)(rand())/rand2;
            PP.Addpoint(x,y);
        }
    }
    else
    {
        cout<<"Please input the points' position:"<<endl;
        for(i=0;i<maxnumber;i++)
        {
            cin>>x>>y;
            PP.Addpoint(x,y);
        }
    }*/
    PP.pointsort(0,PP.pointnumber-1);
    //cout<<"The closet distance is "<<PP.mindis()<<endl;
    cout<<"The closet distance is "<<PP.devide(0,PP.pointnumber)<<endl;
    cout<<"The point position:"<<endl;
    PP.printpos();
}