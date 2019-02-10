/* DRN_X11.C
 *
 * DRN functions that interact with the X server
 * 
 */

#include "drn_x11.h"
#include "ert_log.h"
#include <stdlib.h>
#include <X11/Xlib.h>

Display *
open_display(void)
{
    Display *Xdisp = XOpenDisplay(NULL); /* NULL for default display */
    if (!Xdisp) {
	log_err("Failed to open X display");
    }

    return Xdisp;
}

void
close_display(Display *Xdisp)
{
    if(XCloseDisplay(Xdisp))
	log_warn("Failed to close X display");

    return;
}

void
set_rootname(Display *Xdisp, const char *str, size_t len)
{
    if (!(str[len] == '\0')) {
	log_err("String is not null terminated");
	exit(1);
    }

    XStoreName(Xdisp, DefaultRootWindow(Xdisp), str);
    XSync(Xdisp, False);

    log_info("Root window name set: %s", str);
    
    return;
}
