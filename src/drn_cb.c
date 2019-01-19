#include <stdio.h>
#include <stdlib.h>
#include "ert_log.h"

#define MAXSTR 200+1
#define MAXPATH 2000+1

char *
create_buf(size_t size)
{
    char *buf = calloc(sizeof size);
    if (!buf) {
	log_err("Memory Error");
	buf = NULL;
    }

    return buf;
}
    

int
sysfs_read(const char *pathname, char **sys_out)
{
    char *pathname = create_buf(MAXPATH);
    if (!pathname) goto err1;
    
    FILE *sysfile = fopen(pathname, "r");
    if (!sysfile) {
	log_err("Failed to open sysfile %s", pathname);
	goto err2;
    }

    if ( !fgets(*sys_out, MAXSTR, sysfile) ) {
	log_err("Failed to read sysfile %s", pathname);
	goto err3;
    }

    int rc = fclose(sysfile);
    if (!rc) log_warn("Failed to close sysfile");

    free(s
    

    return 0;

 err3:
    fclose(sysfile);
 err2:
    free(str);
 err1:
    return 1;
}

/* Returns capacity from SYS_BAT0/capacity */
char *
drn_test1(void)
{
    //char *str = sysfs_read("/sys/class/power_supply/BAT0/capacity");

    return "God";
}

char *
drn_test2(void)
{
    return "My";
}
char *
drn_test3(void)
{
    return "Oh";
}

    


