#include "seq.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

// The implementation for for the sequence
//   ADT.
// Initially just has function stubs.

typedef struct node{
    struct node *fw; //direction from head to tail
    struct node *bw; //direction from tail to head
    ETYPE a; // one ETYPE of data
} Node;

struct seq_struct {
    Node *head;
    Node *tail;
    int numN; //number of nodes
};

/*****************************************/

/**
* DESCRIPTION: initializes an empty sequence and
*   returns corresponding pointer.
*/
extern Seq seq_create(){
    Seq Temp = malloc( sizeof(struct seq_struct) );
    Temp->numN = 0;
    Temp->head = NULL;
    Temp->tail = NULL;
    return Temp;
}

/**
* DESCRIPTION:  deallocates the sequence.  Does NOT free
*   any of the value/element pointers in generic mode
*   (they are the *   responsibility of the client).
*  RUNTIME:  O(n)
*/
extern void seq_free(Seq seq){
    Node *temp; //create a placeholder
    while( seq->head != NULL){
	temp = seq->head; //assign the  placeholder
	seq->head = seq->head->fw; //move forward
	free( temp); //delete the "head" (placeholder)
    }
    free(seq);
}

/**
* DESCRIPTION:  Prints elements of sequence in order.
*   in the generic case, a client-supplied printing
*   function is provided and called to print individual
*   elements.
* RUNTIME REQ: O(n)
*/
#ifdef GENERIC
extern void seq_print(Seq seq, void (* print_func)(void *)){
    Node *temp;
    temp = seq->head;
    while( temp != NULL){
	print_func(temp->a);
	temp = temp->fw;
    }
}
#else
extern void seq_print(Seq seq){
    printf("WITHOUT THE FLAG\n");
}
#endif

/**
* DESCRIPTION:  returns the number of elements in
*   the parameter Seq.
* RUNTIME REQ: O(1)
*/
extern int seq_size(Seq seq){
    return seq->numN;
}

/**
* DESCRIPTION:  adds a new element with given val
*    to the "front" of the seq.
* RUNTIME REQ: O(1)
*/
extern void seq_add_front(Seq seq, ETYPE val){
    Node* front;
    front = (Node*)malloc(sizeof(Node) );
    front->a = val;
    if( seq_size(seq) >= 1){ //at least one node (head and tail exist)
	front->fw = seq->head; //point the new front to the list
	seq->head->bw = front; //point the "old" front to the new front
    }
    else{ //the seq has 0 elems; front is both "front" and "end"
	seq->head = front;
	seq->tail = front;
	front->fw = NULL;
    }
    front->bw = NULL;
    seq->head = front;
    ++(seq->numN); //reflect addition of new node
}

/**
* DESCRIPTION:  deletes the front/first element
*   in the seq and returns its value.
*   If the seq is empty, the seq is unchanged and
*     the return value has no meaning (i.e., this is
*     the client's fault!
* RUNTIME REQ: O(1)
*/
extern ETYPE seq_del_front(Seq seq){
    if( seq_size(seq) <= 0){
	return (ETYPE)-1;
    }
    Node* nodehold = seq->head;
    if( seq_size(seq) == 1){
	seq->tail = NULL;	
    }
    //if the list is at least 2 big, the tail will not change
    ETYPE holder = seq->head->a;
    //even if one node, should set head to null;
    seq->head = seq->head->fw;
    seq->head->bw = NULL;
    free(nodehold);
    --(seq->numN);
    return holder;
}

/**
* DESCRIPTION:  adds a new element to the "back" of
*  the seq
* RUNTIME REQ: O(1)
*/
extern void seq_add_back(Seq seq, ETYPE val){
    Node* back;
    back = (Node*)malloc(sizeof(Node) );
    back->a = val;
    if( seq_size(seq) >= 1){ //at least one node (head and tail exist)
	back->bw = seq->tail; //point the new tail to the list
	seq->tail->fw = back; //point the "old" tail to the new end
    }
    else{ //the seq has 0 elems; back is both "front" and "end"
	seq->head = back;
	seq->tail = back;
	back->bw = NULL;
    }
    back->fw = NULL;
    seq->tail = back;
    ++(seq->numN); //reflect addition of new node
}

