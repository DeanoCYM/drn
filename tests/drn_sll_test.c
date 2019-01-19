#include <stdio.h>
#include <stdlib.h>

#include "ert_log.h"
#include "drn_sll.h"

int main(int argc, char *argv[])
{
    log_info("argc == %d", argc);

    int i;
    struct SLL *Print, *Head = NULL;
    
    for (i = 1; i < argc; ++i)
	sll_push(&Head, argv[i]);

    for (i = 1, Print = Head; Print; ++i, Print = Print->Next)
	log_info("Node %d \"%s\" (len %zuB)", i, Print->str, Print->len);

    sll_destroy(&Head);

    return 0;
}
