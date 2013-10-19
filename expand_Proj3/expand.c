#include <stdio.h>
#include <stdio.h>

#include "hmap.h"
#include "line.h"

// GENERAL FUNCTIONS
int gen_parse_args( int, char**, char**);
int gen_test_open_file( char*);

//~~~~~~~~~~THE GOLDEN FUNCTION~~~~~~~~~~~~
int jump_into( FILE**, HMAP_PTR, Line);

int main( int argc, char *argv[]){
  char* nexus_file_name;
  if( gen_parse_args( argc, argv, &nexus_file_name) ){
    FILE* nexus = fopen( nexus_file_name, "r");
    HMAP_PTR hmap;
    Line line = line_init(); 
    //HMAP_PTR hmap = hmap_create(0,.75);
    //hmap_set_hfunc(hmap, NAIVE_HFUNC);
    //hmap_set(hmap, nexus_file_name, NULL);
   
    jump_into( &nexus, hmap, line); //the magic

    //hmap_free( hmap, 0);
    fclose( nexus);
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
    else return 0;
}

/* param: FILE (ALREADY OPENED) to be read from
 * parma: HMAP_PTR used to record the cyclic dependencies
 * func:

//open a file
//while not EOF
//read line
//parse line
  //if there is a # sign, treat it different
  //else print

 * return:
 *  1: success
 *  0:
 */
int jump_into( FILE** f, HMAP_PTR phmap, Line l){
  
  return 1;
}
