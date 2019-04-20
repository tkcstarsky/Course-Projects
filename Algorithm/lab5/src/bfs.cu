#include<stdlib.h> 
#include<stdio.h> 
#include<string.h> 
#include<math.h> 
#include<cutil.h> 
#include<cuda.h>
#include<time.h>

struct node
{
    int start;
    int num_edges;
}; 

__global__ 
void Kernel(node *g_node,int *g_edges,bool *g_frontier,bool *g_visited,int *g_cost,bool *g_over,int num_nodes)
{
    int tid=blockIdx.x*128+threadIdx.x;
    if(tid<num_nodes&&g_frontier[tid])
    {
        g_frontier[tid]=false;
        g_visited[tid]=true;
        for(int i=g_node[tid].start;i<g_node[tid].start+g_node[tid].num_edges;i++)
        {
            int id=g_edges[i];
            if(!g_visited[id])
            {
                g_cost[id]=g_cost[tid]+1;
                g_frontier[id]=true;
                *g_over=true;
            }
        }
    }
}
void bfs_graph(int num_vertex,int num_edges_list)
{
    int i,k,vertex,edges,id,cost,source;
    scanf("%d",&num_vertex);
    node *h_node=(node*)malloc(sizeof(node)*num_vertex);
    bool *h_frontier=(bool*)malloc(sizeof(bool)*num_vertex);
    bool *h_visited=(bool*)malloc(sizeof(bool)*num_vertex);
    int *h_cost = (int*)malloc( sizeof(int)*num_vertex);
    for(i=0;i<num_vertex;i++)
    {
        scanf("%d %d",&vertex,&edges);
        h_node[i].start=vertex;
        h_node[i].num_edges=edges;
        h_frontier[i]=false;
        h_visited[i]=false;
        h_cost[i]=-1;
    }
    scanf("%d",&source);
    h_frontier[source]=true;
    h_cost[source]=0;
    scanf("%d",&num_edges_list);
    int* h_edges=(int*)malloc(sizeof(int)*num_edges_list);
    for(i=0;i<num_edges_list;i++)
    {
        scanf("%d %d",&id,&cost);
        h_edges[i]=id;
    }
    clock_t begin,end;
    double time;
    begin=clock();
    node *d_node;
    CUDA_SAFE_CALL(cudaMalloc((void**)&d_node,sizeof(node)*num_vertex));
    CUDA_SAFE_CALL(cudaMemcpy(d_node,h_node,sizeof(node)*num_vertex,cudaMemcpyHostToDevice));
    int *d_edges,*d_cost;
    CUDA_SAFE_CALL(cudaMalloc((void**)&d_edges,sizeof(int)*num_edges_list));
    CUDA_SAFE_CALL(cudaMemcpy(d_edges,h_edges,sizeof(int)*num_edges_list,cudaMemcpyHostToDevice));
    CUDA_SAFE_CALL(cudaMalloc((void**)&d_cost,sizeof(int)*num_vertex));
    CUDA_SAFE_CALL(cudaMemcpy(d_cost,h_cost,sizeof(int)*num_vertex,cudaMemcpyHostToDevice));
    bool *d_frontier,*d_visited;
    CUDA_SAFE_CALL(cudaMalloc((void**)&d_frontier,sizeof(bool)*num_vertex));
    CUDA_SAFE_CALL(cudaMemcpy(d_frontier,h_frontier,sizeof(bool)*num_vertex,cudaMemcpyHostToDevice));
    CUDA_SAFE_CALL(cudaMalloc((void**)&d_visited,sizeof(bool)*num_vertex));
    CUDA_SAFE_CALL(cudaMemcpy(d_visited,h_visited,sizeof(bool)*num_vertex,cudaMemcpyHostToDevice));
    bool *d_over;
    CUDA_SAFE_CALL(cudaMalloc((void**)&d_over,sizeof(bool)));
    dim3 grid(128,1,1);
    dim3 threads(128,1,1);
    bool stop;
    do
    {
        stop=false;
        cudaMemcpy(d_over,&stop,sizeof(bool),cudaMemcpyHostToDevice); 
        Kernel<<<grid,threads,0>>>(d_node,d_edges,d_frontier,d_visited,d_cost,d_over,num_vertex);
        CUDA_SAFE_CALL(cudaThreadSynchronize());
        CUT_CHECK_ERROR("Kernel execution failed");
        CUDA_SAFE_CALL(cudaMemcpy(&stop,d_over,sizeof(bool),cudaMemcpyDeviceToHost));
        k++;
    } while(stop);
    CUDA_SAFE_CALL(cudaMemcpy(h_cost,d_cost,sizeof(int)*num_vertex,cudaMemcpyDeviceToHost));
    for(i=0;i<num_vertex;i++)
        printf("%d cost:%d\n",i,h_cost[i]);
    end=clock();
    time=(double)(end-begin)/CLOCKS_PER_SEC;
    printf("\n%f",time);
}

int main()
{
    int num_vertex=0;
    int num_edges_list=0;
    bfs_graph(num_vertex,num_edges_list);
    return 0;
}