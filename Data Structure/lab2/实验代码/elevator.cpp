#include"elevator.h"
elevater::elevater(int initfloor)
{
    int i;
    people=0;
    cur_floor=initfloor;
    for(i=1;i<=max_floor;i++)
    {
        floors[i]=NULL;
    }
    dir=up;
    sta=run;
}

int elevater::waittime(int locfloor,int direction)
{
    if(locfloor==cur_floor) return 0;
    if(cur_floor>locfloor)
    {
        
        if(dir==up) 
        {
            if(direction==dir) return toler_up_time;
            return (13-cur_floor+13-locfloor)*elevater_speed;
        }
        else return (cur_floor-locfloor)*elevater_speed;
    }
    else{
        if(direction==dir) return toler_up_time;
        if(dir==up) return (locfloor-cur_floor)*elevater_speed;
        else {
            if(direction==dir) return toler_up_time;
            return (cur_floor-1+locfloor-1)*elevater_speed;
        }
    }              
}

int elevater::findnextfloor(int *que)
{
    if(dir==up)
    {
        for(int i=cur_floor+1;i<=max_floor;i++)
        {
            if((*(que+i)>0)||(floors[i]!=NULL))
            {
                int sub=i-cur_floor;
                cur_floor=i;
                return sub*elevater_speed;
            }
        }
        dir=down;
        for(int i=cur_floor;i>=1;i--)
        {
            if((*(que+i)>0)||(floors[i]!=NULL))
            {
                int sub=cur_floor-i;
                cur_floor=i;
                return sub*elevater_speed;
            }
        }
        return 0;
    }
    else{
        for(int i=cur_floor-1;i>=1;i--)
        {
            if((*(que+i)>0)||(floors[i]!=NULL))
            {
                int sub=cur_floor-i;
                cur_floor=i;
                return sub*elevater_speed;
            }
        }
        dir=up;
        for(int i=cur_floor;i<=max_floor;i++)
        {
            if((*(que+i)>0)||(floors[i]!=NULL))
            {
                int sub=i-cur_floor;
                cur_floor=i;
                return sub*elevater_speed;
            }
        }
        return 0;
    }
}

bool elevater::getin(person *p,int T)
{
    if(people==max_people) return false;
    p->next=NULL;
    people++;
    person *fl=floors[p->des_floor];
    if(fl!=NULL)
    {
        while(fl->next!=NULL)
            fl=fl->next;
        fl->next=p;
        p->next=NULL;
    }
    else floors[p->des_floor]=p;
    printf("T=%d:No.%d person in,arr_floor:%d,des_floor:%d,use time:%d\n",T,p->num,p->arr_floor,p->des_floor,p->total_time);
    return true;
}

int elevater::getout(int T)
{
    person *fl=floors[cur_floor],*p;
    int k=0;
    while(fl!=NULL)
    {
        fl->total_time=T-fl->arr_time;
        printf("T=%d:No.%d person out,arr_floor:%d,des_floor:%d,use time:%d\n",T,fl->num,fl->arr_floor,fl->des_floor,fl->total_time);
        //p=fl;
        fl=fl->next;
        //free(p);
        k++;
    }
    floors[cur_floor]=NULL;
    people-=k;
    //printf("out %d\n",k);
    return k;
}

quene::quene()
{
    for(int i=1;i<=max_floor;i++)
    {
        num[i]=0;
        que[i]=NULL;
    }
}

bool quene::insert(person *p)
{
    if(num[p->arr_floor]>=max_floor) return false;
    num[p->arr_floor]++;
    person *fl=que[p->arr_floor];
    if(fl!=NULL)
    {
        while(fl->next!=NULL)
            fl=fl->next;
        fl->next=p;
        p->next=NULL;
    }
    else que[p->arr_floor]=p;
    return true;
}

person* quene::quit(int floor,int T,int *check)
{
    if(num[floor]==0) return NULL;
    person *p=que[floor];
    //printf("floor=%d\n",p->arr_floor);
    //printf("kkk\n");
    if(p!=NULL)
    {
        que[floor]=p->next;
        //printf("kkk2\n");
        if((T-p->arr_time)>p->tol_time)
        {
            printf("T=%d:No.%d person give up waiting\n",p->arr_time+p->tol_time,p->num);
            *check=*check+1;
            p=quit(floor,T,check);
        }
        num[floor]--;
    }
    //printf("kkk3\n");
    return p;
}

int quene::waitsum()
{
    int sum=0;
    for(int i=1;i<=max_floor;i++)
    {
        sum+=num[i];
    }
    return sum;
}