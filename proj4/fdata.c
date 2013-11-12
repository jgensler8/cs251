#include <stdlib.h>
#include <string.h>

#include "fdata.h"

typedef struct file_data_struct{
  char* fileName;
  int timeStamp; //last time the file was updated; use with Hmap
  int flag; // set on the current build run
  char **dependsOn; //for use with the Graph
  int numDepends;
} *Fdata;

/*
 *
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

/*
 *
 */
Fdata fdata_free( Fdata f){
  int i;
  for( i=0; i<f->numDepends; ++i){
    free( (f->dependsOn)[i] );
  }
  free( f->dependsOn);
  free( f->fileName);
  free( f);
}

/*
 *
 */
Fdata fdata_add_dep( Fdata f, char* fname){
  char** newDepends = (char**)malloc( (1+f->numDepends)*sizeof(char*));
  int i;
  for( i=0; i<f->numDepends; ++i){
    newDepends[i] = (f->dependsOn)[i];
  }
  free( f->dependsOn);
  f->dependsOn = newDepends;
  f->numDepends += 1;
  (f->dependsOn)[f->numDepends-1] =
      (char*)malloc( (1+strlen(fname))*sizeof(char));
  strcpy( (f->dependsOn)[f->numDepends-1], fname);
}
