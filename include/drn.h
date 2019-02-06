/* DRN.H
 * (c) Ellis Rhys Thomas <e.rhys.thomas@gmail.com>
 * See LICENCE file for details
 */

#ifndef DRN
#define DRN

#include <stdlib.h>
#include <X11/Xlib.h>
#include <signal.h>

#include "drn_sll.h"

#define MAX_LEN 100
#define CB_SO "lib/libdrn_cb.so"

volatile sig_atomic_t done;

typedef char *(*getstr)(void);

/* Used to open and close the default X display */
Display *open_display(void);
void close_display(Display *xdisplay);

/* Reading callbacks from shared library, and actions to perform on
   success or failure */
getstr read_cb(void *libdrn_cb, const char *libfn);
int read_cb_failure(struct SLL **Strings);
int read_cb_success(struct SLL **Strings, char *str);

/* Set the name of the root window of chosen display to str */
void set_rootname(Display *xdisplay, const char *str, size_t len);

char *strings_combine(struct SLL *Strings, char *delimiter);
int strings_generate(void *lib, char **cbname, size_t cbcount,
		     struct SLL **List);

/* signal handler */
void term(int signum);

#endif	/* DRN */
