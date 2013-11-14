#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "hmap.h"
#include "line.h"
#include "fdata.h"

//************* agg data + fxs **********************
typedef struct agg_data_struct{ //aggregate data
  HMAP_PTR hmap; //values of hash point to an Fdata
  int numFdata;
  Fdata* files; //array of files + depends
  int clock;
} *Agg_Data;

/* func: allocate a new Agg_data struct
 * ret: the new struct
 */
Agg_Data agg_data_init(){
  Agg_Data ad = malloc( sizeof( struct agg_data_struct) );
  ad->hmap = hmap_create(0. ,75);
  hmap_set_hfunc( ad->hmap, NAIVE_HFUNC);
  ad->files = NULL;
  ad->numFdata = 0;
  ad->clock = 1; //AS SPECIFIED IN THE HANDOUT
  return ad;
}

/* param: Agg_Data
 * func:  deallocate the memory associated with the struct
 */
void agg_data_free( Agg_Data AD){
  hmap_free( AD->hmap, 0);
  int i;
  for( i=0; i<AD->numFdata; ++i)
    fdata_free( (AD->files)[i] );
  free( AD->files);
  free( AD);
}

/* param: Agg_Data
 * param: Fdata, completed
 * func:  add the Fdata to the AggData list,
 *        AND to the hmap
 */
void agg_data_add_fdata( Agg_Data AD, Fdata fdata){
  int i, size = sizeof_fdata();
  Fdata* bigger = (Fdata*)malloc( (1+(AD->numFdata))*size );
  for( i=0; i<AD->numFdata; ++i)
    bigger[i] = (AD->files)[i];
  free(AD->files);
  AD->numFdata += 1;
  AD->files = bigger;
  bigger[ AD->numFdata - 1] = fdata;
  char* name = get_name( fdata);
  hmap_set( AD->hmap, name, fdata );
  free( name);
}

//************************ GEN FXS****************
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
  char *holder = NULL;
  holder = strchr( *line, target);
  if( holder != NULL) *holder = dest;
}

/* param: char*
 * func:  determine if the line is a target line vs a basic line
 * ret:
 *    0: line refers to basic file
 *    1: line refers to target line
 */
int is_target( char* line){
  return NULL == strchr( line, ':') ? 0 : 1;
}

/* param: char*, target line
 * param: Agg_Data
 * func:  check that target doesn't exist in the table
 * ret:
 *    0: target does already exists (bad!)
 *    1: target does not exist (good!)
 */
int is_unique( Agg_Data AD, char* name){
  return !hmap_contains( AD->hmap, name);
}

/* param: char*, line
 * func:  determine if the line has any alpha numeric chars on it
 * ret:
 *    0: file is not valid
 *    1: file is valid
 */
int is_valid_line( char* name){
  int i, flag = 0;
  for( i=0; i<strlen( name); ++i)
    if( isalnum(name[i]) ) ++flag;
  return flag > 0 ? 1 : 0;
}

/* param: Agg_Data
 * param: char*, target line
 * func:  store target and dependencies in HMAP,
 *        abort if target is already declared
 */
void evaluate_target( Agg_Data AD, char* start){
  start = strtok( start, " \t");
  char* name = start;
  Fdata fdata = fdata_init( name);
  //printf("target:%s:\n", start);
  if( is_unique( AD, start) ){
    start = strtok(NULL, " \t");
    start = strtok(NULL, " \t");
    while( NULL != start){
      //printf("depends:%s:\n", start);
      fdata_add_dep( fdata, start);
      start = strtok(NULL, " \t");
    }
  }
  else{
    fprintf(stderr,"ERROR, REDEFINITION OF TARGET!\n");
    abort();
  } 
  agg_data_add_fdata( AD, fdata);
}

/* param: HMAP_PTR
 * param: char*, basic line
 * func:  store the basic file in the hmap
 */
void evaluate_basic( Agg_Data AD, char* start){
  Fdata fdata = fdata_init( start); 
  start = strtok( start, " \t");
  while( start == NULL)
    start = strtok( NULL, " \t");
  //printf("basic:%s:\n", start);
  if( is_unique( AD, start)) agg_data_add_fdata( AD, fdata);
  else{
    fprintf(stderr, "ERROR, BASIC FILE NAME EXISTS");
    abort();
  }
}

/* param: Agg_Data
 * param: char*, fakemake file
 * func:  read the fake make file,
 *        store every file read in the table,
 *        abort if file is already entered
 */
