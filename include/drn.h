/* DRN.H
 * (c) Ellis Rhys Thomas <e.rhys.thomas@gmail.com>
 * See LICENCE file for details
 */

#ifndef DRN
#define DRN

#include <X11/Xlib.h>
#include <stdlib.h>

/* Open shared object */
void *open_library(char *libpathname);
void *close_library(void *so);

/* Blocking signals to allow cleanup before termination */
void start_signal_handler(void);

/* Used to open and close the default X display */
Display *open_display(void);
void close_display(Display *xdisplay);

int drn_loop(int argc, char **argv);

/* Reading callbacks from shared library, and actions to perform on
   success or failure */
static getstr read_cb(void *libdrn_cb, const char *libfn);
static int read_cb_failure(struct SLL **Strings);
static int read_cb_success(struct SLL **Strings, char *str);

/* Set the name of the root window of chosen display to str */
static void set_rootname(Display *xdisplay, const char *str, size_t len);

static char *strings_combine(struct SLL *Strings, char *delimiter);
static int strings_generate(void *lib, char **cbname, size_t cbcount,
		     struct SLL **List);

/* signal handler */
static void term(int signum);

#endif	/* DRN */
