/* Singley linked list for holding strings */

#ifndef DRN_STRINGS
#define DRN_STRINGS

#include <stddef.h>

struct List {
    char **strings;
    size_t count;
    size_t max_len;		/* Max char in a string */
    char *delimiter;		/* Delimiter to be applied to strings */
};
    
/* Allocate required memory for an empty structure.
 *
 * Returns NULL on failure. */
struct List *List_create(size_t max_len, size_t count, const char *delimiter);

/* Populates the 'List' structure with strings read from the shared
 * object 'so' using the strings 'symbols'.
 *
 * Returns number of strings successfully read */
int List_populate(struct List *Strings, void *so, char **symbols);

/* Combines each string in List struct into a single string separated
 * the delimiter.
 *
 * Returns the combined string or NULL on error */
char * List_combine(struct List *Strings);

/* Free all memory strings, retains count */
void List_depopulate(struct List *Strings);

/* Deallocate List memory */
void List_destroy(struct List *Strings);

#endif
