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
  printf("timeStamp : %d\n", fdata->timeStamp);
  printf("flag      : %d\n", fdata->flag);
  int i;
  for(i=0; i<fdata->numDepends; ++i)
    printf("\t%d: %s\n", i, (fdata->dependsOn)[i]);
}

/* param: Fdata
 * param: int
 * func:  set Fdata's timestamp to the passed int
 */
void set_time_stamp( Fdata fdata, int n){
  fdata->timeStamp = n;
}

/* param: Fdata
 * ret:   Fdata's timestamp
 */
int get_time_stamp( Fdata fdata){
  return fdata->timeStamp;
}

/* param: Fdata
 * ret:   Fdata's numDepends
 */
int get_num_depends( Fdata fdata){
  return fdata->numDepends;
}

/* param: Fdata
 * ret:   a pointer to the 2d arr of dependencies
 */
char** get_depends_on( Fdata fdata){
  return fdata->dependsOn;
}

/* param: Fdata
 * param: int
 * func:  set Fdata's flag to int
 */
void set_flag( Fdata fdata, int val){
  fdata->flag = val;
}

 /* param: char*, filename
 * param: Agg_Data
 * func:  Do a DFS starting at char*
 *        flag vlaues:
 *          0: not discovered on current search
 *          1: discovered (but not done)( to find cycles)
 *          2: finished (so that cross edges can still use finished nodes
 * ret:
 *    0: no bad edge (good)
 *    1: has bad edge (bad)
 */
int has_bad_edge( char* filename, HMAP_PTR hmap, Fdata* files, int numFData){
  //printf("filename:%s:\n", filename);
  if( hmap_contains( hmap, filename) ){
    Fdata Curfile = hmap_get( hmap, filename);
    Curfile->flag = 1;
    int i;
    for( i=0; i<Curfile->numDepends; ++i){ //for every dependency
     // printf("CHECKING DEPEND:%s:\n", Curfile->dependsOn[i]);
      Fdata depends = (Fdata)hmap_get( hmap, (Curfile->dependsOn)[i] );
      if(depends->flag == 1) return 1; //back edge
      if( has_bad_edge( (Curfile->dependsOn)[i], hmap, files, numFData) ) return 1;
    }
    Curfile->flag = 2;
    return 0;
  }
  else{ //file never existed
    fprintf(stderr, "ERROR, file doesn't exist\n");
    return 1;
  }
}

/* param: Agg_Data
 * func:  determine if the map contains any cycles (namely back edges)
 * ret:
 *    0: not valid, contains cycles
 *    1: valid
 */
int is_valid_map( HMAP_PTR hmap, Fdata* files, int numFdata){
  int is_valid = 1, i;
  for( i=0; i<numFdata; ++i){
    //printf("HasBadEdge:%s:\n", files[i]->fileName);
    if( has_bad_edge( files[i]->fileName, hmap, files, numFdata ) ){
      is_valid = 0;
      break; //if invalid, no need to keep looking
    }
    //set_flag_all( files, numFdata, );
  } 
  return is_valid;
}

/* param: Fdata*, the Fdata's to change
 * param: int, number of Fdata's
 * param: int, target
 * param: int, change target to this
 * func:  replace all targets with the replacement 
 */
int set_flag_all( Fdata* files, int numFdata, int from, int to){
  int i;
  for( i=0; i<numFdata; ++i)
    if( (files[i])->flag == from) (files[i])->flag = to;
}

