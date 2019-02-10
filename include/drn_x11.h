/* DRN_X11.H
 *
 * DRN functions that interact with the X server
 * 
 */
#ifndef DRN_X11_H
#define DRN_X11_H

#include <X11/Xlib.h>
#include <stddef.h>

/* Used to open and close the default X display */
Display *open_display(void);
void close_display(Display *Xdisp);

/* Set the name of the root windown in display 'Xdisp' to string 'str' of
   maximum length 'len' */
void set_rootname(Display *Xdisp, const char *str, size_t len);

#endif
