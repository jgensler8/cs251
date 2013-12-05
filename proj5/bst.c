#include "bst.h"
#include <stdio.h>
#include <stdlib.h>

struct bst_node {
    int val;
    struct bst_node *left;
    struct bst_node *right;
    int sizeLeft;
    int sizeRight;
};
typedef struct bst_node NODE;


struct bst {
    NODE *root;
};

BST_PTR bst_create(){
  BST_PTR t = malloc(sizeof(struct bst));
  t->root = NULL;
  return t;
}

// a nlog n?
// b. n  
// c n^2
static NODE * from_arr(int *a, int n){
    int m;
    NODE *p;
    if(n==0)
        return NULL;
    m=n/2;
    p = malloc(sizeof(NODE));

    //INSERT BEGIN
    p->sizeLeft = m;
    p->sizeRight = n-(m+1);
    //INSET END
    p->val = a[m];
    p->left = from_arr(a, m);
    p->right = from_arr(&(a[m+1]), n-(m+1));
    return p;
}

BST_PTR bst_from_sorted_arr(int *a, int n){
  BST_PTR t = bst_create();
  t->root = from_arr(a, n);
  return t;
}

static void free_r(NODE *r){
    if(r==NULL) return;
    free_r(r->left);
    free_r(r->right);
    free(r);
}
void bst_free(BST_PTR t){
    free_r(t->root);
    free(t);
}

static NODE * insert(NODE *r, int x){
    NODE *leaf;
    if(r == NULL){
      leaf = malloc(sizeof(NODE));
      leaf->left = NULL;
      leaf->right = NULL;
      leaf->val = x;
      return leaf;
    }

    if(r->val == x)
        return r;
    if(x < r->val){
        ++r->sizeLeft;
        r->left = insert(r->left, x);
        return r;
    }
    else {
        ++r->sizeRight;
        r->right = insert(r->right, x);
        return r;
    }
}

// TODO
// how about an iterative version?
static NODE *insert_i(NODE *r, int x){
  //INSERT BEGIN
  // **** THIS FUNCITON ASSUMES THE INPUT DOESNT EXIST ****
  NODE *leaf;
  leaf = malloc(sizeof(NODE));
  leaf->left = NULL;
  leaf->right = NULL;
  leaf->val = x;
  leaf->sizeLeft = 0;
  leaf->sizeRight = 0;

  if( r == NULL) return leaf; //the first node added
  else{ // node 2 and above
    NODE *temp = r;
    while( temp != NULL){ //will always be true but here for safety
      if( x < temp->val ){ //left tree
        ++temp->sizeLeft;
        if( temp->left != NULL) temp = temp->left; //traverse left
        else{
          temp->left = leaf;
          break;
        }
      }
      else{ //right tree
        ++temp->sizeRight;
        if( temp->right != NULL) temp = temp->right; //traverse right
        else{
          temp->right = leaf;
          break;
        }
      }
    }
  }
  return r;
  //INSERT END
  //return NULL;  
}


void bst_insert(BST_PTR t, int x){
  //INSERT BEGIN
  if( !bst_contains( t, x) ){

    // TODO:  uncomment the line below if you 
    //   implemented iterative insertion (above)
    //   (and of course delete the call to insert()
    //   above.
    //t->root = insert(t->root, x); //OPTION 1 (recursive)
    t->root = insert_i(t->root, x); //OPTION 2 (iterative)
  }
  //INSERT END
}

int bst_contains(BST_PTR t, int x){
    NODE *p = t->root;

    while(p != NULL){

        if(p->val == x)
            return 1;
        if(x < p->val){
            p = p->left;
        }
        else
            p = p->right;
    }
    return 0;  
}

// helper functions to find min/max elements in
//   tree rooted at r
static int min_h(NODE *r){
  if(r==NULL)
    return -1; // should never happen!
  while(r->left != NULL)
      r = r->left;
  return r->val;
}

static int max_h(NODE *r){
  if(r==NULL)
    return -1; // should never happen!
  while(r->right != NULL)
      r = r->right;
  return r->val;
}