/**
* DESCRIPTION:  deletes the back/last element
*   in the seq and returns its value.
*   If the seq is empty, the seq is unchanged and
*     the return value has no meaning (i.e., this is
*     the client's fault!)
* RUNTIME REQ: O(1)
*/
extern ETYPE seq_del_back(Seq seq){
    if( seq_size(seq) <= 0){
	return (ETYPE)-1;
    }
    Node* nodehold = seq->tail;
    if( seq_size(seq) == 1){
	seq->head = NULL;	
    } 
    //if the list is at least 2 big, the head will not change
    ETYPE holder = seq->tail->a;
    //even if one node, should set tail to null
    seq->tail = seq->tail->bw;
    seq->tail->fw = NULL;
    free(nodehold);
    --(seq->numN);
    return holder;
}

/**
* DESCRIPTION:  returns the front element
*  of the sequence; does not modify the
*  sequence.  If sequence is empty then
*  return value has no meaning (client's
*  fault!
* RUNTIME REQ:  O(1)
*/
extern ETYPE seq_peek_front(Seq seq){
    if( seq_size(seq) <= 0){
	return (ETYPE)-1;
    }
    return seq->head->a;
}


/**
* DESCRIPTION:  returns the last/back element
*  of the sequence; does not modify the
*  sequence.  If sequence is empty then
*  return value has no meaning (client's
*  fault!
* RUNTIME REQ:  O(1)
*/
extern ETYPE seq_peek_back(Seq seq){
    if( seq_size(seq) <= 0){
	return (ETYPE)-1;
    }
    return seq->tail->a;
}

/**
* DESCRIPTION:  returns a deep copy of the given
*   sequence.  (Note:  there is no way for actual
*   elements to be cloned in this framework).
* RUNTIME REQ: O(n)
*/
extern Seq seq_clone(Seq seq){
    Seq cpy = seq_create();
    Node *hopper = seq->head;
    while(hopper != NULL){
	seq_add_back( cpy, hopper->a); //cpy the NODE with the value at that node
	hopper = hopper->fw; //move forward
    }
    return cpy;
}

/**
* DESCRIPTION:  s1 becomes the result of concatenating
*    s1 and s2.
*
* postconditions:  s1 and s2 become empty sequences.
*
* Notes:  if the client passes the same sequence as
*   both s1 and s2
*    
* RUNTIME REQ: O(1)
*/
extern Seq seq_concat(Seq s1, Seq s2){
    if( s1->head == s2->head)
	return NULL;

    Seq nseq = seq_create();
    nseq->head = s1->head;
    nseq->tail = s2->head;

    s1->tail->fw = s2->head;
    s2->head->bw = s1->tail;
    nseq->numN = s1->numN + s2->numN;

    return nseq;
}



/*----------------------------------*/
/**
* DESCRIPTION:  returns an array containing
*  all of the elements of the seq in sequence.,
*  the seq itself remains unchanged.
* RUNTIME REQ: O(n)
*/
extern ETYPE * seq_to_array(Seq seq){
    ETYPE *arr = (ETYPE*)malloc( (seq->numN)*(sizeof(ETYPE)) );
    int i;
    Node *temp = seq->head;
    for( i =0; i < seq->numN; ++i){
	arr[i] = temp->a;
	temp = temp->fw;
    }
    return arr;
}

#ifndef GENERIC

/*** int versions of filter ******/
/**
* DESCRIPTION:  removes all elements equal
*   to x and uses these elements to populate
*   a new sequence which is returned.
* RUNTIME REQ: O(n)
*/
int ETYPE_eq( ETYPE x, ETYPE y){
}


extern Seq seq_filter_eq(Seq seq,
ETYPE x){
}

/**
* DESCRIPTION:  same as seq_filter_eq
*   except criteria is "less than x"
*   instead of "equal to x"
* RUNTIME REQ: O(n)
*/
extern Seq seq_filter_lt(Seq seq,
ETYPE x){
}

/**
* DESCRIPTION:  same as seq_filter_eq
*   except criteria is "greater than x"
*   instead of "equal to x"
* RUNTIME REQ: O(n)
*/

extern Seq seq_filter_gt(Seq seq,
ETYPE x){
}
#else

