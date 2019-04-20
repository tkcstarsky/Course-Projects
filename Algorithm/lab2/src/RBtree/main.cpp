#include"RBtree.h"

int main()
{
    RBtree *rb; 
    int k=1;
    int mode,buf;
    rb=new RBtree();
    printf("Welcome to build a RBtree!\n");
    printf("To build a RBtree,please input the root number:\n");
    scanf("%d",&buf);
    rb->createtree(buf);
    while(k)
    {
        printf("Choose the operation for RBtree:\n");
        printf("No.1:Insert a node.\n");
        printf("No.2:Delete a node.\n");
        printf("No.3:Draw the RBtree.\n");
        scanf("%d",&mode);
        switch(mode)
        {
            case 1:{
                printf("Input the insert number:\n");
                scanf("%d",&buf);
                if(!rb->RBinsert(buf))
                    printf("Error:This number already exsit!\n");
                break;
            }
            case 2:{
                printf("Input the delete number:\n");
                scanf("%d",&buf);
                if(!rb->RBDelete(buf))
                    printf("Error:This number does not exsit!\n");
                break;
            }
            case 3:{
                rb->printtree();
                exit(0);
            }
            default:{
                break;
            }
        }

    }
}