// recursive helper function to remove x
// returns root of tree after deletion.
// *success is used to communicate if the deletion
//    actually happened or not.
static NODE *remove_r(NODE *r, int x, int *success){
NODE   *tmp;
int sanity;

  if(r==NULL){
    *success = 0;  // no match
    return NULL;
  }
  if(r->val == x){
    *success = 1;

    if(r->left == NULL){
        tmp = r->right;
        free(r);
        return tmp;
    }
    if(r->right == NULL){
        tmp = r->left;
        free(r);
        return tmp;
    }
    // if we get here, r has two children
    r->val = min_h(r->right);
    r->right = remove_r(r->right, r->val, &sanity);
    if(!sanity)
        fprintf(stderr, "ERROR:  remove() failed to delete promoted value?\n");
    return r;
  }
  if(x < r->val){
    r->left = remove_r(r->left, x, success);
  }
  else {
    r->right = remove_r(r->right, x, success);
  }
  return r;  // root unchanged

}


int bst_remove(BST_PTR t, int x){
    int success;
    t->root = remove_r(t->root, x, &success);
    return success;
}


static int size(NODE *r){
  //INSERT BEGIN
  if( r == NULL) return 0; // zero nodes added
  else return (r->sizeLeft + r->sizeRight + 1);
  //INSERT END

    //if(r==NULL) return 0;
    //return size(r->left) + size(r->right) + 1;
}

/** TODO:  modify / augment data structure so bst_size
    takes only O(1) time.
**/
int bst_size(BST_PTR t){
    return size(t->root);
}

static int height(NODE *r){
    int l_h, r_h;

    if(r==NULL) return -1;
    l_h = height(r->left);
    r_h = height(r->right);
    return 1 + (l_h > r_h ? l_h : r_h);

}
int bst_height(BST_PTR t){
    return height(t->root);

}

int bst_min(BST_PTR t){
    return min_h(t->root);
}

int bst_max(BST_PTR t){
    return max_h(t->root);
}

static void indent(int m){
    int i;
    for(i=0; i<m; i++)
        printf("-");
}

static void inorder(NODE *r){
  if(r==NULL) return;
  inorder(r->left);
  printf("[%d]\n", r->val);
  inorder(r->right);

}
void bst_inorder(BST_PTR t){

  printf("========BEGIN INORDER============\n");
  inorder(t->root);
  printf("=========END INORDER============\n");

}

static void preorder(NODE *r, int margin){
  indent(margin);
  if(r==NULL)
    printf(" NULL \n");
  else {
    //BEGIN INSERT
    printf("%d", r->val);
    //printf("\t\tLS:%d RS:%d", r->sizeLeft, r->sizeRight);
    printf("\n");
    //END INSERT
    preorder(r->left, margin + 3);
    preorder(r->right, margin+3);
  }
}

void bst_preorder(BST_PTR t){

  printf("========BEGIN PREORDER============\n");
  preorder(t->root, 0);
  printf("=========END PREORDER============\n");

}

static void postorder(NODE *r, int margin){
  //BEGIN INSERT
  if(r==NULL){
    indent(margin);
    printf("NULL \n");
  }
  else {
    postorder(r->left, margin + 3);
    postorder(r->right, margin+3);
    indent(margin);
    printf("%d", r->val);
    printf("\t\tLS:%d RS:%d", r->sizeLeft, r->sizeRight);
    printf("\n");
  }
  //END INSERT
}

// indentation is proportional to depth of node being printed
//   depth is #hops from root.
void bst_postorder(BST_PTR t){

  printf("========BEGIN POSTORDER============\n");
  postorder(t->root, 0);
  printf("=========END POSTORDER============\n");

}


