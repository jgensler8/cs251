#include <stdio.h>

typedef struct mutex_ll_struct *Mutex_ll;

extern Mutex_ll Mutex_ll_init();
extern char* Mutex_ll_grow( Mutex_ll);
extern void  Mutex_ll_remove( Mutex_ll);
