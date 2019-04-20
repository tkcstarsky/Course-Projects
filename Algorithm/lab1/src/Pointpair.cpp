#include "Pointpair.h"
Pointpair::Pointpair(int maxpoint)
{
    points=new Point[maxpoint+10];
    pointnumber=0;
}

void Pointpair::Addpoint(double x,double y)
{
    points[pointnumber].x=x;
    points[pointnumber].y=y;
    pointnumber++;
}

double Pointpair::dis(int p1,int p2)
{
    return sqrt((points[p2].y-points[p1].y)*(points[p2].y-points[p1].y)+(points[p2].x-points[p1].x)*(points[p2].x-points[p1].x));
}

bool cmpxy(const Point& p1,Point& p2)
{
    if(p1.x!=p2.x)
        return p1.x<p2.x;
    return p1.y<p2.y;
}

void Pointpair::pointsort(int left,int right)
{
    std::sort(points,points+pointnumber-1,cmpxy);
}

double Pointpair::devide(int left,int right)
{
    double dmin=1e10,dl,dr;
    int i,j;
    if(left==right) return dmin;                        //只有一个点时返回无限大
    if(left+1==right) return dis(left,right);           //只有两个点时返回该点对距离
    int mid=(left+right)/2;
    dl=devide(left,mid);                                //对左右两个区域进行分治得到最近距离
    dr=devide(mid+1,right);
    if(dl<dr) dmin=dl;
    else dmin=dr;                                       //选取左右区域更小的点
    for(i=left;i<=right;i++)                            //确定中间区域的最左结点编号
    {
        if(fabs(points[mid].x-points[i].x)<=dmin)
        {
            left=i;
            break;
        }
    }
    for(i=left;i<=right;i++)                            //确定中间区域的最右结点编号
    {
        if(fabs(points[mid].x-points[i].x)>dmin)
        {
            right=i-1;
            break;
        }
    }
    if(mid==right) mid--;
    for(i=left;i<=mid;i++)                              //遍历该中间区域求得最近距离
        for(j=mid+1;j<=right;j++)
            if(dis(i,j)<dmin) dmin=dis(i,j);
    return dmin;
}

double Pointpair::mindis()
{
    int i,j;
    double mind=1e20;
    for(i=0;i<pointnumber;i++)
        for(j=i+1;j<pointnumber;j++)
        {
            if(dis(i,j)<mind) 
            {
                mind=dis(i,j);
                minl=i;
                minr=j;
            }
        }
    return mind;
}

void Pointpair::printpos()
{
    printf("x1=%lf,y1=%lf\n",points[minl].x,points[minl].y);
    printf("x2=%lf,y2=%lf\n",points[minr].x,points[minr].y);
}