#include "RBtree.h"
RBtree::RBtree()
{
    nil=new tree;
    nil->data=0;
    nil->left=NULL;
    nil->right=NULL;
    nil->col=black;
    nil->p=NULL;
    root=NULL;
    total=0;
    outnum=0;
}

tree* RBtree::RBtreenode(int num)
{
    tree *node=new tree;
    node->data=num;
    node->left=nil;
    node->right=nil;
    node->col=black;
    return node;
}

tree* RBtree::createtree(int num)
{
    root=new tree;
    root->data=num;
    root->left=nil;
    root->right=nil;
    root->col=black;
    root->p=nil;
    total++;
    return root;
}

void RBtree::leftrotate(tree *x)
{
    tree *y;
    y=x->right;
    x->right =y->left;
    if(y->left!=nil)
        y->left->p=x;
    y->p=x->p;
    if(x->p==nil)
        root = y;
    else 
    {
        if (x==x->p->left)
            x->p->left=y;
        else
            x->p->right=y;
    }
    y->left=x;
    x->p=y;
}

void RBtree::rightrotate(tree *x)
{
    tree *y;
    y=x->left;
    x->left=y->right;
    if(y->right!=nil)
        y->right->p=x;
    y->p=x->p;
    if(x->p==nil)
        root=y;
    else 
    {
        if (x==x->p->left)
            x->p->left=y;
        else
            x->p->right=y;
    }
    y->right=x;
    x->p=y;
}

tree* RBtree::insertnode(tree* par,int num)
{
    if(num<par->data)
    {   if(par->left==nil)
        {
            tree *inserttree=RBtreenode(num);
            par->left=inserttree;
            inserttree->p=par;
            inserttree->col=red;
            return inserttree;
        }
        else return insertnode(par->left,num);
    }
    else if(num>par->data)
    {   if(par->right==nil)
        {
            tree *inserttree=RBtreenode(num);
            par->right=inserttree;
            inserttree->p=par;
            inserttree->col=red;
            return inserttree;
        }
        else return insertnode(par->right,num);
    }
    return NULL;
}

void RBtree::insertfixcolor(tree *z)
{
    tree *y;
    while ((z->p) && (z->p->col == red))
    {
        if (z->p == z->p->p->left)  
        {          
            y=z->p->p->right;
            if ((y!=nil) && (y->col == red))       //case 1
            {
                y->col = black;
                z->p->col = black;
                z->p->p->col = red;
                z = z->p->p;
            }
            else
            {                              
                if (z == z->p->right)              //case 2
                {
                    z = z->p;
                    leftrotate(z);
                }
                
                z->p->col = black;                 //case 3
                z->p->p->col = red;
                rightrotate(z->p->p);
            }
        }
        else
        {                                          //case 4,5,6
            y = z->p->p->left;
            if (y->col == red)                     //case 4
            {
                y->col = black;
                z->p->col = black;
                z->p->p->col = red;
                z = z->p->p;
            }
            else
            {
                if (z == z->p->left)               //case 5
                {
                    z = z->p;
                    rightrotate(z);
                }
                z->p->col=black;                   //case 6
                z->p->p->col = red;
                leftrotate(z->p->p);
            }
        }
    }
    root->col = black;
}

bool RBtree::RBinsert(int num)
{
    tree *z=insertnode(root,num);
    if (z==NULL) return false;
    insertfixcolor(z);
    total++;
    return true;
}

void RBtree::RBTransplant(tree *u,tree *v)
{    
    if (u->p == nil)
        root = v;
    else 
    {
        if (u == u->p->left)
            u->p->left = v;
        else
        u->p->right = v;
    }
    v->p = u->p;
}

tree* RBtree::TreeMinimum(tree *x)
{    
    while (x->left!=nil)
        x = x->left;
    return x;
}

void RBtree::RBnodeDelete(tree *z)
{
    tree *y=z,*x;
    color y_original_color = y->col;
    if (z->left == nil)
    {
        x = z->right;
        RBTransplant(z, z->right);
    }
    else if (z->right == nil)
    {
        x = z->left;
        RBTransplant(z, z->left);
    }
    else
    {
        y = TreeMinimum(z->right);
        y_original_color = y->col;
        x = y->right;
        if (y->p == z)
            x->p = y;
        else
        {
            RBTransplant(y, y->right);
            y->right = z->right;
            y->right->p = y;
        }
        RBTransplant(z, y);
        y->left = z->left;
        y->left->p = y;
        y->col = z->col;
    }
    if (y_original_color == black)
        RBnodeDeleteFixup(x);
}

void RBtree::RBnodeDeleteFixup(tree *x)
{
    tree *w;
    while ((x != root) && (x->col == black))
        if (x == x->p->left)
        {
            w = x->p->right;
            if (w->col == red)
            {
                w->col = black;
                x->p->col = red;
                leftrotate(x->p);
                w = x->p->right;
            }
            if ((w->left->col == black) && (w->right->col == black))
            { 
                w->col=red;
                x = x->p;
            }
            else
            {
                if (w->right->col == black)
                {
                    w->left->col = black;
                    w->col = red;
                    rightrotate(w);
                    w = x->p->right;
                }
                w->col = x->p->col;
                x->p->col = black;
                w->right->col = black;
                leftrotate(x->p);
                x = root;
            }
        }
        else
        {
            w = x->p->left;
            if (w->col == red)
            {
                w->col = black;
                x->p->col = red;
                rightrotate(x->p);
                w = x->p->left;
            }
            if ((w->right->col == black) && (w->left->col == black))
            {
                w->col = red;
                x = x->p;
            }
            else
            {
                if (w->left->col == black)
                {
                    w->right->col = black;
                    w->col = red;
                    leftrotate(w);
                    w = x->p->left;
                }
                w->col = x->p->col;
                x->p->col = black;
                w->left->col = black;
                rightrotate(x->p);
                x =root;
            }
        }
    x->col = black;
}

bool RBtree::RBDelete(int num)
{
    tree *z=SearchRBnode(root,num);
    if(z==NULL) return false;
    RBnodeDelete(z);
    total--;
    return true;
}

void RBtree::treepreorderTraversal(tree *nextnode)
{
    //ofstream out;
    if(nextnode==nil) return;
    outtree[0][outnum]=nextnode->data;
    outtree[1][outnum]=nextnode->col;
    outnum++;
    //cout<<outnum;
    treepreorderTraversal(nextnode->left);
    treepreorderTraversal(nextnode->right);
    //out->open("tree_out->txt",ios::app);
    //out->close();
}

void RBtree::treeinorderTraversal(tree *nextnode)
{
    if(nextnode==nil) return;
    treeinorderTraversal(nextnode->left);
    
    outtree[2][outnum]=nextnode->data;
    outnum++;
    treeinorderTraversal(nextnode->right);
}

void RBtree::printtree()
{
    ofstream out;
    int i,j;
    out.open("input.txt");
    outnum=0;
    treepreorderTraversal(root);
    outnum=0;
    treeinorderTraversal(root);
    out<<total<<endl;
    for(j=0;j<3;j++)
    {
        for(i=0;i<total;i++)
        {
            out<<outtree[j][i]<<' ';
        }
        out<<endl;
    }
    out.close();
}

tree* RBtree::SearchRBnode(tree *per,int num)
{
    if(per==nil) return NULL;
    if(per->data==num) return per;
    if(per->data>num) return SearchRBnode(per->left,num);
    if(per->data<num) return SearchRBnode(per->right,num);
    return NULL;
}

