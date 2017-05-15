#include <iostream>
#include<memory.h>

using namespace std;

template <class T>
class RBtree {
	struct node_st
	{
		node_st *left, *right;
		T value;
		bool red;
		void Show(ostream &out, int level) const;
	};
	node_st *tree_root;
	int nodes_count;
private:
	node_st* NewNode(T value)
	{
		nodes_count++;
		node_st *node = new node_st;
		node->value = value;
		node->left = nullptr;
		node->right = nullptr;
		node->red = true;
		return node;
	}
	void DelNode(node_st*);
	void Clean(node_st*);
	node_st* Rotate21(node_st* node) //по часовой
	{
		node_st *right = node->right;
		node_st *root_st = right->left;
		right->left = node;
		node->right = root_st;
		return right;
	}
	node_st* Rotate12(node_st* node)
	{
		node_st *left = node->left;
		node_st *root_st = left->right;
		left->right = node;
		node->left = root_st;
		return left;
	}
	void BalanceInsert(node_st**);
	bool BalanceRemove1(node_st**);
	bool BalanceRemove2(node_st**);
	bool Insert(T&, node_st**);
	bool GetMin(node_st**, node_st**);
	bool Remove(node_st**, T&);
	bool isEqual(node_st* root2, const node_st* root1)
	{
		return (root2&&root1 ? root2->value == root1->value&&isEqual(root2->left, root1->left) && isEqual(root2->right, root1->right) : !root2 && !root1);
	};

public:
	bool isEmpty()
	{
		node* root1 = nullptr;
		return isEqual(tree_root, root1);
	}
	bool Get_Color()
	{
		return red;
	}
	void Show(ostream &out) const;
	RBtree(std::initializer_list<T> list)
	{
		root = nullptr;
		for (auto& item : list)
		{
			Insert(item);
		}

	}
	RBtree();
	~RBtree();
	void Clean();
	void Insert(T);
	void Remove(T);
	int GetNodesCount() const;
	bool operator ==(const RBtree<T> &a)
	{

		return isEqual(tree_root, a.tree_root);
	}
};


