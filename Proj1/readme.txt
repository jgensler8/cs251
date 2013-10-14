README:

compile: gcc tsort.c
(I use a manual include of each separate file)

1) tsort.c 
calls the two basic functions of the program:
  1.1) allocation and reading from stream
  1.2) sorting the data

2) scanAll
 refered to in 1.1: reads from stream and allocates a dynamic sized table
 THE FIRST LINE OF INPUT MUST BE CORRECT (can't be an empty line).
 This is the one assumption in my  program.

3) tSort.c
 refered to in 1.2: sorts the table with qsort, using an optional flag to call
 the appropriate function. The qsort is not stable (to my knowledge).
