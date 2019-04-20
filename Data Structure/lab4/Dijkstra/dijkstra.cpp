#include"dijkstra.h"
using namespace std;
bool dijkstra(const char *ifname)
{
        int i,j,n,k,start,end,tmp;
        long min;
        MGraph map;
        int P[MAX_VERTEX_NUM][MAX_VERTEX_NUM];  // Path Martix
        int D[MAX_VERTEX_NUM];                  // Short Path Table
        bool final[MAX_VERTEX_NUM];             // Point Color Table

        //读入节点信息
        ifstream in(ifname);
        in>>map.vexnum>>start>>end;
        while(!in.eof())
        {
            for(i=0;i<=map.vexnum-1;i++)
                for(j=0;j<=map.vexnum-1;j++)
                    in>>map.arc[i][j];
        }

        in.close();

        //完善矩阵信息
        for(i=1;i<map.vexnum;i++)
            for(j=0;j<i;j++)
               map.arc[i][j]=map.arc[j][i];
        for(i=0;i<map.vexnum;i++)
            for(j=0;j<map.vexnum;j++)
                if(map.arc[i][j]<=0)
                   map.arc[i][j]=INFINITY;

        //初始化
        for(i=0;i<map.vexnum;i++)
        {   
            final[i]=false;
            D[i]=map.arc[start][i];
            for(j=0;j<map.vexnum;j++)
               P[i][j]=-1;
            P[i][start]=0;
        }

        D[start]=0;
        final[start]=true;

        //dijkstra loop
        for(i=1;i<map.vexnum;i++)           // n-1论算法执行
        {
            min=INFINITY;
            for(j=0;j<map.vexnum;j++)
                if(!final[j])
                    if(D[j]<min)
                    {
                        n=j;
                        min=D[j];
                    }
            final[n]=true;

            //更改路径信息
            int max=0;
            for(k=0;k<map.vexnum;k++)
            {
                if(P[n][k]>=max)
                    {
                        tmp=k;
                        max=P[n][k];
                    }
            }

            if(tmp!=n)
                P[n][n]=max+1;


            for(j=0;j<map.vexnum;j++)
            {
                if(!final[j]&&(min+map.arc[n][j]<D[j]))
                    {
                       D[j]=min+map.arc[n][j];
                       for(k=0;k<map.vexnum;k++)
                           if(k!=j)
                               P[j][k]=P[n][k];

                       int max;
                       max=0;
                       for(k=0;k<map.vexnum;k++)
                       {
                           if(P[j][k]>=max&&(P[j][k]!=map.vexnum-1))
                               max=P[j][k];
                       }
                       P[j][j]=max+1;
                   }
            }


        }

    // 输出答案，图形化界面中无需
    /*printf("Min=%d\nPath:",D[end]);
    for(i=0;i<map.vexnum;i++)
        for(j=0;j<map.vexnum;j++)
            if(P[end][j]==i)
                printf("%d ",j);*/
    
    // 输出dot格式的图像描述文件
    ofstream out;
    out.open("/Users/tkc/Desktop/test/dijkstra.dot");
    out<<"graph first2"<<endl;
    out<<"{"<<endl;
    out<<"node [shape = circle];"<<endl;
    out<<"label=\"MIN:"<<D[end]<<"\t";
    out<<"PATH:";
    for(i=0;i<map.vexnum;i++)
        for(j=0;j<map.vexnum;j++)
            if(P[end][j]==i)
                out<<j<<"\t";
    out<<"\""<<";";
    for(i=0;i<map.vexnum;i++)
        {
            if(P[end][i]>=0)
                out<<i<<"[color=red]"<<endl;
            else
                out<<i<<endl;
        }
    for(i=0;i<map.vexnum-1;i++)
        for(j=i+1;j<map.vexnum;j++)
            if(map.arc[i][j]<INFINITY)
                out<<i<<"--"<<j<<"[label="<<map.arc[i][j]<<"]"<<";"<<endl;
    out<<"}"<<endl;

    out.close();               
    
    system("./a.sh");
    system("pwd");

    return true;
}
