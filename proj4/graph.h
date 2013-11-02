typedef struct graph_struct *Graph;

extern Graph graph_init( int);
extern void graph_free( Graph);
extern void graph_add_edge( Graph, int , int);
extern void graph_print( Graph);
extern int* graph_dfs( Graph, int);
extern int graph_get_size( Graph);
