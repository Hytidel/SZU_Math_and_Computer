#include <iostream>
#include <string>
using namespace std;
class BiTreeNode {
public:
	char  data;					//数据域
	BiTreeNode  *leftChild,  *rightChild;	//左右子树指针
	BiTreeNode():leftChild(NULL), rightChild(NULL){}
	~BiTreeNode() {}
};

class BiTree {
private:
	BiTreeNode *root;	//根结点指针
	string sTree;		//建树字符串
	int pos;			//标识建树字符串的当前字符位置
	BiTreeNode * CreateTree();//建树私有函数
	void PreOrder(BiTreeNode  *t);	//先序遍历实现
	void InOrder(BiTreeNode  *t);	//中序遍历实现
	void PostOrder(BiTreeNode  *t);	//后序遍历实现
public:
	BiTree():root(NULL) {};	
	void Create(string vArray);	//建树公有接口，参数是特定的先序遍历字符串
	void PreOrder();			//先序遍历公有接口
	void InOrder();				//中序遍历公有接口
	void PostOrder();			//后序遍历公有接口
};
//二叉树公有接口的实现
void BiTree::Create(string vArray) 
{	pos=0;
	sTree.assign(vArray);	//把参数保存到内部字符串
	root = CreateTree();	//建树成功后root指向根结点
}
void BiTree::PreOrder()
{	PreOrder(root);	}
void BiTree::InOrder()
{	InOrder(root);	}
void BiTree::PostOrder()
{	PostOrder(root);	}

//请完成上述类内部的私有函数实现
BiTreeNode* BiTree::CreateTree() {
    if (pos == BiTree::sTree.length()) return NULL;

    if (BiTree::sTree[pos] == '0') {
        pos++;
        return NULL;
    }

    BiTreeNode* cur = new BiTreeNode();
    cur->data = BiTree::sTree[pos++];
    cur->leftChild = CreateTree(), cur->rightChild = CreateTree();
    return cur;
}

void BiTree::PreOrder(BiTreeNode* t) {
    cout << t->data;
    if (t->leftChild) PreOrder(t->leftChild);
    if (t->rightChild) PreOrder(t->rightChild);
}

void BiTree::InOrder(BiTreeNode* t) {
    if (t->leftChild) InOrder(t->leftChild);
    cout << t->data;
    if (t->rightChild) InOrder(t->rightChild);
}

void BiTree::PostOrder(BiTreeNode* t) {
    if (t->leftChild) PostOrder(t->leftChild);
    if (t->rightChild) PostOrder(t->rightChild);
    cout << t->data;
}
//主函数
int main()
{	int t;
	string vArray;
	cin>>t;
	while(t--)
	{	cin>>vArray;
		BiTree myTree;
		myTree.Create(vArray);
		myTree.PreOrder();		cout<<endl;
		myTree.InOrder();		cout<<endl;
		myTree.PostOrder();		cout<<endl;
	}
	return 0;
}