
/*** BINARY SEARCH TREE MODULE FOR STORING SETS OF
     INTEGERS ****/

// actual struct hidden
typedef struct bst *BST_PTR;

extern BST_PTR bst_create();

extern void bst_free(BST_PTR t);

/** TODO:  change code so insert does not use
      recursion.  Helper stub insert_i already
      in place **/
extern void bst_insert(BST_PTR t, int x);

// constructs a balanced binary search tree from
//   the data in a[0..n-1] under the assumption that
//   it is already sorted.
extern BST_PTR bst_from_sorted_arr(int *a, int n);

// returns 0/1 (true/false) depending on whether x appears
//   in the tree
extern int bst_contains(BST_PTR t, int x);


// returns 0/1 (true/false) depending on whether x appeared
//   in the tree (resulting in a successfuly removal0
extern int bst_remove(BST_PTR t, int x);

/** TODO:  change data structure so bst_size is
  done in O(1) time **/
extern int bst_size(BST_PTR t);


// Note:  height of an empty tree is -1
extern int bst_height(BST_PTR t);

// return value undefined if t is empty (callers fault!)
extern int bst_min(BST_PTR t);

// return value undefined if t is empty (callers fault!)
extern int bst_max(BST_PTR t);


// printing functions
extern void bst_inorder(BST_PTR t);

extern void bst_preorder(BST_PTR t);

/** TODO:  complete a post-order implementation with
     appropriate indentation so that the tree structure
     can be inferred
**/
extern void bst_postorder(BST_PTR t);



// TODO:  bst_ith_smallest
// i ranges from 1..n where n is the number of elements
//   in the tree.  If i is outside this range, the return
//   value is undefined (callers fault!).  You can print
//   an error message to stderr in this case if you want.
// Runtime:  O(h) where h is the tree height
extern int bst_ith_smallest(BST_PTR t, int i);

// TODO:  bst_num_in_range
// reports the number of elements in the tree between
//   low and hi inclusive.
// low and hi are NOT indices -- the are actual values that
//   might be stored -- e.g., how many entries are there
//   between 44 and 109?
// Runtime:  O(h) where h is the tree height
extern int bst_num_in_range(BST_PTR t, int low, int hi);

// TODO:  bst_extract_range
// allocates and populates an array with all elements in the
//   tree between low and hi inclusive.  The array is returned.
// low and hi are NOT indices -- the are actual values that
//   might be stored -- e.g., how many entries are there
//   between 44 and 109?
// Runtime:  O(h+m) where h is the tree height and m is
//   the number of elements in the range.
// The number of elements (and the size of the allocated 
//   array is communicated via *num).
extern int * bst_extract_range(BST_PTR t, int low, int hi, int *num);


/**** TODO - modify all relevant functions so that the tree
        maintains the SIZE-BALANCED property with a minimum
        amount of additional incremental work.


        Remember:

            - Any node x has two subtrees; let MIN be the number of
		nodes in the smaller subtree and MAX be the 
		number of nodes in the larger.

		x is size-balanced if and only if MAX <= 2*MIN +1

		This allows about a 1/3 - 2/3 degree of imbalance.

	    - A BST is size-balanced if and only if all of its nodes
		are size-balanced.

	    - An insertion of deletion may result in a violation of 
		the size-balanced property.  Furthermore, there
		may be multiple violating nodes in the tree.

		When this happens you re-balance the violating node
		CLOSEST TO THE ROOT.  When you rebalance a subtree, 
		you make it as balanced as possible.  This operation
		should take time proportional to the size of the
		subtree.  But notice that after the rebalancing, 
		it will take many operations before that node may have
		to be rebalanced again -- this is the trick and will
		yield log n ammortized runtime.

		Hint:  when rebalancing a subtree, consider doing it
		in multiple steps:  extract the elements of the subtree into
		an array; build a perfectly balanced tree on them; 
		remove the existing subtree; replace it with the one
		you just built.  (There are some variants on this where
		you may avoid freeing and re-allocating nodes, but they are
		fundamentally the same).

***/
