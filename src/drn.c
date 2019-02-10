/* DRN (dwm root name)
 *
 */

/* (c) Ellis Rhys Thomas <e.rhys.thomas@gmail.com>
 * See LICENCE file for details
 */

#include "drn.h"
#include "drn_sll.h"		/* singly linked list */
#include "drn_x11.h"		/* x11 display */
#include "drn_dlsym.h"		/* shared object */
#include "drn_signal.h"		/* signal blocking */
#include "ert_log.h"		/* error logs */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <signal.h>
#include <X11/Xlib.h>

/* Forward declarations for static functions */

static int
read_cb_failure(struct SLL **Strings);

static int
read_cb_success(struct SLL **Strings, char *str);

static char *
strings_combine(struct SLL *Strings, char *delimiter, size_t maxlen);

static int
strings_generate(void *lib, char **cbname, size_t cbcount, struct SLL **List);

/* Extern functions */

/* Event loop for drn.
 * 
 * Repeatedly reads callbacks read from shared object 'so' that match all the
 * symbols in argv (symbols start at argv+2).
 *
 * X11 root window name is then updated after the strings are concatenated along
 *  with the delimiter.
 *
 * Returns a count of the number of failures.  
 * */
int
drn_loop(int count, char **sym, void *so, Display *Xdisp, size_t max_len)
{
    int rc = EXIT_SUCCESS;
    char *rootname = NULL;
    struct List *strings = List_create(max_len);


    do {			/* event loop */

	strings_generate(so, sym, count, strings);

	rootname = strings_combine(Strings, argv[1], maxlen);
	if (!rootname) {
	    log_err("Failed to process strings");
	    rc = EXIT_FAILURE;
	    break;
	}

	set_rootname(Xdisp, rootname, strlen(rootname));

	rc = sll_destroy(&Strings);
	if (rc < argc-2)
	    log_warn("Possible memory leak");

	free(rootname); rootname = NULL;

	sleep(5);

    } while (!done);		/* breaks on SIGINT OR SIGTERM */

    return rc;
}

/* Generate a linked list of strings from callbacks, returns number of
   strings successfully read */
int
strings_generate(void *lib, char **cbname, size_t cbcount, struct List strings)
{
    size_t n = 0;		/* strings sucessfully read */
    getstr cb = NULL;		/* callback to get string */

    for (size_t i = 0; i < cbcount; ++i) {
	cb = read_cb(lib, cbname[i]);

	if (cb)
	    read_cb_success(strings, cb(), &n);
	else
	    read_cb_failure(strings);
    }

    if (cbcount != n)
	log_warn("Failed to process %zu strings", cbcount - n);

    return n;
}

/* Static functions */

/* Combine all strings seperated by the delimiter. Returns combined
   string or NULL on error. */
static char *
strings_combine(struct SLL *Strings, char *delimiter, size_t maxlen)
{
    char *str = calloc(1, maxlen);
    char *buf = calloc(1, maxlen);

    size_t cur_len = 0;

    if (!str || !buf) goto memerr;
    
    if (!Strings) {
	log_err("Invalid list of strings");
	return NULL;
    }

    while (Strings) {

	cur_len += Strings->len + strlen(delimiter);

	if ( cur_len >= maxlen ) {
	    log_warn("String size of %zuB exceeds max of %zuB",
		     cur_len, maxlen);
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
static int
read_cb_failure(struct SLL **Strings)
{
    sll_push(Strings, strdup(""));
    log_warn("Failed to process string, pushing empty string");
    return EXIT_FAILURE;
}

/* Push string into linked list returns 0 on success or 1 on failure */
static int
read_cb_success(struct SLL **Strings, char *str, size_t *n)
{
    log_debug("Pushed %s", str);
    int rc = list_push(Strings, str);
    if (rc)
	return EXIT_FAILURE;

    ++(*n);
    return EXIT_SUCCESS;
}

