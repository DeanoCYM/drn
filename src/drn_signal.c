/* DRN_SIGNAL_C
 *
 * DRN functions for signal handling
 *
 * Suppression of interuppt and termination signal permits cleanup of
 * resources proir to exit.
 * 
 */

#include <signal.h>
#include <string.h>

volatile sig_atomic_t done = 0;

/* Handler sets done to one, should be called when signal is recieved */
static void
term(__attribute__((unused)) int signum)
{
    done = 1;
    return;
}

/* Blocks SIGINT and SIGTERM */
void
start_signal_handler(void)
{
    struct sigaction action;
    memset(&action, 0, sizeof action);
    action.sa_handler = term;
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);
    
   return;
}

/* Not yet implemented */
void
stop_signal_handler(void)
{
    return;
}

