#include <iostream>
#include <math.h>
#include <cstdio> 
#include <algorithm>
using namespace std;
typedef struct 
{
    double x;
    double y;
} Point;
class Pointpair
{
private:
    Point *points;
    int minl,minr;
public:
    int pointnumber;
    Pointpair(int maxpoint);
    double dis(int p1,int p2);
    void Addpoint(double x,double y);
    void pointsort(int left,int right);
    double devide(int left,int right);
    double mindis();
    void printpos();
};