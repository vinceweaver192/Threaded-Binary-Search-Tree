/*bstt.h*/

//
// Threaded binary search tree
//

#pragma once

#include <iostream>

using namespace std;

template<typename KeyT, typename ValueT>
class bstt
{
private:
  struct NODE
  {
    KeyT   Key;
    ValueT Value;
    NODE*  Left;
    NODE*  Right;
    bool   isThreaded;
  };

  NODE* Root;  // pointer to root node of tree (nullptr if empty)
  int   Size;  // # of nodes in the tree (0 if empty)
  NODE* Yoo;


  
public:
  //
  // default constructor:
  //
  // Creates an empty tree.
  //
  bstt()
  {
    Root = nullptr;
    Size = 0;
  }
  // copy helper function
  void _copy(NODE* cur){
    if (cur == NULL){
	  return;
	}
	else{
	  this->insert(cur->Key, cur->Value);
	  _copy(cur->Left);
	  if (cur->isThreaded != true){
		  _copy(cur->Right);
	  }
	}
  }
  //
  // copy constructor
  //
  bstt(const bstt& other)
  {
    //
    // TODO
    //
    // check for empty tree
    if (other.Root == NULL){
	  this->Root = NULL;
	}
	else {
	  this->Root = new NODE();
	  
	  if (this->Root->isThreaded != true){
		  _copy(other.Root);
	  }	 
	  
	  this->Size = other.Size;
	}

  }


  //
  // clear:
  //
  // Clears the contents of the tree, resetting the tree to empty.
  //
 void deleteAllNodes(NODE * cur){
	  if(cur == NULL){
		  return;
	  }	  
	  deleteAllNodes(cur->Left);
	  if (cur->isThreaded != true){
		  deleteAllNodes(cur->Right);
	  }
	  
	  delete cur;
  }
  
   // destructor:
  //
  // Called automatically by system when tree is about to be destroyed;
  // this is our last chance to free any resources / memory used by
  // this tree.
  // 
  virtual ~bstt(){
		deleteAllNodes(Root);
  }
  	
	
  void clear(){
	  if(Root == NULL){
		  return;
	  }	  
	  deleteAllNodes(Root);
	  
	  Root = 0;
	  Size = 0;
  }

  //
  // operator=
  //
  // Clears "this" tree and then makes a copy of the "other" tree.
  //
  bstt& operator=(const bstt& other)
  {
    //
    // TODO:
    //

    deleteAllNodes(this->Root);
	
    if (other.Root == NULL){
	  this->Root = NULL;
	}
	else {
	  this->Root = new NODE();
	  
	  _copy(other.Root);
	  this->Size = other.Size;
	}
	
    //return *this;
  }
  
  // 
  // size:
  //
  // Returns the # of nodes in the tree, 0 if empty.
  //
  // Time complexity:  O(1) 
  //
  int size() const
  {
    return Size;
  }

  // 
  // search:
  //
  // Searches the tree for the given key, returning true if found
  // and false if not.  If the key is found, the corresponding value
  // is returned via the reference parameter.
  //
  // Time complexity:  O(lgN) on average
  //
  bool search(KeyT key, ValueT& value) const
  {
    //
    // TODO
    //

    NODE* cur = Root;

    while (cur != nullptr)
    {
      if (key == cur->Key)
      {  // already in tree
       value = cur->Value;
       return true;
      }
      else if (key < cur->Key)  // search left:
      {
         cur = cur->Left;
      }
      else if (key > cur->Key && cur->isThreaded != true)
      {// has a right value
         cur = cur->Right;
      }
      else{
         return false;
      }
    }//while  
  
    // if get here, not found 
    return false;
  }
  // return a node of the one with a key greater than the key of current
//   void _makeThread(NODE* cur, stack<NODE*> & nodeKeyStack){ 


//   }
  //
  // insert
  //
  // Inserts the given key into the tree; if the key has already been insert then
  // the function returns without changing the tree.
  //
  // Time complexity:  O(lgN) on average
  //
  void insert(KeyT key, ValueT value)
  {

    NODE* prev = nullptr;
    NODE* cur = Root;

    //
    // 1. Search to see if tree already contains key:
    //
    while (cur != nullptr)
    {
      if (key == cur->Key){  // already in tree
        return;
	  }
// 	  if (cur->Right == NULL){ // declare threaded node
// 		  cur->isThreaded = true;
// 	  }
// 	  else {
// 		  cur->isThreaded = false;
	  
      if (key < cur->Key)  // search left:
      {
        prev = cur;
        cur = cur->Left;
      }
      else
      {
        prev = cur;
		if (cur->isThreaded != true)
			cur = cur->Right;
		else 
			cur = NULL;
		
      }
    }//while
	
	
    //
    // 2. if we get here, key is not in tree, so allocate
    // a new node to insert:
    // 

    NODE *n = new NODE();
    n->Key = key;
	n->Value = value; // allocate new value into node
	
    n->Left = nullptr;
    n->Right = nullptr;

    //
    // 3. link in the new node:
    //
    // NOTE: cur is null, and prev denotes node where
    // we fell out of the tree.  if prev is null, then
    // the tree is empty and the Root pointer needs 
    // to be updated.
    //

   if ( prev == nullptr) {
	  Root = n;
	  n->isThreaded = true;
   }
   else if (key < prev->Key) {
	  prev->Left = n;
	  n->Right = prev;
	  n->isThreaded = true;
   }
   else {
		   n->Right = prev->Right;
		   prev->Right = n;
		   prev->isThreaded = false;
		   n->isThreaded = true;
	   }
   

    // 
    // 4. update size and we're done:
    //
    
    Size++; // = size();    
  }// end of insert

