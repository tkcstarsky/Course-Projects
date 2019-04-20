#include"INTtree.h"

int main()
{
    INTtree *intree; 
    int k=1;
    int mode,low,high;
    intree=new INTtree();
    printf("Welcome to build a INTtree!\n");
    printf("To build a RBtree,please input the root interval:\n");
    scanf("%d%d",&low,&high);
    intree->createtree(low,high);
    while(k)
    { 
        printf("Choose the operation for INTtree:\n");
        printf("No.1:Insert a node.\n");
        printf("No.2:Delete a node.\n");
        printf("No.3:Search an interval in INTtree.\n");
        printf("No.4:Draw the INTtree.\n");
        scanf("%d",&mode);
        switch(mode)
        {
            case 1:{
                printf("Input the insert number:\n");
                scanf("%d%d",&low,&high);
                if(!intree->INTinsert(low,high))
                    printf("Error:This intercal has the same begin with else!\n");
                break;
            }
            case 2:{
                printf("Input the delete number:\n");
                scanf("%d%d",&low,&high);
                if(!intree->INTDelete(low,high))
                    printf("Error:This interval does not exsit!\n");
                break;
            }
            case 3:{
                printf("Input the interval:\n");
                scanf("%d%d",&low,&high);
                tree *x;
                if((x=intree->INTSearch(low,high))!=NULL) 
                    printf("The cover interval is [%d,%d].\n",x->inter->low,x->inter->high);
                else printf("Can't find it!\n");
                break;
            }
            case 4:{
                intree->printtree();
                exit(0);
            }
            default:{
                break;
            }
        }

    }
}