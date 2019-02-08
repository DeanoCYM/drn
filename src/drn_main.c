/* DRN_MAIN
 *
 * Updates the name of the root window in X.  Useful for dwm which
 * displays the name of the root window as a status bar.
 */

/* (c) Ellis Rhys Thomas <e.rhys.thomas@gmail.com>
 * See LICENCE file for details
 */

#include "config.h"
#include "drn.h"
#include "ert_log.h"

int main(int argc, char *argv[])
{
    if (argc < 3) {
	log_err("USAGE: %s <delimiter> <callbacks...>", argv[0]);
	return EXIT_FAILURE;
    }

    void *so = open_library(CB_SO);
    if (!so)
	return EXIT_FAILURE;
    
    Display *xdefault = open_display();
    if (!xdefault) {
	close_library(xdefault);
	return EXIT_FAILURE;
    }
    
    start_signal_handler();

    int EC = drn_loop(argc, argv, so);

    close_display(xdefault);
    close_library(so);

    return EC;
}
