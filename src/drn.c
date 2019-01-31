/* DRN (dwm root name)
 *
 * Updates the name of the root window in X.  Useful for dwm which
 * displays the name of the root window as a status bar.
 */

/* (c) Ellis Rhys Thomas <e.rhys.thomas@gmail.com>
 * See LICENCE file for details
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <dlfcn.h>
#include <X11/Xlib.h>

#include "ert_log.h"
#include "drn_sll.h"

#define MAX_LEN 100

#define CB_SO "lib/libdrn_cb.so"
typedef char *(*getstr)(void);

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
    
/* Combine all strings seperated by the delimiter */
char *
strings_combine(struct SLL *Strings, char *delimiter)
{
    size_t cur_len = 0;
    char *str = calloc(1, MAX_LEN);
    char *buf = calloc(1, MAX_LEN);

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

	/* NOTE: future format string alterations: ensure that cur_len
	   is incrememted if any characters are added */
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


/* When reading the callback fails, an empty string is pushed to the list. This allows the remaining callbacks to be processed aswell as preventing failures during cleanup using (ssl_destroy()). Returns 1. */
int
read_cb_failure(struct SLL **Strings)
{
    sll_push(Strings, strdup(""));
    return EXIT_FAILURE;
}

/* Push string into linked list */
int
read_cb_success(struct SLL **Strings, char *str)
{
    return sll_push(Strings, str);
}


int main(int argc, char *argv[])
{
    int EC = EXIT_SUCCESS;	/* exit code */
    void *libdrn_cb;		/* shared object used to get callbacks */
    getstr cb;			/* callback from shared object */
    struct SLL *Strings = NULL;	/* linked list of processed strings */
    char *del, *mes;		/* delimiter and message for printing */

    if (argc < 3) {
	log_err("USAGE: %s <delimiter> <callbacks...>", argv[0]);
	EC = EXIT_FAILURE;
	goto out1;
    }

    libdrn_cb = dlopen(CB_SO, RTLD_LAZY);
    del = argv[1];

    for (int i = 0; i < argc - 2; ++i) {

	cb = read_cb(libdrn_cb, argv[i + 2]);

	if (!cb) {
	    EC = read_cb_failure(&Strings);
	} else {
	    EC = read_cb_success(&Strings, cb());
	    if (EC) goto out2;
	}
    }

    Display *xdefault = open_display();
    if (!xdefault) {
	    EC = EXIT_FAILURE;
	    goto out2;
    }
    
    /* combine each string with the delimiter */
    mes = strings_combine(Strings, del);
    if (!mes) {
	log_err("Failed to process strings");
	goto out3;
    }

    // send concatinated string to X
    set_rootname(xdefault, mes, strlen(mes));

    free(mes);

 out3:
    if (sll_destroy(&Strings) != argc - 2)
	log_warn("Possible memory leak");
    close_display(xdefault);

 out2:
    dlclose(libdrn_cb);

 out1:
    return EC;
}