void gen_parse_file( Agg_Data AD, char* fileName){
  FILE* file = fopen( fileName, "r");
  Line l = line_init( file);

  line_read_line( l);
  char* chunk = get_line( l);
  while( chunk != NULL){ //reading lines from fakefile
    gen_swap_first( &chunk, '\n', '\0');
    if( is_valid_line( chunk) ){
      if( is_target(chunk) ) evaluate_target( AD, chunk);
      else evaluate_basic( AD, chunk);
    }
    free(chunk);
    if( !line_read_line( l) ) break; //EOF
    chunk = get_line( l);
  }

  line_free( l);
  fclose( file);
}

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

/****************** CMD FXS *******************/
/* param: Agg_Data
 * func:  determine if the map contains any cycles (namely back edges)
 * ret:
 *    0: not valid, contains cycles
 *    1: valid
 */
int is_valid_map( Agg_Data AD){
  //DFS with back edge
  //uses flags
}

/*
 *
 */
char* cmd_parse_line( char** line){
  char* chunk = strtok( *line, "\t ");
  chunk = strtok(NULL, "\t ");
  printf("chunk3:%s:\n", chunk);
  //chunk = strtok(NULL, "\t "); //skip over the command
  //printf("chunk3:%s:\n", chunk);
  //while( chunk == NULL) chunk = strtok( NULL, "\t ");
  gen_swap_first( &chunk, '\n', '\0');
  printf("chunk3:%s:\n", chunk);
  return chunk;
}

/* param: Agg_Data
 * func:  print the time of the clock
 */
void cmd_time( Agg_Data AD){
  printf("CLOCK: %d\n", AD->clock);
}

/* param: line, to parse (contains both command and target file)
 * param: Agg_Data
 * func:  update the timestamp of the specified file
 */
void cmd_touch( char* line, Agg_Data AD){
  char* chunk = cmd_parse_line( &line);
  if( hmap_contains( AD->hmap, chunk) ){
    set_time_stamp( (Fdata)hmap_get(AD->hmap, chunk), AD->clock);
    ++AD->clock;
  }
  else printf("Sorry, that file doesn't exist1\n");
}

/* param: line, to parse
 * param: Agg_Data
 * func:  print the timestamp of the specified file
 */
int cmd_timestamp( char* line, Agg_Data AD){
  char* chunk = cmd_parse_line( &line);
  if( hmap_contains( AD->hmap, chunk) ){
    return get_time_stamp( (Fdata)hmap_get( AD->hmap, chunk) );
  }
  else printf("Sorry, that file doesn't exist1\n");
  return -1;
}

/* param: line, to parse
 * param: Agg_Data
 * func:  print all of the time stamps of all files
 */
void cmd_timestamps( Agg_Data AD){
  printf("TIMESTAMPS:\n");
  int i;
  for( i=0; i<AD->numFdata; ++i){
    char* name = get_name( (AD->files)[i] );
    printf("%s: %d\n", name, get_time_stamp( (AD->files)[i] ) );
    free(name);
  }
  printf("\n");
}

/* param: line, to parse
 * param: Agg_Data
 * func:  simulate a build with a specified source vertex,
 *        only update timestamps if needed
 *        (if the build requires no updating, nothing will change)
 */
void cmd_make( char* line, Agg_Data AD){
  //probs want to use helper
  //recurse through the files, updating when needed (++clock)
}

/************** main **********************/ 
int main(int argc, char** argv){
  char* fileName = NULL, command;
  int quit = 1, temp, numRead;
  Agg_Data aggdata = agg_data_init();

  gen_parse_args( argc, argv, &fileName);
  if( fileName == NULL) fprintf( stderr, "NO VALID FILE CLARG SUPPLIED\n");
  else{
    gen_parse_file( aggdata, fileName);
    if( !is_valid_map(aggdata) ){
      printf("ERROR BUILDING\n");
      abort();
    }
    else{
      Line line = line_init( stdin);
      while(quit){
        printf("Command: ");
        line_read_line( line);
        char* input = get_line( line);
        if( strstr(input, "quit") ) quit = 0;
        else if( strstr(input, "timestamps") ) cmd_timestamps( aggdata);
        else if( strstr(input, "timestamp") ) cmd_timestamp( input, aggdata);
        else if( strstr(input, "time") ) cmd_time( aggdata);
        else if( strstr(input, "touch") ) cmd_touch( input, aggdata);
        else if( strstr(input, "make") ) cmd_make( input, aggdata);
        free( input);
      }
      line_free( line);
    } 
  }
  agg_data_free( aggdata);
  return 0;
}

