/* Singley linked list for holding strings */

#ifndef DRN_SLL
#define DRN_SLL
struct SLL { char *str; size_t len; struct SLL *Next; };

/* Push a new node to the linked list
 * 
 * List: address of ptr to head node of a linking list
 * str:  pointer to string literal
 * 
 * Returns: 0 on success, 1 on failure
 */
int sll_push(struct SLL **List, char *str);

/* Free and destroy all nodes, and their elements, in the linked
 * list
 *
 * List: address of ptr to head node of a linked list
 *
 * Returns: number of nodes destroyed
 */
int sll_destroy(struct SLL **List);

#endif
	
    
