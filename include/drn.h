/* DRN.H
 * (c) Ellis Rhys Thomas <e.rhys.thomas@gmail.com>
 * See LICENCE file for details
 */

#ifndef DRN
#define DRN

#include <X11/Xlib.h>
#include <stddef.h>

int
drn_loop(size_t count, char **symbols, void *so,
	 Display *Xdisp,
	 size_t max_len, char *delimiter);

#endif	/* DRN */
