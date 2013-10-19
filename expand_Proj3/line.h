#include <stdio.h>
typedef struct line_struct *Line;

extern Line line_init();
extern void line_grow( Line);
extern int line_read_line( Line, FILE*);
extern void set_cur_size( Line);
