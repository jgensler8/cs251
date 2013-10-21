#include <stdio.h>
#include <stdlib.h>
#include "mutex_ll.h"

/*
 * STACK
 * LIFO
 */
typedef struct node_struct{
  struct node_struct *next;
  char val;
} Node;

typedef struct mutex_ll_struct{
  Node* head;
} *Mutex_ll;

/* func: allocate the space and initalize a mutex_ll
 * ret: a pointer to this space;
 */
Mutex_ll Mutex_ll_init(){
  Mutex_ll ret = malloc( sizeof(struct mutex_ll_struct));
  ret->head = NULL;
  return ret;
}

/* param: mutex_ll to free
 * func: free mutex_arr's members and "body"
 */
void Mutex_ll_free( Mutex_ll m){
  Node* hopper = m->head;
  Node* temp;
  while( hopper != NULL){
    temp = hopper;
    hopper = hopper->next;
    free(temp);
  }
  free(m);
}

/* param: Mutex_ll
 * func: append a Node to the ll; 
 * ret: pointer to the char of storage
 */
char* Mutex_ll_grow( Mutex_ll m){
  Node* add = malloc( sizeof( Node) );
  add->val = '\0';
  add->next = m->head;
  m->head = add;
  return &(add->val);
}

/* param: Mutex_ll
 * func: remove the first node of the list
 */
void Mutex_ll_remove( Mutex_ll m){
  Node* temp = m->head;
  if( m->head != NULL){
    m->head = m->head->next;
    free(temp);
  }
}
