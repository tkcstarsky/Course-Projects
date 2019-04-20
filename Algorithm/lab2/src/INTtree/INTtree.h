#include <iostream>
#include <fstream>
#include <sstream>
#define NIL -10000
using namespace std;


enum color{
    red=0,
    black=1
};

typedef struct interval{
    int low;
    int high;
} interval;

typedef struct tree{
    interval *inter;
    int data;
    int max;
    color col;
    tree *left,*right,*p;
}tree;

class INTtree{
public:
    int outtree[3][100];
    int outnum;
    INTtree();
    tree *INTtreenode(int low,int high);
    tree *createtree(int low,int high);
    void set_max(tree *z);
    bool INTinsert(int low,int max);
    void INTnodeDelete(tree *z);
    tree *SearchINTnode(tree *per,int low,int high);
    bool INTDelete(int low,int high);
    void treeinorderTraversal(tree *nextnode);
    void treepreorderTraversal(tree *nextnode);
    void printtree();
    tree *INTSearch(int low,int high);
private:
    tree *root;
    tree *nil;
    int total;
    void leftrotate(tree *x);
    void rightrotate(tree *x);
    tree* insertnode(tree* par,int low,int high);
    void insertfixcolor(tree *z);
    void INTnodeDeleteFixup(tree *x);
    void INTTransplant(tree *u,tree *v);
    tree* TreeMinimum(tree *x);
};