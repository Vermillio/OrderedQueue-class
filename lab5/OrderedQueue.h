#pragma once
#include "BinaryTree.h"
#include <iostream>
#include <vector>

using namespace std;


namespace QueBinTr
{
	template<class T>
	struct Node
	{
		T data;
		Node *l, *r;
		Node() : l(nullptr), r(nullptr) {};
		Node(T _data) : data(_data), l(nullptr), r(nullptr) {};
	};

	template<class T>
	class OrderedQueue
	{
		TNode<T> *fr, *bk;
		BinaryTree<T> BinTree;
		int sz;
		bool order;

	public:

		enum ord { desc, asc };

		//Test whether container is empty
		bool empty();

		//Access next element
		T *front();

		//Access last element
		T *back();

		//Insert element
		void enqueue(T data);

		//Remove next element
		void dequeue();

		//Remove all elements
		void clear();



		//Change order of elements to ascending / descending
		void setOrder(bool asc);

		int size() const;

		void test();

		OrderedQueue() : sz(0), fr(nullptr), bk(nullptr), order(asc) {};
	};

	template<class T>
	inline bool OrderedQueue<T>::empty()
	{
		return BinTree.empty();
	}

	template<class T>
	inline T * OrderedQueue<T>::front()
	{
		if (!fr)
			return nullptr;
		return &fr->data;
	}

	template<class T>
	inline T * OrderedQueue<T>::back()
	{
		if (!bk)
			return nullptr;
		return &bk->data;
	}

	template<class T>
	inline void OrderedQueue<T>::dequeue()
	{
		if (!bk)
			return;
		TNode<T> *tmp;
		if (order)
			tmp = min(bk->parent, bk->child[tleft]);
		else
			tmp = max(bk->parent, bk->child[tright]);
		BinTree.remove(bk->data);
		bk = tmp;
		--sz;
		if (sz == 0)
			fr = nullptr;
	}

	template<class T>
	inline void OrderedQueue<T>::clear()
	{
		Clear(BinTree.getRoot());
	}

	template<class T>
	inline void OrderedQueue<T>::enqueue(T data)
	{
		++sz;
		BinTree.insert(data);
		if (sz==1)
		{
			fr = BinTree.getRoot();
			bk = BinTree.getRoot();
		}
		else if (order)
		{
			if (fr->child[tleft])
				fr = fr->child[tleft];
			if (bk->child[tright])
				bk = bk->child[tright];
		}
		else {
			if (fr->child[tright])
				fr = fr->child[tright];
			if (bk->child[tleft])
				bk = bk->child[tleft];
		}
	}

	template<class T>
	inline void OrderedQueue<T>::setOrder(bool _order)
	{
		order = _order;
		swap(fr, bk);
	}

	template<class T>
	inline int OrderedQueue<T>::size() const
	{
		return sz;
	}

	template<class T>
	inline void OrderedQueue<T>::test()
	{
		int choise = -1;
		cout << "1 - PUSH 2 - POP 3 - FRONT 4 - BACK 5 - SET ORDER 6 - CLEAR 0 - EXIT" << endl;
		T data, *t;
		char choise2;
		while (true)
		{
			while (choise < 0 || choise > 6)
				cin >> choise;
			if (!choise)
				break;
			switch (choise)
			{
			case 1:
				cout << "Enter : ";
				cin >> data;
				enqueue(data);
				break;
			case 2:
				dequeue();
				break;
			case 3:
				t = front();
				if (t)
					cout << *t << endl;
				else cout << "empty" << endl;
				break;
			case 4:
				t = back();
				if (t)
					cout << *t << endl;
				else cout << "empty" << endl;
				break;
			case 5:
				cout << "Ascending? (y / n)" << endl;
				cin >> choise2;
				if (choise2 == 'y')
					setOrder(asc);
				else if (choise2 == 'n')
					setOrder(desc);
				else cout << "Unknown" << endl;
				break;
			case 6:
				clear();
				cout << "Successfully cleared." << endl;
				break;
			case 0:
				clear();
				return;
			}
			choise = -1;
		}

	}

}