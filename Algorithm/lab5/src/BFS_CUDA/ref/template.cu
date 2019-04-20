/* 
Author: Juliet(ñÒÑÞÀû)
QQ: 773917068
Date: Mar 10
About: BFS_CUDA

*/
#include <stdio.h>
#include <cutil.h>


struct Node
{
	int start;
	int num;
};
#include <kernel1.cu>
void BFSGraph(int argc, char** argv);

int main(int argc, char** argv)
{
	BFSGraph(argc,argv);
	CUT_EXIT(argc,argv);
}

void BFSGraph(int argc, char** argv)
{

	//the graph description
	struct Node node[4];
	node[0].start=0;
	node[0].num=2;
	node[1].start=2;
	node[1].num=1;
	node[2].start=3;
	node[2].num=1;
	node[3].start=3; //attention node[i]'s judgement
	node[3].num=0;
	int edges[]={1,2,3,3};

	bool frontier[4]={false};
	bool visited[4]={false};
	int cost[4]={0};

	int source=0;
	frontier[source]=true;
	
	Node* d_node;
	CUDA_SAFE_CALL(cudaMalloc((void**)&d_node,sizeof(Node)*4));
	CUDA_SAFE_CALL(cudaMemcpy(d_node,node,sizeof(Node)*4,cudaMemcpyHostToDevice));

	int* d_edges;
	CUDA_SAFE_CALL(cudaMalloc((void**)&d_edges,sizeof(Node)*4));
	CUDA_SAFE_CALL(cudaMemcpy(d_edges,edges,sizeof(Node)*4,cudaMemcpyHostToDevice));

	bool* d_frontier;
	CUDA_SAFE_CALL(cudaMalloc((void**)&d_frontier,sizeof(bool)*4));
	CUDA_SAFE_CALL(cudaMemcpy(d_frontier,frontier,sizeof(bool)*4,cudaMemcpyHostToDevice));

	bool* d_visited;
	CUDA_SAFE_CALL(cudaMalloc((void**)&d_visited,sizeof(bool)*4));
	CUDA_SAFE_CALL(cudaMemcpy(d_visited,visited,sizeof(bool)*4,cudaMemcpyHostToDevice));

	int* d_cost;
	CUDA_SAFE_CALL(cudaMalloc((void**)&d_cost,sizeof(int)*4));
	CUDA_SAFE_CALL(cudaMemcpy(d_cost,cost,sizeof(int)*4,cudaMemcpyHostToDevice));

	dim3 grid(1,1,1);
	dim3 threads(4,1,1);

	bool stop;
	bool* d_stop;
	CUDA_SAFE_CALL(cudaMalloc((void**)&d_stop,sizeof(bool)));

	do{
		stop=false;
		CUDA_SAFE_CALL(cudaMemcpy(d_stop,&stop,sizeof(bool),cudaMemcpyHostToDevice));
		Kernel1<<<grid,threads,0>>>(d_node,d_edges,d_frontier,d_visited,d_cost,d_stop);
		CUT_CHECK_ERROR("kernel1 execution failed");


		CUDA_SAFE_CALL(cudaMemcpy(&stop,d_stop,sizeof(bool),cudaMemcpyDeviceToHost));
	}while(stop);

	CUDA_SAFE_CALL(cudaMemcpy(cost,d_cost,sizeof(int)*4,cudaMemcpyDeviceToHost));

	for(int i=0;i<4;i++)
		printf("%d	",cost[i]);

}