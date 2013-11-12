typedef struct file_data_struct *Fdata;

extern Fdata fdata_init( char* name);
extern Fdata fdata_free( Fdata f);
extern Fdata fdata_add_dep( Fdata f, char* fname);
