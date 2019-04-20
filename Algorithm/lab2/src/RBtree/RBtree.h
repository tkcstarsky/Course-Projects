#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

enum color{
    red=0,
    black=1
};
typedef struct tree{
    int data;
    color col;
    tree *left,*right,*p;
} tree;

class RBtree{
public:
    int outtree[3][100];
    int outnum;
    RBtree();
    tree *RBtreenode(int num);
    tree *createtree(int num);
    bool RBinsert(int num);
    void RBnodeDelete(tree *z);
    tree *SearchRBnode(tree *per,int num);
    bool RBDelete(int num);
    void treeinorderTraversal(tree *nextnode);
    void treepreorderTraversal(tree *nextnode);
    void printtree();
private:
    tree *root;
    tree *nil;
    int total;
    void leftrotate(tree *x);
    void rightrotate(tree *x);
    tree* insertnode(tree* par,int num);
    void insertfixcolor(tree *z);
    void RBnodeDeleteFixup(tree *x);
    void RBTransplant(tree *u,tree *v);
    tree* TreeMinimum(tree *x);
};