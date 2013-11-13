#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "fdata.h"

typedef struct file_data_struct{
  char* fileName;
  int timeStamp; //last time the file was updated; use with Hmap
  int flag; // set on the current build run
  char **dependsOn; //for use with the Graph
  int numDepends;
} *Fdata;

/* param: char*, file name
 * func:  allocate the fdata and set its name field to char*
 * ret:   allocated Fdata
 */
Fdata fdata_init( char* name){
  Fdata f = malloc( sizeof( struct file_data_struct) );
  f->fileName = (char*)malloc( (1+strlen(name))*sizeof(char) );
  strcpy( f->fileName, name);
  f->timeStamp = 0;
  f->flag = 0;
  f->dependsOn = NULL;
  f->numDepends = 0;
}

/* Param: Fdata
 * func:  free the Fdata and its contents
 */
void fdata_free( Fdata f){
  int i;
  for( i=0; i<f->numDepends; ++i)
    free( (f->dependsOn)[i] );
  if( f->numDepends != 0) free( f->dependsOn);
  free( f->fileName);
  free( f);
}

/* param: Fdata
 * param: char*, the file name
 * func:  add a dependency to the passed Fdata
 */
void fdata_add_dep( Fdata f, char* fname){
  char** newDepends = (char**)malloc( (1+f->numDepends)*sizeof(char*));
  int i;
  for( i=0; i<f->numDepends; ++i)
    newDepends[i] = (f->dependsOn)[i];
  free( f->dependsOn);
  f->dependsOn = newDepends;
  f->numDepends += 1;
  (f->dependsOn)[f->numDepends-1] =
      (char*)malloc( (1+strlen(fname))*sizeof(char));
  strcpy( (f->dependsOn)[f->numDepends-1], fname);
}

/* param: Fdata
 * func:  return a malloc block that contains Fdata's name
 * ret:   malloced block of name
 */
char* get_name( Fdata f){
  char* name = (char*)malloc( (1+strlen( f->fileName))*sizeof(char));
  strcpy( name, f->fileName);
  return name;
}

/* func:
 * ret: size of a file_data_struct
 */
int sizeof_fdata(){
  return sizeof(struct file_data_struct);
}

/*
 *
 */
void fdata_print( Fdata fdata){
  printf("\nname: %s\n", fdata->fileName);
  printf("numDepends: %d\n", fdata->numDepends);
  int i;
  for(i=0; i<fdata->numDepends; ++i)
    printf("\t%d: %s\n", i, (fdata->dependsOn)[i]);
}