template <class T>
RBtree<T>::RBtree()
{
	tree_root = 0;
	nodes_count = 0;
}
template <class T>
RBtree<T>::~RBtree()
{
	Clean(tree_root);
}
template <class T>
int RBtree<T>::GetNodesCount() const
{
	return nodes_count;
}
template <class T>
void RBtree<T>::DelNode(node_st *node)
{
	nodes_count--;
	delete node;
}
template <class T>
void RBtree<T>::Clean(node_st *node)
{
	if (!node) return;
	Clean(node->left);
	Clean(node->right);
	DelNode(node);
}
template <class T>
void RBtree<T>::node_st::Show(ostream &out, int level) const
{
	const node_st *tr = this;

	if (tr) tr->right->Show(out, level + 1);
	for (int i = 0; i<level; i++)
		out << "   ";

	if (tr)
	{
		char *is_red = "black";
		if (red) is_red = "red";
		out << tr->value << "\n";
		for (int i = 0; i<level; i++)
			out << "   ";
		out << is_red << endl;
	}
	else out << "End" << endl;
	if (tr) tr->left->Show(out, level + 1);
}
template <class T>
void RBtree<T>::BalanceInsert(node_st **root)
{
	node_st *left, *right, *lx1, *rx1;
	node_st *node = *root;
	if (node->red) return;
	left = node->left;
	right = node->right;
	if (left && left->red)
	{
		rx1 = left->right;
		if (rx1 && rx1->red) left = node->left = Rotate21(left);
		lx1 = left->left;
		if (lx1 && lx1->red) {
			node->red = true;
			left->red = false;
			if (right && right->red) {	// отделаемся перекраской вершин
				lx1->red = true;
				right->red = false;
				return;
			}
			*root = Rotate12(node);
			return;
		}
	}
	// тоже самое в другую сторону
	if (right && right->red) {
		lx1 = right->left;
		if (lx1 && lx1->red) right = node->right = Rotate12(right);
		rx1 = right->right;
		if (rx1 &&rx1->red) {
			node->red = true;
			right->red = false;
			if (left && left->red) {	// отделаемся перекраской вершин
				rx1->red = true;
				left->red = false;
				return;
			}
			*root = Rotate21(node);
			return;
		}
	}
}
template <class T>
bool RBtree<T>::BalanceRemove1(node_st **root)
{
	node_st *node = *root;
	node_st *left = node->left;
	node_st *right = node->right;
	if (left && left->red)
	{
		left->red = false;
		return false;
	}
	if (right && right->red)
	{
		node->red = true;
		right->red = false;
		node = *root = Rotate21(node);
		if (BalanceRemove1(&node->left)) node->left->red = false;
		return false;
	}

	unsigned int mask = 0;
	node_st *p21 = right->left;
	node_st *p22 = right->right;
	if (p21 && p21->red) mask |= 1;
	if (p22 && p22->red) mask |= 2;
	switch (mask)
	{
	case 0:
		right->red = true;
		return true;
	case 1:
	case 3:
		right->red = true;
		p21->red = false;
		right = node->right = Rotate12(right);
		p22 = right->right;
	case 2:
		right->red = node->red;
		p22->red = node->red = false;
		*root = Rotate21(node);
	}
	return false;
}
template <class T>
bool RBtree<T>::BalanceRemove2(node_st **root)
{
	node_st *node = *root;
	node_st *left = node->left;
	node_st *right = node->right;
	if (right && right->red)
	{
		right->red = false;
		return false;
	}
	if (left && left->red) {
		node->red = true;
		left->red = false;
		node = *root = Rotate12(node);
		if (BalanceRemove2(&node->right)) node->right->red = false;
		return false;
	}
	unsigned int mask = 0;
	node_st *p11 = left->left;
	node_st *p12 = left->right;
	if (p11 && p11->red) mask |= 1;
	if (p12 && p12->red) mask |= 2;
	switch (mask) {
	case 0:
		left->red = true;
		return true;
	case 2:
	case 3:
		left->red = true;
		p12->red = false;
		left = node->left = Rotate21(left);
		p11 = left->left;
	case 1:
		left->red = node->red;
		p11->red = node->red = false;
		*root = Rotate12(node);
	}
	return false;
}
template <class T>
bool RBtree<T>::Insert(T &value, node_st **root)
{
	node_st *node = *root;
	if (!node) *root = NewNode(value);
	else {
		if (value == node->value) return true;
		if (Insert(value, value<node->value ? &node->left : &node->right)) return true;
		BalanceInsert(root);
	}
	return false;
}
template <class T>
bool RBtree<T>::GetMin(node_st **root, node_st **res)
{
	node_st *node = *root;
	if (node->left) {
		if (GetMin(&node->left, res)) return BalanceRemove1(root);
	}
	else {
		*root = node->right;
		*res = node;
		return !node->red;
	}
	return false;
}
template <class T>
bool RBtree<T>::Remove(node_st **root, T &value)
{
	node_st *t, *node = *root;
	if (!node) return false;
	if (node->value<value) {
		if (Remove(&node->right, value))
			return BalanceRemove2(root);
	}
	else if (node->value>value) {
		if (Remove(&node->left, value))	return BalanceRemove1(root);
	}
	else {
		bool res;
		if (!node->right) {
			*root = node->left;
			res = !node->red;
		}
		else {
			res = GetMin(&node->right, root);
			t = *root;
			t->red = node->red;
			t->left = node->left;
			t->right = node->right;
			if (res) res = BalanceRemove2(root);
		}
		DelNode(node);
		return res;
	}
	return 0;
}
template <class T>
void RBtree<T>::Show(ostream &out) const
{
	tree_root->Show(out, 0);
}
template <class T>
void RBtree<T>::Insert(T value)
{
	Insert(value, &tree_root);
	if (tree_root) tree_root->red = false;
}
template <class T>
void RBtree<T>::Remove(T value)
{
	Remove(&tree_root, value);
}
template <class T>
void RBtree<T>::Clean()
{
	Clean(tree_root);
	tree_root = nullptr;
}
