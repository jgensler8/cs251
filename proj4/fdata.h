typedef struct file_data_struct *Fdata;

extern Fdata fdata_init( char*);
extern void fdata_free( Fdata);
extern void fdata_add_dep( Fdata, char*);
extern char* get_name( Fdata);
extern int sizeof_fdata();
extern void fdata_print( Fdata);
