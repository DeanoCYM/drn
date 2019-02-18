/* DRN.H
 * (c) Ellis Rhys Thomas <e.rhys.thomas@gmail.com>
 * See LICENCE file for details
 */

#ifndef DRN
#define DRN

#include <X11/Xlib.h>
#include <stddef.h>

int
drn_loop(Display Xdisp,
	 void *so, char **symbols, size_t count,
	 char *delimiter, size_t max_len);

#endif	/* DRN */
