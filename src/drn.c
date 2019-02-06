/* DRN (dwm root name)
 *
 * Updates the name of the root window in X.  Useful for dwm which
 * displays the name of the root window as a status bar.
 */

/* (c) Ellis Rhys Thomas <e.rhys.thomas@gmail.com>
 * See LICENCE file for details
 */

#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include "drn.h"
#include "ert_log.h"


/* Returns handle for current display, or NULL on error */
Display *
open_display(void)
{
    Display *xdisplay = XOpenDisplay(NULL); /* NULL for default display */
    if (!xdisplay) {
	log_err("Failed to open X display");
    }

    return xdisplay;
}

void
close_display(Display *xdisplay)
{
    if(XCloseDisplay(xdisplay))
	log_warn("Failed to close X display");

    return;
}

/* Sets the name of the root window to str */
void
set_rootname(Display *xdisplay, const char *str, size_t len)
{
    assert(str[len] == '\0');

    XStoreName(xdisplay, DefaultRootWindow(xdisplay), str);
    XSync(xdisplay, False);

    log_info("Root window name set: %s", str);
    
    return;
}

/* Reads and returns the function 'libfn' from the .so 'drn_cb' */
getstr
read_cb(void *libdrn_cb, const char *libfn)
{
    if (strcmp(libfn, "") == 0) {
	log_warn("Empty string passed as command line arguement");
	return NULL;
    }

    getstr cb = dlsym(libdrn_cb, libfn);
    if (!cb) {
	log_warn("Failed to read %s() from %s", libfn, CB_SO);
	return NULL;
    }

    return cb;
}
    
/* Combine all strings seperated by the delimiter. Returns combined
   string or NULL on error. */
char *
strings_combine(struct SLL *Strings, char *delimiter)
{
    char *str = calloc(1, MAX_LEN);
    char *buf = calloc(1, MAX_LEN);

    size_t cur_len = 0;

    if (!str || !buf) goto memerr;
    
    if (!Strings) {
	log_err("Invalid list of strings");
	return NULL;
    }

    while (Strings) {

	cur_len += Strings->len + strlen(delimiter);

	if ( cur_len >= MAX_LEN ) {
	    log_warn("String size of %zuB exceeds max of %d",
		     cur_len, MAX_LEN);
	    break;
	}

	/* NOTE: ensure that cur_len is incrememted if any characters
	   are added to format string in the future */
	sprintf(str, "%s%s%s", buf, delimiter, Strings->str);
	strcpy(buf, str);
	Strings = Strings->Next;
    }

    free(buf);

    str = realloc(str, cur_len + 1);
    if (!str) goto memerr;
    str[cur_len] = '\0';
    
    return str;
 memerr:
    log_err("Memory Error");
    return NULL;
}

/* When reading the callback fails, an empty string is pushed to the
   list. This allows the remaining callbacks to be processed aswell as
   preventing failures during cleanup using (ssl_destroy()). Returns
   1. */
int
read_cb_failure(struct SLL **Strings)
{
    sll_push(Strings, strdup(""));
    log_warn("Pushing empty string");
    return EXIT_FAILURE;
}

/* Push string into linked list returns 0 on success or 1 on failure */
int
read_cb_success(struct SLL **Strings, char *str)
{
    return sll_push(Strings, str);
}

/* Generate a linked list of strings from callbacks, returns number of
   strings successfully read */
int
strings_generate(void *lib, char **cbname, size_t cbcount, struct SLL **List)
{
    size_t n = 0;		/* strings sucessfully read */
    getstr cb = NULL;		/* callback to get string */

    for (size_t i = 0; i < cbcount; ++i) {
	cb = read_cb(lib, cbname[i]);
	
	if (cb) {
	    if (!read_cb_success(List, cb()))
		++n;
	} else {
	    read_cb_failure(List);
	}
    }

    if (cbcount != n)
	log_warn("Failed to process %zu strings", cbcount - n);

    return n;
}

void
term(__attribute__((unused)) int signum)
{
    done = 1;
    return;
}
