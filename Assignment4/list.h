/*  
*   A linked list describing each row of a sparse matrix.
*   Items in the list are integers representing the column index and the value.
*/

#ifndef __LIST_H__
#define __LIST_H__

#include "matrix_element.h"

typedef struct list{
    element_t data;
    struct list *next;
} list_t;

/* Add the given data to the end of a list */
void list_add( element_t data, list_t *list)
{
    list_t *added = (list_t*)malloc(sizeof(list_t));
    if (added == NULL){
        fprintf(stderr, "Error: unable to allocate required memory\n");
        return;
    }
    list_t * last = list;
    while (last->next)
    {
        last = last->next;
    }
    last->next = added;
    added->next = NULL;
    added->data = data;
}

/* Insert the given data to the next position of current list */
void list_insert( element_t data, list_t *current)
{
    list_t *added = (list_t*)malloc(sizeof(list_t));
    if (added == NULL){
        fprintf(stderr, "Error: unable to allocate required memory\n");
        return;
    }
    list_t * next = current->next;

    current->next = added;
    added->next = next;
    added->data = data;
}

/* Find the corresponding column index in the given list */
list_t * list_find( const int col_id, list_t *list)
{
    if (col_id <= 0)
    {
        fprintf(stderr, "Invalid column index number");
        return NULL;
    }
    list_t *temp = list;
    while (temp)
    {
        if (temp->data.col_id == col_id)
        {
            break;
        }
        temp = temp->next;
    }
    return temp;
}

/* Releases all the memory associated with the list. */
void list_free(list_t *head){
    list_t *temp;
    while(head){
        temp=head;
        head=head->next;
        free(temp);
    }
}


#endif