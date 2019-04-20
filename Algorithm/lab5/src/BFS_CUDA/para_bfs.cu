#include<stdlib.h> 
#include<stdio.h> 
#include<iostream>
#include<string> 
#include<math.h>
#include<fstream>
#include<sstream>
#include<cuda.h>
#include<time.h>

#define SIZE 82000

using namespace std;

size_t threadsPerBlock;
size_t numberOfBlocks;

enum color {
    white,
    black,
    grew
};

typedef struct edge_node
{
    int vex;
    edge_node *next;
} edge_node;

typedef struct vex_node
{
    int vex_num;
    color col;
    edge_node *edges;
} vex_node; 


int graph_size;

__global__ void search_kernel(int N,vex_node *g,bool &findx,bool &findy,int &tmpx,int &tmpy)
{
    int index= threadIdx.x + blockIdx.x * blockDim.x;
    int gridStride = gridDim.x * blockDim.x;
    for (int i = index; i < N; i += gridStride)
    {
        if(g[i].vex_num==y)
        {
            findy=true;
            tmpy=i;
        }
        if(g[i].vex_num==x)
        {
            findx=true;
            tmpx=i;
        }
        //if(findx&&findy) break;
    }
}
void creategraph(char *filename,vex_node *g)
{
    int i;
    int x,y;
    int sta=0;
    int tmpx,tmpy;
    bool findx,findy;
    ifstream in(filename);
    
    while(!in.eof())
    {
        // new input
        in>>x>>y;
        // map old point
        findx=false;
        findy=false;

        search_kernel<<<numberOfBlocks, threadsPerBlock>>>(sta,g,findx,findy,tmpx,tmpy);
        /*for(i=0;i<sta;i++)
        {
            if(g[i].vex_num==y)
            {
                findy=true;
                tmpy=i;
            }
            if(g[i].vex_num==x)
            {
                findx=true;
                tmpx=i;
            }
            if(findx&&findy) break;
        }*/ 

        //add new node
        if(!findx) 
        {
            tmpx=sta;
            g[tmpx].vex_num=x;
            g[tmpx].col=white;
            g[tmpx].edges=NULL;
            sta++;
        } 
        if(!findy)
        {
            tmpy=sta;
            g[tmpy].vex_num=y;
            g[tmpy].col=white;
            g[tmpy].edges=NULL;
            sta++;
        }

        // update node edges table 
        edge_node *p=g[tmpx].edges;
        edge_node *q;
        cudaMallocManaged(&q, sizeof(edge_node));
        q->vex=tmpy;
        q->next=NULL;
        if(p==NULL) g[tmpx].edges=q;
        else
        {
            while(p->next!=NULL)
                p=p->next;
            p->next=q;
        }
        
        // cheak map
        if(y!=g[tmpy].vex_num) 
            printf("Error:map wrong!\n");

        //printf("current vex number: %d\n",sta);
    }
    graph_size=sta;
}

void printgraph(vex_node *g)
{
    int i;
    int total=0,max=0;
    for(i=0;i<graph_size;i++)
    {
        //printf("No.%d: num=%d,edges:",i,g[i].vex_num);
        edge_node *p=g[i].edges;
        total=0;
        while(p!=NULL)
        {
            //printf("%d ",g[p->vex].vex_num);
            total++;
            p=p->next;
        }
        if(max<total) max=total;
        //printf("\n");
    }
    printf("max deepth:%d",max);
}

void bfs(int src)
{

}

int main(int argc,char *argv[])
{
    cudaGetDevice(&deviceId);
    cudaDeviceGetAttribute(&numberOfSMs, cudaDevAttrMultiProcessorCount, deviceId);
    threadsPerBlock = 256;
    numberOfBlocks = 32 * numberOfSMs;

    clock_t begin,end;
    double time;

    vex_node *g;
    size_t size = SIZE * sizeof(vex_node);
    cudaMallocManaged(&g, size);

    begin=clock();
    creategraph(argv[1],g);
    end=clock();
    time=(double)(end-begin)/CLOCKS_PER_SEC;
    printf("\nread time:%.8fs",time);

    printgraph(g);
}

