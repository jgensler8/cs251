#include <stdlib.h>
#include <stdio.h>
#include "graph.h"
#define PRINT_STYLE " %d,"

typedef struct node_struct{
  struct node_struct *next;
  int val;
} Node;

typedef struct queue_struct{
  Node* front;  // [front] <-- [..node..] <-- [back]
  Node* back;
} *Queue;

/* linked list implementation
 * adj[0] --> [Node] --> ..
 * adj[1] --> .. etc .. 
 * adj[2] --> ..
 * ...
 * adj[maxSize-1] --> ..
 */
struct graph_struct{
  Node** adjList;
  int maxSize;
};

/*
 *************** GRAPH FXS ****************************
 */

/* param: int
 * func:  initialize the graph struct with an array of param's size
 * ret:   initialized graph struct
 */
Graph graph_init( int n){
  Graph ret = malloc( sizeof( struct graph_struct) );
  ret->maxSize = n;
  ret->adjList = (Node**)malloc( n*sizeof( Node*) );
  int i;
  for( i=0; i < n; ++i)
    (ret->adjList)[i] = NULL;
  return ret;
}

/* param: Graph
 * func:  deallocated the Graph
 */
void graph_free( Graph g){
  //free the adjList
  int i;
  Node *hopper, *toRemove;
  for( i = 0; i < g->maxSize; ++i){
    hopper = (g->adjList)[i]; //free row
    while( hopper != NULL){
      toRemove = hopper;
      hopper = hopper->next;
      free( toRemove);
    }
  }
  free( g->adjList);
  free( g);
}

/* param: Graph
 param: int, new size
 * func:  resize the Graph to hold int's worth of data
 */
void graph_grow( Graph g, int newSize){
  Node** temp = (Node**)malloc( newSize*sizeof( Node*) );
  int i;
  for( i=0; i < g->maxSize; ++i)
    temp[i] = (g->adjList)[i];
  for( i=g->maxSize; i < newSize; ++i)
    temp[i] = NULL;
  free(g->adjList);
  g->adjList = temp;
  g->maxSize = newSize;
}

/* param: Graph
 * param: int, (non negative) edge start
 * param: int, (non negative) edge end
 * func: validate that the edge is not in the graph
 * ret:
 *    0: the edge does not exist
 *    1: the edge exists
 */
int graph_edge_exists( Graph g, int from, int to){
  Node* hopper = (g->adjList)[from];
  while( hopper != NULL){
    if( hopper->val == to) return 1;
    hopper = hopper->next;
  }
  return 0;
}

/* param: Graph
 * param: int, (non negative) edge start
 * param: int, (non negative) edge end
 * func:  try and add edge to graph
 *        if needed, grow the graph
 */
void graph_add_edge( Graph g, int from, int to){
  if( to > from && to >= g->maxSize) graph_grow( g, to+1);
  else if( from > to && from >= g->maxSize) graph_grow( g, from+1);
  else if( to > g->maxSize) graph_grow( g, to+1);
  if( graph_edge_exists( g, from, to) )
    fprintf( stderr, "ERROR, edge exists\n");
  else{
    Node* temp = (Node*)malloc(sizeof( Node) );
    temp->val = to;
    temp->next = NULL;
    if( (g->adjList)[from] != NULL) temp->next = ( (g->adjList)[from] );
    (g->adjList)[from] = temp;
  }
}

/* param: Graph
 * func:  Crude representation of adj matrix
 */
void graph_print( Graph g){
  int i;
  Node* hopper;
  for( i=0; i < g->maxSize; ++i){
    hopper = (g->adjList)[i];
    printf("%d:", i);
    while( hopper != NULL){
      fprintf( stdout, PRINT_STYLE, hopper->val);
      hopper = hopper->next;
    }
    fprintf( stdout, "\n");
  }
}

/*
 *************** QUEUE FXS ****************************
 */

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

/* param: Graph
 * param: Int, start
 * func:  classic depth first search
 * ret:   pointer to an integer array
 *        the size is the maxSize of Graph g
 */
int* graph_dfs( Graph g, int start){
  Queue q = queue_init( g->maxSize);
  int* distance = (int*)malloc( (g->maxSize)*sizeof(int));
  int i;
  for( i=0; i < g->maxSize; ++i)
    distance[i] = -1;
  Node* neighbors;

  queue_enqueue( q, start);
  distance[ start] = 0;
  while( !queue_is_empty( q) ){
    i = queue_dequeue( q);
    neighbors = (g->adjList)[i];
    while( neighbors != NULL){
      if( distance[neighbors->val] == -1){
        queue_enqueue( q, neighbors->val);
        distance[neighbors->val] = distance[i] + 1;
      }
      neighbors = neighbors->next;
    }
  }

  queue_free( q);
  return distance;  
}

/* param: Graph
 * ret:   Graph's maxSize
 */
int graph_get_size( Graph g){
  return g->maxSize;
}
