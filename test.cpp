/*test01.cpp*/

//
// Unit tests for threaded binary search tree
//

#include <iostream>
#include <vector>

#include "bstt.h"

#include "catch.hpp"

using namespace std;


TEST_CASE("(1) empty tree") 
{
  bstt<int, int>  tree;
	
  // empty tree
  REQUIRE(tree.size() == 0);
	
  // testing insert
  
  tree.insert(10, 5);
  tree.insert(20, 10);
  tree.insert(5, 1);
  tree.insert(15, 10);
  tree.insert(7, 7);
  tree.insert(1, 0);
	
  REQUIRE(tree.size() == 6);
	
  // testing copy
  bstt<int, int> newTree(tree);
  REQUIRE(newTree.size() == 6);
  
  // testing search
  int x;
  REQUIRE(newTree.search(1, x));
  REQUIRE(x == 0);
  REQUIRE(newTree.search(10, x));
  REQUIRE(x == 5);
  x = tree[20];
  REQUIRE(x == 10);
	
  // testing next
  int key;
  tree.begin();
  while (tree.next(key)){
	  cout << key << endl;
  }
  
  // testing clear
  tree.clear();
  REQUIRE(tree.size() == 0);
}

TEST_CASE("(2) operator [] tree") 
{
	bstt<int, int>  tree;
	
	REQUIRE( tree[101] == 0 );
}

TEST_CASE("(3) check dump") 
{
	bstt<int, int>  tree;
	
	tree.insert(1, 2);
	tree.insert(3, 6);
	tree.insert(7, 5);
	tree.insert(5, 20);
	
	int x;
	
	tree.dump(cout);
	tree.begin();
	
	while (tree.next(x)){
	  cout << x << endl;
    }
}

TEST_CASE("(4) check operator ()") 
{
	bstt<int, int>  tree;
	
	tree.insert(1, 12);
	tree.insert(2, 9);
	tree.insert(3, 6);
	tree.insert(4, 7);
	
	REQUIRE(tree(1) == 2);
	REQUIRE(tree(2) == 3);
	REQUIRE(tree(3) == 4);
	
	tree.begin();
}

