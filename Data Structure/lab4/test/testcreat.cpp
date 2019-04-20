#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<iomanip>
#include<fstream>
#include<string>
#define MAX 100

using namespace std;
int main(int argc,char *argv[])
{
    ofstream out;
    out.open(argv[1]);
    int n,i,j,weigh;
    int gra[MAX][MAX];
    
    scanf("%d%d%d",&n,&i,&j);
    out<<n<<endl<<i<<' '<<j<<endl;
    //printf("%d\n%d %d\n",n,i,j);

    for(i=0;i<n;i++)
        for(j=0;j<n;j++)
            gra[i][j]=-1;

    cin>>i>>j>>weigh;        
    while(i<n)
    {
        gra[i][j]=weigh;
        gra[j][i]=weigh;
        cin>>i>>j>>weigh;
    }
    
    for(i=0;i<n;i++)
        gra[i][i]=0;
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
            out<<gra[i][j]<<' ';
            //printf("%d ",gra[i][j]);
        //printf("\n");
        out<<endl;
    }
}