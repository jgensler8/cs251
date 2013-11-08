#include <stdlib.h>
#include <stdio.h>

#include "graph.h"
#include "line.h"
#include "sfile.c" //TODO

void print_arr( int* array, int n, int from);
void graph_dfs_display( Graph g, int from, int* (*)(Graph, int),
    void (*)(int*, int, int) );
void gen_parse_args( int argc, char** argv, char** file_name);
// int gen_get_time( );
// int gen_get_timestamp( );
// void touch( char* );
// void make( char* );

/************** main **********************/ 
int main(int argc, char** argv){
  char* fileName = NULL;
  int quit = 1, temp;
  HMAP_PTR hmap = hmap_create(0, .75);
  hmap_set_hfunc( hmap, NAIVE_HFUNC);

  gen_parse_args( argc, argv, &fileName);
  if( fileName == NULL) fprintf( stderr, "NO VALID FILE CLARG SUPPLIED\n");
  else{
    gen_parse_file( hmap, fileName);
    //if( !attempt build() )
    //  errr
    // else 
        // wait for commands
  }
  return 0;
}

/* param: int, argc
 * param: char**, argv
 * param: char**, the address of a char*
 * func:  tries to open all of the command line args. the last "openable"
 *        file is assigned to the passed file_name param
 */
void gen_parse_args( int argc, char** argv, char** fileName){
  int i; FILE *test;
  for( i=1; i < argc; ++i){
    test = fopen( argv[i], "r");
    if( test != NULL){
      *fileName = argv[i];
      fclose( test);
    }
  }
}

/*
 *
 */
char** gen_parse_file( HMAP_PTR hmap, char* fileName){
  FILE* file = fopen( fileName, "r");
  Line l = line_init( file);
  sFile unseen = sfile_init();
  char* chunk = line_read_line( l);
  while( chunk != EOF){
    if( line_is_target(chunk) ) evaluate_target( hmap, chunk);
    else( evaluate_basic( hmap, chunk)

  }

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
 // int size = graph_get_size( g);
 // print_arr_fx( array, size, from);
  free( array);
}


