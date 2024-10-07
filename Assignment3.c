// Implement efficient data structure and algorithm for finding shortest
// path from the start cell s to the destination cell t in a maze of dimension m × n.
//
// author: C. H. Chen
// date: 2024/10/7

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "position.h"
#include "path.h"
#include "queue.h"
#include "maze.h"

#define INFTY 2147483647

typedef struct {
    short int v, h;
}direction;

direction move[4] = {{-1,0},{0,1},{1,0},{0,-1}};

int* mazeBFS(const maze_t* maze){
    assert(maze != NULL);
    int height = maze->height;
    int width = maze->width;
    int size = width * height;
    int distance[size];
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            position_t temp = {row, col};
            switch (maze->cells[offset(&maze, temp)])
            {
                case START:
                    distance[offset(&maze, temp)] = 0;
                    break;
                case BLOCKED:
                    distance[offset(&maze, temp)] = -1;
                    break;
                default:
                    distance[offset(&maze, temp)] = INFTY;
                    break;
            }
        }
    }
    list_t *l;
    list_add(maze->start,&l);
    queue_t *q;
    queue_initialize(&q);
    enqueue(&q, &l);
    while (!queue_empty(&q))
    {
        for (int i = 0; i < 4; i++)
        {
            position_t temp;
            temp.col = q->head->solution->position.col+move[i].v;
            temp.row = q->head->solution->position.row+move[i].h;
            switch (maze->cells[offset(&maze, temp)])
            {
            case OPEN:
                enqueue(&q, &l);
                maze->cells[offset(&maze, temp)] = VISITED;
                distance[offset(&maze, temp)] = distance[offset(&maze, q->head->solution->position)] + 1;
                break;
            case END:
                distance[offset(&maze, temp)] += 1;
                break;
            default:
                break;
            }
        }
    }
}


int main(void){
    FILE * fp;
    fp = fopen ("maze.txt", "r");
    maze_t *maze = readMaze(fp);
    fclose(fp);

    return 0;
}