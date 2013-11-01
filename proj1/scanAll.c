/*
 * scanAll ( ... ) reads input from stdin 
 * and puts the input in a 2D array of integers
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLINELEN 201
#define MAXCYCLES 100

int countCols( char* data);
void addRow(int** numrows, int* numcols, int**** head, char* linedata );
void printArray(int rows, int cols, int** head);
int isValidRow( int cols, char* row);

int scanAll(int* numRows, int* numCols, int*** head){
    char *line;
    line = (char*)malloc( sizeof(char)*MAXLINELEN);
  
    //find how many cols
    fgets( line, MAXLINELEN, stdin);
    *numCols = countCols( line);
    //add the first line
    addRow( &numRows, numCols, &head, line);
    //read MAXCYCLES lines from stdin
    int counter;
    for(counter = 0; counter < MAXCYCLES; ++counter){
	fgets( line, MAXLINELEN, stdin);
	if( 1 == isValidRow( *numCols, line) )
	    addRow( &numRows, numCols, &head, line);
    }
    free(line);
    return 1;
}

/* countcols returns the amount of token counted in the passed cstring
 * the argument passed must be terminated with a newline or an eof flag
*/
int countCols( char* line){
    int amount = 0;
    int val, n;
    //the row  MUST have 1 value at least (or it wouldn't translate to a column)
    while( 1 <= sscanf( line, "%d%n", &val, &n)){
	line += n;
	amount++;
    }
    return amount;
}


/* addRow adds rows to a 2d array already measured by numRows()
 * this function changes the head and the numrows
 */

void addRow(int** numrows, int* numcols, int**** head, char* line){
    ++(**numrows);
    **head = (int**)realloc( **head, sizeof(int*)*(**numrows) );
    
    int* newRow;
    newRow = (int*)malloc( sizeof(int)*(*numcols) );
    int n, val, j = 0;
    while( 1 <= sscanf( line, "%d%n", &val, &n)){
	line += n;
	newRow[j] = val;
	j++;	
    }

    (**head)[**numrows - 1] = newRow;
}

/* printf array prints the rows x cols array with pointer of head
 * only accepts integer arrays
 */
void printArray(int rows, int cols, int** head){
    int i, j;
    for( i = 0; i < rows; ++i){
	printf("row %d:\t", i);
	for( j = 0; j < cols; ++j){
	    printf("%6d:\t", head[i][j]);
	}
	printf("\n");
    }
}

/* isValidRow checks if the row is empty and that the row contain the right 
 * amount of values (the number of columns)
 */
Tint isValidRow( int cols, char* row){
    if( row[0] == '\n' || row[0] == '\0')
	return 0;

    // *******************************************
	// I WOULD HAVE USED THIS TO LIMIT TO 20 
	// COLS BUT THE PROGRAM CAN ACCOMMODATE MORE
    // *******************************************
    if( countCols( row) != cols){
	printf("line doesn't have the correct ammount of terms!\n");
	printf("\tgot: %d expected: %d\n", countCols( row), cols);
	printf("This error is also thrown when a string is parsed");
	exit(1);
    }

    char temp;
    int i;
    for( i = 0; i < MAXLINELEN; i++){
	temp = row[i];
	if( temp == '\n')
 	    return 1;
    }

    return 0;
}
