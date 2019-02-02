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
#include <signal.h>
#include <unistd.h>

#include "ert_log.h"
#include "drn_sll.h"

#define MAX_LEN 100

#define CB_SO "lib/libdrn_cb.so"
typedef char *(*getstr)(void);

volatile sig_atomic_t done = 0;

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


/* When reading the callback fails, an empty string is pushed to the
   list. This allows the remaining callbacks to be processed aswell as
   preventing failures during cleanup using (ssl_destroy()). Returns
   1. */
int
read_cb_failure(struct SLL **Strings)
{
    sll_push(Strings, strdup(""));
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
strings_generate(void *lib, char **cbname, int cbcount, struct SLL **List)
{
    int n = 0;			/* strings sucessfully read */
    getstr cb = NULL;		/* callback to get string */

    for (int i = 0; i < cbcount; ++i) {
	cb = read_cb(lib, cbname[n]);
	
	if (cb) {
	    if (!read_cb_success(List, cb()))
		++n;
	} else {
	    read_cb_failure(List);
	}
    }

    return n;
}

void term(int signum)
{
    done = 1;
}

int main(int argc, char *argv[])
{
    int EC = EXIT_SUCCESS;	/* exit code */
    void *libdrn_cb;		/* shared object used to get callbacks */
    struct SLL *Strings = NULL;	/* linked list of processed strings */
    char *mes;		      /* delimiter and message for printing */

    if (argc < 3) {
	log_err("USAGE: %s <delimiter> <callbacks...>", argv[0]);
	EC = EXIT_FAILURE;
	goto out1;
    }

    if (!(libdrn_cb = dlopen(CB_SO, RTLD_LAZY))) {
	log_err("Shared object %s could not be opened", CB_SO);
	EC = EXIT_FAILURE;
	goto out1;
    }
    
    Display *xdefault = open_display();
    if (!xdefault) {
	EC = EXIT_FAILURE;
	goto out2;
    }
    
    struct sigaction action;
    memset(&action, 0, sizeof action);
    action.sa_handler = term;
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);

    do {			/* event loop */

	if (strings_generate(libdrn_cb, argv+2, argc-2, &Strings) < argc-2) {
	    log_warn("Not all strings processed");
	    EC = EXIT_FAILURE;
	}

	mes = strings_combine(Strings, argv[1]);
	if (!mes) {
	    log_err("Failed to process strings");
	    EC = EXIT_FAILURE;
	    goto out3;
	}

	set_rootname(xdefault, mes, strlen(mes));

	sll_destroy(&Strings);
	free(mes); mes = NULL;

	sleep(5);

    } while (!done);		/* breaks on SIGINT OR SIGTERM */

    /* Cleanup */

 out3:
    close_display(xdefault);
 out2:
    if (dlclose(libdrn_cb))
	log_warn("Failed to close %s", CB_SO);
 out1:
    return EC;
}
