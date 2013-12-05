#include <assert.h>
#include "bst.h"
#include <stdio.h>


// example of using assertions
void test_insert(BST_PTR t){

  int i, x;

  for(i=0; i<10; i++){
    x = 400 + rand() % 100;
    bst_insert(t, x);
    assert(bst_contains(t,x));
  }
}

void test_contains(BST_PTR t){
  bst_insert(t, 1234);

  assert(bst_contains(t, 1234));

}

int main(){
  int i;

  int a[] = {8, 2, 7, 9, 11, 3, 2, 6};
  //TEST IF sizeLeft sizeRight AUGMENT WORKS
  //int b[] = {1, 3, 5, 6, 7, 8, 10, 14, 15, 40, 60, 100, 120, 150};
  //BST_PTR test = bst_from_sorted_arr( b, 14);
  //bst_preorder( test);

  BST_PTR t = bst_create();

  for(i=0; i<8; i++)
    bst_insert(t, a[i]);
  
  //TEST
  bst_preorder(t);
 
  assert(bst_size(t) == 7);

  test_insert(t);

  test_contains(t);

  bst_inorder(t);

  bst_preorder(t);

  bst_postorder(t);

  //TEST
  int smallest = bst_ith_smallest( t, 2);
  printf("smallest is: %d\n", smallest);

  //TEST
  int numInRange = bst_num_in_range( t, 400, 500);
  printf("Improved NIR is: %d\n", numInRange);
  
  //TEST
  int size = 0;
  int *arr = bst_extract_range( t, 400, 500, &size);
  printf("SIZE: %d\n", size);
  for( i=0; i<size; ++i)
    printf("%d: %d\n", i, arr[i]);
  free( arr);

  bst_free(t);
}
