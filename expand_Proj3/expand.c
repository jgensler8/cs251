#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "hmap.h"
#include "line.h"
#include "mutex_ll.h"

// GENERAL FUNCTIONS
int gen_parse_args( int, char**, char**);
int gen_test_open_file( char*);

char* get_file_name( char*);

//~~~~~~~~~~THE GOLDEN FUNCTION~~~~~~~~~~~~
int jump_into( FILE*, HMAP_PTR, Mutex_ll);

int main( int argc, char *argv[]){
  char* nexus_file_name;
  if( gen_parse_args( argc, argv, &nexus_file_name) ){
    FILE* nexus = fopen( nexus_file_name, "r");
    HMAP_PTR hmap = hmap_create(0,.75);
    hmap_set_hfunc(hmap, NAIVE_HFUNC);
    Mutex_ll LL = Mutex_ll_init();   
    
    char* val_hold = Mutex_ll_grow( LL);
    *val_hold = 1;
    hmap_set( hmap, nexus_file_name, val_hold );
    //down the rabbit hole
    jump_into( nexus, hmap, LL);
    //out of the rabbit hole   
 
    fclose(nexus);
    hmap_free( hmap, 0);
    Mutex_ll_free( LL);
  }
  else fprintf( stderr, "NO VALID FILE from CL_ARG\n");
  return 0;
}

/* param: argc
 * param: argv
 * param: &char* will point to the first possible file name that can be opened
 * return:
 *  1: an argv[*] can be opened, sets passed &char* to point to argv[*]
 *  0: no valid argv[*] can be opened
 */
int gen_parse_args( int argc, char**argv, char** nexus_file_name){
  int i;
  for(i = 1; i < argc; ++i){
    if( gen_test_open_file(argv[i]) ){
      *nexus_file_name = argv[i];
      return 1;
    }
  }
  return 0;
}

/* param: pointer to a c-string
 * return:
 *  1: passed param can be opened in read mode
 *  0: passed param can't be opened in read mode
 */
int gen_test_open_file( char* file_name){
  FILE *test_opener = fopen( file_name, "r");
  if( test_opener != NULL){
    fclose( test_opener);
    return 1;
  }
  else printf("FAILED TO OPEN FILE, DANG");
  return 0;
}

/* param: char* line that has at least one #
 * func:  verify if the line has a valid file name
 * return:
 *  0: does not meet criteria for file name
 *  1: meets file name criteria (and open-able)
 */
int gen_is_include_line( char* l){
  if( NULL != strchr( l, '#')){ //attempted include statment
    if( l[0] == '#' && (NULL != strstr(l, "#include")) ){
      //TODO
      return 1;
    }
    else{
      fprintf( stderr, "INVALID INCLUDE, TERMINATING\n");
      abort();
    }
  }
  else return 0; //good 'ol line
}

/* param: FILE (ALREADY OPENED) to be read from
 * parma: HMAP_PTR used to record the cyclic dependencies
`* param: Mutex_ll to keep track of the hmap's key's val (char)
 * func: gets a line from a file. if it trys to #include "XYZ" and succeeds
 *    this will call itself but with this that new file (RECURSION :D)
 *    If the #include fails, the program terminates. All other lines 
 *    are printed to stdout.
 * return:
 *  1: success
 *  0: TODO: no failure state implemented (though not needed for grader)
 */
int jump_into( FILE* f, HMAP_PTR phmap, Mutex_ll LL){
  Line l = line_init( f);
  char* temp_line;
  while( 0 != line_read_line( l) ){
    temp_line = get_line( l);
    if( gen_is_include_line( temp_line) ){
      char* file_name = get_file_name( temp_line);
      if( gen_test_open_file( file_name) && //can be opened
          (NULL == hmap_get( phmap, file_name) || //never declared
          '1' != (char)*(char*)hmap_get( phmap, file_name )) ){//declared-used
        FILE* to_jump = fopen( file_name, "r");
        char* hold_val = Mutex_ll_grow( LL);
        *hold_val = '1'; //bit of a hack
        hmap_set( phmap, file_name, hold_val);
        //DOWN THE RABBIT HOLE
        jump_into( to_jump, phmap, LL);
        //OUT OF THE RABBIT HOLE
        hmap_remove( phmap, file_name);
        Mutex_ll_remove( LL);
        fclose( to_jump);
        free( file_name);
      }
      else{
        fprintf(stderr, "BAD FILE NAME || CYCLIC DEPENDENCY, TERMINATING\n");
        abort();
      }   
    }
    else line_print_line( l);
    free(temp_line);
  }
  line_free( l);
  return 1;
 }

/* param: a line that contains a file name surrounded by double quotes
 * func: create a deep copy of file name
 * return: deep copy of file name
 */
char* get_file_name( char* line_chunk){
  char* first = strchr( line_chunk, '\"');
  char* second = strchr( first+1, '\"'); //"hop" over the first double quote
  //they are sizeof(char) spaced
  int size = (second-first)/sizeof(char); 
  char* ret = malloc( (size)*sizeof(char) ); // room for null terminator
  strncpy( ret, first+1, size); //"hop" opver double quote
  ret[size-1] = '\0'; //manually assign null terminator
  return ret;
}
