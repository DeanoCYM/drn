/* Singley linked list for holding strings */

#ifndef DRN_SLL
#define DRN_SLL

#include <stddef.h>

struct List {
    size_t max_len;
    size_t cur_len;
    size_t nodec;
    struct Node *Head;
};
    
/* Create an empty list
 *
 * max_len: maxcumilative length of strings in all nodes (excluding
 * null bytes)
 * */
struct List * List_create(size_t max_len);

/* Push a new node to the linked list
 * 
 * List: address of ptr to head node of a linked list
 * str:  pointer to string literal
 * 
 * Returns: 0 on success, 1 on failure */
int List_push(struct List *list, char *str);

/* Free and destroy all nodes, and their elements, in the linked
 * list
 *
 * List: address of ptr to head node of a linked list
 *
 * Returns: number of nodes destroyed */
int List_destroy(struct List *list);

#endif
