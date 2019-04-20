#include"elevator.h"
#include <math.h>
#include <stdlib.h>
#define max_text_people 1000

int T=0;
int Tmax;
int delay=0;
int n;
int rep=1;
int check_in=0,check_out=0,check_giveup=0;
person waitque[max_text_people];
elevater *ele=new elevater(1);
quene *qu=new quene();

void addperson()
{
    int i;
    for(i=0;i<n;i++)
        if(waitque[i].arr_time==T)
            qu->insert(&waitque[i]);
}

void onetp()
{
    addperson();
    //printf("delay=%d\n",delay);
    if((ele->sta==run)&&(delay>0)&&(*(qu->num+ele->cur_floor)>0)&&(ele->people<max_people))
    {
        delay=door_time-delay;
        ele->sta=in;
        //printf("here\n");
    }// interfere the situation: a person arrive while the door is closing.
    if(delay==0)
    {
        switch(ele->sta)
        {
            case run:
            {
                delay=ele->findnextfloor(qu->num);
                ele->sta=opd;
                if((delay==0)&&(*(qu->num+ele->cur_floor)==0))
                {
                    ele->sta=run;
                    if(rep)
                        printf("T=%d:The elevator is waiting for people at floor %d.\n",T,ele->cur_floor);
                    rep=0;
                }
                else {
                    rep=1;
                    printf("T=%d:The elevator is running to floor %d.\n",T,ele->cur_floor);
                }
                break;
            }
            case opd:
            {
                //printf("opd\n");
                printf("T=%d:The elevator is opening door at floor %d.\n",T,ele->cur_floor);
                delay=door_time;
                ele->sta=out;
                break;
            }
            case out:
            {
                //printf("out\n");
                printf("T=%d:People is getting out at floor %d.\n",T,ele->cur_floor);
                int k=ele->getout(T);
                delay=k*get_in_time;
                check_out+=k;
                ele->sta=in;
                break;
            }
            case in:
            {
                //printf("in\n");
                printf("T=%d:People is getting in at floor %d.\n",T,ele->cur_floor);
                int k=0;
                bool m=true;
                while((*(qu->num+ele->cur_floor)>0)&&(m))
                {
                    
                    if(ele->people<max_people)
                    {
                        person *p;
                        p=qu->quit(ele->cur_floor,T,&check_giveup);
                        if(p!=NULL)
                        {
                        ele->getin(p,T);
                        k++;
                        }
                        else m=false;
                    }
                    else m=false;
                }
                delay=k*get_in_time;
                check_in+=k;
                ele->sta=cld;
                break;
            }
            case cld:
            {
                //printf("cld\n");
                printf("T=%d:The elevator is closing door at floor %d.\n",T,ele->cur_floor);
                delay=door_time;
                ele->sta=run;
                break;
            }
        }
    }
    else delay--;
}

void randpeople()
{
    srand((unsigned)time(NULL));
    for(int i=0;i<n;i++)
    {
        waitque[i].next=NULL;
        waitque[i].num=i+1;
        waitque[i].arr_time=(rand()%Tmax)*tp;
        waitque[i].arr_floor=1+(rand()%(max_floor));
        do waitque[i].des_floor=1+(rand()%(max_floor));
        while(waitque[i].des_floor==waitque[i].arr_floor);
        waitque[i].tol_time=((rand()%(4*max_floor*elevater_speed))+toler_up_time)*tp;
        waitque[i].total_time=10000*tp;
        printf("No.%d arrive person:T=%d,arr_floor:%d,des_floor:%d.\n",waitque[i].num,waitque[i].arr_time,waitque[i].arr_floor,waitque[i].des_floor);
    }
}

int main()
{
    
    printf("Please input elevator running max time:\n");
    scanf("%d",&Tmax);
    Tmax=Tmax*tp;
    printf("Please input random people number:\n");
    scanf("%d",&n);
    randpeople();
    while(T<=Tmax)
    {
        onetp();
        T+=tp;
    }
    printf("===============================\n");
    printf("Report:Total people:%d\nGet in people:%d\nGive up people:%d\nWaiting people:%d\nGet out people:%d\nElevator people:%d\n",n,check_in,check_giveup,qu->waitsum(),check_out,ele->people);
}