  //
  // []
  //
  // Returns the value for the given key; if the key is not found,
  // the default value ValueT{} is returned.
  //
  // Time complexity:  O(lgN) on average
  //
  ValueT operator[](KeyT key) const
  {
    //
    // TODO
    //
    int x;

	
    if (search(key, x)){
		return x;
	}
	else {
		return ValueT{};
	}
	
  }

  //
  // ()
  //
  // Finds the key in the tree, and returns the key to the "right".
  // If the right is threaded, this will be the next inorder key.
  // if the right is not threaded, it will be the key of whatever
  // node is immediately to the right.
  //
  // If no such key exists, or there is no key to the "right", the
  // default key value KeyT{} is returned.
  //
  // Time complexity:  O(lgN) on average
  //
  KeyT operator()(KeyT key) const
  {
    NODE* cur = Root;

    while (cur != nullptr)
    {
      if (key == cur->Key)
      {  // the right is null so default
		  if (cur->Right != nullptr){
			  return cur->Right->Key;
		  }
		  else {
			  break;
		  }
      }
	  if (key < cur->Key){
		  cur = cur->Left;
	  }
	  else{
		  if (cur->isThreaded){
			  //return KeyT{ };
			  cur = nullptr;
		  }
		  else{
			  cur = cur->Right;
		  }
	  }
    }//while  	
    return KeyT{ };
  }

  //
  // begin
  //
  // Resets internal state for an inorder traversal.  After the 
  // call to begin(), the internal state denotes the first inorder
  // key; this ensure that first call to next() function returns
  // the first inorder key.
  //
  // Space complexity: O(1)
  // Time complexity:  O(lgN) on average
  //
  // Example usage:
  //    tree.begin();
  //    while (tree.next(key))
  //      cout << key << endl;
  //
  void begin()
  {
    //
    // TODO
    //

	Yoo = Root;
		
	if (size() == 0){
		// tree is empty
		return;
	}
	else {
		while (Yoo->Left != NULL){
			Yoo = Yoo->Left;
		}	
	}
	
}// end of begin

  //
  // next
  //
  // Uses the internal state to return the next inorder key, and 
  // then advances the internal state in anticipation of future
  // calls.  If a key is in fact returned (via the reference 
  // parameter), true is also returned.
  //
  // False is returned when the internal state has reached null,
  // meaning no more keys are available.  This is the end of the
  // inorder traversal.
  //
  // Space complexity: O(1)
  // Time complexity:  O(lgN) on average
  //
  // Example usage:
  //    tree.begin();
  //    while (tree.next(key))
  //      cout << key << endl;
  //
  bool next(KeyT& key)
  {
    //
    // TODO
    //
	// use begin to point to smallest
	// Yoo = smallest value in node tree
	
	if (Yoo == NULL) {
		// empty tree
		//key = KeyT{ };
		return false;
	}
	key = Yoo->Key;
	
	if (Yoo->isThreaded == true){
			//key = Yoo->Key;
			Yoo = Yoo->Right;
			return true;
	}
	else {
		// its not threaded and has nodes
		Yoo = Yoo->Right; // go to the right once
		while (Yoo->Left != nullptr){
			Yoo = Yoo->Left;
		}
		//key = Yoo->Key;
		//return true;
		//return false;
	}
	return true;	

  }


void traversal(NODE* cur, std::ostream& output)const
{
// 	begin();

	if (cur == nullptr){
		return;
	}
	else{
		traversal(cur->Left, output);
		
		if (cur->isThreaded && cur->Right != nullptr){
			output << "(" << cur->Key << "," << cur->Value << "," << cur->Right->Key << ")" << endl;
		}
		else{
			output << "(" << cur->Key << "," << cur->Value << ")" << endl;                                 
			traversal(cur->Right, output);
		}
	}
}// end of traversal

  //
  // dump
  // 
  // Dumps the contents of the tree to the output stream, using a
  // recursive inorder traversal.
  //
  void dump(ostream& output) const
  {
    output << "**************************************************" << endl;
    output << "********************* BSTT ***********************" << endl;

    output << "** size: " << this->size() << endl;

    //
    // inorder traversal, with one output per line: either 
    // (key,value) or (key,value,THREAD)
    //
    // (key,value) if the node is not threaded OR thread==nullptr
    // (key,value,THREAD) if the node is threaded and THREAD denotes the next inorder key
    //

    //
    // TODO
    //

	traversal(Root, output);

    output << "**************************************************" << endl;
  }
	
};

