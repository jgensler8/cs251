#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "line.h"
#define DEFAULT_SIZE 10

typedef struct line_struct{
  char* line;
  int max_size;
  int cur_size;
  int flag; //may turn out useful later
} *Line;

/* param: &Line
 * func:  initializes Line's members
 */
Line line_init(){
  Line l = malloc(sizeof(struct line_struct) );
  l->line = malloc(DEFAULT_SIZE*sizeof(char) );
  l->max_size = DEFAULT_SIZE;
  l->cur_size = 0;
  l->flag = 0;
  return l;
}

/* param: &Line
 * func:  doubles the size of &Line's line and copys old contends over
 */
void line_grow( Line l){
  char* bigger = malloc(l->max_size*2); //double the size
  strcpy( bigger, l->line);
  free( l->line);
  l->max_size *= 2;
}

/* param: &Line storage of input
 * param: FILE file to read input from
 * func:  reads from passed file, stores the line in Line's line
 * return: unimplemented
 */
int line_read_line( Line l, FILE* f){
  char* temp_line = malloc( (DEFAULT_SIZE+1)*sizeof(char) );
  fgets( temp_line, DEFAULT_SIZE, f);
  return 1;
}

/* param: &Line
 * func: updates the current size based on the length of Line's line
 */
void set_cur_size( Line l){
  l->cur_size = strlen( l->line);
}
