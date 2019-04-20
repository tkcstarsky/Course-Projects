#include "INTtree.h"
/*tree::tree(int low,int high)
{
    inter.low=low;
    inter.high=high;

}*/

INTtree::INTtree()
{
    nil=new tree;
    nil->inter=new interval;
    nil->inter->low=NIL;
    nil->inter->high=NIL+1;
    nil->data=nil->inter->low;
    nil->max=nil->inter->high;
    nil->left=NULL;
    nil->right=NULL;
    nil->col=black;
    nil->p=NULL;
    root=nil;
    total=0;
    outnum=0;
}
  
tree* INTtree::INTtreenode(int low,int high)
{
    tree *node=new tree;
    node->inter=new interval;
    node->inter->low=low;
    node->inter->high=high;
    node->data=node->inter->low;
    node->max=node->inter->high;
    node->left=nil;
    node->right=nil;
    node->col=black;
    node->p=nil;
    return node;
}

tree* INTtree::createtree(int low,int high)
{
    root=INTtreenode(low,high);
    total++;
    return root;
}

void INTtree::set_max(tree *z)
{
    int max;
    if(z->left->max>z->right->max)
        max=z->left->max;
    else max=z->right->max;
    if(max<z->inter->high)
        max=z->inter->high;
    z->max=max;
}

void INTtree::leftrotate(tree *x)
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
    set_max(x);
    set_max(y);
}

void INTtree::rightrotate(tree *x)
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
    set_max(x);
    set_max(y);
}

tree* INTtree::insertnode(tree* par,int low,int high)
{
    if(low<par->data)
    {   if(par->left==nil)
        {
            tree *inserttree=INTtreenode(low,high);
            par->left=inserttree;
            inserttree->p=par;
            inserttree->col=red;
            return inserttree;
        }
        else return insertnode(par->left,low,high);
    }
    else if(low>par->data)
    {   if(par->right==nil)
        {
            tree *inserttree=INTtreenode(low,high);
            par->right=inserttree;
            inserttree->p=par;
            inserttree->col=red;
            return inserttree;
        }
        else return insertnode(par->right,low,high);
    }
    return NULL;
}

void INTtree::insertfixcolor(tree *z)
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

bool INTtree::INTinsert(int low,int max)
{ 
    tree *z=insertnode(root,low,max);
    if (z==NULL) return false;
    set_max(z);
    insertfixcolor(z);
    total++;
    return true;
}

void INTtree::INTTransplant(tree *u,tree *v)
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

tree* INTtree::TreeMinimum(tree *x)
{    
    while (x->left!=nil)
        x = x->left;
    return x;
}

void INTtree::INTnodeDelete(tree *z)
{
    tree *y=z,*x;
    color y_original_color = y->col;
    if (z->left == nil)
    {
        x = z->right;
        INTTransplant(z, z->right);
    }
    else if (z->right == nil)
    {
        x = z->left;
        INTTransplant(z, z->left);
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
            INTTransplant(y, y->right);
            y->right = z->right;
            y->right->p = y;
        }
        INTTransplant(z, y);
        y->left = z->left;
        y->left->p = y;
        y->col = z->col;
    }
    if (y_original_color == black)
        INTnodeDeleteFixup(x);
}

void INTtree::INTnodeDeleteFixup(tree *x)
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

bool INTtree::INTDelete(int low,int high)
{
    tree *z=SearchINTnode(root,low,high);
    if(z==NULL) return false;
    INTnodeDelete(z);
    total--;
    return true;
}

void INTtree::treepreorderTraversal(tree *nextnode)
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

void INTtree::treeinorderTraversal(tree *nextnode)
{
    if(nextnode==nil) return;
    treeinorderTraversal(nextnode->left);
    
    outtree[2][outnum]=nextnode->data;
    outnum++;
    treeinorderTraversal(nextnode->right);
}

void INTtree::printtree()
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

tree* INTtree::SearchINTnode(tree *per,int low,int high)
{
    if(per==nil) return NULL;
    if(per->data==low) 
    {
        if(per->inter->high==high)
            return per;
        else return NULL;
    }
    if(per->data>low) return SearchINTnode(per->left,low,high);
    if(per->data<low) return SearchINTnode(per->right,low,high);
    return NULL;
}

tree* INTtree::INTSearch(int low,int high)
{
    tree *x=root; 
    while ((x!=nil)&&((low >= x->inter->high)||(high <= x->inter->low)))
        if ((x->left!=nil)&&(x->left->max>=low))
            x=x->left;
        else
            x=x->right;
    if(x==nil) return NULL;
    return x;
}
