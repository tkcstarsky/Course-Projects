#include<iostream>
#include <fstream>
#include <sstream>
using namespace std;

int hashkey[1000];

// 线性探测法实现hash表
void hash1(int *hash,int n,int key)
{
    
    int i,val,tmp;
    int *stime=new int[key];
    int *failtime=new int[key];
    hash=new int[key];

    // init hash
    for(i=0;i<key;i++)
    {
        hash[i]=-1;
        stime[i]=0;
    }

    // create hash
    int suctime;
    for(i=0;i<n;i++)
    {
        val=hashkey[i]%key;
        suctime=1;
        if(hash[val]==-1)
        {
            hash[val]=hashkey[i];
            stime[val]=suctime;
        }
        else
        {
            tmp=val;
            val++;
            while(val!=tmp)
            {
                if(val>=key) val=val-key;
                suctime++;
                if(hash[val]==-1)
                {
                    hash[val]=hashkey[i];
                    stime[val]=suctime;
                    break;
                }
                val++;
            }
            if(val==tmp)
            {
                printf("The hash is full!\n");
                printf("Insert %d failed and quited.\n",hashkey[i]);
                exit(0);
            }
            //printf("Insert %d success!\n",hashkey[i]);
        }
    }
    
    // fail search time
    for(i=0;i<key;i++)
    {
        failtime[i]=1;
        if(hash[i]!=-1) 
        {
            tmp=i;
            while(hash[tmp]!=-1)
            {
                failtime[i]++;
                tmp++;
                if(tmp>=key) 
                    tmp=tmp-key;
                if(tmp==i-1) break;
            }
        }
    }

    // output hash
    float suc_total=0,fail_total=0;
    printf("哈希表的地址：\t");
    for(i=0;i<key;i++)
    {
        printf("%d\t",i);
    }
    printf("\n表中的关键字：\t");
    for(i=0;i<key;i++)
    {
        if(hash[i]!=-1)
            printf("%d\t",hash[i]);
        else printf("-\t");
    }
    printf("\n成功查找次数：\t");
    for(i=0;i<key;i++)
    {
        suc_total+=stime[i];
        printf("%d\t",stime[i]);
    }
    printf("\n失败查找次数：\t");
    for(i=0;i<key;i++)
    {
        fail_total+=failtime[i];
        printf("%d\t",failtime[i]);
    }
    printf("\n查找成功的平均查找长度：%.2f\n",suc_total/n);
    printf("查找失败的平均查找长度：%.2f\n",fail_total/key);
}

// 拉链法节点定义
typedef struct hashnode{
    int num;            // key
    int seatime;        // success search time
    hashnode *next;     // next pointer
} hashnode;

// 拉链法实现hash表
void hash2(hashnode *hash,int n,int key)
{
    int i,val,tmp;
    int *failtime=new int[key];
    hash=new hashnode[key];

    // init hash
    for(i=0;i<key;i++)
    {
        hash[i].num=-1;
        hash[i].seatime=0;
        hash[i].next=NULL;
    }

    // create hash
    int suctime;
    for(i=0;i<n;i++)
    {
        val=hashkey[i]%key;
        suctime=1;
        if(hash[val].num==-1)
        {
            hash[val].num=hashkey[i];
            hash[val].seatime=suctime;
        }
        else
        {
            // malloc new node
            hashnode *p=new hashnode;
            p->num=hashkey[i];
            p->next=NULL;

            // set the node to the tail
            hashnode *q=(hash+val);
            suctime++;
            while(q->next!=NULL)
            {
                suctime++;
                q=q->next;
            }
            q->next=p;
            p->seatime=suctime;
            
            //printf("Insert %d success!Deep: %d!\n",hashkey[i],suctime);
        }
    }
    
    // fail search time
    for(i=0;i<key;i++)
    {
        hashnode *q=(hash+i);
        while(q->next!=NULL)
        {
            q=q->next;
        }
        failtime[i]=q->seatime+1;
    }

    // output hash
    float suc_total=0,fail_total=0;
    printf("Address： Key：\n");
    for(i=0;i<key;i++)
    {
        printf("%d\t",i);
        hashnode *q=(hash+i);
        while(q!=NULL)
        {
            if(q->num!=-1)
                printf("%d\t",q->num);
            else printf("-\t");
            q=q->next;
        }
        printf("\n");
    }

    printf("Address： Successtime：\n");
    for(i=0;i<key;i++)
    {
        printf("%d\t",i);
        hashnode *q=(hash+i);
        while(q!=NULL)
        {
            printf("%d\t",q->seatime);
            suc_total+=q->seatime;
            q=q->next;
        }
        printf("\n");
    }

    printf("Address：\t");
    for(i=0;i<key;i++)
    {
        printf("%d\t",i);
    }
    printf("\nFailtime：\t");
    for(i=0;i<key;i++)
    {
        fail_total+=failtime[i];
        printf("%d\t",failtime[i]);
    }

    printf("\nSearching successfully average length：%.2f\n",suc_total/n);
    printf("Searching fail average length：%.2f\n",fail_total/key);
}


int main(int argc,char *argv[])
{
    int n,i,key;
    int *hash;
    hashnode *hashla;
    ifstream in(argv[1]);

    in>>n;
    for(i=0;i<n;i++)
    {
        in>>hashkey[i];
    }
    in>>key;
    hash1(hash,n,key);
    hash2(hashla,n,key);

}