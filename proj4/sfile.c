#include <iostream>

typedef struct file_data_struct{
  char* fileName;
  int timeStamp; //last time the file was updated; use with Hmap
  int user; // set on the current build run
  struct file_data_struct **dependsOn; //for use with the Graph
} fdata;

typedef struct seen_file_struct{
  fdata* files;
  int numFiles;
  fdata* targets;
  int numTargets;
} *sFile;

/****** sFile fxs *************/
/* func:  allocate space to sfile
 * ret:   initialized sfile
 */
sFile sFile_init(){
  sFile sfile = malloc( sizeof( seen_file_struct) );
  sfile->files = NULL;
  sfile->numFiles = 0;
  sfile->target = NULL;
  sfile->targetFile = 0;
}

/* param: sFile, to be freed
 * func:  free the contents of sFile
 */
void sFile_free( sFile sfile){
  int i;
  for( i=0; i < sfile->numFiles; ++i)
    free( sfile->numFiles[i]
  free( sfile->files);
  for( i=0; i < sfile->numTargets; ++i)
    free( sfile->targets[i]);
  free( sfile->targets);
  
  free( sfile);
}

/* param: sFile
 * param: char*, pointer to file name to store
 * func:  add the file name to sFile's files[]
 */
void sFile_add_file( sFile sfile, char* fileName){
  //copy old stuff
  //add new stuff
}

/* param: mFile
 * param: char*, pointer to target name to store
 * func:  add the file name to mFile's target[]
 */
void sFile_add_target( sFile sfile, char* fileName){
  //copy old stuff
  //add new stuff
}


