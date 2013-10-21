#include <stdio.h>
#include "mutex_ll.h"

#define DEFAULT_SIZE

typedef struct mutex_array_struct{
  Node* vals;
} *Mutex_arr;

typedef struct node_struct{
  struct node_struct *next;
  char* val;
} Node;

/* func: allocate the space and initalize a mutex_arr
 * ret: a pointer to this space;
 */
Mutex_arr mutex_arr_init(){
  //TODO: malloc struct 
  return ret;
}

/* param: mutex_arr to free
 * func: free mutex_arr's members and "body"
 */
void mutex_arr_free( Mutex_arr m){
  //TODO: add linked list free
  free(m);
}

/* param: Mutex_arr
 * func: grow Mutex_arr's vals and free old space;
 * ret: pointer to the char of storage
 */
int mutex_add_empty( Mutex_arr m){
  //TODO: errthang
}

/*
 *
 */
int get_next( Mutex_arr m){
  return mutex_add_empty( Mutex_arr m);
}
