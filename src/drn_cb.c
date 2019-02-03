#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ert_log.h"

#define MAXSTR 200+1
#define MAXPATH 2000+1

#define BAT_CAPACITY "/sys/class/power_supply/BAT0/capacity"
#define BAT_STATUS "/sys/class/power_supply/BAT0/status"

static char *
create_buf(size_t size)
{
    char *buf = calloc(1, size);
    if (!buf) {
	log_err("Memory Error");
	buf = NULL;
    }

    return buf;
}
    
/* Reads a line from stream into str_out, recording the number of
   characters into len_out */
static int
read_line(FILE *stream, char **str_out, size_t *len_out)
{
    if (!fgets(*str_out, *len_out, stream)) {
	log_err("Failed to read file");
	return 1;
    }

    /* remove newline character if present */
    *len_out = strnlen(*str_out, *len_out);
    if (((*str_out)[*len_out - 1]) == '\n') {
	(*str_out)[*len_out - 1] = '\0';
	--(*len_out);
    }

    log_debug("String: %s, Length: %zu", *str_out, *len_out);

    return 0;
}

static int
read_sysfs(const char *pathname, char **sys_out, size_t *len_out)
{
    if (!pathname || !sys_out || !(*sys_out)) {
	log_err("Invalid arguments");
	return 1;
    }

    FILE *sysfile = fopen(pathname, "r");
    if (!sysfile) {
	log_err("Failed to open %s", pathname);
	return 1;
    }

    if (read_line(sysfile, sys_out, len_out)) {
	log_err("Failed to read %s", pathname);
	return 1;
    }

    if (fclose(sysfile))
	log_warn("Failed to close %s", pathname);

    return 0;
}

/* Battery capacity */
char *
batt_capacity()
{
    size_t capl = MAXSTR;
    char *capacity = create_buf(capl);
    if (!capacity) return NULL;
    if (read_sysfs(BAT_CAPACITY, &capacity, &capl)) return NULL;
    
    const char *units = "%";

    if ((MAXSTR - capl) < strlen(units)) {
	log_warn("Buffer too full to append units");	
	return capacity;
    }

    return strncat(capacity, units, MAXSTR);
}

/* Battery status (charging or discharging) */
char *
batt_status()
{
    size_t statl = MAXSTR;
    char *status = create_buf(statl);
    if (!status) return NULL;
    if (read_sysfs(BAT_STATUS, &status, &statl)) return NULL;

    return status;
}

char *
local_time()
{
    char *disptime = create_buf(MAXSTR);
    if (!disptime) return NULL;

    /* simple time is the number of seconds elapsed since 00:00:00 on
       January 1, 1970, Coordinated Universal Time */
    const time_t simpletime = time(NULL);
    if ((time_t)(-1) == simpletime) goto out;

    /* broken down time is a binary representation of calendar time
       separated into year, month, day, and so on */
    struct tm *brokendowntime = localtime(&simpletime);
    if (!brokendowntime) goto out;

    if (!(strftime(disptime, MAXSTR, "%a %d/%m/%y %I:%M%p", brokendowntime)))
	goto out;

    return disptime;
 out:
    log_err("Cannot determine time");
    free(disptime);
    return NULL;
}
    


