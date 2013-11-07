#include "seq.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printInt( ETYPE x){
    printf("%d\n", x);
}
void printString( ETYPE x){
    printf("%s\n", x);
}

int cmp( ETYPE x, ETYPE y){
    if( x < y) return -1;
    else if( x > y) return 1;
    else return 0;
}

int main(){
    //int test[5] = {1, 2, 3, 4};
    char *test[] = {"abcasdfasdf", "jtqwtrqwrear", "doddddddddreymi"};
    Seq tseq;
    tseq = seq_create();
    seq_add_front( tseq, test[2]);
    seq_print( tseq, printString);
    seq_del_back( tseq);
    seq_del_back( tseq);
    seq_print( tseq, printString);
    
seq_free( tseq);
    return 0;
}
