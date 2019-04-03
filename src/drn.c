/* DRN (dwm root name)
 *
 * Event loop for drn.
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

/* Event loop for drn.
 * 
 * Repeatedly reads callbacks read from shared object 'so' that match all the
 * symbols in argv (symbols start at argv+2 in main).
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
    int passc = 0;
    char *rootname = NULL;
    struct List *Strings = NULL;

    Strings = List_create(max_len, count, delimiter);
    if (!Strings)
	goto fail;

    do {
	passc = List_populate(Strings, so, symbols);

	rootname = List_combine(Strings);
	if (!rootname)
	    goto fail;

	set_rootname(Xdisp, rootname, Strings->max_len * Strings->count);

	List_depopulate(Strings);
	free(rootname);
	rootname = NULL;
	sleep(5);

    } while (!done);		/* breaks on SIGINT OR SIGTERM */

    List_destroy(Strings);

    return passc;

 fail:
    List_destroy(Strings);
    log_err("Event loop fatal error");
    return -1;
}
