#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

 struct TreeNode {
    int val;
    int color;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x, int y) : val(x), color(y), left(NULL), right(NULL) {}
};

 struct sequence {
    int val;
    int color;
};


class Solution {
	TreeNode* buildTreeHelper(std::vector<sequence>& preorder, int& pin, int start, int end, std::vector<int>& inorder, std::map<int, int>& mp){
        if( start > end){
            return nullptr;
        }

        auto curr = preorder[pin++].val;
        auto node = new TreeNode(curr, preorder[pin-1].color);
        auto place = mp[curr];



        node->left = buildTreeHelper(preorder, pin, start, place-1, inorder, mp );
        node->right = buildTreeHelper(preorder, pin,  place+1, end, inorder, mp );
        return node;
    }
public:
	TreeNode* buildTree(std::vector<sequence>& preorder, std::vector<int>& inorder) {
        int pstart=0;
        std::map<int, int> mp;
        for(int i=0; i < inorder.size(); ++i){
            mp[inorder[i]] = i;
        }
        if( preorder.size() ==0 || inorder.size() == 0 || preorder.size() != inorder.size())
            return nullptr;
        return buildTreeHelper(preorder, pstart, 0, inorder.size()-1, inorder, mp  );

    }


    void PreOrder(TreeNode* T)//先序递归遍历
    {
        if(T!=NULL)
        {
            std::cout<<T->val<<"("<<T->color<<")"<<" ";
            PreOrder(T->left);
            PreOrder(T->right);
        }
    }


    void InOrder(TreeNode* T)//先序递归遍历
    {
        if(T!=NULL)
        {

            InOrder(T->left);

            std::cout<<T->val<<"("<<T->color<<")"<<" ";

            InOrder(T->right);
        }
    }


};

void input_tree_info(char* file_name, std::vector<sequence>& preorder, std::vector<int>& inorder)
{
    FILE* input = fopen(file_name, "r");

    int tree_size;
    int file_tmp = fscanf(input, "%d", &tree_size);

    std::cout<<"tree_size: "<<tree_size<<std::endl;
    preorder.resize(tree_size);
    inorder.resize(tree_size);

    int val;
    int i=0;

    while(i<tree_size && fscanf(input, "%d", &val)!=EOF)
    {

        preorder[i].val = val;

        i++;
    }



    i=0;

    while(i<tree_size && fscanf(input, "%d", &val)!=EOF)
    {

         preorder[i].color = val;

        i++;
    }


    i=0;

    while(i<tree_size && fscanf(input, "%d", &val)!=EOF)
    {
         inorder[i] = val;

        i++;
    }

    fclose(input);

}



void PreOrder_dot(TreeNode* T)//先序递归遍历
{
    if(T!=NULL)
    {
        if(T->left!=NULL)
        {
            std::cerr<<T->val<<"->"<<T->left->val<<";"<<std::endl;
        }

        if(T->right!=NULL)
        {
            std::cerr<<T->val<<"->"<<T->right->val<<";"<<std::endl;
        }


        PreOrder_dot(T->left);
        PreOrder_dot(T->right);
    }
}

void generate_dot_file(std::vector<sequence>& preorder, TreeNode* T)
{

    std::cerr<<"digraph first2{"<<std::endl;

    for(int i =0; i < preorder.size(); i++)
    {
        std::cerr<<preorder[i].val<<" [color=";

        if(preorder[i].color == 0)
        {
            std::cerr<<"\"red\"];"<<std::endl;
        }
        else
        {
            std::cerr<<"\"black\"];"<<std::endl;
        }

    }

    PreOrder_dot(T);

    std::cerr<<"}";
}

int main(int argc, char** argv)
{


    std::vector<sequence> preorder;
    std::vector<int> inorder;



	input_tree_info(argv[1], preorder, inorder);



	Solution tree;

	TreeNode* root = tree.buildTree(preorder, inorder);

	TreeNode* T;


    T = root;
	tree.PreOrder(T);
	std::cout<<std::endl;
	T = root;
	tree.InOrder(T);
	std::cout<<std::endl;


	generate_dot_file(preorder, root);

	return 0;
}
