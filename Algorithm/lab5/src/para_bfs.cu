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
int deviceId;

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
    edge_node *tail;
} vex_node; 


int graph_size;

__global__ void search_kernel(int N,vex_node *g,bool *findx,bool *findy,int *tmpx,int *tmpy,int x,int y)
{
    int index= threadIdx.x + blockIdx.x * blockDim.x;
    int gridStride = gridDim.x * blockDim.x;
    for (int i = index; i < N; i += gridStride)
    {
        if(g[i].vex_num==y)
        {
            *findy=true;
            *tmpy=i;
        }
        if(g[i].vex_num==x)
        {
            *findx=true;
            *tmpx=i;
        }
        //if(findx&&findy) break;
    }
}

void printgraph(vex_node *g)
{
    int i;
    int total=0,max=0;
    for(i=0;i<graph_size;i++)
    {
        printf("No.%d: num=%d,edges:",i,g[i].vex_num);
        edge_node *p=g[i].edges;
        total=0;
        while(p!=NULL)
        {
            printf("%d ",g[p->vex].vex_num);
            total++;
            p=p->next;
        }
        if(max<total) max=total;
        printf("\n");
    }
}

void creategraph(char *filename,vex_node *g)
{
    int i;
    int x=1,y;
    int sta;
    //int tmpx=0,tmpy=0;
    //bool findx,findy;

    //int *d_x,*d_y;
    int *d_tmpx,*d_tmpy;
    bool *d_findx,*d_findy;
    //cudaMallocManaged(&d_x,sizeof(int));
    //cudaMallocManaged(&d_y,sizeof(int));
    cudaMallocManaged(&d_tmpx,sizeof(int));
    cudaMallocManaged(&d_tmpy,sizeof(int));
    cudaMallocManaged(&d_findx,sizeof(bool));
    cudaMallocManaged(&d_findy,sizeof(bool));
    *d_tmpx=0;
    *d_tmpy=0;

    sta=0;
    edge_node *q;
    ifstream in(filename);
    //printf("here2\n");
    while(!in.eof())
    {
        // new input
        in>>x>>y;
        // map old point
        *d_findx=false;
        *d_findy=false;

        /*cudaMemcpy(d_x,&x,sizeof(int),cudaMemcpyHostToDevice);
        cudaMemcpy(d_y,&y,sizeof(int),cudaMemcpyHostToDevice);
        cudaMemcpy(d_findx,&findx,sizeof(bool),cudaMemcpyHostToDevice);
        cudaMemcpy(d_findy,&findy,sizeof(bool),cudaMemcpyHostToDevice);*/

        numberOfBlocks=sta/threadsPerBlock+1;
        search_kernel<<<numberOfBlocks, threadsPerBlock>>>(sta,g,d_findx,d_findy,d_tmpx,d_tmpy,x,y);
        cudaDeviceSynchronize();

        /*cudaMemcpy(&tmpx,d_tmpx,sizeof(int),cudaMemcpyDeviceToHost);
        cudaMemcpy(&tmpy,d_tmpy,sizeof(int),cudaMemcpyDeviceToHost);
        cudaMemcpy(&findx,d_findx,sizeof(bool),cudaMemcpyDeviceToHost);
        cudaMemcpy(&findy,d_findy,sizeof(bool),cudaMemcpyDeviceToHost);*/
        
        //add new node
        if(!*d_findx) 
        {
            *d_tmpx=sta;
            g[*d_tmpx].vex_num=x;
            g[*d_tmpx].col=white;
            g[*d_tmpx].edges=NULL;
            g[*d_tmpx].tail=NULL;
            sta++;
        } 
        //printf("here4\n");
        if(!*d_findy)
        {
            *d_tmpy=sta;
            g[*d_tmpy].vex_num=y;
            g[*d_tmpy].col=white;
            g[*d_tmpy].edges=NULL;
            g[*d_tmpx].tail=NULL;
            sta++;
        }
        
        // update node edges table 
        edge_node *p=g[*d_tmpx].tail;
        cudaMallocManaged(&q, sizeof(edge_node));
        q->vex=*d_tmpy;
        q->next=NULL;

        if(p==NULL) 
        {
            g[*d_tmpx].edges=q;
            g[*d_tmpx].tail=q;
        }
        else
        {
            p->next=q;
            g[*d_tmpx].tail=q;
        }
        
        // cheak map
        if(y!=g[*d_tmpy].vex_num) 
            printf("Error:map wrong!\n");
        //graph_size=*sta;
        //printgraph(g);
        printf("current vex number: %d\n",sta);
    }
    graph_size=sta;
}

__global__ void bfs_kernel(vex_node *g,int N,bool *change)
{
    int index= threadIdx.x + blockIdx.x * blockDim.x;
    int gridStride = gridDim.x * blockDim.x;
    for (int i = index; i < N; i += gridStride)
    {
        if(g[i].col==grew)
        {
            edge_node *p=g[i].edges;
            while(p!=NULL)
            {
                if(g[p->vex].col==white) {
                    *change=true;
                    //printf("get point\n");
                }
                g[p->vex].col=grew;
                p=p->next;
            }
            g[i].col==black;
        }
    }
}

void bfs(vex_node *g)
{
    int i;
    bool *change;
    int total=0;
    cudaMallocManaged(&change,sizeof(bool));
    for(i=0;i<graph_size;i++)
    {
        if(g[i].col==white)
        {
            g[i].col=grew;
            *change=true;
            while(*change)
            {
                *change=false;
                //if(!*change) printf("clear\n");
                total++;
                cudaDeviceSynchronize();
                bfs_kernel<<<numberOfBlocks, threadsPerBlock>>>(g,graph_size,change);
                cudaDeviceSynchronize();
                //if(*change) printf("next bfs\n");
            }
        }
    }
    printf("\ntotal bfs times:%d\n",total);
}

int main(int argc,char *argv[])
{
    
    int numberOfSMs;
    cudaGetDevice(&deviceId);
    cudaDeviceGetAttribute(&numberOfSMs, cudaDevAttrMultiProcessorCount, deviceId);
    threadsPerBlock = 256;
    numberOfBlocks = 32 * numberOfSMs;

    clock_t begin,end;
    double time1,time2;

    vex_node *g;
    size_t size = SIZE * sizeof(vex_node);
    cudaMallocManaged(&g, size);

    begin=clock();
    creategraph(argv[1],g);
    end=clock();
    time1=(double)(end-begin)/CLOCKS_PER_SEC;

    printgraph(g);

    begin=clock();
    bfs(g);
    end=clock();
    time2=(double)(end-begin)/CLOCKS_PER_SEC;
    printf("\nread time:%.8fs",time1);
    printf("\nbfs time:%.8fs\n",time2);
}

