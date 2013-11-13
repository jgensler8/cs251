#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hmap.h"
#include "graph.h"
#include "line.h"
#include "fdata.h"

void print_arr( int* array, int n, int from);
void graph_dfs_display( Graph g, int from, int* (*)(Graph, int),
    void (*)(int*, int, int) );
void gen_parse_args( int argc, char** argv, char** file_name);
void gen_parse_file( HMAP_PTR hmap, char* fileName);
void gen_swap_first( char**, char, char);
int is_target( char* line);
void evaluate_target( HMAP_PTR hmap, char* start);
void evaluate_basic( HMAP_PTR hmap, char* line);
int is_valid_target( char* name, HMAP_PTR hmap);
int is_valid_line( char* name);
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
    //  printf("ERROR BUILDING\n");
    //  abort();
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

/* param: char&* by reference
 * param: char* target
 * param: char* replace with
 * func: replace the first instance of the target with the replacement
 */
void gen_swap_first( char** line, char target, char dest){
  char *holder;
  holder = strchr( *line, target);
  *holder = dest;
}

/* HMAP_PTR, to record the files to be used
 * char*, fakemake file
 * func:  read the fake make file,
 *        store every file read in the table,
 *        abort if file is already entered
 */
void gen_parse_file( HMAP_PTR hmap, char* fileName){
  FILE* file = fopen( fileName, "r");
  Line l = line_init( file);

  line_read_line( l);
  char* chunk = get_line( l);
  while( chunk != NULL){ //reading lines from fakefile
    gen_swap_first( &chunk, '\n', '\0');
    if( is_valid_line( chunk) ){
      if( is_target(chunk) ) evaluate_target( hmap, chunk);
      else evaluate_basic( hmap, chunk);
    }
    free(chunk);
    if( !line_read_line( l) ) break; //EOF
    chunk = get_line( l);
  }

}

/* param: char*
 * func:  determine if the line is a target line vs a basic line
 * ret:
 *    0: line refers to basic file
 *    1: line refers to target line
 */
int is_target( char* line){
  return NULL == strchr( line, (int)':') ? 0 : 1;
}

/* param: HMAP_PTR
 * param: char*, target line
 * func:  store target and dependencies in HMAP,
 *        abort if target is cyclic TODO
 */
void evaluate_target( HMAP_PTR hmap, char* start){
  start = strtok( start, " \t");
  printf("target:%s:\n", start);
  if( is_valid_line( start) ){
    Fdata f = fdata_init( start);
    start = strtok(NULL, " \t");
    start = strtok(NULL, " \t");
    while( NULL != start){ //XXX XXX XXX XXX XXX
      printf("depends:%s:\n", start);
      start = strtok(NULL, " \t");
    }
  }
  else{
    printf("invalid target name!\n");
    abort();
  }
}

/* param: HMAP_PTR
 * param: char*, basic line
 * func:  store the basic file in the hmap
 */
void evaluate_basic( HMAP_PTR hmap, char* start){
  //tokenize and pull first non empty chunk
  start = strtok( start, " \t");
  while( start == NULL)
    start = strtok( NULL, " \t");
  printf("BASIC:%s:\n", start);
}

/* param: char*, target line
 * param: HMAP_PTR
 * func:  check that target doesn't exist in the table
 * ret:
 *    0: target does already exists (bad!)
 *    1: target does not exist (good!)
 */
int is_valid_target( char* name, HMAP_PTR hmap){
  return !hmap_contains( hmap, name);
}

/*
 *
 */
int is_valid_line( char* name){
  int i, flag = 0;
  for( i=0; i<strlen( name); ++i)
    if( isalnum(name[i]) ) ++flag;
  return flag > 0 ? 1 : 0;
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


