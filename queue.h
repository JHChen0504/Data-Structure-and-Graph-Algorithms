/* Queue data structure for storing partial solutions to the maze problem */

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdbool.h>
#include "path.h"

/* List-backed Queue consists of nodes, each of which has a reference to a
 * solution (path), as well as the next node inthe queue.
 */
typedef struct node {
    list_t * solution;
    struct node * next;
} node_t;

/* Queue structure contains references to the front and rear nodes for easy
 * insertion at the end and deletion from the front */
typedef struct queue {
    node_t * head;
    node_t * tail;
} queue_t;

/* Initialize a queue data structure */
void queue_initialize(queue_t * queue){
    while (queue->head)
    {
        node_t *temp = queue->head;
        queue->head = queue->head->next;
        free(temp);
    }
    queue->tail = NULL;
}

/* Determine whether a queue is empty */
bool queue_empty (const queue_t * queue){
    return queue->head == NULL;
}

/* Insert a solution at the end of a queue */
bool enqueue (queue_t * queue, list_t* path){
    node_t *newnode = (node_t*)malloc(sizeof(node_t));
    list_t *solution = (list_t*)malloc(sizeof(list_t));
    solution->position = path->position;
    if (newnode == NULL){
        fprintf(stderr, "Error: unable to allocate required memory\n");
        return false;
    }
    newnode->solution = solution;
    newnode->next = NULL;
    if(queue_empty(queue)){
        queue->head = newnode;
    }else{
        queue->tail->next = newnode;
    }
    queue->tail = newnode;
    return true;
}

/* Remove and return the item at the front of the queue */
list_t * dequeue (queue_t * queue){
    //check if the queue is empty
    if(!queue->head)
        return NULL;
    list_t * result = (list_t*)malloc(sizeof(list_t));
    result->position.col = queue->head->solution->position.col;
    result->position.row = queue->head->solution->position.row;
    result->next = NULL;
    node_t *temp = queue->head;
    queue->head = queue->head->next;
    if(!queue->head){
        queue->tail = NULL;
    }
    free(temp);
    return result;
}

/* Return the solution at the front of the queue */
list_t* queue_front (const queue_t * queue){
    return queue->head->solution;
}

#endif