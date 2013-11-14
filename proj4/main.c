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
 * ret:
 *    0: failure evaluating
 *    1: success evaluating
 */
int evaluate_target( Agg_Data AD, char* start){
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
    fdata_free( fdata);
    return 0;
  } 
  agg_data_add_fdata( AD, fdata);
  return 1;
}

/* param: HMAP_PTR
 * param: char*, basic line
 * func:  store the basic file in the hmap
 * ret:
 *    0: failure evaluating
 *    1: success evaluating
 */
int evaluate_basic( Agg_Data AD, char* start){
  Fdata fdata = fdata_init( start); 
  start = strtok( start, " \t");
  while( start == NULL)
    start = strtok( NULL, " \t");
  //printf("basic:%s:\n", start);
  if( is_unique( AD, start)) agg_data_add_fdata( AD, fdata);
  else{
    fprintf(stderr, "ERROR, BASIC FILE NAME EXISTS\n");
    fdata_free( fdata);
    return 0;
  }
  return 1;
}

/* param: Agg_Data
 * param: char*, fakemake file
 * func:  read the fake make file,
 *        store every file read in the table,
 *        abort if file is already entered
 * ret:
 *    0: failure evaluating
 *    1: success evaluating
 */
int gen_parse_file( Agg_Data AD, char* fileName){
  FILE* file = fopen( fileName, "r");
  Line l = line_init( file);
  int no_errors = 1;

  line_read_line( l);
  char* chunk = get_line( l);
  while( chunk != NULL){ //reading lines from fakefile
    gen_swap_first( &chunk, '\n', '\0');
    if( is_valid_line( chunk) ){
      if( is_target(chunk) ){
        if( !evaluate_target( AD, chunk) ) no_errors = 0;
      }
      else{
        if( !evaluate_basic( AD, chunk) ) no_errors = 0;
      }
    }
    free(chunk);
    if( !line_read_line( l) ) break; //EOF
    chunk = get_line( l);
  }

  line_free( l);
  fclose( file);
  return no_errors;
}

/****************** CMD FXS *******************/
/*
 *
 */
char* cmd_parse_line( char** line){
  char* chunk = strtok( *line, "\t ");
  chunk = strtok(NULL, "\t ");
  if( chunk != NULL){
    gen_swap_first( &chunk, '\n', '\0');
    //printf("chunk:%s:\n", chunk);
  }
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
  if( chunk != NULL && hmap_contains( AD->hmap, chunk) ){
    set_time_stamp( (Fdata)hmap_get(AD->hmap, chunk), AD->clock);
    ++AD->clock;
  }
  else printf("Sorry, that file doesn't exist1\n");
}

/* param: line, to parse
 * param: Agg_Data
 * param: int, flag: 0->don't print, 1->print
 * func:  print the timestamp of the specified file
 */
void cmd_timestamp( char* line, Agg_Data AD){
  char* chunk = cmd_parse_line( &line);
  if( chunk != NULL && hmap_contains( AD->hmap, chunk) ){
    printf("%s :%d\n", chunk,
        get_time_stamp( (Fdata)hmap_get( AD->hmap, chunk) ) );
  }
  else printf("Sorry, that file doesn't exist1\n");
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
 *        file flag states:
 *        0: not visited
 *        1: visited, currently expanding
 *        2: visited, done expanding
 */
int get_child_ts( char* fileName, Agg_Data AD){
  Fdata Cur = (Fdata)hmap_get( AD->hmap, fileName), Dep;
  set_flag( Cur, 1);  
  int numDepends = get_num_depends( Cur), i, updateLater = 0;
  char** depends = get_depends_on( Cur);
  for( i=0; i<numDepends; ++i){
    Dep = (Fdata)hmap_get( AD->hmap, depends[i]);
    if( get_time_stamp(Cur) < get_child_ts( depends[i], AD) )
      updateLater = 1;
  }
  if( updateLater){
    set_time_stamp( Cur, AD->clock);
    ++AD->clock;
  }
  return get_time_stamp( Cur);
}
void cmd_make( char* line, Agg_Data AD){
  char* chunk = cmd_parse_line( &line);
  if( chunk != NULL && hmap_contains( AD->hmap, chunk))
    get_child_ts( chunk, AD);
  else printf("Sorry, you didn't specify a file OR the file doesn't exist\n");
  set_flag_all( AD->files, AD->numFdata, 2, 0);
  set_flag_all( AD->files, AD->numFdata, 1, 0);
}

/************** main **********************/ 
int main(int argc, char** argv){
  char* fileName = NULL, command;
  int quit = 1, temp, numRead;
  Agg_Data aggdata = agg_data_init();

  gen_parse_args( argc, argv, &fileName);
  if( fileName == NULL) fprintf( stderr, "NO VALID FILE CLARG SUPPLIED\n");
  else{
    if( !gen_parse_file( aggdata, fileName) )
      fprintf(stderr, "ERROR PARSING\n");
    else if( !is_valid_map(aggdata->hmap, aggdata->files, aggdata->numFdata) )
      fprintf(stderr, "ERROR BUILDING\n");
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

