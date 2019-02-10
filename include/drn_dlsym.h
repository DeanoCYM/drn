/* DRN_DLSYM.C
 *
 * DRN functions that operate on a shared object dynamically loaded at runtime
 * 
 */

typedef char *(*getstr)(void);

/* Open and close the shared object */
void *open_library(char *libpathname);
void close_library(void *so);

/* Returns a function pointer read from the  */
getstr
read_cb(void *so, const char *libfn);
