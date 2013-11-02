#include <stdlib.h>
#include <stdio.h>

#include "graph.h"

void print_arr( int* array, int n, int from);
void graph_dfs_display( Graph g, int from, int* (*)(Graph, int),
    void (*)(int*, int, int) );
 
int main(){
  Graph g = graph_init(10);

  graph_add_edge( g, 1, 2);
  graph_add_edge( g, 1, 3);
  graph_add_edge( g, 2, 3);
  graph_add_edge( g, 3, 2);
  graph_add_edge( g, 2, 5);
  graph_print( g);

  graph_dfs_display( g, 1, graph_dfs, print_arr);

  graph_free( g);
  return 0;
}

void print_arr( int* array, int n, int from){
  int i;
  for( i=0; i < n; ++i){
    fprintf( stdout, "%d-->%d\t== %d\n", from, i, array[i]);
  }
}

void graph_dfs_display( Graph g, int from, int* (*dfs_fx)(Graph, int),
    void (*print_arr_fx)(int*, int, int) ){
  int* array = dfs_fx( g, from);
  int size = graph_get_size( g);
  print_arr_fx( array, size, from);
  free( array);
}
