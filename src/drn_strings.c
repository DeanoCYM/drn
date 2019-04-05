/* DRN_STRINGS.C
 * 
 * Implements a structure used to manage strings used by drn
 *
 * */

#include "drn_strings.h"
#include "drn_dlsym.h"
#include "ert_log.h"

#include <stdio.h>
#include <stdlib.h>

struct List *
List_create(size_t max_len, size_t count, const char *delimiter)
{
    if (!count) {
	log_err("Invalid string count; count must be greater than 0");
	return NULL;
    }

    if (!delimiter) {
	log_err("No delimiter provided, use \"\" for empty string.");
	return NULL;
    }

    struct List *Strings = calloc(1, sizeof *Strings);
    if (!Strings) {
	log_err("Memory error");
	return NULL;
    }

    Strings->strings = calloc(count, sizeof *Strings->strings);
    if (!Strings->strings) {
	log_err("Memory error");
	return NULL;
    }

    Strings->count = count;
    Strings->max_len = max_len;
    Strings->delimiter = strndup(delimiter, max_len + 1);

    return Strings;
}

int
List_populate(struct List *Strings, void *so, char **symbols)
{
    size_t i = 0;
    getstr so_callback;
    char *string = NULL;

    for (i = 0; i < Strings->count; ++i) {

	/* Read callback */
	so_callback = read_cb(so, symbols[i]);
	if (so_callback) {
	    string = so_callback();
	}

	if (string) {		/* check if valid string */
	    Strings->strings[i] = strndup(string, Strings->max_len);
	    free(string);
	    string = NULL;
	} else {
	    Strings->strings[i] = strdup("");
	    log_warn("Empty string appended");
	}
    }

    return i;
}

char *
List_combine(struct List *Strings)
{
    char *buf = calloc(Strings->count * (Strings->max_len + 1), 1);
    if (!buf) {
	log_err("Memory error");
	return NULL;
    }

    for (size_t i = 0; i < Strings->count; ++i) {
	strncat(buf, Strings->strings[i], Strings->max_len);
	if (i != Strings->count - 1)
	    strncat(buf, Strings->delimiter, Strings->max_len);
    }

    return buf;
}

void
List_depopulate(struct List *Strings)
{
    if (!Strings) {
	log_warn("Invalid List");
	return;
    }

    for (size_t i = 0; i < Strings->count; i++)
	if(Strings->strings[i]) {
	    free(Strings->strings[i]);
	    Strings->strings[i] = NULL;
	}

    return;
}

void
List_destroy(struct List *Strings)
{
    if (!Strings) {
	log_warn("Invalid List");
	return;
    }

    List_depopulate(Strings);

    Strings->max_len = 0;

    if (Strings->delimiter) {
	free(Strings->delimiter);
	Strings->delimiter = NULL;
    }

    Strings->count = 0;

    if (Strings->strings) {
	free(Strings->strings);
	Strings->strings = NULL;
    }
    
    free(Strings);
    Strings = NULL;

    return;
}
    
