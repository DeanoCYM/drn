/* DRN_STRINGS.C
 * 
 * Implements a simple singly linked list to manage strings
 *
 * */

#include "drn_strings.h"
#include "drn_dlsym.h"
#include "ert_log.h"

#include <stdio.h>
#include <stdlib.h>

struct Node {
    char *string;
    size_t len;
    struct Node *Next;
};

static struct Node *Node_create(char *string, size_t len);
static void List_append(struct List Strings, struct Node Head);

struct List *
List_create(size_t max_len, size_t count, const char *delimiter)
{
    if (!count) {
	log_err("Invalid string count; count must be greater than 0");
	return NULL;
    }

    if (!delimiter) {
	log_err("No delimiter provided");
	return NULL;
    }
    
    struct List *Strings = calloc(count, sizeof *Strings);
    if (!Strings) {
	log_err("Memory error");
	return NULL;
    }

    Strings->max_len = max_len;
    Strings->count = count;
    Strings->delimiter = strndup(delimiter, max_len + 1);

    /* There will be count-1 delimiters required */
    if (max_len < (count - 1) * strnlen(delimiter, max_len)) {
	log_err("Delimiter is too long");
	List_destroy(Strings);
	return NULL;
    }

    Strings->combined_len = (count - 1) * strnlen(delimiter, max_len);

    return Strings;
}

int
List_populate(struct List *Strings,
	      void *so, char *symbols, size_t count)
{
    int nodec = 0;
    getstr callback;
    char *string;
    size_t len;
    
    for (nodec = 0; nodec < count; ++nodec) {

	so_callback = read_cb(so, symbol[nodec]);
	string = so_callback();

	len = strnlen(string, Strings->max_len);
	if (Strings->max_len == len)
	    log_warn("Overly long string: '%s'", string);

	string[len] = '\0';

	struct Node *Head = Node_create(string, len);
	List_append(List, Head)
	
    }

    return nodec;
}

char *
List_combine(struct List *Strings)
{
    char *string = calloc(Strings->combined_len, 1);

    strncat(string, List_pop_node, List->conbined_len);
    
    for (int i = 0, i < 

    return;
}

static struct Node *
Node_create(char *string, size_t len)
{
    struct Node *Node = calloc(1, sizeof *Node);
    if (!Node) {
	log_err("Memory error");
	return NULL;
    }

    Node->string = strndup(string, len);
    Node->len = len;

    return Node;
}

static
List_append(struct List *Strings, struct Node *Head)
{
    if (!Strings || !Head) {
	log_err("Invalid List");
	return;
    }

    if ( Strings->max_len < (Strings->combined_len + Head->len)) {
	log_warn("Failed to process all strings (out of buffer space)");
	return;
    }

    Strings->combined_len += Head->len;

    Head->Next = Strings->Head;
    Strings->Head = Head;

    return;
}
