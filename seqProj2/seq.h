/*instructions:
*
*     You will complete the implementation of
*     an ADT for a "sequence" of elements.  
*
*     This file (seq.h) specifies the interface
*     to the ADT:  operations that can
*     be performed with sequences (things
*     of type Seq).
*
*     A sequence has two ends:  the front and back.
*
*     Four of the operations together are the union
*     of traditional stack and queue operations:
*
*       seq_add_front  
*       seq_del_front
*       seq_add_back
*       seq_del_back
*
*        add_front 4
*            [4]
*        add_front 2
*            [2 4]
*        add_back 7
*            [2 4 7]
*        add_back 9
*            [2 4 7 9]
*        del_front
*            [4 7 9]
*        del_back
*            [4 7]
*/

/**  
*    We can compile in two different modes:
*     generic or non-generic.  
*
*    In generic mode, elements are
*     specified as void pointers where the
*     data pointed to is determined by
*     the client.
*
*    In non-generic mode, elements are
*     simply integers.  You can receive up to
*     75% credit if your implementation is only
*     for the non-generic case.
*
*    In generic mode, elements are handled as
*     void pointers.
*
*    The mode of an implementation is handled
*     with the preprocessor (see the #ifdef, etc.).
*    
*/

/*
* DEFAULT:  non-generic / int type
*  for Generic mode, use -DGENERIC flag
*  to gcc.
*/

#ifdef GENERIC
typedef void* ETYPE;
#else
typedef int ETYPE;
#endif


/*****************************************/
/*
* incompletely specified struct.  actual struct
*   contents specified in seq.c
*/

typedef struct seq_struct *Seq;
/*****************************************/

/**
* DESCRIPTION: initializes an empty sequence and
*   returns corresponding pointer.
*/
extern Seq seq_create();

/**
* DESCRIPTION:  deallocates the sequence.  Does NOT free
*   any of the value/element pointers in generic mode
*   (they are the *   responsibility of the client).
*  RUNTIME:  O(n)
*/
extern void seq_free(Seq seq);

/**
* DESCRIPTION:  Prints elements of sequence in order.
*   in the generic case, a client-supplied printing
*   function is provided and called to print individual
*   elements.
* RUNTIME REQ: O(n)
*/
#ifdef GENERIC
extern void seq_print(Seq seq, void (* print_func)(void *));
#else
extern void seq_print(Seq seq);
#endif

/*
* DESCRIPTION:  returns the number of elements in
*   the parameter Seq.
* RUNTIME REQ: O(1)
*/
extern int seq_size(Seq seq);

/**
* DESCRIPTION:  adds a new element with given val
*    to the "front" of the seq.
* RUNTIME REQ: O(1)
*/
extern void seq_add_front(Seq seq, ETYPE val);

/**
de* n){
    printf("A:%d\tB:%d\n", n->a, n->b);
}

* DESCRIPTION:  deletes the front/first element
*   i tniseq and returns its value.
*   If the seq is empty, the seq is unchanged and
*     the return value has no meaning (i.e., this is
*     the client's fault!
* RUNTIME REQ: O(1)
*/
extern ETYPE seq_del_front(Seq seq);

/**
* DESCRIPTION:  adds a new element to the "back" of
*  the seq
* RUNTIME REQ: O(1)
*/
extern void seq_add_back(Seq seq, ETYPE val);

/**
* DESCRIPTION:  deletes the back/last element
*   in the seq and returns its value.
*   If the seq is empty, the seq is unchanged and
*     the return value has no meaning (i.e., this is
*     the client's fault!)
* RUNTIME REQ: O(1)
*/
extern ETYPE seq_del_back(Seq seq);

/**
* DESCRIPTION:  returns the front element
*  of the sequence; does not modify the
*  sequence.  If sequence is empty then
*  return value has no meaning (client's
*  fault!
* RUNTIME REQ:  O(1)
*/
extern ETYPE seq_peek_front(Seq seq);

/**
* DESCRIPTION:  returns the last/back element
*  of the sequence; does not modify the
*  sequence.  If sequence is empty then
*  return value has no meaning (client's
*  fault!
* RUNTIME REQ:  O(1)
*/
extern ETYPE seq_peek_back(Seq seq);

/**
* DESCRIPTION:  returns a deep copy of the given
*   sequence.  (Note:  there is no way for actual
*   elements to be cloned in this framework).
* RUNTIME REQ: O(n)
*/
extern Seq seq_clone(Seq seq);

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
extern Seq seq_concat(Seq s1, Seq s2);


/*----------------------------------*/
/**
* DESCRIPTION:  returns an array containing
*  all of the elements of the seq in sequence.,
*  the seq itself remains unchanged.
* RUNTIME REQ: O(n)
*/
extern ETYPE * seq_to_array(Seq seq);
#ifndef GENERIC

/*** int versions of filter ******/
/**
* DESCRIPTION:  removes all elements equal
*   to x and uses these elements to populate
*   a new sequence which is returned.
* RUNTIME REQ: O(n)
*/
extern Seq seq_filter_eq(Seq seq,
ETYPE x);

/**
* DESCRIPTION:  same as seq_filter_eq
*   except criteria is "less than x"
*   instead of "equal to x"
* RUNTIME REQ: O(n)
*/
extern Seq seq_filter_lt(Seq seq,
ETYPE x);

/**
* DESCRIPTION:  same as seq_filter_eq
*   except criteria is "greater than x"
*   instead of "equal to x"
* RUNTIME REQ: O(n)
*/
extern Seq seq_filter_gt(Seq seq,
ETYPE x);

#else

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
ETYPE x);

/**
* DESCRIPTION:  same as seq_filter_eq
*   except criteria is "less than x"
*   instead of "equal to x"
* RUNTIME REQ: O(n)
*/
extern Seq seq_filter_lt(Seq seq,
int (*cmp)(ETYPE a, ETYPE b),
ETYPE x);

/**
* DESCRIPTION:  same as seq_filter_eq
*   except criteria is "greater than x"
*   instead of "equal to x"
* RUNTIME REQ: O(n)
*/
extern Seq seq_filter_gt(Seq seq,
int (*cmp)(ETYPE a, ETYPE b),
ETYPE x);

#endif
