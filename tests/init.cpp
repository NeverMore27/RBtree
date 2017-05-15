
#include <tree.hpp>
#include <catch.hpp>


SCENARIO("null")
{
	RBtree<int> a;
	REQUIRE(a.isEmpty());
}
SCENARIO("add")
{
	RBtree <int> a;
	a.Insert(5);
	REQUIRE(!a.isEmpty());
}
SCENARIO("BST delete non inserted element", "[delete]")
{
	RBtree<int> tree = { 8 };
	tree.Remove(4);
	REQUIRE(!tree.isEmpty());
}
SCENARIO("BST delete root with one element", "[delete]")
{
	RBtree<int> tree = { 8 };
	tree.Remove(8);
	REQUIRE(tree.isEmpty());
}
SCENARIO("BST delete root without child", "[delete]")
{
	RBtree<int> tree = { 8, 4, 3 };
	tree.Remove(8);
	RBtree<int> tr ={ 4, 3 };
	REQUIRE(tree ==tr);
}
SCENARIO("BST delete root with two children", "[delete]")
{
	RBtree<int> tree = { 8, 4, 3, 10, 9, 13, 11, 12 };
	tree.Remove(4);
	RBtree<int>tr=={ 9, 8,  11, 3, 10, 13, 12 };
	REQUIRE(tree ==tr );
}
SCENARIO("BST delete non root without children", "[delete]")
{
	RBtree<int> tree = { 8, 4, 3, 10, 9, 13, 11, 12 };
	tree.Remove(3);
	RBtree<int>tr= { 9,  4, 11, 10, 12, 13, 8 };
	REQUIRE(tree == tr);
}
SCENARIO("BST delete non root with one child", "[delete]")
{
	RBtree<int> tree = { 8, 4, 3, 10, 9, 13, 11, 12 };
	tree.Remove(12);
	RBtree<int> tr ={ 9,  4, 11, 10, 4, 13, 8 }
	REQUIRE(tree == tr);
}
