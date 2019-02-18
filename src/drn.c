/* DRN (dwm root name)
 *
 */

/* (c) Ellis Rhys Thomas <e.rhys.thomas@gmail.com>
 * See LICENCE file for details
 */

#include "drn.h"
#include "drn_strings.h"
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

/* Forward Declarations For Static Functions */

static int
callback_read_failure(struct List Strings);

static int
callback_read_success(struct List Strings, char *str);

/*
 *
 * ******************** Interface Functions ********************
 *
*/

/* Event loop for drn.
 * 
 * Repeatedly reads callbacks read from shared object 'so' that match all the
 * symbols in argv (symbols start at argv+2).
 *
 * X11 root window name is then updated after the strings are concatenated along
 *  with the delimiter.
 *
 * Returns zero on success or a count of the number of failures.  
 * */
int
drn_loop(size_t count, char **symbols, void *so,
	 Display *Xdisp,
	 size_t max_len, char *delimiter)
{
    int rc = 0;
    int passc = 0;
    char *rootname = NULL;
    struct List Strings = NULL;

    Strings = List_create(max_len, count, delimiter);
    if (!Strings)
	goto fail;

    do {

	passc = List_populate(Strings, so, symbols);
	rootname = List_combine(Strings);
	if (!rootname)
	    goto fail;

	set_rootname(Xdisp, rootname, Strings->combined_len);

	if (List_depopulate(Strings) != passc)
	    log_warn("Possible memory leak");
	free(rootname); rootname = NULL;

	sleep(5);

    } while (!done);		/* breaks on SIGINT OR SIGTERM */

    List_destroy(Strings);

    return passc;

 fail:
    List_destroy(Strings);
    log_err("Event loop fatal error");
    return -1;
}

/* Populate an array 'strings' with strigngs obtained from callbacks
 * to a shared object file 'so'.
 *
 * Returns number of strings successfully read */
static size_t
strings_populate(void *lib, char **cbname, size_t cbcount, char **strings)
{
    size_t n = 0;		/* strings sucessfully read */
    getstr callback = NULL;

    for (size_t i = 0; i < cbcount; ++i) {
	callback = callback_read(lib, cbname[i]);

	if (callback)
	    callback_read_success(strings, callback(), &n);
	else
	    callback_read_failure(strings);
    }

    if (cbcount != n)
	log_warn("Failed to process %zu strings", cbcount - n);

    return n;
}

/*
 *
 * ******************** Static Functions ********************
 *
*/

/* Combine all strings seperated by the delimiter. Returns combined
   string or NULL on error. */
static char *
strings_combine(struct List *Strings, char *delimiter, size_t maxlen)
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

/* When reading the callback fails, an empty string is added to the
   array. This allows the remaining callbacks to be processed. */
static int
read_cb_failure(struct List **strings)
{
    
    
    sll_push(Strings, strdup(""));
    log_warn("Failed to process string, pushing empty string");
    return EXIT_FAILURE;
}

/* Push string into linked list returns 0 on success or 1 on failure */
static int
read_cb_success(struct list Strings, char *str, size_t *n)
{
    log_debug("Pushed %s", str);
    int rc = list_push(Strings, str);
    if (rc)
	return EXIT_FAILURE;

    ++(*n);
    return EXIT_SUCCESS;
}