/*
Seq seq_filter_void(Seq seq, 
int (*compF)( ETYPE, ETYPE ),
ETYPE x) {
    Seq found = seq_create();
    Seq notFound = seq_create();
    Node* hopper = seq->head;
    while( hopper != NULL){ ///O(N)
	if( compF( hopper->a, x) )
	    seq_add_back( found, hopper->a); ///O(1)
	else
	    seq_add_back( notFound, hopper->a); ///O(1)
	hopper = hopper->fw;
    }
    //reflect the changes in seq from the notFound seq
    //   frees the Nodes, not the "body"
    while( seq->head != NULL){
	hopper = seq->head; //assign the  placeholder
	seq->head = seq->head->fw; //move forward
	free( hopper); //delete the "head" (placeholder)
    }
    // asssign new values to the "body"
    seq->numN = notFound->numN;
    seq->head = notFound->head;
    seq->tail = notFound->tail;
    free(notFound); //frees the "body," not the Nodes   

    return found;
}
*/

/** Generic filter functions ***/
/**
* DESCRIPTION:  removes all elements equal
*   to x according to comparator function
*   cmp() and uses these elements to populate
*   a new sequence which is returned.
* RUNTIME REQ: O(n)
*/
extern Seq seq_filter_eq(Seq seq,
int (*cmp)(ETYPE a, ETYPE b),
ETYPE x){
    Seq found = seq_create();
    Seq notFound = seq_create();
    Node* hopper = seq->head;
    while( hopper != NULL){ ///O(N)
	if( 0 == cmp( hopper->a, x) )
	    seq_add_back( found, hopper->a); ///O(1)
	else
	    seq_add_back( notFound, hopper->a); ///O(1)
	hopper = hopper->fw;
    }
    //reflect the changes in seq from the notFound seq
    //   frees the Nodes, not the "body"
    while( seq->head != NULL){
	hopper = seq->head; //assign the  placeholder
	seq->head = seq->head->fw; //move forward
	free( hopper); //delete the "head" (placeholder)
    }
    // asssign new values to the "body"
    seq->numN = notFound->numN;
    seq->head = notFound->head;
    seq->tail = notFound->tail;
    free(notFound); //frees the "body," not the Nodes   

    return found;
}

/**
* DESCRIPTION:  same as seq_filter_eq
*   except criteria is "less than x"
*   instead of "equal to x"
* RUNTIME REQ: O(n)
*/
extern Seq seq_filter_lt(Seq seq,
int (*cmp)(ETYPE a, ETYPE b),
ETYPE x){
    Seq found = seq_create();
    Seq notFound = seq_create();
    Node* hopper = seq->head;
    while( hopper != NULL){ ///O(N)
	if( 0 > cmp( hopper->a, x) )
	    seq_add_back( found, hopper->a); ///O(1)
	else
	    seq_add_back( notFound, hopper->a); ///O(1)
	hopper = hopper->fw;
    }
    //reflect the changes in seq from the notFound seq
    //   frees the Nodes, not the "body"
    while( seq->head != NULL){
	hopper = seq->head; //assign the  placeholder
	seq->head = seq->head->fw; //move forward
	free( hopper); //delete the "head" (placeholder)
    }
    // asssign new values to the "body"
    seq->numN = notFound->numN;
    seq->head = notFound->head;
    seq->tail = notFound->tail;
    free(notFound); //frees the "body," not the Nodes   

    return found;
}

/**
* DESCRIPTION:  same as seq_filter_eq
*   except criteria is "greater than x"
*   instead of "equal to x"
* RUNTIME REQ: O(n)
*/
extern Seq seq_filter_gt(Seq seq,
int (*cmp)(ETYPE a, ETYPE b),
ETYPE x){
    Seq found = seq_create();
    Seq notFound = seq_create();
    Node* hopper = seq->head;
    while( hopper != NULL){ ///O(N)
	if( 0 < cmp( hopper->a, x) )
	    seq_add_back( found, hopper->a); ///O(1)
	else
	    seq_add_back( notFound, hopper->a); ///O(1)
	hopper = hopper->fw;
    }
    //reflect the changes in seq from the notFound seq
    //   frees the Nodes, not the "body"
    while( seq->head != NULL){
	hopper = seq->head; //assign the  placeholder
	seq->head = seq->head->fw; //move forward
	free( hopper); //delete the "head" (placeholder)
    }
    // asssign new values to the "body"
    seq->numN = notFound->numN;
    seq->head = notFound->head;
    seq->tail = notFound->tail;
    free(notFound); //frees the "body," not the Nodes   

    return found;
}

#endif
