/* Routines for a linked list describing a solution to the maze (in reverse
   order). Items in the list are integers representing the position within the
   maze. */

#ifndef __PATH_H__
#define __PATH_H__

#include "position.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct list{
    position_t position;
    struct list *next;
} list_t;


/* Add the given position to the front of a list */
list_t * list_add (position_t position, list_t* current){
    list_t *added = (list_t*)malloc(sizeof(list_t));
    if (added == NULL){
        fprintf(stderr, "Error: unable to allocate required memory\n");
        return;
    }     
    added->next = NULL;
	added->position = position;
    return added;
}

/* Get the front of the list (the most recently added item) */
position_t list_front (list_t* current){
    return current->position;
}


/* Prints the given list in reverse order */
void list_print_reverse (list_t* list){
    // Base case
    if (list == NULL)   return;
    // print the list after head node
    printReverse(list->next);
    // After everything else is printed, print head
	printf("%d,%d\n", list->position.row,list->position.col);
}

/* Releases all the memory associated with the list. */
void list_free ( list_t* list){
    list_t *current, *prev;
	current = list;
	while(current != NULL) {
		prev = current;
		current = current->next;
		free(prev);
	}
}

#endif