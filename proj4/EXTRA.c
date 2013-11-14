
/*************** QUEUE FXS ********************/

typedef struct node_struct{
  struct node_struct *next;
  int val;
} Node;

typedef struct queue_struct{
  Node* front;  // [front] <-- [..node..] <-- [back]
  Node* back;
} *Queue;

/* param: Int, size to make int array
 * func: initialize a queue
 * ret: initialized Queue
 */
Queue queue_init( int n){
  Queue ret = malloc( sizeof( struct queue_struct) );
  ret->front = NULL;
  ret->back = NULL;
  return ret;
}


/* param: Queue 
 * func:  deallocate memory associated with the Queue
 */
void queue_free( Queue q){
  Node *hopper = q->back, *toremove;
  while( hopper != NULL){
    toremove = hopper;
    hopper = hopper->next;
    free( toremove);
  }
  free( q);
}

/* param: Queue
 * func:  check if queue is empty
 * ret:
 *    0: the Queue is not empty
 *    1: the Queue is empty
 */
int queue_is_empty( Queue q){
  return q->back == NULL ? 1 : 0;
}

/* param: Queue
 * func:  print the contents of the queue
 */
void queue_print( Queue q){
  fprintf( stdout, "HERE IS THE QUEUE:\n");
  Node* hopper = q->back;
  while( hopper != NULL){
    fprintf( stdout, ":%d:\n", hopper->val);
    hopper = hopper->next;
  }
  fprintf( stdout, "\n");
}

/* param: Queue, to push to
 * param: int, val to push
 * func: place the val at the end of the queue
 */
void queue_enqueue( Queue q, int val){
  Node* temp = (Node*)malloc( sizeof( Node));
  temp->val = val;
  temp->next = NULL;
  if( queue_is_empty( q) ){
    q->front = temp;
    q->back = temp;
  }
  else{
    temp->next = q->back;
    q->back = temp;
  }
}

/* param: Queue, to push to
 * func: pop the first val from the front
 *       illustration for else{}
 *       hopper V      V front
 *              [] --> [] --> NULL
 * ret:
 *    -1: invalid value
 *    otherwise: poppped integet (always posistive)
 */
int queue_dequeue( Queue q){
  if( queue_is_empty( q) ) return -1;
  else{ // >= 1 node (not empty)
    int ret = q->front->val;
    if( q->back->next == NULL){ //one node
      free( q->front);
      q->front = NULL;
      q->back = NULL;
    }
    else{ // > 1 node
      Node* hopper = q->back, *realFront;
      while( hopper->next->next != NULL) hopper = hopper->next;
      realFront = hopper->next;
      free( realFront);
      q->front = hopper;
      hopper->next = NULL;
    }
    return ret;
  }
}
