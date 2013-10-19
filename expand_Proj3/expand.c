#include <stdio.h>
#include <stdio.h>
#include "hmap.h"

int gen_parse_args( int, char**, char**);
int gen_test_open_file( char*);

int main( int argc, char *argv[]){
  FILE* nexus;
  char* nexus_file_name;
  if( gen_parse_args( argc, argv, &nexus_file_name) ){
    nexus = fopen( nexus_file_name, "r");
    //do the whole program
    fclose( nexus);
  }
  else fprintf( stderr, "NO VALID FILE from CL_ARG\n");
  return 0;
}

/* Takes argc, argv, and reference to a char* that will point to the first
 * possible file name that can be opened.
 * If a file can be opened, point the char* to the corresponding argv
 * and return 1;
 * If a file name can't be opened, return 0;
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

/* take a pointer to a c-string
 * attempts to open a file with that string
 * returns 1 if successful
 * returns 0 if faliure
 */
int gen_test_open_file( char* file_name){
    FILE *test_opener = fopen( file_name, "r");
    if( test_opener != NULL){
      fclose( test_opener);
      return 1;
    }
    else return 0;
}
