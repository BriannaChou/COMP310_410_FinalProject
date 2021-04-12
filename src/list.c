
#include <stddef.h>
#include "list.h"


/*
 * listAdd
 *
 * Adds newElement to a linked list pointed to by list. When calling this function, pass the address of list head.
 *
 */
void listAdd(struct listElement **head, struct listElement *newElement){
    struct listElement *iterator = (struct listElement*)head ;

    // Link element b into the list between iterator and iterator->next.
    newElement->next = iterator->next ;
    newElement->prev = iterator ;

    iterator->next = newElement ;
    
    if(newElement->next != NULL){
        newElement->next->prev = newElement ;
    }
}


/*
 * listDelete
 *
 * Deletes an element from a doubly linked list.
 */
void listRemove(struct listElement *b)
{
	if(b->next != NULL)
		b->next->prev = b->prev ;
	
	b->prev->next = b->next ;

	// NULLify the element's next and prev pointers to indicate
	// that it is not linked into a list.
	b->next = NULL ;
	b->prev = NULL ;
}



	
