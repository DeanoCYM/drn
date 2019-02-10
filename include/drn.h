/* DRN.H
 * (c) Ellis Rhys Thomas <e.rhys.thomas@gmail.com>
 * See LICENCE file for details
 */

#ifndef DRN
#define DRN

#include <X11/Xlib.h>
#include <stddef.h>

/* Blocking signals to allow cleanup before termination */
void start_signal_handler(void);

int drn_loop(int argc, char **argv, void *so, Display *Xdisp, size_t maxlen);

#endif	/* DRN */
