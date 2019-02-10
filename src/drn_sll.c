/* DRN_SSL.C
 * 
 * Implements a simple singly linked list to manage strings
 *
 * */

#include <stdio.h>
#include <stdlib.h>

#include "ert_log.h"
#include "drn_sll.h"

#define NULLFREE "Attempt made to free NULL pointer"

struct Node { char *str; size_t len; struct SLL *next; };

List *
list_create(size_t max_len)
{
    List *list;
    list = calloc(1, sizeof *list);
    if (!list) {
	log_err("Memory error");
	return NULL;
    }

    list->max_len = max_len;
    list->cur_len = 0;
    list->nodec = 0;
    list->Head = NULL;

    return list;
}
    
int
list_destroy(struct List *list);
{
    if (!list) {
	log_err("Invalid list");
	return -1;
    }

    int n = 0;

    do {
	struct List next = list->node->next;
	if (list->node) {
	    free(list->node);
	    list->node = NULL;
	    ++n;
	}
    } while ( (list->head) = next);

    if (list->nodec != n)
	log_warn("Invalid list, possible memory leak");

    list->max_len = 0;
    list->cur_len = 0;
    list->nodec = 0;
    free(list);
    list = NULL;

    return n;
}	

int
list_push(struct List *list, const char *str);
{
    struct Node *new;
    new = calloc(1, sizeof *new);
    
    new->len = strlen(str);
    new->string = strndup(str, new->len);
    new->next = list->head;

    list->head = new;
    list->cur_len += new->len;
    ++(list->nodec);
}



    
    
