#include<iostream>
//#include<vector>
using namespace std;
//vector<int*> polyvect;
//vector<int*>::iterator polyiter;

struct poly{
    int len;
    int *degree;
    struct poly *next;
};
struct poly *polylist=NULL;

// polynomial plus 
int *pluspoly(int *a,int la,int *b,int lb)
{
    int i,l;
    if(la>lb) l=la;
    else l=lb;
    int *c=new int[l];
    for(i=0;i<=l;i++)
    {
        if(i>la) *(c+i)=*(b+i);
        if(i>lb) *(c+i)=*(a+i);
        if((i<=la)||(i<lb))
        *(c+i)=*(a+i)+*(b+i);
    }
    return c;
}

// polynomial subtraction
int *subpoly(int *a,int la,int *b,int lb)
{
    int i,l;
    if(la>lb) l=la;
    else l=lb;
    int *c=new int[l];
    for(i=0;i<=l;i++)
    {
        if(i>la) *(c+i)=-*(b+i);
        if(i>lb) *(c+i)=*(a+i);
        if((i<=la)||(i<lb))
        *(c+i)=*(a+i)-*(b+i);
    }
    return c;
}

// polynomial multiplication
int *mulpoly(int *a,int la,int *b,int lb)
{
    int i,j,l;
    l=la+lb;
    int *c=new int[l];
    for(i=0;i<=l;i++)
    {
        *(c+i)=0;
    }
    for(i=0;i<=la;i++)
        for(j=0;j<=lb;j++)
        {
            *(c+i+j)=*(c+i+j)+(*(a+i))*(*(b+j));
        }
    return c;
}

void caculist()
{
    printf("============================================================\n");
    printf("0.Display all polynomials.\n");
    printf("1.Input a new polynomial.\n");
    printf("2.Delete a polynomial.\n");
    printf("3.Plus two polynomials.\n");
    printf("4.Sub two polynomials.\n");
    printf("5.Mul two polynomials.\n");
    printf("Please input the number or any words else to quit:\n");
}

void display(int k)
{
    int x,y,i;
    struct poly *p,*q;
    int d,e,la,lb;
    int *a,*b;
    switch (k)
    {
        case 0:
            
            p=polylist;
            int j;
            i=1;
            while(p!=NULL)
            {
                int kp;
                kp=0;
                printf("No.%d:   ",i);
                i++;
                if(*(p->degree)!=0)
                {
                    printf("%d",*(p->degree));
                    kp=1;
                }
                for(j=1;j<=p->len;j++)
                {
                    if(*(p->degree+j)!=0)
                    {
                        if((j==1)&&(*(p->degree)!=0)&&(*(p->degree+j)>0)) printf("+");
                        if((j>1)&&(*(p->degree+j)>0)) printf("+");
                        printf("%dx^%d",*(p->degree+j),j);
                        kp=1;
                    }
                }
                if(kp==0) printf("0");
                printf("\n");
                p=p->next;
            }
            getchar();
            getchar();
            break;
            /*for(polyiter=polyvect.begin();polyiter!=polyvect.end();polyiter++)
            {
                int *a=*polyiter;

            }*/

        case 1:
            p=new struct poly;
            printf("Please input the polynomial's max degree:\n");
            scanf("%d",&x);
            p->len=x;
            printf("Plese input each degree(from x^0 to x^n):\n");
            a=new int[x+1];
            for(i=0;i<=x;i++)
            {
                scanf("%d",&y);
                *(a+i)=y;
            }
            p->degree=a;
            p->next=NULL;
            if(polylist==NULL) polylist=p;
            else
            {
                q=polylist;
                while(q->next!=NULL)
                    q=q->next;
                q->next=p;
            }
            printf("Create successfully!\n");
            getchar();
            getchar();
            break;

        case 2:
            p=polylist;
            printf("Please input the polynomial's number which need to be deleted:\n");
            scanf("%d",&y);
            if(y==1)
            {
                polylist=polylist->next;
                free(p);
            } 
            else
            {
                for(i=1;i<y;i++)
                {
                    q=p;
                    p=p->next;
                }
                q->next=p->next;
                free(p);
            }
            printf("Delete successfully!\n");
            getchar();
            getchar();
            break;

        case 3:
            p=polylist;
            a=NULL;
            b=NULL;
            printf("Please input two plus polynomial's number:\n");
            scanf("%d%d",&d,&e);
            i=1;
            while(p!=NULL)
            {
                if(i==d) 
                {
                    a=p->degree;
                    la=p->len;
                }
                if(i==e) 
                {
                    b=p->degree;
                    lb=p->len;
                }
                i++;
                q=p;
                p=p->next;
            }
            if((a==NULL)||(b==NULL))
            printf("The polynomial does not exist!\n");
            else
            {
                p=new struct poly;
                p->next=NULL;
                if(la>lb) p->len=la;
                else p->len=lb;
                p->degree=pluspoly(a,la,b,lb);
                q->next=p;
            }
            printf("Plus successfully!\n");
            getchar();
            getchar();
            break;
        
        case 4:
            p=polylist;
            a=NULL;
            b=NULL;
            printf("Please input two sub polynomial's number:\n");
            scanf("%d%d",&d,&e);
            i=1;
            while(p!=NULL)
            {
                if(i==d) 
                {
                    a=p->degree;
                    la=p->len;
                }
                if(i==e) 
                {
                    b=p->degree;
                    lb=p->len;
                }
                i++;
                q=p;
                p=p->next;
            }
            if((a==NULL)||(b==NULL))
            printf("The polynomial does not exist!\n");
            else
            {
                p=new struct poly;
                p->next=NULL;
                if(la>lb) p->len=la;
                else p->len=lb;
                p->degree=subpoly(a,la,b,lb);
                q->next=p;
            }
            printf("Sub successfully!\n");
            getchar();
            getchar();
            break;

        case 5:
            p=polylist;
            a=NULL;
            b=NULL;
            printf("Please input two mul polynomial's number:\n");
            scanf("%d%d",&d,&e);
            i=1;
            while(p!=NULL)
            {
                if(i==d) 
                {
                    a=p->degree;
                    la=p->len;
                }
                if(i==e) 
                {
                    b=p->degree;
                    lb=p->len;
                }
                i++;
                q=p;
                p=p->next;
            }
            if((a==NULL)||(b==NULL))
            printf("The polynomial does not exist!\n");
            else
            {
                p=new struct poly;
                p->next=NULL;
                p->len=la+lb;
                p->degree=mulpoly(a,la,b,lb);
                q->next=p;
            }
            printf("Plus successfully!\n");
            getchar();
            getchar();
            break;

        default:
            printf("Thank you for using!\n");
            break;
    }
}

int main()
{
    int k=0;
    printf("Welcome to use this polynomial caculation!\n");
    do
    {
        caculist();
        scanf("%d",&k);
        display(k);
    } while ((k==0)||(k==1)||(k==2)||(k==3)||(k==4)||(k==5));
}