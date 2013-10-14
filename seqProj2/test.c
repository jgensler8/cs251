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
    int test[5] = {1, 2, 3, 4};
    //char *test[] = {"abcasdfasdf", "jtqwtrqwrear", "doddddddddreymi"};
    Seq tseq;
    tseq = seq_create();
    seq_add_front( tseq, test[2]);
    seq_add_front( tseq, test[1]);
    seq_add_front( tseq, test[0]);

    Seq fourth = seq_clone( tseq);
    Seq last = seq_concat( tseq, fourth);

    Seq jar = seq_filter_eq( last, cmp, 2);
    seq_print( jar, printInt);
printf("not filtered:\n");
    seq_print( last, printInt);
//array test

    free( tseq);
    free( fourth);
    seq_free( last);
    return 0;
}
