#include "drn_x11.h"
#include "ert_log.h"

int
main(int argc, char *argv[])
{
    Display *x = open_display();
    if (!x) {
	log_err("Failed to open display");
	return 1;
    } else {
	log_info("Display successfully opened");
    }

    set_rootname(x, "Hi", 3);
    
    close_display(x);

    return 0;
}