// ***************** FUNCTION INSERT BEGIN *****************
int bst_i_r( NODE *r, int i){
  //DEBUG
  //printf("val ls rs %d %d %d\n",r->val, r->sizeLeft, r->sizeRight);
  if( r == NULL){
    fprintf( stderr, "ERROR DELETING SMALLEST ELEMENT\n");
    return -1;
  }
  else{
    if( r->sizeLeft == i ) return r->left->val; //found it
    else if( i < r->sizeLeft ) //in the left tree
      return bst_i_r( r->left, i);
    else //in the right tree
      return bst_i_r( r->right, i-r->sizeLeft);
  }
}
int bst_ith_smallest( BST_PTR t, int i){
  if( t->root == NULL || i > (1 + t->root->sizeLeft + t->root->sizeRight)){ 
    fprintf( stderr, "TREE EMPTY OR iTH NUMBER TOO LARGE\n");
    return -1;
  }
  else
    return bst_i_r( t->root, i);
}
// END ith SMALLEST

int bst_num_in_range_r( NODE* r, int low, int hi){
  if( r == NULL) return 0;
  else if( r->val <= hi && r->val >= low){ //if in range
    return 1 + bst_num_in_range_r( r->right, low, hi)
      + bst_num_in_range_r( r->left, low, hi);
  }
  else if( r->val < low){ //left boundary
    return bst_num_in_range_r( r->right, low, hi);
  }
  else if( r->val > hi){ //right boundary
    return bst_num_in_range_r( r->left, low, hi);
  }
}
// parent value
int bst_num_in_range_r2( NODE* r, int low, int hi, int paVal){
  if( r == NULL) return 0;
  else if( r->val < low){ //leftmost tree will never get expanded
    return bst_num_in_range_r2( r->right, low, hi, r->val);
  }
  else if(r->val > hi){ //rightmost tree will never get expanded
    return bst_num_in_range_r2( r->left, low, hi, r->val);
  }
  else{ // I am in the range
    if( paVal <= hi && paVal >= low){ //my parent is in the rage
      //don't need to traverse area between parent & me
      if( r->val > paVal){ 
        return 1 + r->sizeLeft
          + bst_num_in_range_r2( r->right, low, hi, r->val);
      }
      else{ // im < parent
        return 1 + r->sizeRight
          + bst_num_in_range_r2( r->left, low, hi, r->val);
      }
    }
    else{ //my parent is not in the range
      //TODO CAN THIS BE OPTIMIZED?
      return 1 + bst_num_in_range_r2( r->left, low, hi, r->val)
          + bst_num_in_range_r2( r->right, low, hi, r->val);
    }
  }
}

int bst_num_in_range( BST_PTR t, int low, int hi){
  /*
  if( t->root != NULL){
    int realTest =  bst_num_in_range_r( t->root, low, hi);
    printf("Long(er) NIR: %d\n", realTest);
  }
  */

  if( t->root == NULL) return -1;
  else if( t->root->val >= low && t->root->val <= hi)
    return 1 + bst_num_in_range_r2( t->root->left, low, hi, t->root->val)
      + bst_num_in_range_r2( t->root->right, low, hi, t->root->val);
  else
    return bst_num_in_range_r2( t->root->left, low, hi, t->root->val)
      + bst_num_in_range_r2( t->root->right, low, hi, t->root->val);
  
}
//END num in range

void bst_extract_range_r( NODE* r, int low, int hi, int *num, int** arr){
  if( r == NULL) return;
  else if( r->val <= hi && r->val >= low){ //if in range
    (*arr)[ *num] = r->val;
    ++(*num);
    bst_extract_range_r( r->right, low, hi, num, arr);
    bst_extract_range_r( r->left, low, hi, num, arr);
  }
  else if( r->val < low){ //left boundary
    bst_extract_range_r( r->right, low, hi, num, arr);
  }
  else if( r->val > hi){ //right boundary
    bst_extract_range_r( r->left, low, hi, num, arr);
  }
}
int *bst_extract_range( BST_PTR t, int low, int hi, int *num){
  if( t->root == NULL){
    fprintf(stderr, "ERROR, EMPTY TREE\n");
    return NULL;
  }
  else{
    int elems = bst_num_in_range( t, low, hi);
    int *array = (int*)malloc( elems*sizeof(int));
    //printf("NUM %d\n", *num); //DEBUG
    bst_extract_range_r(t->root, low, hi, num, &array);
    //printf("NUM %d\n", *num); //DEBUG
    return array;
  }
}
//END extract range





