/* Routines for a linked list describing a solution to the maze (in reverse
   order). Items in the list are integers representing the position within the
   maze. */

#ifndef __PATH_H__
#define __PATH_H__

#include "position.h"

typedef struct list{
    position_t position;
    struct list *next;
} list_t;


/* Add the given position to the front of a list */
list_t * list_add (position_t position, list_t* current){
    list_t *added = (list_t*)malloc(sizeof(list_t));
    if (added == NULL){
        fprintf(stderr, "Error: unable to allocate required memory\n");
        return NULL;
    }
    added->next = current;
	added->position = position;
    return added;
}

/* Get the front of the list (the most recently added item) */
position_t list_front (list_t* current){
    list_t *front;
    front = current->next;
    while (front->next==NULL)
    {
        front = front->next;
    }
    return front->position;
}


/* Prints the given list in reverse order */
void list_print_reverse (list_t* list){
    // Base case
    if (!list)   return;
    // print the list after head node
    list_print_reverse(list->next);
    // After everything else is printed, print head
	printf("(%d,%d)", list->position.row,list->position.col);
}

/* Releases all the memory associated with the list. */
void list_free ( list_t* list){
    list_t *temp;
    while (list!=NULL) {
        temp = list;
        list = list->next;
        free(temp);
    }
    free(list);
}

#endif
