#include <stdio.h>
#include <stdlib.h>

int maxRec( int a[], int i , int j){
if( i == j)
    return a[i];
int left = maxRec(a , i , (i+j)/2);
int right = maxRec(a , (i+j)/2 + 1 , j);
return ( left > right ) ? left : right ;
}

int main(){
int a[10] = {1,2,3,4,5,6,7,8,9,20};

printf("%d\n", maxRec( a, 0, 9) );

return 0;
}
