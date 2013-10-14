#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int* row;
    int val;
} Elem;
/**
  For reference:  the parameters to qsort.
  void qsort(void *base, size_t nmemb, size_t size,
             int (*compar)(const void *, const void *));
**/
/** Write your comparison functions here **/
int qS_asc( const void *ELA, const void *ELB){
    Elem *pELA = (Elem*)ELA;
    Elem *pELB = (Elem*)ELB;

    if( pELA->val < pELB->val){
        return -1;
    }
    else if( pELA->val > pELB->val){
        return 1;
    }
    else
	return 0;
}

int qS_dec( const void *ELA, const void *ELB){
    Elem *pELA = (Elem*)ELA;
    Elem *pELB = (Elem*)ELB;

    if( pELA->val > pELB->val){
        return -1;
    }
    else if( pELA->val < pELB->val){
        return 1;
    }
    else
	return 0;
}

int compareValues( int** head, char flag, int colToSort, int numCols, int numRows)
{
    --colToSort; // adjust for human input
    if( colToSort < 0 || colToSort > numCols)
	return 0;

    Elem List[numRows];
    int i;
    for( i = 0; i < numRows; i++){
	List[i].row = head[i];
	List[i].val = head[i][colToSort];	
    }

    if( flag == 'd')
        qsort( List, numRows, sizeof(Elem ), qS_dec );
    else
        qsort( List, numRows, sizeof(Elem ), qS_asc );
	
    //reassign the sorted ROW values
    for( i = 0; i < numRows; i++){
	head[i] = List[i].row;
    }
    printArray( numRows, numCols, head);

    return 1;
}


