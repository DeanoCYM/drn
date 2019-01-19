#include <stdio.h>
#include <stdlib.h>

#include "ert_log.h"
#include "drn_sll.h"

#define NULLFREE "Attempt made to free NULL pointer"

static void
free_str(char *str)
{
    if (str) {
	free(str);
	str = NULL;
    } else {
	log_warn(NULLFREE);
    }

    return;
}
static void
free_sll(struct SLL *Head)
{
    if (Head) {
	Head->len = 0;
	free_str(Head->str);
	Head->Next = NULL;
	free(Head);
    } else {
	log_warn(NULLFREE);
    }

    return;
}

/* Frees the memory of, and sets to 0, all elements of all nodes in
   the linked list. Returns the number of nodes destroyed. */
int
sll_destroy(struct SLL **Next)
{
    int n;
    struct SLL *Head;

    if (!Next) {
	log_warn(NULLFREE);
	return 0;
    }

    for (n = 0; (Head = *Next) && Head->str; ++n) {
	*Next = (*Next)->Next;
	free_sll(Head);
    }

    *Next = NULL;

    return n;
}

int
sll_push(struct SLL **Next, char *str)
{
    struct SLL *Head;

    if (!Next) {
	log_err("Cannot dereference list, has it been initialised?");
	return 1;
    }

    if (!str) {
	log_err("Invalid string");
	return 1;
    }

    Head = malloc(sizeof (struct SLL));
    if (!Head) {
	log_err("Memory error");
	return 1;
    }

    Head->len = strlen(str);
    Head->str = str;
    Head->Next = *Next;
    *Next = Head;
    
    return 0;
}
