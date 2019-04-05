/* DRN_MAIN
 *
 * Updates the name of the root window in X.  Useful for dwm which
 * displays the name of the root window as a status bar.
 */

/* (c) Ellis Rhys Thomas <e.rhys.thomas@gmail.com>
 * See LICENCE file for details
 */

#include "drn.h"
#include "drn_x11.h"
#include "drn_dlsym.h"
#include "drn_signal.h"
#include "ert_log.h"
#include "stdlib.h"

#define CB_SO "lib/libdrn_cb.so"
#define MAX_LEN 100

/* Prepeare the environment and run the event loop.
 *
 * The event loop continuously reads the shared object file 'CB_SO'
 * using symbol names from argv. The retrieved strings are combined
 * with a delimiter and applied to the root window name.
 *
 * The event loop is exited and all resources cleaned up upon
 * receiving SIGTERM or SIGINT.
 * 
 */
int main(int argc, char *argv[])
{
    if (argc < 3) {
	log_err("USAGE: %s <delimiter> <callbacks...>\n", argv[0]);
	return EXIT_FAILURE;
    }

    void *so = open_library(CB_SO);
    if (!so)
	return EXIT_FAILURE;
    
    Display *Xdisp = open_display();
    if (!Xdisp) {
	close_library(Xdisp);
	return EXIT_FAILURE;
    }
    
    start_signal_handler();

    int EC = drn_loop(argc-2, argv+2, so, Xdisp, MAX_LEN, argv[1]);

    close_display(Xdisp);
    close_library(so);

    return EC;			/* number of failed callbacks */
}
