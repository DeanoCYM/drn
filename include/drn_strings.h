/* Singley linked list for holding strings */

#ifndef DRN_STRINGS
#define DRN_STRINGS

#include <stddef.h>

struct List {
    struct Node *Head;		/* linked list of all strings */
    size_t max_len;		/* Max number of characters in string */
    size_t combined_len;        /* Number of chars in all strings and delimiter */
    size_t count;		/* Node count */
    char *delimiter		/* Delimiter to be applied to strings */
};
    
/* Allocate memory for an empty list of strings.
 *
 * Returns NULL on failure. */
struct List *List_create(size_t max_len, size_t count, char *delimiter);

/* Populates the 'List' structure with nodes containing strings read
 * from the shared object 'so' using the strings 'symbols'.
 *
 * Returns number of nodes read or -1 on critical error */
int List_populate(struct List *Strings,
		  void *so, char **symbols, size_t count);

/* Combines each of the node strings into a single string separated
 * the delimiter.
 *
 * String will have a number of characters List->cur_len
 *
 * Returns the combined string or NULL on error */
char * List_combine(struct List *Strings);

/* Free all memory in all List Nodes and reset counters. Delimiter is
 * retained. */
void List_depopulate(struct List *Strings);

/* Deallocate List memory and all nodes, will not fail if Strings is
   invalid or NULL */
void List_destroy(struct List *Strings);

