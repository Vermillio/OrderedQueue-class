
#define max(a, b) a > b ? a : b
#define min(a, b) a < b ? a : b
#pragma once

#include <string>
#include <fstream>

using namespace std;

enum	Dir {
	tleft = 0,
	tright = 1
};

template<class T>
struct	TNode {
	int height;
	T data;
	TNode **child;
	TNode *parent;

	TNode();
	TNode(T x);
	TNode(T x, TNode *l, TNode *r);
};

template<class T>
bool operator< (TNode<T> &x, TNode<T> &y)
{
	if (!x)
		return true;
	if (!y)
		return false;
	return x->data < y->data;
}

template<class T>
class	BinaryTree
{

	TNode<T>		*root;

	int				nodeNum;
	
	void consoleOutput(TNode<T> * t, int n);

	TNode<T>   *avlInsert(TNode<T> *t, T data);

	TNode<T> *avlRemove(TNode<T> *t, T data);

	//TNode<T> *avlRemove(TNode<T> *t);

	int		delta_h(TNode<T> *t);

	TNode<T>	*rotate_once(TNode<T> *t, bool dir);

	void	countheight(TNode<T> *t);

	int		height(TNode<T> *t);

	TNode<T>	*balance(TNode<T> *t);

	public:

		bool empty();

		TNode<T>* getRoot();

		void	Print();

		void	insert(T data);

		void	remove(T data);

		BinaryTree() : root(nullptr), nodeNum(0) {};
};


template<class T>
void	Clear	(TNode<T> *t);

template<class T>
TNode<T>::TNode()
{
	child = new TNode*[2];
	child[0] = nullptr;
	child[1] = nullptr;
	parent = nullptr;
}

template<class T>
TNode<T>::TNode(T x) : data(x),
child(new TNode*[2]),
parent(nullptr)
{
	height = 0;
	child[tleft] = nullptr;
	child[tright] = nullptr;
}

template<class T>
inline bool BinaryTree<T>::empty()
{
	return root != nullptr;
}

template<class T>
inline TNode<T>* BinaryTree<T>::getRoot()
{
	return root;
}

template<class T>
void BinaryTree<T>::Print()
{
	consoleOutput(root, 0);
	cout << endl;
}

template<class T>
void Clear(TNode<T> *t)
{
	if (!t)
		return;
	Clear(t->child[0]);
	Clear(t->child[1]);
	delete[] t->child;
	delete t;
	t = nullptr;
}

template<class T>
void BinaryTree<T>::consoleOutput(TNode<T> *p, int level)
{
	if (p)
	{
		consoleOutput(p->child[tright], level + 1);
		for (int i = 0; i < level; i++)
			cout << "     ";
		cout << "(" << p->data << ")" << endl;
		consoleOutput(p->child[tleft], level + 1);
	}
	return;
}

template<class T>
TNode<T>::TNode(T x, TNode<T> * l, TNode<T> * r) : data(x), child(new TNode<T>*[2]), parent(nullptr), red(true), height(0)
{
	child[0] = l;
	child[1] = r;
}

template<class T>
void  BinaryTree<T>::insert(T data)
{
	root = avlInsert(root, data);
	++nodeNum;
}

template<class T>
TNode<T>* BinaryTree<T>::avlInsert(TNode<T>* t, T data)
{
	if (!t)
	{
		TNode<T> *p = new TNode<T>(data);
		p->parent = t;
		return p;
	}
	if (data < t->data)
		t->child[tleft] = avlInsert(t->child[tleft], data);
	else
		t->child[tright] = avlInsert(t->child[tright], data);
	return balance(t);
}

template<class T>
void  BinaryTree<T>::remove(T data)
{
	root = avlRemove(root, data);
	--nodeNum;
}

template<class T>
TNode<T>* BinaryTree<T>::avlRemove(TNode<T>* t, T data) // k key deletion from p tree
{
	if (!t) return nullptr;
	if (data < t->data)
		t->child[tleft] = avlRemove(t->child[tleft], data);
	else if (data > t->data)
		t->child[tright] = avlRemove(t->child[tright], data);
	else
	{
		TNode<T>* q = t->child[tleft];
		TNode<T>* r = t->child[tright];

		if (q && r)
		{
			while (q->child[tright])
			{
				q = q->child[tright];
			}
			t->data = q->data;
			q = avlRemove(q, data);
			return q;
		}

		TNode<T> *x;

		if (q)
			x = q;
		else
			x = r;

		if (!t->parent)
			root = x;
		else {
			if (t->parent->child[tleft] && t == t->parent->child[tleft])
				t->parent->child[tleft] = x;
			else
				t->parent->child[tright] = x;
			if (x)
				x->parent = t->parent;
		}
		return balance(x);
	}
}

template<class T>
void  BinaryTree<T>::countheight(TNode<T> *t)
{
	if (!t)
		return;
	countheight(t->child[tleft]);
	countheight(t->child[tright]);
	t->height = max(height(t->child[tleft]), height(t->child[tright])) + 1;
}

template<class T>
int	  BinaryTree<T>::height(TNode<T> *t)
{
	return t ? t->height : 0;
}

template<class T>
TNode<T>  *BinaryTree<T>::balance(TNode<T> *t)
{
	countheight(t);
	if (delta_h(t) >= 2)
	{
		if (delta_h(t->child[tleft]) < 0)
			t->child[tleft] = rotate_once(t->child[tleft], tleft);
		t = rotate_once(t, tright);
	}
	else if (delta_h(t) <= -2)
	{
		if (delta_h(t->child[tright]) > 0)
			t->child[tright] = rotate_once(t->child[tright], tright);
		t = rotate_once(t, tleft);
	}
	//countheight(t);
	return t;
}

template<class T>
TNode<T> *BinaryTree<T>::rotate_once(TNode<T> * t, bool dir)
{
	TNode<T> *pivot = t->child[!dir];
	t->child[!dir] = pivot->child[dir];
	if (t->child[!dir])
		pivot->child[dir]->parent = t;
	pivot->child[dir] = t;
	pivot->parent = t->parent;
	t->parent = pivot;

	balance(t);
	balance(pivot);

	return pivot;
}

template<class T>
int	BinaryTree<T>::delta_h(TNode<T> *t)
{
	if (!t)
		return 0;
	return height(t->child[tleft]) - height(t->child[tright]);
}