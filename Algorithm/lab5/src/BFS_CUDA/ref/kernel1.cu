#ifndef _KERNEL1_H_
#define _KERNEL1_H_

__global__ void Kernel1(Node* d_node,int* d_edges,bool* d_frontier, bool* d_visited,int* d_cost,bool* d_stop)
{
	int tid=threadIdx.x; //我的图4个结点，对应一个block里的4个线程。
	if(d_frontier[tid]==true && d_visited[tid]==false)
	{
		d_frontier[tid]=false;
		for(int i=d_node[tid].start;i<d_node[tid].start+d_node[tid].num;i++)
		{
			int id=d_edges[i];
			if(d_frontier[id]==false && d_visited[id]==false)
			{
				d_cost[id]=d_cost[tid]+1;
				d_frontier[id]=true;
				*d_stop=true;

			}
		}
		d_visited[tid]=true;
		
	}

}

#endif