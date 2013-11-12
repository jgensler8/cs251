#include <stdlib.h>
#include <stdio.h>

#include "hmap.h"
#include "graph.h"
#include "line.h"
#include "fdata.h"

void print_arr( int* array, int n, int from);
void graph_dfs_display( Graph g, int from, int* (*)(Graph, int),
    void (*)(int*, int, int) );
void gen_parse_args( int argc, char** argv, char** file_name);
char** gen_parse_file( HMAP_PTR hmap, char* fileName);
int is_target( char* line);
void evaluate_target( HMAP_PTR hmap, char* start);
void evaluate_basic( HMAP_PTR hmap, char* line);
int is_valid_target( char* name, HMAP_PTR hmap);
int is_valid_name( char* name);
// int gen_get_time( );
// int gen_get_timestamp( );
// void touch( char* );
// void make(44 char* );

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

/*
 *
 */
char** gen_parse_file( HMAP_PTR hmap, char* fileName){
  FILE* file = fopen( fileName, "r");
  Line l = line_init( file);

  line_read_line( l);
  char* chunk = get_line( l);
  while( chunk != NULL){ //reading lines from fakefile
    printf("line:%s:\n", chunk);
    if( is_target(chunk) ) evaluate_target( hmap, chunk);
    else evaluate_basic( hmap, chunk);
    free(chunk);
    chunk = get_line( l);
  }

}

/*
 *
 */
int is_target( char* line){
  return NULL == strchr( line, (int)':') ? 0 : 1;
}

/*
 *
 */
void evaluate_target( HMAP_PTR hmap, char* start){
  char *end;
  start = strtok( start, "\n \t");
  printf("target:%s:\n", start);
  scanf("%c",start);
  if( is_valid_name( start) ){
    Fdata f = fdata_init( start);
    start = strtok(NULL, "\n \t");
    while( NULL != start){
      //if(
      printf("depends:%s:\n", start);
      start = strtok(NULL, "\n \t");
    }
  }
  else{
    printf("invalid target name!\n");
    abort();
  }
}

/*
 *
 */
void evaluate_basic( HMAP_PTR hmap, char* line){
  if( is_valid_name( line) ){
    //printf("valid file name");
  }
  else{
    //printf("invalid file name\n");
  }
}

/*
 *
 */
int is_valid_target( char* name, HMAP_PTR hmap){
  //return is_valid_name( name) && !hmap_conatains( hmap, name);
  return 1;
}

/*
 *
 */
int is_valid_name( char* name){
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


