/* DRN_MAIN
 *
 * Updates the name of the root window in X.  Useful for dwm which
 * displays the name of the root window as a status bar.
 */

/* (c) Ellis Rhys Thomas <e.rhys.thomas@gmail.com>
 * See LICENCE file for details
 */

#include <dlfcn.h>
#include <unistd.h>
#include "ert_log.h"
#include "drn.h"

volatile sig_atomic_t done = 0;

int main(int argc, char *argv[])
{
    int EC = EXIT_SUCCESS;	/* exit code */

    if (argc < 3) {
	log_err("USAGE: %s <delimiter> <callbacks...>", argv[0]);
	EC = EXIT_FAILURE;
	goto out1;
    }

    /* Shared object used to get callbacks */
    void *libdrn_cb;		
    if (! (libdrn_cb = dlopen(CB_SO, RTLD_LAZY)) ) {
	log_err("Shared object %s could not be opened", CB_SO);
	EC = EXIT_FAILURE;
	goto out1;
    }
    
    /* Defualt X11 display */
    Display *xdefault = open_display();
    if (!xdefault) {
	EC = EXIT_FAILURE;
	goto out2;
    }
    
    /* Blocking of singals allows clean exit from event loop */
    struct sigaction action;
    memset(&action, 0, sizeof action);
    action.sa_handler = term;
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);

    struct SLL *Strings = NULL;	/* linked list of processed strings */
    char *rootname = NULL;	/* string to be printed */

    do {			/* event loop */

	if ( strings_generate(libdrn_cb, argv+2, argc-2, &Strings) < argc-2 )
	    EC = EXIT_FAILURE;

	rootname = strings_combine(Strings, argv[1]);
	if (!rootname) {
	    log_err("Failed to process strings");
	    EC = EXIT_FAILURE;
	    goto out3;
	}

	set_rootname(xdefault, rootname, strlen(rootname));

	if ( sll_destroy(&Strings) < argc-2)
	    log_warn("Possible memory leak");
	free(rootname); rootname = NULL;

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
