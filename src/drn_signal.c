/* DRN_SIGNAL_C
 *
 * DRN functions for signal handling
 * 
 */

#include <signal.h>
#include <string.h>

volatile sig_atomic_t done = 0;

static void
term(__attribute__((unused)) int signum)
{
    done = 1;
    return;
}

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

void
stop_signal_handler(void)
{
    return;
}

