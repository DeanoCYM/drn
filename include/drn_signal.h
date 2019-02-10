/* DRN_SIGNAL_H
 *
 * DRN functions for signal handling
 * 
 */

#ifndef DRN_SIGNAL_H
#define DRN_SIGNAL_H

#include <signal.h>

extern volatile sig_atomic_t done;

/* Blocks SIGINT and SIGTERM */
void start_signal_handler(void);
void stop_signal_handler(void);
#endif
