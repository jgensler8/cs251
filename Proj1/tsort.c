#include <stdio.h>
#include <stdlib.h>
#include "scanAll.c"
#include "lab.c"

int main( int argc, char* argv[]){
    int rows, cols, **head, colToSort;
    char flag;
    flag = 'a';
    colToSort = 1;
    rows = 0;
    cols = 0;
    head = (int**)malloc( sizeof(int*));
    *head = (int*)malloc( sizeof(int));

    if( argc >= 2 && argv[1][1] == 'c' )
	colToSort = atoi(argv[2]);

    if( argc >= 4 &&  argv[3][1] == 'd' )
	flag = 'd';

    printf("starting scanall\n");
    scanAll( &rows, &cols, &head);     
    printf("sorting the array\n");
    compareValues( head, flag, colToSort, cols, rows);
    //free head
   /* int i;
    for( i = 0; i < rows; i++);
	free( head[i] );
*/
    return 0;
}
