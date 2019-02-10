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
#include "ert_log.h"
#include "stdlib.h"

#define CB_SO "lib/libdrn_cb.so"
#define MAX_LEN 100

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

    int EC = drn_loop(argc-2, argv+2, so, Xdisp, MAX_LEN);

    close_display(Xdisp);
    close_library(so);

    return EC;
}
